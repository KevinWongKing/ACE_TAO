// $Id$

// Implementation of Named Value List and NamedValue classes

#include "tao/NVList.h"

#if !defined (TAO_HAS_MINIMUM_CORBA)

#include "tao/Exception.h"
#include "tao/Environment.h"
#include "tao/ORB.h"
#include "tao/debug.h"
#include "ace/Auto_Ptr.h"

#if !defined (__ACE_INLINE__)
# include "tao/NVList.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, NVList, "$Id$")

// Reference counting for DII Request object

CORBA::ULong
CORBA_NamedValue::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_, 0);
  return this->refcount_++;
}

CORBA::ULong
CORBA_NamedValue::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  delete this;
  return 0;
}

CORBA_NamedValue::~CORBA_NamedValue (void)
{
  if (this->name_)
    {
      CORBA::string_free (this->name_);
      this->name_ = 0;
    }
  // the any will be destroyed by itself
}

// ****************************************************************

CORBA::ULong
CORBA_NVList::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_, 0);
  return this->refcount_++;
}

CORBA::ULong
CORBA_NVList::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  delete this;
  return 0;
}

CORBA_NVList::~CORBA_NVList (void)
{
  // initialize an iterator and delete each NamedValue
  ACE_Unbounded_Queue_Iterator<CORBA::NamedValue_ptr> iter (this->values_);

  for (iter.first (); !iter.done (); iter.advance ())
    {
      CORBA::NamedValue_ptr *nv;
      (void) iter.next (nv);
      delete *nv;
    }

  this->max_ = 0;

  // Remove the CDR stream if it is present.
  delete this->incoming_;
}

// add an element and just initialize its flags
CORBA::NamedValue_ptr
CORBA_NVList::add (CORBA::Flags flags,
                   CORBA::Environment &ACE_TRY_ENV)
{
  // call the helper to allocate a NamedValue element (if necessary)
  return this->add_element (flags, ACE_TRY_ENV);
}

// add an element and just initialize its flags and name
CORBA::NamedValue_ptr
CORBA_NVList::add_item (const char *name,
                        CORBA::Flags flags,
                        CORBA::Environment &ACE_TRY_ENV)
{
  // call the helper to allocate a NamedValue element
  CORBA::NamedValue_ptr nv = this->add_element (flags, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  if (nv)
    {
      // now initialize the fields
      nv->name_ = CORBA::string_dup (name);
      return nv;
    }
  else
    return 0;
}

// add a value. If necessary, increment the list
CORBA::NamedValue_ptr
CORBA_NVList::add_value (const char *name,
                         const CORBA::Any &value,
                         CORBA::Flags flags,
                         CORBA::Environment &ACE_TRY_ENV)
{
  // call the helper to allocate a NamedValue element
  CORBA::NamedValue_ptr nv = this->add_element (flags, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  if (nv)
    {
      // now initialize the fields
      nv->name_ = CORBA::string_dup (name);
      if (ACE_BIT_ENABLED (flags, CORBA::IN_COPY_VALUE))
        // IN_COPY_VALUE means that the parameter is not "borrowed" by
        // the ORB, but rather that the ORB copies its value.
        //
        // Initialize the newly allocated memory using a copy
        // constructor that places the new "Any" value at just the right
        // place, and makes a "deep copy" of the data.
        nv->any_ = value;
      else
        {
          // The normal behaviour for parameters is that the ORB "borrows"
          // their memory for the duration of calls.
          //
          if (value.value_)
            {
              nv->any_.replace (value.type_,
                                value.value_,
                                0,
                                ACE_TRY_ENV);
            }
          else
            {
              nv->any_._tao_replace (value.type_,
                                     value.cdr_,
                                     ACE_TRY_ENV);
            }
          ACE_CHECK_RETURN (0);
        }
      return nv;
    }
  else
    return 0;
}

// add an element and just initialize its flags and name
CORBA::NamedValue_ptr
CORBA_NVList::add_item_consume (char *name,
                                CORBA::Flags flags,
                                CORBA::Environment &ACE_TRY_ENV)
{

  // call the helper to allocate a NamedValue element
  CORBA::NamedValue_ptr nv = this->add_element (flags, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  if (nv)
    {
      // now initialize the fields

      // consume the name
      nv->name_ = name;
      return nv;
    }
  else
    return 0;
}

// add a value. If necessary, increment the list
CORBA::NamedValue_ptr
CORBA_NVList::add_value_consume (char * name,
                                 CORBA::Any * value,
                                 CORBA::Flags flags,
                                 CORBA::Environment & ACE_TRY_ENV)
{
  // call the helper to allocate a NamedValue element
  CORBA::NamedValue_ptr nv = this->add_element (flags, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  if (nv)
    {
      // now initialize the fields

      // consume name
      nv->name_ = name;

      // consume the value @@ (ASG) have we? we may need to destroy
      // the in parameter
      nv->any_ = *value;
      return nv;
    }
  else
    return 0;
}

//CORBA::Status
void
CORBA_NVList::remove (CORBA::ULong /*n*/, CORBA::Environment &/*env*/)
{
  // not implemented
  // @@ (ASG) - TODO
}

// Helper method
CORBA::NamedValue_ptr
CORBA_NVList::add_element (CORBA::Flags flags,
                           CORBA::Environment &ACE_TRY_ENV)
{
  this->evaluate (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::NamedValue::_nil ());

  if (ACE_BIT_DISABLED (flags,
                        CORBA::ARG_IN | CORBA::ARG_OUT | CORBA::ARG_INOUT))
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);

  CORBA::NamedValue_ptr nv;

  // allocate a new NamedValue
  ACE_NEW_RETURN (nv, CORBA::NamedValue, 0);

  // set the flags and enqueue in the queue
  nv->flags_ = flags;
  if (this->values_.enqueue_tail (nv) == -1)
    {
      delete nv;
      return 0;
    }

  this->max_++;
  return nv; // success
}

// return the item at location n
CORBA::NamedValue_ptr
CORBA_NVList::item (CORBA::ULong n, CORBA::Environment &ACE_TRY_ENV)
{
  this->evaluate (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::NamedValue::_nil ());

  if (n >= this->max_) // 0 based indexing
    ACE_THROW_RETURN (CORBA::TypeCode::Bounds (), 0);

  CORBA::NamedValue_ptr *nv;

  this->values_.get (nv, n);
  return *nv;
}

void
CORBA_NVList::_tao_incoming_cdr (TAO_InputCDR &cdr,
                                 int flag,
                                 int lazy_evaluation,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  if (!lazy_evaluation)
    {
      this->_tao_decode (cdr,
                         flag,
                         ACE_TRY_ENV);
      return;
    }
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_);
  if (this->incoming_ != 0)
    {
      delete this->incoming_;
      this->incoming_ = 0;
    }

  ACE_NEW (this->incoming_, TAO_InputCDR (cdr));
  this->incoming_flag_ = flag;
}

void
CORBA_NVList::_tao_encode (TAO_OutputCDR &cdr,
                           TAO_ORB_Core *orb_core,
                           int flag,
                           CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_);
  if (this->incoming_ != 0)
    {
      // Then unmarshal each "in" and "inout" parameter.
      ACE_Unbounded_Queue_Iterator<CORBA::NamedValue_ptr> i (this->values_);

      for (i.first (); !i.done (); i.advance ())
        {
          CORBA::NamedValue_ptr *item;
          (void) i.next (item);

          CORBA::NamedValue_ptr nv = *item;

          if (ACE_BIT_DISABLED (nv->flags (), flag))
            continue;

          if (TAO_debug_level > 3)
            {
              const char* arg = nv->name ();
              if (arg == 0)
                arg = "(nil)";

              ACE_DEBUG ((LM_DEBUG,
                          "NVList::_tao_encode - parameter <%s>\n",
                          arg));
            }
          CORBA::TypeCode_var tc = nv->value ()->type ();
          cdr.append (tc.in (),
                      this->incoming_,
                      ACE_TRY_ENV);
          ACE_CHECK;
        }

      delete this->incoming_;
      this->incoming_ = 0;
      return;
    }

  // The list is already evaluated, we cannot optimize the copies, go
  // ahead with the slow way to do things.

  // Then unmarshal each "in" and "inout" parameter.
  ACE_Unbounded_Queue_Iterator<CORBA::NamedValue_ptr> i (this->values_);

  for (i.first (); !i.done (); i.advance ())
    {
      CORBA::NamedValue_ptr *item;
      (void) i.next (item);

      CORBA::NamedValue_ptr nv = *item;

      if (ACE_BIT_DISABLED (nv->flags (), flag))
        continue;

      nv->value ()->_tao_encode (cdr,
                                 orb_core,
                                 ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
CORBA_NVList::_tao_decode (TAO_InputCDR &incoming,
                           int flag,
                           CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_debug_level > 3)
    ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) : NVList::_tao_decode\n"));

  // Then unmarshal each "in" and "inout" parameter.
  ACE_Unbounded_Queue_Iterator<CORBA::NamedValue_ptr> i (this->values_);

  for (i.first (); !i.done (); i.advance ())
    {
      CORBA::NamedValue_ptr *item;
      (void) i.next (item);

      CORBA::NamedValue_ptr nv = *item;

      // check if it is an in or inout parameter
      // @@ this is where we assume that the NVList is coming from
      //    a Server-side request, we could probably handle both
      //    cases with a flag, but there is no clear need for that.
      if (ACE_BIT_DISABLED (nv->flags (), flag))
        continue;

      if (TAO_debug_level > 3)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) : NVList::_tao_decode - %s\n",
                    nv->name ()?nv->name ():"(no name given)" ));

      CORBA::Any_ptr any = nv->value ();
      any->_tao_decode (incoming,
                        ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
CORBA_NVList::evaluate (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->refcount_lock_);
  if (this->incoming_ == 0)
    return;

  auto_ptr<TAO_InputCDR> incoming (this->incoming_);
  this->incoming_ = 0;

  this->_tao_decode (*(incoming.get ()),
                     this->incoming_flag_,
                     ACE_TRY_ENV);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node<CORBA::NamedValue_ptr>;
template class ACE_Unbounded_Queue<CORBA::NamedValue_ptr>;
template class ACE_Unbounded_Queue_Iterator<CORBA::NamedValue_ptr>;
template class ACE_Auto_Basic_Ptr<TAO_InputCDR>;
template class auto_ptr<TAO_InputCDR>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node<CORBA::NamedValue_ptr>
#pragma instantiate ACE_Unbounded_Queue<CORBA::NamedValue_ptr>
#pragma instantiate ACE_Unbounded_Queue_Iterator<CORBA::NamedValue_ptr>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_InputCDR>
#pragma instantiate auto_ptr<TAO_InputCDR>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* TAO_HAS_MINIMUM_CORBA */
