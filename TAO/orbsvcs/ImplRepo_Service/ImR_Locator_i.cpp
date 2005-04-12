// $Id$

#include "ImR_Locator_i.h"
#include "utils.h"
#include "Iterator.h"

#include "orbsvcs/Time_Utilities.h"

#include "tao/IORTable/IORTable.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/ORB_Core.h"
#include "tao/default_ports.h"
#include "tao/Messaging/Messaging.h"

#include "ace/ARGV.h"
#include "ace/OS_NS_sys_time.h"

static const int DEFAULT_START_LIMIT = 1;

static const int PING_RETRY_SCHEDULE[] = {0, 10, 100, 500, 1000, 1000, 1000, 1000, 5000, 5000};

static const ACE_Time_Value DEFAULT_SERVER_TIMEOUT(0, 10 * 1000); // 10ms
// We want to give shutdown a little more time to work, so that we
// can guarantee to the tao_imr utility that it has shutdown. The tao_imr
// utility prints a different message depending on whether shutdown succeeds
// or times out.
static const ACE_Time_Value DEFAULT_SHUTDOWN_TIMEOUT(0, 5000 * 1000);

static PortableServer::POA_ptr
createPersistentPOA(PortableServer::POA_ptr root_poa, const char* poa_name ACE_ENV_ARG_DECL) {

  PortableServer::LifespanPolicy_var life =
    root_poa->create_lifespan_policy(PortableServer::PERSISTENT ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(PortableServer::POA::_nil());

  PortableServer::IdAssignmentPolicy_var assign =
    root_poa->create_id_assignment_policy(PortableServer::USER_ID ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(PortableServer::POA::_nil());

  CORBA::PolicyList pols;
  pols.length(2);
  pols[0] = PortableServer::LifespanPolicy::_duplicate(life.in());
  pols[1] = PortableServer::IdAssignmentPolicy::_duplicate(assign.in());

  PortableServer::POAManager_var mgr = root_poa->the_POAManager();
  PortableServer::POA_var poa =
    root_poa->create_POA(poa_name, mgr.in(), pols ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(PortableServer::POA::_nil());

  life->destroy(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN(PortableServer::POA::_nil());
  assign->destroy(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN(PortableServer::POA::_nil());

  return poa._retn();
}

ImR_Locator_i::ImR_Locator_i ()
: forwarder_(*this)
, ins_locator_(*this)
, debug_(0)
, read_only_(false)
{
}

int
ImR_Locator_i::init_with_orb (CORBA::ORB_ptr orb, Options& opts ACE_ENV_ARG_DECL)
{
  orb_ = CORBA::ORB::_duplicate(orb);
  debug_ = opts.debug();
  read_only_ = opts.readonly();
  startup_timeout_ = opts.startup_timeout();
  ping_interval_ = opts.ping_interval();

  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  this->root_poa_ = PortableServer::POA::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  ACE_ASSERT(! CORBA::is_nil(this->root_poa_.in()));

  this->forwarder_.init(orb ACE_ENV_ARG_PARAMETER);
  this->adapter_.init(& this->forwarder_);

  // Register the Adapter_Activator reference to be the RootPOA's
  // Adapter Activator.
  root_poa_->the_activator (&this->adapter_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Use a persistent POA so that any IOR
  this->imr_poa_ = createPersistentPOA(this->root_poa_.in(),
    "ImplRepo_Service" ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;
  ACE_ASSERT(! CORBA::is_nil(this->imr_poa_.in()));

  waiter_svt_.debug(debug_ > 1);
  PortableServer::ObjectId_var id = PortableServer::string_to_ObjectId ("ImR_AsyncStartupWaiter");
  this->imr_poa_->activate_object_with_id(id.in(), &waiter_svt_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  obj = this->imr_poa_->id_to_reference (id.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  if (startup_timeout_ > ACE_Time_Value::zero)
  {
    obj = set_timeout_policy(obj.in(), startup_timeout_);
  }
  waiter_ = ImplementationRepository::AsyncStartupWaiter::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  id = PortableServer::string_to_ObjectId ("ImplRepo_Service");
  this->imr_poa_->activate_object_with_id(id.in(), this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  obj = this->imr_poa_->id_to_reference (id.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  CORBA::String_var ior = this->orb_->object_to_string(obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Register the ImR for use with INS
  obj = orb->resolve_initial_references ("IORTable" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  IORTable::Table_var ior_table = IORTable::Table::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  ACE_ASSERT(! CORBA::is_nil(ior_table.in()));
  ior_table->bind ("ImplRepoService", ior.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  ior_table->bind ("ImR", ior.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  ior_table->set_locator (&this->ins_locator_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Set up multicast support (if enabled)
  if (opts.multicast())
  {
    ACE_Reactor* reactor = orb->orb_core()->reactor();
    if (this->setup_multicast (reactor, ior.in()) != 0)
      return -1;
  }

  // Initialize the persistent storage. This will load any values that
  // may have been persisted before.
  // The init can return 1 if there is no persistent file yet. In
  // that case, we need not do anything for now.
  int init_result =
    this->repository_.init (opts.repository_mode(), opts.persist_file_name());
  if (init_result == -1)
  {
    ACE_ERROR_RETURN ((LM_ERROR, "Repository failed to initialize\n"), -1);
  }

  // Activate the two poa managers
  PortableServer::POAManager_var poaman =
    this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  poaman->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  poaman = this->imr_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  poaman->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // We write the ior file last so that the tests can know we are ready.
  if (opts.ior_filename().length() > 0)
  {
    FILE* fp = ACE_OS::fopen(opts.ior_filename().c_str(), "w");
    if (fp == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
        "ImR: Could not open file: %s\n", opts.ior_filename().c_str()), -1);
    }
    ACE_OS::fprintf(fp, "%s", ior.in());
    ACE_OS::fclose(fp);
  }

  return 0;
}

int
ImR_Locator_i::init(Options& opts ACE_ENV_ARG_DECL)
{
  ACE_CString cmdline = opts.cmdline();
  cmdline += " -orbcollocation no";
  ACE_ARGV av(cmdline.c_str());
  int argc = av.argc();
  char** argv = av.argv();

  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "TAO_ImR_Locator" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);
  int err = this->init_with_orb(orb.in(), opts ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);
  return err;
}

int
ImR_Locator_i::run(ACE_ENV_SINGLE_ARG_DECL)
{
  if (debug_ > 0)
  {
    ACE_DEBUG((LM_DEBUG, "Implementation Repository: Running\n"
      "\tPing Interval : %dms\n"
      "\tStartup Timeout : %ds\n"
      "\tPersistence : %s\n"
      "\tMulticast : %s\n"
      "\tDebug : %d\n"
      "\tLocked : %s\n\n",
      ping_interval_.msec(),
      startup_timeout_.sec(),
      repository_.repo_mode(),
      ior_multicast_.reactor() != 0 ? "Enabled" : "Disabled",
      debug(),
      read_only_ ? "True" : "False"));
  }
  this->auto_start_servers(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->orb_->run(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
ImR_Locator_i::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
  {
    if (debug_ > 0)
      ACE_DEBUG((LM_DEBUG, "ImR: Shutting down...\n"));

    this->root_poa_->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    this->orb_->destroy(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (debug_ > 0)
      ACE_DEBUG((LM_DEBUG, "ImR: Shut down successfully.\n"));
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ImR_Locator_i::fini");
    ACE_RE_THROW;
  }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
ImR_Locator_i::setup_multicast (ACE_Reactor* reactor, const char* ior)
{
  ACE_ASSERT (reactor != 0);
  ACE_ASSERT (ior != 0);
#if defined (ACE_HAS_IP_MULTICAST)

  TAO_ORB_Core* core = TAO_ORB_Core_instance();
  // See if the -ORBMulticastDiscoveryEndpoint option was specified.
  ACE_CString mde (core->orb_params()->mcast_discovery_endpoint());

  if (mde.length () != 0)
  {
    if (this->ior_multicast_.init(ior,
      mde.c_str(), TAO_SERVICEID_IMPLREPOSERVICE) == -1)
    {
      return -1;
    }
  }
  else
  {
    // Port can be specified as param, env var, or default
    CORBA::UShort port = core->orb_params()->service_port(IMPLREPOSERVICE);
    if (port == 0)
    {
      // Check environment var. for multicast port.
      const char* port_number = ACE_OS::getenv ("ImplRepoServicePort");

      if (port_number != 0)
        port = ACE_OS::atoi (port_number);
    }
    if (port == 0)
      port = TAO_DEFAULT_IMPLREPO_SERVER_REQUEST_PORT;

    if (this->ior_multicast_.init(ior, port,
      ACE_DEFAULT_MULTICAST_ADDR, TAO_SERVICEID_IMPLREPOSERVICE) == -1)
    {
      return -1;
    }
  }

  // Register event handler for the ior multicast.
  if (reactor->register_handler (&this->ior_multicast_,
    ACE_Event_Handler::READ_MASK) == -1)
  {
    if (debug_ >= 1)
      ACE_DEBUG ((LM_DEBUG, "ImR: cannot register Event handler\n"));
    return -1;
  }

  return 0;
#else /* ACE_HAS_IP_MULTICAST*/
  ACE_UNUSED_ARG (reactor);
  ACE_UNUSED_ARG (ior);
  return 0;
#endif /* ACE_HAS_IP_MULTICAST*/
}

CORBA::Long
ImR_Locator_i::register_activator (const char* aname,
                                   ImplementationRepository::Activator_ptr activator
                                   ACE_ENV_ARG_DECL)
                                   ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_ASSERT(aname != 0);
  ACE_ASSERT(! CORBA::is_nil(activator));

  // Before we can register the activator, we need to ensure that any existing
  // registration is purged.
  this->unregister_activator_i(aname);
  ACE_CHECK_RETURN(0);

  CORBA::String_var ior =
    this->orb_->object_to_string (activator ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::Long token = ACE_OS::gettimeofday().msec();

  int err = this->repository_.add_activator (aname, token, ior.in(), activator);
  ACE_ASSERT(err == 0);
  ACE_UNUSED_ARG(err);

  if (this->debug_ > 0)
    ACE_DEBUG ((LM_DEBUG, "ImR: Activator registered for %s.\n", aname));

  return token;
}

void
ImR_Locator_i::unregister_activator (const char* aname,
                                     CORBA::Long token
                                     ACE_ENV_ARG_DECL)
                                     ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_ASSERT(aname != 0);
  Activator_Info_Ptr info = this->get_activator (aname);

  if (! info.null())
  {
    if (info->token != token && this->debug_ > 0)
    {
      ACE_DEBUG((LM_DEBUG, "ImR: Ignoring unregister activator:%s. Wrong token.\n", aname));
      return;
    }

    this->unregister_activator_i(aname);
    ACE_CHECK;

    if (this->debug_ > 0)
      ACE_DEBUG ((LM_DEBUG, "ImR: Activator %s unregistered.\n", aname));
  }
  else
  {
    if (this->debug_ > 0)
      ACE_DEBUG((LM_DEBUG, "ImR: Ignoring unregister activator:%s. Unknown activator.\n", aname));
  }
}

void
ImR_Locator_i::unregister_activator_i(const char* aname)
{
  ACE_ASSERT(aname != 0);
  int err = this->repository_.remove_activator (aname);
  ACE_UNUSED_ARG(err);
}


void
ImR_Locator_i::notify_child_death (const char* name)
                  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_ASSERT(name != 0);

  if (this->debug_ > 1)
    ACE_DEBUG((LM_DEBUG,
               "ImR: Server has died <%s>.\n",
               name));

  Server_Info_Ptr info = this->repository_.get_server(name);
  if (! info.null())
    {
      info->ior = "";
      info->partial_ior = "";

      int err = this->repository_.update_server(*info);
      ACE_ASSERT(err == 0);
      ACE_UNUSED_ARG(err);
    }
  else
    {
      if (this->debug_ > 1)
        ACE_DEBUG((LM_DEBUG,
                   "ImR: Failed to find server in repository.\n"));
    }
}

void
ImR_Locator_i::activate_server (const char* server ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException,
                ImplementationRepository::NotFound,
                ImplementationRepository::CannotActivate))
{
  if (debug_ > 1)
    ACE_DEBUG ((LM_DEBUG, "ImR: Manually activating server <%s>\n", server));

  // This is the version called by tao_imr to activate the server, manually
  // starting it if necessary.
  activate_server_by_name(server, true ACE_ENV_ARG_PARAMETER);
}

char*
ImR_Locator_i::activate_server_by_name (const char* name, bool manual_start ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException,
                ImplementationRepository::NotFound,
                ImplementationRepository::CannotActivate))
{
  // Activate the server, starting it if necessary. Don't start MANUAL
  // servers unless manual_start=true
  ACE_ASSERT(name != 0);

  Server_Info_Ptr info = this->repository_.get_server(name);
  if (info.null())
  {
    ACE_ERROR ((LM_ERROR, "ImR: Cannot find info for server <%s>\n", name));
    ACE_THROW_RETURN(ImplementationRepository::NotFound (), 0);
  }

  return activate_server_i(*info, manual_start ACE_ENV_ARG_PARAMETER);
}

char*
ImR_Locator_i::activate_server_by_object (const char* object_name ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException,
                ImplementationRepository::NotFound,
                ImplementationRepository::CannotActivate))
{
  ACE_ASSERT(object_name != 0);

  // We assume that the first part of the object name is the server name.
  // So a name of foo/bar means that the server name is foo.
  ACE_CString server_name(object_name);
  ssize_t pos = server_name.find ('/');
  if (pos != ACE_CString::npos)
    server_name = server_name.substr(pos + 1);

  return activate_server_by_name(server_name.c_str(), false ACE_ENV_ARG_PARAMETER);
}

char*
ImR_Locator_i::activate_server_i (Server_Info& info, bool manual_start ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException,
                ImplementationRepository::NotFound,
                ImplementationRepository::CannotActivate))
{
  if (info.activation_mode == ImplementationRepository::PER_CLIENT)
  {
    return activate_perclient_server_i(info, manual_start ACE_ENV_ARG_PARAMETER);
  }

  while(true)
  {
    if (is_alive(info))
    {
      if (debug_ > 1)
      {
        ACE_DEBUG((LM_DEBUG, "ImR: Successfully activated <%s> at \n\t%s\n",
          info.name.c_str(), info.partial_ior.c_str()));
      }
      info.start_count = 0;

      waiter_svt_.unblock_all(info.name.c_str());

      return CORBA::string_dup(info.partial_ior.c_str());
    }

    info.reset();

    if (! info.starting && info.start_count >= info.start_limit)
    {
      if (this->debug_ > 0)
      {
        ACE_DEBUG((LM_DEBUG,
          "ImR: Cannot Activate <%s>.\n", info.name.c_str()));
    }

      waiter_svt_.unblock_all(info.name.c_str());

      ACE_THROW_RETURN (ImplementationRepository::CannotActivate
        (CORBA::string_dup ("Cannot start server.")), 0);
    }

    // Note : We already updated info with StartupInfo in server_is_running()
    ImplementationRepository::StartupInfo_var si =
    start_server(info, manual_start ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);
  }
}

char*
ImR_Locator_i::activate_perclient_server_i (Server_Info info, bool manual_start ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException,
                ImplementationRepository::NotFound,
                ImplementationRepository::CannotActivate))
{
  do
  {
    ImplementationRepository::StartupInfo* psi =
      start_server(info, manual_start ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);

    if (psi != 0)
    {
      ImplementationRepository::StartupInfo_var si = psi;
      ACE_ASSERT(info.name == si->name.in());
      info.partial_ior = si->partial_ior.in();
      info.ior = si->ior.in();

      if (is_alive(info))
      {
        if (debug_ > 1)
        {
          ACE_DEBUG((LM_DEBUG, "ImR: Successfully activated <%s> at \n\t%s\n",
            info.name.c_str(), info.partial_ior.c_str()));
        }
        return CORBA::string_dup(info.partial_ior.c_str());
      }
      info.reset();
    }
  } while (info.start_count < info.start_limit);

  if (this->debug_ > 0)
  {
    ACE_DEBUG((LM_DEBUG,
      "ImR: Cannot Activate <%s>.\n", info.name.c_str()));
  }
  ACE_THROW_RETURN (ImplementationRepository::CannotActivate
    (CORBA::string_dup ("Cannot start server.")), 0);
}

ImplementationRepository::StartupInfo*
ImR_Locator_i::start_server(Server_Info& info, bool manual_start ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException,
                ImplementationRepository::NotFound,
                ImplementationRepository::CannotActivate))
{
  if (info.activation_mode == ImplementationRepository::MANUAL && ! manual_start)
  {
    if (debug_ > 0)
      ACE_DEBUG((LM_DEBUG, "ImR: Cannot start server <%s>. ActivationMode=MANUAL\n", info.name.c_str()));
    ACE_THROW(ImplementationRepository::CannotActivate
      (CORBA::string_dup ("Cannot implicitly activate MANUAL server.")));
  }
  if (info.cmdline.length() == 0)
  {
    if (debug_ > 0)
      ACE_DEBUG((LM_DEBUG, "ImR: Cannot start server <%s>."
        " No command line.\n", info.name.c_str()));
    ACE_THROW(ImplementationRepository::CannotActivate
      (CORBA::string_dup ("No command line registered for server.")));
  }

  Activator_Info_Ptr ainfo = get_activator(info.activator);

  if (ainfo.null() || CORBA::is_nil(ainfo->activator.in()))
  {
    if (debug_ > 0)
      ACE_DEBUG((LM_DEBUG, "ImR: Cannot start server <%s>. "
        "Activator <%s> not found.\n", info.name.c_str(), info.activator.c_str()));
    ACE_THROW(ImplementationRepository::CannotActivate
      (CORBA::string_dup ("No activator registered for server.")));
  }

  ACE_TRY
  {
    ++ info.waiting_clients;

    if (info.waiting_clients <= 1 || info.activation_mode == ImplementationRepository::PER_CLIENT)
    {
      info.starting = true;
      ++info.start_count;
      ACE_ASSERT(info.start_count <= info.start_limit);
  if (this->debug_ > 0)
  {
        ACE_DEBUG ((LM_DEBUG, "ImR: Starting server <%s>. Attempt %d/%d.\n",
      info.name.c_str(), info.start_count, info.start_limit));
  }
    ainfo->activator->start_server(
      info.name.c_str(),
      info.cmdline.c_str(),
      info.dir.c_str(),
      info.env_vars
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    }

    if (info.partial_ior.length() == 0)
    {
      if (this->debug_ > 0)
      {
        ACE_DEBUG ((LM_DEBUG, "ImR: Waiting for <%s> to start...\n", info.name.c_str()));
      }

      ImplementationRepository::StartupInfo_var si =
        waiter_->wait_for_startup(info.name.c_str() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      -- info.waiting_clients;
      info.starting = false;

      return si._retn();
    }
    else // The server_is_running() came in before the wait_for_startup()
    {
      if (this->debug_ > 0)
      {
        ACE_DEBUG ((LM_DEBUG, "ImR: <%s> Skipping wait. Already started.\n", info.name.c_str()));
      }
      -- info.waiting_clients;
      info.starting = false;
    }
  }
  ACE_CATCH(CORBA::TIMEOUT, ex)
  {
    -- info.waiting_clients;
    info.starting = false;
    // We may have connected successfully, because the timeout could occur before
    // the AsyncStartupWaiter manages to return. In fact, when the ImR is very busy
    // this is the most likely code path.
    if (info.partial_ior.length() == 0)
  {
    if (debug_ > 0)
        ACE_DEBUG((LM_DEBUG, "ImR : Timeout waiting for <%s> to start.\n", info.name.c_str()));
    info.reset();
  }
}
  ACE_CATCH(ImplementationRepository::CannotActivate, ex)
{
    -- info.waiting_clients;
    info.starting = false;
    info.reset();
    if (debug_ > 0)
      ACE_DEBUG((LM_DEBUG, "ImR: Activator cannot start <%s>.\n", info.name.c_str()));
    }
    ACE_CATCHANY
    {
    -- info.waiting_clients;
    info.starting = false;
      if (debug_ > 0)
      ACE_DEBUG((LM_DEBUG, "ImR: Unexpected exception while starting <%s>.\n", info.name.c_str()));
      if (debug_ > 1)
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "");
      ainfo->reset();
    info.reset();
    }
    ACE_ENDTRY;
  return 0; // This is not a corba call, so a zero should be ok
}

CORBA::Object_ptr
ImR_Locator_i::set_timeout_policy(CORBA::Object_ptr obj, const ACE_Time_Value& to)
{
  CORBA::Object_var ret(CORBA::Object::_duplicate(obj));

  ACE_TRY_NEW_ENV
  {
    TimeBase::TimeT timeout;
    ORBSVCS_Time::Time_Value_to_TimeT (timeout, to);
    CORBA::Any tmp;
    tmp <<= timeout;

    CORBA::PolicyList policies(1);
    policies.length(1);
    policies[0] = orb_->create_policy(Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE, tmp ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    ret = obj->_set_policy_overrides (policies, CORBA::ADD_OVERRIDE ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    policies[0]->destroy(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (CORBA::is_nil(ret.in()))
    {
      if (this->debug_ > 0)
      {
        ACE_DEBUG ((LM_DEBUG, "ImR: Unable to set timeout policy.\n"));
      }
      ret = CORBA::Object::_duplicate(obj);
    }
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ImR_Locator_i::set_timeout_policy()");
  }
  ACE_ENDTRY;

  return ret._retn();
}

void
ImR_Locator_i::register_server (const char* server,
                                const ImplementationRepository::StartupOptions &options
                                ACE_ENV_ARG_DECL)
                                ACE_THROW_SPEC ((CORBA::SystemException,
                                ImplementationRepository::AlreadyRegistered,
                                ImplementationRepository::NotFound))
{
  register_server_i(server, options, false ACE_ENV_ARG_PARAMETER);
}

void
ImR_Locator_i::reregister_server (const char* server,
                                  const ImplementationRepository::StartupOptions &options
                                  ACE_ENV_ARG_DECL )
                                  ACE_THROW_SPEC ((CORBA::SystemException,
                                  ImplementationRepository::AlreadyRegistered,
                                  ImplementationRepository::NotFound))
{
  register_server_i(server, options, true ACE_ENV_ARG_PARAMETER);
}
void
ImR_Locator_i::register_server_i (const char* server,
                                  const ImplementationRepository::StartupOptions &options,
                                  bool allow_updates
                                  ACE_ENV_ARG_DECL )
                                  ACE_THROW_SPEC ((CORBA::SystemException,
                                  ImplementationRepository::AlreadyRegistered,
                                  ImplementationRepository::NotFound))
{
  ACE_ASSERT(server != 0);

  if (this->read_only_)
  {
    ACE_DEBUG((LM_DEBUG, "ImR: Cannot register server <%s> due to locked database.\n", server));
    ACE_THROW (CORBA::NO_PERMISSION (
      CORBA::SystemException::_tao_minor_code (TAO_IMPLREPO_MINOR_CODE, 0),
      CORBA::COMPLETED_NO));
  }

  int limit = options.start_limit;
  if (limit < 0)
  {
    limit = -limit;
  }
  else if (limit == 0)
  {
    limit = 1;
  }

  Server_Info_Ptr info = this->repository_.get_server(server);
  if (info.null())
  {
    if (this->debug_ > 1)
      ACE_DEBUG((LM_DEBUG, "ImR: Adding server <%s>.\n", server));

    this->repository_.add_server (server,
      options.activator.in(),
      options.command_line.in (),
      options.environment,
      options.working_directory.in (),
      options.activation,
      limit);
  }
  else if (! allow_updates)
  {
    ACE_ERROR ((LM_ERROR,
      "ImR: Server <%s> already registered.\n", server));
    ACE_THROW (ImplementationRepository::AlreadyRegistered());
  }
  else
  {
    if (this->debug_ > 1)
      ACE_DEBUG((LM_DEBUG, "ImR: Updating server <%s>.\n", server));

    info->activator = options.activator.in();
    info->cmdline = options.command_line.in();
    info->env_vars = options.environment;
    info->dir = options.working_directory.in();
    info->activation_mode = options.activation;
    info->start_limit = limit;
    info->start_count = 0;
    int err = this->repository_.update_server(*info);
    ACE_ASSERT(err == 0);
    ACE_UNUSED_ARG(err);
  }

  if (debug_ > 0)
    ACE_DEBUG ((LM_DEBUG, "ImR: Add/Update server <%s>.\n", server));

  if (this->debug_ > 1)
  {
    // Note : The info var may be null, so we use options.
    ACE_DEBUG ((LM_DEBUG, "ImR: Server: %s\n"
      "\tActivator: %s\n"
      "\tCommand Line: %s\n"
      "\tWorking Directory: %s\n"
      "\tActivation: %s\n"
      "\tStart Limit: %d\n"
      "\n",
      server,
      options.activator.in(),
      options.command_line.in (),
      options.working_directory.in (),
      ImR_Utils::activationModeToString(options.activation).c_str(),
      limit
      ));

    for (CORBA::ULong i = 0; i < options.environment.length(); ++i)
      ACE_DEBUG ((LM_DEBUG, "Environment variable %s=%s\n",
      options.environment[i].name.in(),
      options.environment[i].value.in()));
  }
}

void
ImR_Locator_i::remove_server (const char* name ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ACE_ASSERT(name != 0);
  if (this->read_only_)
  {
    ACE_ERROR ((LM_ERROR,
      "ImR: Can't remove server <%s> due to locked database.\n", name));
    ACE_THROW (CORBA::NO_PERMISSION (
      CORBA::SystemException::_tao_minor_code (TAO_IMPLREPO_MINOR_CODE, 0),
      CORBA::COMPLETED_NO));
  }

  // Note : This will be safe, because any Server_Info_Ptr objects will still
  // be valid, and the actual Server_Info will be destroyed when the last
  // one goes out of scope.

  Server_Info_Ptr info = this->repository_.get_server(name);
  if (! info.null())
  {
    if (this->repository_.remove_server (name) == 0)
    {
      if (this->debug_ > 0)
        ACE_DEBUG ((LM_DEBUG, "ImR: Removed Server <%s>.\n", name));
    }
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
      "ImR: Can't remove unknown server <%s>.\n", name));
    ACE_THROW (ImplementationRepository::NotFound ());
  }
}

void
ImR_Locator_i::shutdown_server (const char* server ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ACE_ASSERT(server != 0);

  if (this->debug_ > 0)
    ACE_DEBUG ((LM_DEBUG, "ImR: Shutting down server <%s>.\n", server));

  Server_Info_Ptr info = this->repository_.get_server(server);
  if (info.null())
  {
    ACE_ERROR ((LM_ERROR,
      "ImR: shutdown_server() Cannot find info for server <%s>\n", server));
    ACE_THROW(ImplementationRepository::NotFound());
  }

  connect_server(*info);

  if (CORBA::is_nil(info->server.in()))
  {
    ACE_ERROR ((LM_ERROR,
      "ImR: shutdown_server() Cannot connect to server <%s>\n", server));
    ACE_THROW(ImplementationRepository::NotFound());
  }

  ACE_TRY_NEW_ENV
  {
    CORBA::Object_var obj = set_timeout_policy(info->server.in(), DEFAULT_SHUTDOWN_TIMEOUT);
    ImplementationRepository::ServerObject_var server =
      ImplementationRepository::ServerObject::_unchecked_narrow (obj.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCH(CORBA::TIMEOUT, ex)
  {
    info->reset();
    int err = this->repository_.update_server(*info);
    ACE_ASSERT(err == 0);
    ACE_UNUSED_ARG(err);
    // Note : This is a good thing. It means we didn't waste our time waiting for
    // the server to finish shutting down.
    if (this->debug_ > 1)
    {
      ACE_DEBUG((LM_DEBUG, "ImR: Timeout while waiting for <%s> shutdown.\n", server));
    }
    ACE_RE_THROW;
  }
  ACE_CATCHANY
  {
    if (this->debug_ > 1)
    {
      ACE_DEBUG((LM_DEBUG, "ImR: Exception ignored while shutting down <%s>\n", server));
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "");
    }
  }
  ACE_ENDTRY;

  info->reset();

  int err = this->repository_.update_server(*info);
  ACE_ASSERT(err == 0);
  ACE_UNUSED_ARG(err);
}

void
ImR_Locator_i::server_is_running (const char* name,
                                  const char* partial_ior,
                                  ImplementationRepository::ServerObject_ptr server
                                  ACE_ENV_ARG_DECL)
                                  ACE_THROW_SPEC ((CORBA::SystemException,
                                  ImplementationRepository::NotFound))
{
  ACE_ASSERT(name != 0);
  ACE_ASSERT(partial_ior != 0);
  ACE_ASSERT(! CORBA::is_nil(server));

  if (this->debug_ > 0)
    ACE_DEBUG ((LM_DEBUG, "ImR: Server %s is running at %s.\n", name, partial_ior));

  CORBA::String_var ior = orb_->object_to_string (server ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_ > 1)
    ACE_DEBUG ((LM_DEBUG, "ImR: Server %s callback at %s.\n", name, ior.in()));

  Server_Info_Ptr info = this->repository_.get_server(name);
  if (info.null())
  {
    if (this->debug_ > 0)
      ACE_DEBUG ((LM_DEBUG, "ImR: Auto adding NORMAL server <%s>.\n", name));

    ImplementationRepository::EnvironmentList env(0);
    this->repository_.add_server (name,
      "", // no activator
      "", // no cmdline
      ImplementationRepository::EnvironmentList(),
      "", // no working dir
      ImplementationRepository::NORMAL,
      DEFAULT_START_LIMIT,
      partial_ior,
      ior.in(),
      ImplementationRepository::ServerObject::_nil() // Will connect at first access
      );
  }
  else
  {
    if (info->activation_mode != ImplementationRepository::PER_CLIENT) {
    info->ior = ior.in();
    info->partial_ior = partial_ior;
      info->server = ImplementationRepository::ServerObject::_nil(); // Will connect at first access

    int err = this->repository_.update_server(*info);
    ACE_ASSERT(err == 0);
    ACE_UNUSED_ARG(err);

      waiter_svt_.unblock_one(name, partial_ior, ior.in(), false);
    } else {
      // Note : There's no need to unblock all the waiting request until
      // we know the final status of the server.
      waiter_svt_.unblock_one(name, partial_ior, ior.in(), true);
    }
  }
}

void
ImR_Locator_i::server_is_shutting_down (const char* server ACE_ENV_ARG_DECL_NOT_USED)
ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ACE_ASSERT(server != 0);
  Server_Info_Ptr info = this->repository_.get_server(server);
  if (info.null())
  {
    if (this->debug_ > 1)
    {
      ACE_DEBUG((LM_DEBUG,
        "ImR_Locator_i::server_is_shutting_down: Unknown server:%s\n", server));
    }
    return;
  }

  if (this->debug_ > 0)
    ACE_DEBUG((LM_DEBUG, "ImR: server <%s> is shutting down.\n", server));

  info->reset();

  int err = this->repository_.update_server(*info);
  ACE_ASSERT(err == 0);
  ACE_UNUSED_ARG(err);
}

void
ImR_Locator_i::find (const char* server,
                     ImplementationRepository::ServerInformation_out imr_info
                     ACE_ENV_ARG_DECL)
                     ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ACE_ASSERT(server != 0);
  ACE_NEW_THROW_EX (imr_info, ImplementationRepository::ServerInformation, CORBA::NO_MEMORY ());

  Server_Info_Ptr info = this->repository_.get_server(server);
  if (info.null())
  {
    ACE_ERROR ((LM_ERROR, "ImR: Cannot find info for server <%s>\n", server));
    ACE_THROW(ImplementationRepository::NotFound());
  }

  imr_info = info->createImRServerInfo(ACE_ENV_SINGLE_ARG_PARAMETER);

  if (this->debug_ > 1)
    ACE_DEBUG ((LM_DEBUG, "ImR: Found server %s.\n", server));
}

void
ImR_Locator_i::list (CORBA::ULong how_many,
                     ImplementationRepository::ServerInformationList_out server_list,
                     ImplementationRepository::ServerInformationIterator_out server_iterator
                     ACE_ENV_ARG_DECL
                     ) ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->debug_ > 0)
    ACE_DEBUG ((LM_DEBUG, "ImR: List servers.\n"));

  // Initialize the out variables, so if we return early, they will
  // not be dangling.
  server_iterator = ImplementationRepository::ServerInformationIterator::_nil();
  ACE_NEW_THROW_EX (server_list,
    ImplementationRepository::ServerInformationList(0), CORBA::NO_MEMORY());

  Locator_Repository::SIMap::ENTRY* entry = 0;
  Locator_Repository::SIMap::ITERATOR it(this->repository_.servers());

  // Number of servers that will go into the server_list.
  CORBA::ULong n = this->repository_.servers().current_size();
  if (how_many > 0 && n > how_many)
  {
    n = how_many;
  }

  server_list->length(n);

  if (this->debug_ > 1)
    ACE_DEBUG ((LM_DEBUG, "ImR_Locator_i::list: Filling ServerList with %d servers\n", n));

  for (CORBA::ULong i = 0; i < n; i++)
  {
    it.next(entry);
    it.advance();
    ACE_ASSERT(entry != 0);

    Server_Info_Ptr info = entry->int_id_;

    ImplementationRepository::ServerInformation_var imr_info = info->createImRServerInfo(ACE_ENV_SINGLE_ARG_PARAMETER);
    server_list[i] = *imr_info;
  }

  if (this->repository_.servers().current_size() > n)
  {
    if (this->debug_ > 1)
      ACE_DEBUG ((LM_DEBUG, "ImR_Locator_i::list: Creating ServerInformation Iterator\n"));

    ImR_Iterator* imr_iter;

    ACE_NEW_THROW_EX (imr_iter,
      ImR_Iterator (n, this->repository_, this->imr_poa_.in()),
      CORBA::NO_MEMORY ());

    PortableServer::ServantBase_var tmp(imr_iter);

    ACE_TRY
    {
      PortableServer::ObjectId_var id =
        this->imr_poa_->activate_object (imr_iter ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::Object_var obj = this->imr_poa_->id_to_reference(id.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      server_iterator = ImplementationRepository::
        ServerInformationIterator::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
    ACE_CATCHANY
    {
      ACE_RE_THROW;
    }
    ACE_ENDTRY;
    ACE_CHECK;
  }
}

Activator_Info_Ptr
ImR_Locator_i::get_activator (const ACE_CString& aname)
{
  Activator_Info_Ptr info = this->repository_.get_activator (aname);
  if (! info.null())
  {
    this->connect_activator (*info);
  }
  return info;
}

void
ImR_Locator_i::connect_activator (Activator_Info& info)
{
  if (! CORBA::is_nil(info.activator.in()) || info.ior.length() == 0)
    return;

  ACE_TRY_NEW_ENV
  {
    CORBA::Object_var obj =
      this->orb_->string_to_object (info.ior.c_str()
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (CORBA::is_nil(obj.in()))
    {
      info.reset();
      return;
    }

    if (startup_timeout_ > ACE_Time_Value::zero)
    {
      obj = set_timeout_policy(obj.in(), startup_timeout_);
    }

    info.activator =
      ImplementationRepository::Activator::_unchecked_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (CORBA::is_nil(info.activator.in()))
    {
      info.reset();
      return;
    }

    if (debug_ > 0)
      ACE_DEBUG((LM_DEBUG, "ImR: Connected to activator <%s>\n", info.name.c_str()));
  }
  ACE_CATCHANY
  {
    info.reset();
  }
  ACE_ENDTRY;
}

void
ImR_Locator_i::auto_start_servers(ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->repository_.servers().current_size() == 0)
    return;

  Locator_Repository::SIMap::ENTRY* server_entry;
  Locator_Repository::SIMap::ITERATOR server_iter(this->repository_.servers());

  // For each of the entries in the Locator_Repository, get the startup
  // information and activate the servers, if they are not already
  // running.
  for (;server_iter.next(server_entry) != 0; server_iter.advance())
  {
    Server_Info_Ptr info = server_entry->int_id_;
    ACE_ASSERT(! info.null());

    ACE_TRY
    {
      if (info->activation_mode == ImplementationRepository::AUTO_START
        && info->cmdline.length() > 0)
      {
        this->activate_server_i (*info, true ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    }
    ACE_CATCHANY
    {
      if (this->debug_ > 1)
      {
        ACE_DEBUG ((LM_DEBUG,
          "ImR: AUTO_START Could not activate <%s>\n",
          server_entry->ext_id_.c_str ()));
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "AUTO_START");
      }
      // Ignore exceptions
    }
    ACE_ENDTRY;
    ACE_CHECK;
  }
}

void
ImR_Locator_i::connect_server(Server_Info& info)
{
  if (! CORBA::is_nil(info.server.in()))
  {
    return; // already connected
  }

  if (info.ior.length() == 0)
  {
    info.reset();
    return; // can't connect
  }

  ACE_TRY_NEW_ENV
  {
    CORBA::Object_var obj = orb_->string_to_object (info.ior.c_str() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (CORBA::is_nil(obj.in()))
    {
      info.reset();
      return;
    }

    obj = set_timeout_policy(obj.in(), DEFAULT_SERVER_TIMEOUT);

    info.server =
      ImplementationRepository::ServerObject::_unchecked_narrow (obj.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (CORBA::is_nil(info.server.in()))
    {
      info.reset();
      return;
    }

    if (debug_ > 1)
      ACE_DEBUG((LM_DEBUG, "ImR: Connected to server <%s>\n", info.name.c_str()));
  }
  ACE_CATCHANY
  {
    info.reset();
  }
  ACE_ENDTRY;
}

bool
ImR_Locator_i::is_alive(Server_Info& info)
{
  for (size_t i = 0; i < sizeof(PING_RETRY_SCHEDULE) + 1; ++i)
  {
    int status = this->is_alive_i(info);
    if (status == 0)
      return false;
    if (status == 1)
      return true;

    // This is evil, but there's not much else we can do for now. We should never
    // reach this code once the ImR Servers are fixed so that they don't lie about
    // server_is_running. Currently, they send this notification during poa creation.
    // We have to run the orb, because the very thing that may be slowing the
    // aliveness of the servers is the fact that they're trying to register more
    // objects with us.
    // In practical testing, we never retried the ping more than once, because
    // the second ping always timed out, even if the servers poa manager had
    // not been activated. The only way we saw multiple retries was if we ran
    // the orb on the server before the poa manager was activated.
    // For this reason, the first retry is immediate, and the orb->run() call
    // is not required. The call will likely timeout, and is_alive will return true.
    if (PING_RETRY_SCHEDULE[i] > 0)
    {
      ACE_Time_Value tv(0, PING_RETRY_SCHEDULE[i] * 1000);
    this->orb_->run(tv);
  }
  }
  if (debug_ > 0)
  {
    ACE_DEBUG((LM_DEBUG,
      "ImR: <%s> Ping retry count exceeded. alive=maybe.\n", info.name.c_str()));
  }
  // We return true here, because the server *might* be alive, it's just not starting in a timely
  // manner. We can't return false, because then we'll just try to start another instance, and the
  // same thing will likely happen.
  info.last_ping = ACE_OS::gettimeofday();
  return true;
}

int
ImR_Locator_i::is_alive_i(Server_Info& info)
{
  // This is used by the ACE_TRY below when exceptions are turned off.
  ACE_DECLARE_NEW_CORBA_ENV;

  if (info.ior.length() == 0 || info.partial_ior.length() == 0)
  {
    if (debug_ > 1)
    {
      ACE_DEBUG((LM_DEBUG,
        "ImR: <%s> not running. alive=false.\n", info.name.c_str()));
    }
    info.last_ping = ACE_Time_Value::zero;
    return 0;
  }

  if (ping_interval_ == ACE_Time_Value::zero)
  {
    if (debug_ > 1)
    {
      ACE_DEBUG((LM_DEBUG,
        "ImR: <%s> Ping verification disabled. alive=true.\n", info.name.c_str()));
    }
    return 1;
  }

  if ((ACE_OS::gettimeofday() - info.last_ping) < ping_interval_)
  {
    if (debug_ > 1)
    {
      ACE_DEBUG((LM_DEBUG,
        "ImR: <%s> within ping interval. alive=true.\n", info.name.c_str()));
    }
    return 1;
  }

  // If we don't have enough information to start the server if it isn't already
  // then we might as well assume it is running. That way the client can get the
  // status directly from the server.
  if (info.cmdline.length() == 0 || ! repository_.has_activator(info.activator))
  {
    if (debug_ > 1)
    {
      ACE_DEBUG((LM_DEBUG,
        "ImR: Ping verification skipped. <%s> not startable.\n", info.name.c_str()));
    }
    return 1;
  }

  connect_server(info);

  if (CORBA::is_nil (info.server.in()))
  {
    if (debug_ > 1)
    {
      ACE_DEBUG((LM_DEBUG,
        "ImR: <%s> Could not connect. alive=false.\n", info.name.c_str()));
    }
    return 0;
  }

  ACE_TRY
  {
    // Make a copy, in case the info is updated during the ping.
    ImplementationRepository::ServerObject_var server = info.server;

    // This will timeout if it takes too long
    server->ping (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (debug_ > 1)
    {
      ACE_DEBUG((LM_DEBUG,
        "ImR: <%s> Ping successful. alive=true\n", info.name.c_str()));
    }
    info.last_ping = ACE_OS::gettimeofday();
  }
  ACE_CATCH(CORBA::TRANSIENT, ex)
  {
    const CORBA::ULong BITS_5_THRU_12_MASK = 0x00000f80;
    switch (ex.minor() & BITS_5_THRU_12_MASK)
    {
    case TAO_INVOCATION_SEND_REQUEST_MINOR_CODE:
      {
        if (debug_ > 1)
        {
          ACE_DEBUG((LM_DEBUG,
            "ImR: <%s> Local TRANSIENT. alive=false.\n", info.name.c_str()));
        }
      }
      info.last_ping = ACE_Time_Value::zero;
      return 0;
    case TAO_POA_DISCARDING:
    case TAO_POA_HOLDING:
      {
        if (debug_ > 1)
        {
          ACE_DEBUG((LM_DEBUG,
            "ImR: <%s> Remote TRANSIENT. alive=maybe.\n", info.name.c_str()));
        }
      }
      return -1; // We keep trying to ping, because returning 1 now, would just lead
                 // to clients getting the same exception. If we can't ping after several
                 // attempts, then we'll give up and return 1, letting the client worry about it.
    default:
      {
        if (debug_ > 1)
        {
          ACE_DEBUG((LM_DEBUG,
            "ImR: <%s> Unknown Local TRANSIENT. alive=false.\n", info.name.c_str()));
        }
      info.last_ping = ACE_Time_Value::zero;
      }
      return 0;
    }
  }
  ACE_CATCH(CORBA::TIMEOUT, ex)
  {
    if (debug_ > 1)
    {
      ACE_DEBUG((LM_DEBUG,
      "ImR: <%s> Ping timed out. alive=true.\n", info.name.c_str()));
    }
    return 1; // This is "alive" as far as we're concerned. Presumably the client
              // will have a less stringent timeout policy, or will want to know
              // about the timeout. In any case, we're only guaranteeing that the
              // server is alive, not that it's responsive.
  }
  ACE_CATCHANY
  {
    if (debug_ > 1)
    {
      ACE_DEBUG((LM_DEBUG, "ImR: <%s> Unexpected Ping exception. alive=false\n", info.name.c_str()));
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "\n");
    }
    info.last_ping = ACE_Time_Value::zero;
    return false;
  }
  ACE_ENDTRY;
  return 1;
}

int
ImR_Locator_i::debug() const
{
  return debug_;
}
