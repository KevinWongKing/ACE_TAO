// -*- C++ -*-
//
// $Id$

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
// and
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html


// TAO_IDL - Generated from
// be/be_visitor_interface/interface_ci.cpp:63

#if !defined (_CORBA_FIXEDDEF___CI_)
#define _CORBA_FIXEDDEF___CI_

ACE_INLINE
CORBA::FixedDef::FixedDef (
    TAO_Stub *objref,
    CORBA::Boolean _tao_collocated,
    TAO_Abstract_ServantBase *servant,
    TAO_ORB_Core *oc
  )
  : ACE_NESTED_CLASS (CORBA, Object) (objref, _tao_collocated, servant, oc),
    the_TAO_FixedDef_Proxy_Broker_ (0)
  
{
  this->CORBA_FixedDef_setup_collocation (_tao_collocated);
}

ACE_INLINE
CORBA::FixedDef::FixedDef (
    IOP::IOR *ior,
    TAO_ORB_Core *oc
  )
  : ACE_NESTED_CLASS (CORBA, Object) (ior, oc),
    the_TAO_FixedDef_Proxy_Broker_ (0)
  
{
}

#endif /* end #if !defined */

// TAO_IDL - Generated from
// be/be_visitor_interface/interface_ci.cpp:63

#if !defined (_CORBA_VALUEMEMBERDEF___CI_)
#define _CORBA_VALUEMEMBERDEF___CI_

ACE_INLINE
CORBA::ValueMemberDef::ValueMemberDef (
    TAO_Stub *objref,
    CORBA::Boolean _tao_collocated,
    TAO_Abstract_ServantBase *servant,
    TAO_ORB_Core *oc
  )
  : ACE_NESTED_CLASS (CORBA, Object) (objref, _tao_collocated, servant, oc),
    the_TAO_ValueMemberDef_Proxy_Broker_ (0)
  
{
  this->CORBA_ValueMemberDef_setup_collocation (_tao_collocated);
}

ACE_INLINE
CORBA::ValueMemberDef::ValueMemberDef (
    IOP::IOR *ior,
    TAO_ORB_Core *oc
  )
  : ACE_NESTED_CLASS (CORBA, Object) (ior, oc),
    the_TAO_ValueMemberDef_Proxy_Broker_ (0)
  
{
}

#endif /* end #if !defined */

// TAO_IDL - Generated from
// be/be_visitor_interface/interface_ci.cpp:63

#if !defined (_CORBA_VALUEDEF___CI_)
#define _CORBA_VALUEDEF___CI_

ACE_INLINE
CORBA::ValueDef::ValueDef (
    TAO_Stub *objref,
    CORBA::Boolean _tao_collocated,
    TAO_Abstract_ServantBase *servant,
    TAO_ORB_Core *oc
  )
  : ACE_NESTED_CLASS (CORBA, Object) (objref, _tao_collocated, servant, oc),
    the_TAO_ValueDef_Proxy_Broker_ (0)
  
{
  this->CORBA_ValueDef_setup_collocation (_tao_collocated);
}

ACE_INLINE
CORBA::ValueDef::ValueDef (
    IOP::IOR *ior,
    TAO_ORB_Core *oc
  )
  : ACE_NESTED_CLASS (CORBA, Object) (ior, oc),
    the_TAO_ValueDef_Proxy_Broker_ (0)
  
{
}

#endif /* end #if !defined */

// TAO_IDL - Generated from
// be/be_visitor_interface/interface_ci.cpp:63

#if !defined (_CORBA_EXTVALUEDEF___CI_)
#define _CORBA_EXTVALUEDEF___CI_

ACE_INLINE
CORBA::ExtValueDef::ExtValueDef (
    TAO_Stub *objref,
    CORBA::Boolean _tao_collocated,
    TAO_Abstract_ServantBase *servant,
    TAO_ORB_Core *oc
  )
  : ACE_NESTED_CLASS (CORBA, Object) (objref, _tao_collocated, servant, oc),
    the_TAO_ExtValueDef_Proxy_Broker_ (0)
  
{
  this->CORBA_ExtValueDef_setup_collocation (_tao_collocated);
}

ACE_INLINE
CORBA::ExtValueDef::ExtValueDef (
    IOP::IOR *ior,
    TAO_ORB_Core *oc
  )
  : ACE_NESTED_CLASS (CORBA, Object) (ior, oc),
    the_TAO_ExtValueDef_Proxy_Broker_ (0)
  
{
}

#endif /* end #if !defined */

// TAO_IDL - Generated from
// be/be_visitor_interface/interface_ci.cpp:63

#if !defined (_CORBA_VALUEBOXDEF___CI_)
#define _CORBA_VALUEBOXDEF___CI_

ACE_INLINE
CORBA::ValueBoxDef::ValueBoxDef (
    TAO_Stub *objref,
    CORBA::Boolean _tao_collocated,
    TAO_Abstract_ServantBase *servant,
    TAO_ORB_Core *oc
  )
  : ACE_NESTED_CLASS (CORBA, Object) (objref, _tao_collocated, servant, oc),
    the_TAO_ValueBoxDef_Proxy_Broker_ (0)
  
{
  this->CORBA_ValueBoxDef_setup_collocation (_tao_collocated);
}

ACE_INLINE
CORBA::ValueBoxDef::ValueBoxDef (
    IOP::IOR *ior,
    TAO_ORB_Core *oc
  )
  : ACE_NESTED_CLASS (CORBA, Object) (ior, oc),
    the_TAO_ValueBoxDef_Proxy_Broker_ (0)
  
{
}

#endif /* end #if !defined */

// TAO_IDL - Generated from
// be/be_visitor_interface/cdr_op_ci.cpp:72

TAO_IFR_Client_Export CORBA::Boolean operator<< (
    TAO_OutputCDR &,
    const CORBA::FixedDef_ptr
  );

TAO_IFR_Client_Export CORBA::Boolean operator>> (
    TAO_InputCDR &,
    CORBA::FixedDef_ptr &
  );

// TAO_IDL - Generated from
// be/be_visitor_structure/cdr_op_ci.cpp:70

ACE_INLINE
CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const CORBA::ValueMember &_tao_aggregate
  )
{
  return
    (strm << _tao_aggregate.name.in ()) &&
    (strm << _tao_aggregate.id.in ()) &&
    (strm << _tao_aggregate.defined_in.in ()) &&
    (strm << _tao_aggregate.version.in ()) &&
    (strm << _tao_aggregate.type.in ()) &&
    CORBA::Object::marshal (
        _tao_aggregate.type_def.in (),
        strm
      ) &&
    (strm << _tao_aggregate.access);
}

ACE_INLINE
CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    CORBA::ValueMember &_tao_aggregate
  )
{
  return
    (strm >> _tao_aggregate.name.out ()) &&
    (strm >> _tao_aggregate.id.out ()) &&
    (strm >> _tao_aggregate.defined_in.out ()) &&
    (strm >> _tao_aggregate.version.out ()) &&
    (strm >> _tao_aggregate.type.out ()) &&
    (strm >> _tao_aggregate.type_def.out ()) &&
    (strm >> _tao_aggregate.access);
}

// TAO_IDL - Generated from
// be/be_visitor_sequence/cdr_op_ci.cpp:81

#if !defined _TAO_CDR_OP_CORBA_ValueMemberSeq_I_
#define _TAO_CDR_OP_CORBA_ValueMemberSeq_I_

CORBA::Boolean TAO_IFR_Client_Export operator<< (
    TAO_OutputCDR &,
    const CORBA::ValueMemberSeq &
  );

CORBA::Boolean TAO_IFR_Client_Export operator>> (
    TAO_InputCDR &,
    CORBA::ValueMemberSeq &
  );

#endif /* _TAO_CDR_OP_CORBA_ValueMemberSeq_I_ */

// TAO_IDL - Generated from
// be/be_visitor_interface/cdr_op_ci.cpp:72

TAO_IFR_Client_Export CORBA::Boolean operator<< (
    TAO_OutputCDR &,
    const CORBA::ValueMemberDef_ptr
  );

TAO_IFR_Client_Export CORBA::Boolean operator>> (
    TAO_InputCDR &,
    CORBA::ValueMemberDef_ptr &
  );

// TAO_IDL - Generated from
// be/be_visitor_interface/cdr_op_ci.cpp:72

TAO_IFR_Client_Export CORBA::Boolean operator<< (
    TAO_OutputCDR &,
    const CORBA::ValueDef_ptr
  );

TAO_IFR_Client_Export CORBA::Boolean operator>> (
    TAO_InputCDR &,
    CORBA::ValueDef_ptr &
  );

// TAO_IDL - Generated from
// be/be_visitor_structure/cdr_op_ci.cpp:70

ACE_INLINE
CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const CORBA::ValueDef::FullValueDescription &_tao_aggregate
  )
{
  return
    (strm << _tao_aggregate.name.in ()) &&
    (strm << _tao_aggregate.id.in ()) &&
    (strm << CORBA::Any::from_boolean (_tao_aggregate.is_abstract)) &&
    (strm << CORBA::Any::from_boolean (_tao_aggregate.is_custom)) &&
    (strm << _tao_aggregate.defined_in.in ()) &&
    (strm << _tao_aggregate.version.in ()) &&
    (strm << _tao_aggregate.operations) &&
    (strm << _tao_aggregate.attributes) &&
    (strm << _tao_aggregate.members) &&
    (strm << _tao_aggregate.initializers) &&
    (strm << _tao_aggregate.supported_interfaces) &&
    (strm << _tao_aggregate.abstract_base_values) &&
    (strm << CORBA::Any::from_boolean (_tao_aggregate.is_truncatable)) &&
    (strm << _tao_aggregate.base_value.in ()) &&
    (strm << _tao_aggregate.type.in ());
}

ACE_INLINE
CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    CORBA::ValueDef::FullValueDescription &_tao_aggregate
  )
{
  return
    (strm >> _tao_aggregate.name.out ()) &&
    (strm >> _tao_aggregate.id.out ()) &&
    (strm >> CORBA::Any::to_boolean (_tao_aggregate.is_abstract)) &&
    (strm >> CORBA::Any::to_boolean (_tao_aggregate.is_custom)) &&
    (strm >> _tao_aggregate.defined_in.out ()) &&
    (strm >> _tao_aggregate.version.out ()) &&
    (strm >> _tao_aggregate.operations) &&
    (strm >> _tao_aggregate.attributes) &&
    (strm >> _tao_aggregate.members) &&
    (strm >> _tao_aggregate.initializers) &&
    (strm >> _tao_aggregate.supported_interfaces) &&
    (strm >> _tao_aggregate.abstract_base_values) &&
    (strm >> CORBA::Any::to_boolean (_tao_aggregate.is_truncatable)) &&
    (strm >> _tao_aggregate.base_value.out ()) &&
    (strm >> _tao_aggregate.type.out ());
}

// TAO_IDL - Generated from
// be/be_visitor_structure/cdr_op_ci.cpp:70

ACE_INLINE
CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const CORBA::ValueDescription &_tao_aggregate
  )
{
  return
    (strm << _tao_aggregate.name.in ()) &&
    (strm << _tao_aggregate.id.in ()) &&
    (strm << CORBA::Any::from_boolean (_tao_aggregate.is_abstract)) &&
    (strm << CORBA::Any::from_boolean (_tao_aggregate.is_custom)) &&
    (strm << _tao_aggregate.defined_in.in ()) &&
    (strm << _tao_aggregate.version.in ()) &&
    (strm << _tao_aggregate.supported_interfaces) &&
    (strm << _tao_aggregate.abstract_base_values) &&
    (strm << CORBA::Any::from_boolean (_tao_aggregate.is_truncatable)) &&
    (strm << _tao_aggregate.base_value.in ());
}

ACE_INLINE
CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    CORBA::ValueDescription &_tao_aggregate
  )
{
  return
    (strm >> _tao_aggregate.name.out ()) &&
    (strm >> _tao_aggregate.id.out ()) &&
    (strm >> CORBA::Any::to_boolean (_tao_aggregate.is_abstract)) &&
    (strm >> CORBA::Any::to_boolean (_tao_aggregate.is_custom)) &&
    (strm >> _tao_aggregate.defined_in.out ()) &&
    (strm >> _tao_aggregate.version.out ()) &&
    (strm >> _tao_aggregate.supported_interfaces) &&
    (strm >> _tao_aggregate.abstract_base_values) &&
    (strm >> CORBA::Any::to_boolean (_tao_aggregate.is_truncatable)) &&
    (strm >> _tao_aggregate.base_value.out ());
}

// TAO_IDL - Generated from
// be/be_visitor_interface/cdr_op_ci.cpp:72

TAO_IFR_Client_Export CORBA::Boolean operator<< (
    TAO_OutputCDR &,
    const CORBA::ExtValueDef_ptr
  );

TAO_IFR_Client_Export CORBA::Boolean operator>> (
    TAO_InputCDR &,
    CORBA::ExtValueDef_ptr &
  );

// TAO_IDL - Generated from
// be/be_visitor_structure/cdr_op_ci.cpp:70

ACE_INLINE
CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const CORBA::ExtValueDef::ExtFullValueDescription &_tao_aggregate
  )
{
  return
    (strm << _tao_aggregate.name.in ()) &&
    (strm << _tao_aggregate.id.in ()) &&
    (strm << CORBA::Any::from_boolean (_tao_aggregate.is_abstract)) &&
    (strm << CORBA::Any::from_boolean (_tao_aggregate.is_custom)) &&
    (strm << _tao_aggregate.defined_in.in ()) &&
    (strm << _tao_aggregate.version.in ()) &&
    (strm << _tao_aggregate.operations) &&
    (strm << _tao_aggregate.attributes) &&
    (strm << _tao_aggregate.members) &&
    (strm << _tao_aggregate.initializers) &&
    (strm << _tao_aggregate.supported_interfaces) &&
    (strm << _tao_aggregate.abstract_base_values) &&
    (strm << CORBA::Any::from_boolean (_tao_aggregate.is_truncatable)) &&
    (strm << _tao_aggregate.base_value.in ()) &&
    (strm << _tao_aggregate.type.in ());
}

ACE_INLINE
CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    CORBA::ExtValueDef::ExtFullValueDescription &_tao_aggregate
  )
{
  return
    (strm >> _tao_aggregate.name.out ()) &&
    (strm >> _tao_aggregate.id.out ()) &&
    (strm >> CORBA::Any::to_boolean (_tao_aggregate.is_abstract)) &&
    (strm >> CORBA::Any::to_boolean (_tao_aggregate.is_custom)) &&
    (strm >> _tao_aggregate.defined_in.out ()) &&
    (strm >> _tao_aggregate.version.out ()) &&
    (strm >> _tao_aggregate.operations) &&
    (strm >> _tao_aggregate.attributes) &&
    (strm >> _tao_aggregate.members) &&
    (strm >> _tao_aggregate.initializers) &&
    (strm >> _tao_aggregate.supported_interfaces) &&
    (strm >> _tao_aggregate.abstract_base_values) &&
    (strm >> CORBA::Any::to_boolean (_tao_aggregate.is_truncatable)) &&
    (strm >> _tao_aggregate.base_value.out ()) &&
    (strm >> _tao_aggregate.type.out ());
}

// TAO_IDL - Generated from
// be/be_visitor_interface/cdr_op_ci.cpp:72

TAO_IFR_Client_Export CORBA::Boolean operator<< (
    TAO_OutputCDR &,
    const CORBA::ValueBoxDef_ptr
  );

TAO_IFR_Client_Export CORBA::Boolean operator>> (
    TAO_InputCDR &,
    CORBA::ValueBoxDef_ptr &
  );

