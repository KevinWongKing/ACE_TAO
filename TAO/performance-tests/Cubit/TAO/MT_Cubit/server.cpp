// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    server.cpp
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee, and Sergio Flores-Gaitan
//
// ============================================================================

#include "server.h"
#include "ace/Sched_Params.h"

// Global options used to configure various parameters.
static char hostname[BUFSIZ];
static char *ior_file = 0;
static int base_port = ACE_DEFAULT_SERVER_PORT;
static int num_of_objs = 2;
static u_int use_name_service = 1;
static u_int thread_per_rate = 0;
 
Cubit_Task::Cubit_Task (void)
{
  // No-op.
}

Cubit_Task::Cubit_Task (const char *args,
                        const char *orbname,
                        u_int num_of_objs,
                        ACE_Barrier *barrier,
                        u_int task_id)
  : key_ ("Cubit"),
    orbname_ ((char *) orbname),
    orbargs_ ((char *) args),
    num_of_objs_ (num_of_objs),
    servants_ (0),
    barrier_ (barrier),
    servants_iors_ (0),
    task_id_ (task_id)
{
}

int
Cubit_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Beginning Cubit task with args = '%s'\n",
              orbargs_));

  int rc = this->initialize_orb ();
  if (rc == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ORB initialization failed.\n"),
                      -1);

  rc = this->create_servants ();
  if (rc == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Create Servants failed.\n"),
                      -1);

  TAO_TRY
    {
      this->poa_manager_->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->barrier_->wait ();

      // Handle requests for this object until we're killed, or one of
      // the methods asks us to exit.
      if (this->orb_->run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "run"),
                          -1);

      // Shut down the OA.
      this->poa_->destroy (CORBA::B_TRUE,
                           CORBA::B_TRUE,
                           TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("poa->destroy()");
    }
  TAO_ENDTRY;

  // Need to clean up and do a CORBA::release on everything we've
  // created!

  for (u_int i = 0; i < num_of_objs_; i++)
    delete servants_ [i];

  return 0;
}

int
Cubit_Task::initialize_orb (void)
{
  TAO_TRY
    {
      ACE_ARGV args (this->orbargs_);

      int argc = args.argc ();
      char **argv = args.argv ();

      // Initialize the ORB.
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    this->orbname_,
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Initialize the Object Adapter.
      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references("RootPOA");
      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      this->root_poa_ =
        PortableServer::POA::_narrow (poa_object.in(),
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->poa_manager_ =
        this->root_poa_->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::PolicyList policies (2);
      policies.length (2);

      // Id Assignment policy
      policies[0] =
        this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID,
                                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Lifespan policy
      policies[1] =
        this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT,
                                                 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // We use a different POA, otherwise the user would have to
      // change the object key each time it invokes the server.
      this->poa_ =
        this->root_poa_->create_POA ("Persistent_POA",
                                     this->poa_manager_.in (),
                                     policies,
                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Creation of the new POAs over, so destroy the Policy_ptr's.
      for (CORBA::ULong i = 0;
           i < policies.length () && TAO_TRY_ENV.exception () == 0;
           ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy (TAO_TRY_ENV);
        }
      TAO_CHECK_ENV;

      if (use_name_service == 0)
        return 0;

      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to resolve the Name Service.\n"),
                          1);

      this->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in (),
                                           TAO_TRY_ENV);

      // Check the environment and return 1 if exception occurred or
      // nil pointer.
      if (TAO_TRY_ENV.exception () != 0 ||
          CORBA::is_nil (this->naming_context_.in ())==CORBA::B_TRUE )
        return 1;

      // Register the servant with the Naming Context....
      CosNaming::Name cubit_context_name (1);
      cubit_context_name.length (1);
      cubit_context_name[0].id =
        CORBA::string_dup ("MT_Cubit");

      TAO_TRY_ENV.clear ();
      CORBA::Object_var objref =
        this->naming_context_->bind_new_context (cubit_context_name,
                                                 TAO_TRY_ENV);

      if (TAO_TRY_ENV.exception() != 0)
        {
          CosNaming::NamingContext::AlreadyBound_ptr ex =
            CosNaming::NamingContext::AlreadyBound::_narrow (TAO_TRY_ENV.exception());
          if (ex != 0)
            {
              TAO_TRY_ENV.clear ();
              objref = this->naming_context_->resolve (cubit_context_name,
                                                       TAO_TRY_ENV);
              printf("NamingContext::AlreadyBound\n");
            }
          else
            TAO_TRY_ENV.print_exception ("bind() Cubit context object\n");
        }
      TAO_CHECK_ENV;

      this->mt_cubit_context_ =
        CosNaming::NamingContext::_narrow (objref.in (),
                                           TAO_TRY_ENV);

      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("orb_init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

CORBA::String
Cubit_Task::get_servant_ior (u_int index)
{
  if (index >= num_of_objs_)
    return 0;

  return ACE_OS::strdup (this->servants_iors_[index]);
}

int
Cubit_Task::create_servants ()
{
  TAO_TRY
    {
      // Create the array of cubit implementations.
      ACE_NEW_RETURN (this->servants_,
                      Cubit_i *[this->num_of_objs_],
                      -1);

      // Create the array of strings.
      ACE_NEW_RETURN (this->servants_iors_,
                      CORBA::String [this->num_of_objs_],
                      -1);

      char *buffer;
      int l = ACE_OS::strlen (this->key_) + 3;

      ACE_NEW_RETURN (buffer,
                      char[l],
                      -1);

      // This loop creates multiple servants, and prints out their
      // IORs.
      for (u_int i = 0;
           i < this->num_of_objs_;
           i++)
        {
          ACE_OS::sprintf (buffer,
                           "%s%02d",
                           (char *) this->key_,
                           this->task_id_);

          PortableServer::ObjectId_var id =
            PortableServer::string_to_ObjectId (buffer);

          ACE_NEW_RETURN (this->servants_[i],
                          Cubit_i,
                          -1);

          if (this->servants_[i] == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               " (%P|%t) Unable to create "
                               "implementation object #%d\n",
                               i),
                              2);

          this->poa_->activate_object_with_id (id.in (),
                                               this->servants_[i],
                                               TAO_TRY_ENV);
          TAO_CHECK_ENV;

          // Stringify the objref we'll be implementing, and print it
          // to stdout.  Someone will take that string and give it to
          // some client.  Then release the object.

          Cubit_var cubit =
            this->servants_[i]->_this (TAO_TRY_ENV);
          TAO_CHECK_ENV;

          CORBA::String_var str =
            this->orb_->object_to_string (cubit.in (),
                                          TAO_TRY_ENV);
          TAO_CHECK_ENV;

          this->servants_iors_[i] = ACE_OS::strdup (str.in ());

          // Register the servant with the Naming Context....
          CosNaming::Name cubit_name (1);
          cubit_name.length (1);
          cubit_name[0].id =
            CORBA::string_dup (buffer);

          if (CORBA::is_nil (this->mt_cubit_context_.in ()) == CORBA::B_FALSE)
            {
              this->mt_cubit_context_->bind (cubit_name,
                                             cubit.in (),
                                             TAO_TRY_ENV);
              if (TAO_TRY_ENV.exception () != 0)
                TAO_TRY_ENV.print_exception ("Attempt to bind() a cubit object to the name service Failed!\n");
              else
                ACE_DEBUG ((LM_DEBUG,
                            " (%t) Cubit object bound to the name \"%s\".\n",
                            buffer));
            }

#if 0  /* The IOR gets printout out later, so we don't need this. */
          ACE_DEBUG ((LM_DEBUG,
                      " (%t) Object <%s> created\n",
                      this->servants_iors_[i]));
#endif /* 0 */
        }
      delete [] buffer;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("print IOR");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}

// Parses the command line arguments and returns an error status.
// @@ This method should be integrated into one of the classes
// (preferably into an Options singleton) rather than kept as a
// stand-alone function.

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt opts (argc, argv, "sh:p:t:f:r");
  int c;

  if (ACE_OS::hostname (hostname, BUFSIZ) != 0)
    {
      perror ("gethostname");
      return -1;
    }

  while ((c = opts ()) != -1)
    switch (c)
      {
      case 'r':
	thread_per_rate = 1;
	break;
      case 's':
        use_name_service = 0;
        break;
      case 'f':
        ior_file = opts.optarg;
        break;
      case 'h':
        ACE_OS::strcpy (hostname, opts.optarg);
        ACE_DEBUG ((LM_DEBUG, "h\n"));
        break;
      case 'p':
        base_port = ACE_OS::atoi (opts.optarg);
        ACE_DEBUG ((LM_DEBUG, "p\n"));
        break;
      case 't':
        num_of_objs = ACE_OS::atoi (opts.optarg);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-s Means NOT to use the name service] "
                           " [-p <port>]"
                           " [-h <my_hostname>]"
                           " [-t <num_objects>]"
                           " [-f <ior_file>]"
                           " [-r Use thread per rate]"
                           "\n", argv [0]),
                          1);
      }

  if (thread_per_rate == 1)
    num_of_objs = 5;

  // Indicates successful parsing of command line
  return 0;
}

// @@ This method should be integrated into one of the classes rather
// than kept as a stand-alone function.

static int
initialize (int argc, char **argv)
{
#if defined (VXWORKS)
#if defined (VME_DRIVER)
   STATUS status = vmeDrv ();

   if (status != OK)
     printf ("ERROR on call to vmeDrv()\n");

   status = vmeDevCreate ("/vme");

   if (status != OK)
     printf ("ERROR on call to vmeDevCreate()\n");
#endif /* defined (VME_DRIVER) */

#if defined (FORCE_ARGS)
   int argc = 4;
   char *argv[] = { "server",
                    "-s",
                    "-f",
                    "ior.txt" };

#endif /* defined (FORCE_ARGS) */
#endif /* defined (VXWORKS) */

   // Standard command line parsing utilities used.
   if (parse_args (argc, argv) != 0) return 1;

   if (hostname == 0 || base_port == 0)
     ACE_ERROR_RETURN ((LM_ERROR,
                        "usage:  %s"
                        " [-s Means NOT to use the name service] "
                        " [-p port]"
                        " [-h my_hostname]"
                        " [-t num_objects]"
                        " [-f <ior_file>]"
			" [-r Use thread per rate]"
                        "\n", argv [0]),
                       1);

   // Make sure we've got plenty of socket handles.  This call will use
   // the default maximum.
   ACE::set_handle_limit ();

   return 0;
}

// Starts up the servants

// @@ This method should be integrated into one of the classes rather
// than kept as a stand-alone function.

static int
start_servants (ACE_Barrier &start_barrier)
{
  char *args1;

  ACE_NEW_RETURN (args1,
                  char[BUFSIZ],
                  -1);
  int i;

  // Create an array to hold pointers to the Cubit objects.
  CORBA::String *cubits;

  ACE_NEW_RETURN (cubits,
                  CORBA::String [num_of_objs],
                  -1);

  ACE_OS::sprintf (args1,
                   "rate20 -ORBport %d " 
		   "-ORBhost %s "
		   "-ORBobjrefstyle URL "
		   "-ORBsndsock 32768 "
		   "-ORBrcvsock 32768 ",
                   base_port,
                   hostname);

  Cubit_Task *high_priority_task;

  ACE_NEW_RETURN (high_priority_task,
                  Cubit_Task (args1,
                              "internet",
                              1,
                              &start_barrier,
                              0), //task id 0.
                  -1);

#if defined (VXWORKS)
  ACE_Sched_Priority priority = ACE_THR_PRI_FIFO_DEF;
#elif defined (ACE_WIN32)
  ACE_Sched_Priority priority = ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                                                ACE_SCOPE_THREAD);
#else
  ACE_Sched_Priority priority = ACE_THR_PRI_FIFO_DEF + 25;
#endif /* VXWORKS */

  ACE_DEBUG ((LM_DEBUG,
              "Creating servant with high priority %d\n",
              priority));

  // Make the high priority task an active object.
  if (high_priority_task->activate (THR_BOUND | ACE_SCHED_FIFO,
                                    1,
                                    0,
                                    priority) == -1)
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n",
                  "high_priority_task->activate"));
    }

  // Create an array to hold pointers to the low priority tasks.
  Cubit_Task **low_priority_task;

  ACE_NEW_RETURN (low_priority_task,
                  Cubit_Task *[num_of_objs],
                  -1);

  // Drop the priority
  priority = ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                                    priority,
                                                    ACE_SCOPE_THREAD);

  ACE_DEBUG ((LM_DEBUG,
              "Creating %d servants with priority %d\n",
              num_of_objs - 1,
              priority));

  // Create the low priority servants.

  for (i = num_of_objs - 2; i >= 0; i--)
    {
      char *args;

      ACE_NEW_RETURN (args,
                      char [BUFSIZ],
                      -1);

      ACE_OS::sprintf (args,
		       "rate10 -ORBport %d " 
		       "-ORBhost %s "
		       "-ORBobjrefstyle URL "
		       "-ORBsndsock 32768 "
		       "-ORBrcvsock 32768 ",
                       base_port + 1 + i,
                       hostname);

      ACE_NEW_RETURN (low_priority_task [i],
                      Cubit_Task (args, "internet", 1, &start_barrier, i+1),
                      -1);

      // Make the low priority task an active object.
      if (low_priority_task [i]->activate (THR_BOUND | ACE_SCHED_FIFO,
                                           1,
                                           0,
                                           priority) == -1)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t; %p\n",
                      "low_priority_task[i]->activate"));
        }

      // use different priorities on thread per rate.
      if (thread_per_rate == 1)
	priority = ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
							priority,
							ACE_SCOPE_THREAD);

      ACE_DEBUG ((LM_DEBUG,
		  "Created servant %d\n",
		  i));

    }

  start_barrier.wait ();


  // Write the ior's to a file so the client can read them.
  {
    cubits[0] = high_priority_task->get_servant_ior (0);

    for (i = 0; i < num_of_objs-1; ++i)
      cubits[i + 1] = low_priority_task[i]->get_servant_ior (0);

    FILE *ior_f = 0;

    if (ior_file != 0)
      ior_f = ACE_OS::fopen (ior_file, "w+");

    for (i = 0; i < num_of_objs; ++i)
      {
	if (ior_f != 0)
	  {
	    ACE_OS::fprintf (ior_f, "%s\n", cubits[i]);
	  }
	ACE_OS::printf ("cubits[%d] ior = %s\n",
			i,
			cubits[i]);
      }

    if (ior_f != 0)
      ACE_OS::fclose (ior_f);
  }
  return 0;
}

// main routine.

#if defined (VXWORKS)
  // Rename main to server to make it easier to run both client and
  // server on one target.
extern "C"
int
server (int argc, char *argv[])
{
  ACE_Object_Manager ace_object_manager;
#else
int
main (int argc, char *argv[])
{
#endif

#if defined (ACE_HAS_THREADS)
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  if (ACE_OS::sched_params (
        ACE_Sched_Params (
          ACE_SCHED_FIFO,
#if defined (__Lynx__)
          30,
#else  /* ! __Lynx__ */
          ACE_THR_PRI_FIFO_DEF, //ACE_Sched_Params::priority_min (ACE_SCHED_FIFO),
#endif /* ! __Lynx__ */
          ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        ACE_DEBUG ((LM_MAX, "preempt: user is not superuser, "
                    "so remain in time-sharing class\n"));
      else
        ACE_ERROR_RETURN ((LM_ERROR, "%n: ACE_OS::sched_params failed\n%a"),
                          -1);
    }

  if (initialize (argc, argv) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in Initialization\n"),
                      1);

  // Barrier for the multiple clients to synchronize after binding to
  // the servants.
  ACE_Barrier start_barrier (num_of_objs + 1);

  if (start_servants (start_barrier) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error creating the servants\n"),
                      1);

  ACE_DEBUG ((LM_DEBUG,
              "Wait for all the threads to exit\n"));

  // Wait for all the threads to exit.
  ACE_Thread_Manager::instance ()->wait ();
#else
  ACE_DEBUG ((LM_DEBUG,
              "Test not run.  This platform doesn't seem to have threads.\n"));
#endif /* ACE_HAS_THREADS */
  return 0;
}
