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
// be/be_codegen.cpp:151

#ifndef _TAO_IDL_IFR_EXTENDEDC_H_
#define _TAO_IDL_IFR_EXTENDEDC_H_

#include /**/ "ace/pre.h"

#include "tao/corba.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "ifr_client_export.h"
#include "tao/Sequence_T.h"
#include "tao/Objref_VarOut_T.h"
#include "tao/Seq_Var_T.h"
#include "tao/Seq_Out_T.h"
#include "tao/VarOut_T.h"

#include "IFR_BasicC.h"

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_IFR_Client_Export

#if defined (TAO_EXPORT_NESTED_CLASSES)
#  if defined (TAO_EXPORT_NESTED_MACRO)
#    undef TAO_EXPORT_NESTED_MACRO
#  endif /* defined (TAO_EXPORT_NESTED_MACRO) */
#  define TAO_EXPORT_NESTED_MACRO TAO_IFR_Client_Export
#endif /* TAO_EXPORT_NESTED_CLASSES */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#if defined (__BORLANDC__)
#pragma option push -w-rvl -w-rch -w-ccc -w-inl
#endif /* __BORLANDC__ */

// TAO_IDL - Generated from 
// be/be_visitor_root/root_ch.cpp:63

namespace TAO
{
  class Collocation_Proxy_Broker;
}

// TAO_IDL - Generated from
// be/be_visitor_module/module_ch.cpp:48

namespace CORBA
{
  
  // TAO_IDL - Generated from
  // be/be_interface.cpp:601

#if !defined (_CORBA_FIXEDDEF__VAR_OUT_CH_)
#define _CORBA_FIXEDDEF__VAR_OUT_CH_
  
  class FixedDef;
  typedef FixedDef *FixedDef_ptr;
  
  typedef
    TAO_Objref_Var_T<
        FixedDef
      >
    FixedDef_var;
  
  typedef
    TAO_Objref_Out_T<
        FixedDef
      >
    FixedDef_out;

#endif /* end #if !defined */
  
  // TAO_IDL - Generated from
  // be/be_visitor_interface/interface_ch.cpp:53

#if !defined (_CORBA_FIXEDDEF_CH_)
#define _CORBA_FIXEDDEF_CH_
  
  class TAO_IFR_Client_Export FixedDef
    : public virtual CORBA::IDLType
  {
  public:
    friend class TAO::Narrow_Utils<FixedDef>;
    typedef FixedDef_ptr _ptr_type;
    typedef FixedDef_var _var_type;
    
    // The static operations.
    static FixedDef_ptr _duplicate (FixedDef_ptr obj);
    
    static FixedDef_ptr _narrow (
        CORBA::Object_ptr obj
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );
    
    static FixedDef_ptr _nil (void)
      {
        return (FixedDef_ptr)0;
      }
    
    static void _tao_any_destructor (void *);
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual CORBA::UShort digits (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void digits (
        CORBA::UShort digits
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual CORBA::Short scale (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void scale (
        CORBA::Short scale
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_interface/interface_ch.cpp:185
    
    virtual CORBA::Boolean _is_a (
        const char *type_id
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );
    
    virtual const char* _interface_repository_id (void) const;
    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
  private:
    TAO::Collocation_Proxy_Broker *the_TAO_FixedDef_Proxy_Broker_;
  
  protected:
    FixedDef (int collocated = 0);
    
    // These methods travese the inheritance tree and set the
    // parents piece of the given class in the right mode.
    virtual void CORBA_FixedDef_setup_collocation (int collocated);
    
    FixedDef (IOP::IOR *ior,
      TAO_ORB_Core *orb_core = 0);
      
    FixedDef (
        TAO_Stub *objref, 
        CORBA::Boolean _tao_collocated = 0,
        TAO_Abstract_ServantBase *servant = 0,
        TAO_ORB_Core *orb_core = 0
      );
    virtual ~FixedDef (void);
  
  private:
    FixedDef (const FixedDef &);
    void operator= (const FixedDef &);
  };

#endif /* end #if !defined */
  
  // TAO_IDL - Generated from
  // be/be_visitor_typecode/typecode_decl.cpp:44
  
  TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr _tc_FixedDef;
  
  // TAO_IDL - Generated from
  // be/be_type.cpp:258
  
  struct ValueMember;
  
  typedef
    TAO_Var_Var_T<
        ValueMember
      >
    ValueMember_var;
  
  typedef
    TAO_Out_T<
        ValueMember,
        ValueMember_var
      >
    ValueMember_out;
  
  // TAO_IDL - Generated from
  // be/be_visitor_structure/structure_ch.cpp:52
  
  struct TAO_IFR_Client_Export ValueMember
  {
    typedef ValueMember_var _var_type;
    
    static void _tao_any_destructor (void *);
    TAO_String_Manager name;
    TAO_String_Manager id;
    TAO_String_Manager defined_in;
    TAO_String_Manager version;
    CORBA::TypeCode_var type;
    CORBA::IDLType_var type_def;
    CORBA::Visibility access;
  };
  
  // TAO_IDL - Generated from
  // be/be_visitor_typecode/typecode_decl.cpp:44
  
  TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr _tc_ValueMember;
  
  // TAO_IDL - Generated from
  // be/be_visitor_sequence/sequence_ch.cpp:101

#if !defined (_CORBA_VALUEMEMBERSEQ_CH_)
#define _CORBA_VALUEMEMBERSEQ_CH_
  
  class ValueMemberSeq;
  
  typedef
    TAO_VarSeq_Var_T<
        ValueMemberSeq,
        ValueMember
      >
    ValueMemberSeq_var;
  
  typedef
    TAO_Seq_Out_T<
        ValueMemberSeq,
        ValueMemberSeq_var,
        ValueMember
      >
    ValueMemberSeq_out;
  
  class TAO_IFR_Client_Export ValueMemberSeq
    : public
        TAO_Unbounded_Sequence<
            ValueMember
          >
  {
  public:
    ValueMemberSeq (void);
    ValueMemberSeq (CORBA::ULong max);
    ValueMemberSeq (
        CORBA::ULong max,
        CORBA::ULong length,
        ValueMember* buffer, 
        CORBA::Boolean release = 0
      );
    ValueMemberSeq (const ValueMemberSeq &);
    ~ValueMemberSeq (void);
    
    static void _tao_any_destructor (void *);
    
    typedef ValueMemberSeq_var _var_type;
  };

#endif /* end #if !defined */
  
  // TAO_IDL - Generated from
  // be/be_visitor_typecode/typecode_decl.cpp:44
  
  TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr _tc_ValueMemberSeq;
  
  // TAO_IDL - Generated from
  // be/be_interface.cpp:601

#if !defined (_CORBA_VALUEMEMBERDEF__VAR_OUT_CH_)
#define _CORBA_VALUEMEMBERDEF__VAR_OUT_CH_
  
  class ValueMemberDef;
  typedef ValueMemberDef *ValueMemberDef_ptr;
  
  typedef
    TAO_Objref_Var_T<
        ValueMemberDef
      >
    ValueMemberDef_var;
  
  typedef
    TAO_Objref_Out_T<
        ValueMemberDef
      >
    ValueMemberDef_out;

#endif /* end #if !defined */
  
  // TAO_IDL - Generated from
  // be/be_visitor_interface/interface_ch.cpp:53

#if !defined (_CORBA_VALUEMEMBERDEF_CH_)
#define _CORBA_VALUEMEMBERDEF_CH_
  
  class TAO_IFR_Client_Export ValueMemberDef
    : public virtual CORBA::Contained
  {
  public:
    friend class TAO::Narrow_Utils<ValueMemberDef>;
    typedef ValueMemberDef_ptr _ptr_type;
    typedef ValueMemberDef_var _var_type;
    
    // The static operations.
    static ValueMemberDef_ptr _duplicate (ValueMemberDef_ptr obj);
    
    static ValueMemberDef_ptr _narrow (
        CORBA::Object_ptr obj
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );
    
    static ValueMemberDef_ptr _nil (void)
      {
        return (ValueMemberDef_ptr)0;
      }
    
    static void _tao_any_destructor (void *);
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual CORBA::TypeCode_ptr type (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual ::CORBA::IDLType_ptr type_def (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void type_def (
        CORBA::IDLType_ptr type_def
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual CORBA::Visibility access (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void access (
        CORBA::Visibility access
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_interface/interface_ch.cpp:185
    
    virtual CORBA::Boolean _is_a (
        const char *type_id
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );
    
    virtual const char* _interface_repository_id (void) const;
    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
  private:
    TAO::Collocation_Proxy_Broker *the_TAO_ValueMemberDef_Proxy_Broker_;
  
  protected:
    ValueMemberDef (int collocated = 0);
    
    // These methods travese the inheritance tree and set the
    // parents piece of the given class in the right mode.
    virtual void CORBA_ValueMemberDef_setup_collocation (int collocated);
    
    ValueMemberDef (IOP::IOR *ior,
      TAO_ORB_Core *orb_core = 0);
      
    ValueMemberDef (
        TAO_Stub *objref, 
        CORBA::Boolean _tao_collocated = 0,
        TAO_Abstract_ServantBase *servant = 0,
        TAO_ORB_Core *orb_core = 0
      );
    virtual ~ValueMemberDef (void);
  
  private:
    ValueMemberDef (const ValueMemberDef &);
    void operator= (const ValueMemberDef &);
  };

#endif /* end #if !defined */
  
  // TAO_IDL - Generated from
  // be/be_visitor_typecode/typecode_decl.cpp:44
  
  TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr _tc_ValueMemberDef;
  
  // TAO_IDL - Generated from
  // be/be_interface.cpp:601

#if !defined (_CORBA_VALUEDEF__VAR_OUT_CH_)
#define _CORBA_VALUEDEF__VAR_OUT_CH_
  
  class ValueDef;
  typedef ValueDef *ValueDef_ptr;
  
  typedef
    TAO_Objref_Var_T<
        ValueDef
      >
    ValueDef_var;
  
  typedef
    TAO_Objref_Out_T<
        ValueDef
      >
    ValueDef_out;

#endif /* end #if !defined */
  
  // TAO_IDL - Generated from
  // be/be_visitor_interface/interface_ch.cpp:53

#if !defined (_CORBA_VALUEDEF_CH_)
#define _CORBA_VALUEDEF_CH_
  
  class TAO_IFR_Client_Export ValueDef
    : public virtual CORBA::Container,
      public virtual CORBA::Contained,
      public virtual CORBA::IDLType
  {
  public:
    friend class TAO::Narrow_Utils<ValueDef>;
    typedef ValueDef_ptr _ptr_type;
    typedef ValueDef_var _var_type;
    
    // The static operations.
    static ValueDef_ptr _duplicate (ValueDef_ptr obj);
    
    static ValueDef_ptr _narrow (
        CORBA::Object_ptr obj
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );
    
    static ValueDef_ptr _nil (void)
      {
        return (ValueDef_ptr)0;
      }
    
    static void _tao_any_destructor (void *);
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual ::CORBA::InterfaceDefSeq * supported_interfaces (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void supported_interfaces (
        const CORBA::InterfaceDefSeq & supported_interfaces
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual ::CORBA::InitializerSeq * initializers (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void initializers (
        const CORBA::InitializerSeq & initializers
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual ::CORBA::ValueDef_ptr base_value (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void base_value (
        CORBA::ValueDef_ptr base_value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual ::CORBA::ValueDefSeq * abstract_base_values (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void abstract_base_values (
        const CORBA::ValueDefSeq & abstract_base_values
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual CORBA::Boolean is_abstract (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void is_abstract (
        CORBA::Boolean is_abstract
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual CORBA::Boolean is_custom (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void is_custom (
        CORBA::Boolean is_custom
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual CORBA::Boolean is_truncatable (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void is_truncatable (
        CORBA::Boolean is_truncatable
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual CORBA::Boolean is_a (
        const char * id
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_type.cpp:258
    
    struct FullValueDescription;
    
    typedef
      TAO_Var_Var_T<
          FullValueDescription
        >
      FullValueDescription_var;
    
    typedef
      TAO_Out_T<
          FullValueDescription,
          FullValueDescription_var
        >
      FullValueDescription_out;
    
    // TAO_IDL - Generated from
    // be/be_visitor_structure/structure_ch.cpp:52
    
    struct TAO_IFR_Client_Export FullValueDescription
    {
      typedef FullValueDescription_var _var_type;
      
      static void _tao_any_destructor (void *);
      TAO_String_Manager name;
      TAO_String_Manager id;
      CORBA::Boolean is_abstract;
      CORBA::Boolean is_custom;
      TAO_String_Manager defined_in;
      TAO_String_Manager version;
      ACE_NESTED_CLASS (CORBA, OpDescriptionSeq) operations;
      ACE_NESTED_CLASS (CORBA, AttrDescriptionSeq) attributes;
      ACE_NESTED_CLASS (CORBA, ValueMemberSeq) members;
      ACE_NESTED_CLASS (CORBA, InitializerSeq) initializers;
      ACE_NESTED_CLASS (CORBA, RepositoryIdSeq) supported_interfaces;
      ACE_NESTED_CLASS (CORBA, RepositoryIdSeq) abstract_base_values;
      CORBA::Boolean is_truncatable;
      TAO_String_Manager base_value;
      CORBA::TypeCode_var type;
    };
    
    // TAO_IDL - Generated from
    // be/be_visitor_typecode/typecode_decl.cpp:44
    
    static ::CORBA::TypeCode_ptr _tc_FullValueDescription;
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual ::CORBA::ValueDef::FullValueDescription * describe_value (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual ::CORBA::ValueMemberDef_ptr create_value_member (
        const char * id,
        const char * name,
        const char * version,
        CORBA::IDLType_ptr type,
        CORBA::Visibility access
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual ::CORBA::AttributeDef_ptr create_attribute (
        const char * id,
        const char * name,
        const char * version,
        CORBA::IDLType_ptr type,
        CORBA::AttributeMode mode
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual ::CORBA::OperationDef_ptr create_operation (
        const char * id,
        const char * name,
        const char * version,
        CORBA::IDLType_ptr result,
        CORBA::OperationMode mode,
        const CORBA::ParDescriptionSeq & params,
        const CORBA::ExceptionDefSeq & exceptions,
        const CORBA::ContextIdSeq & contexts
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_interface/interface_ch.cpp:185
    
    virtual CORBA::Boolean _is_a (
        const char *type_id
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );
    
    virtual const char* _interface_repository_id (void) const;
    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
  private:
    TAO::Collocation_Proxy_Broker *the_TAO_ValueDef_Proxy_Broker_;
  
  protected:
    ValueDef (int collocated = 0);
    
    // These methods travese the inheritance tree and set the
    // parents piece of the given class in the right mode.
    virtual void CORBA_ValueDef_setup_collocation (int collocated);
    
    ValueDef (IOP::IOR *ior,
      TAO_ORB_Core *orb_core = 0);
      
    ValueDef (
        TAO_Stub *objref, 
        CORBA::Boolean _tao_collocated = 0,
        TAO_Abstract_ServantBase *servant = 0,
        TAO_ORB_Core *orb_core = 0
      );
    virtual ~ValueDef (void);
  
  private:
    ValueDef (const ValueDef &);
    void operator= (const ValueDef &);
  };

#endif /* end #if !defined */
  
  // TAO_IDL - Generated from
  // be/be_visitor_typecode/typecode_decl.cpp:44
  
  TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr _tc_ValueDef;
  
  // TAO_IDL - Generated from
  // be/be_type.cpp:258
  
  struct ValueDescription;
  
  typedef
    TAO_Var_Var_T<
        ValueDescription
      >
    ValueDescription_var;
  
  typedef
    TAO_Out_T<
        ValueDescription,
        ValueDescription_var
      >
    ValueDescription_out;
  
  // TAO_IDL - Generated from
  // be/be_visitor_structure/structure_ch.cpp:52
  
  struct TAO_IFR_Client_Export ValueDescription
  {
    typedef ValueDescription_var _var_type;
    
    static void _tao_any_destructor (void *);
    TAO_String_Manager name;
    TAO_String_Manager id;
    CORBA::Boolean is_abstract;
    CORBA::Boolean is_custom;
    TAO_String_Manager defined_in;
    TAO_String_Manager version;
    CORBA::RepositoryIdSeq supported_interfaces;
    CORBA::RepositoryIdSeq abstract_base_values;
    CORBA::Boolean is_truncatable;
    TAO_String_Manager base_value;
  };
  
  // TAO_IDL - Generated from
  // be/be_visitor_typecode/typecode_decl.cpp:44
  
  TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr _tc_ValueDescription;
  
  // TAO_IDL - Generated from
  // be/be_interface.cpp:601

#if !defined (_CORBA_EXTVALUEDEF__VAR_OUT_CH_)
#define _CORBA_EXTVALUEDEF__VAR_OUT_CH_
  
  class ExtValueDef;
  typedef ExtValueDef *ExtValueDef_ptr;
  
  typedef
    TAO_Objref_Var_T<
        ExtValueDef
      >
    ExtValueDef_var;
  
  typedef
    TAO_Objref_Out_T<
        ExtValueDef
      >
    ExtValueDef_out;

#endif /* end #if !defined */
  
  // TAO_IDL - Generated from
  // be/be_visitor_interface/interface_ch.cpp:53

#if !defined (_CORBA_EXTVALUEDEF_CH_)
#define _CORBA_EXTVALUEDEF_CH_
  
  class TAO_IFR_Client_Export ExtValueDef
    : public virtual CORBA::ValueDef
  {
  public:
    friend class TAO::Narrow_Utils<ExtValueDef>;
    typedef ExtValueDef_ptr _ptr_type;
    typedef ExtValueDef_var _var_type;
    
    // The static operations.
    static ExtValueDef_ptr _duplicate (ExtValueDef_ptr obj);
    
    static ExtValueDef_ptr _narrow (
        CORBA::Object_ptr obj
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );
    
    static ExtValueDef_ptr _nil (void)
      {
        return (ExtValueDef_ptr)0;
      }
    
    static void _tao_any_destructor (void *);
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual ::CORBA::ExtInitializerSeq * ext_initializers (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void ext_initializers (
        const CORBA::ExtInitializerSeq & ext_initializers
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_type.cpp:258
    
    struct ExtFullValueDescription;
    
    typedef
      TAO_Var_Var_T<
          ExtFullValueDescription
        >
      ExtFullValueDescription_var;
    
    typedef
      TAO_Out_T<
          ExtFullValueDescription,
          ExtFullValueDescription_var
        >
      ExtFullValueDescription_out;
    
    // TAO_IDL - Generated from
    // be/be_visitor_structure/structure_ch.cpp:52
    
    struct TAO_IFR_Client_Export ExtFullValueDescription
    {
      typedef ExtFullValueDescription_var _var_type;
      
      static void _tao_any_destructor (void *);
      TAO_String_Manager name;
      TAO_String_Manager id;
      CORBA::Boolean is_abstract;
      CORBA::Boolean is_custom;
      TAO_String_Manager defined_in;
      TAO_String_Manager version;
      ACE_NESTED_CLASS (CORBA, OpDescriptionSeq) operations;
      ACE_NESTED_CLASS (CORBA, ExtAttrDescriptionSeq) attributes;
      ACE_NESTED_CLASS (CORBA, ValueMemberSeq) members;
      ACE_NESTED_CLASS (CORBA, ExtInitializerSeq) initializers;
      ACE_NESTED_CLASS (CORBA, RepositoryIdSeq) supported_interfaces;
      ACE_NESTED_CLASS (CORBA, RepositoryIdSeq) abstract_base_values;
      CORBA::Boolean is_truncatable;
      TAO_String_Manager base_value;
      CORBA::TypeCode_var type;
    };
    
    // TAO_IDL - Generated from
    // be/be_visitor_typecode/typecode_decl.cpp:44
    
    static ::CORBA::TypeCode_ptr _tc_ExtFullValueDescription;
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual ::CORBA::ExtValueDef::ExtFullValueDescription * describe_ext_value (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual ::CORBA::ExtAttributeDef_ptr create_ext_attribute (
        const char * id,
        const char * name,
        const char * version,
        CORBA::IDLType_ptr type,
        CORBA::AttributeMode mode,
        const CORBA::ExceptionDefSeq & get_exceptions,
        const CORBA::ExceptionDefSeq & set_exceptions
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_interface/interface_ch.cpp:185
    
    virtual CORBA::Boolean _is_a (
        const char *type_id
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );
    
    virtual const char* _interface_repository_id (void) const;
    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
  private:
    TAO::Collocation_Proxy_Broker *the_TAO_ExtValueDef_Proxy_Broker_;
  
  protected:
    ExtValueDef (int collocated = 0);
    
    // These methods travese the inheritance tree and set the
    // parents piece of the given class in the right mode.
    virtual void CORBA_ExtValueDef_setup_collocation (int collocated);
    
    ExtValueDef (IOP::IOR *ior,
      TAO_ORB_Core *orb_core = 0);
      
    ExtValueDef (
        TAO_Stub *objref, 
        CORBA::Boolean _tao_collocated = 0,
        TAO_Abstract_ServantBase *servant = 0,
        TAO_ORB_Core *orb_core = 0
      );
    virtual ~ExtValueDef (void);
  
  private:
    ExtValueDef (const ExtValueDef &);
    void operator= (const ExtValueDef &);
  };

#endif /* end #if !defined */
  
  // TAO_IDL - Generated from
  // be/be_visitor_typecode/typecode_decl.cpp:44
  
  TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr _tc_ExtValueDef;
  
  // TAO_IDL - Generated from
  // be/be_interface.cpp:601

#if !defined (_CORBA_VALUEBOXDEF__VAR_OUT_CH_)
#define _CORBA_VALUEBOXDEF__VAR_OUT_CH_
  
  class ValueBoxDef;
  typedef ValueBoxDef *ValueBoxDef_ptr;
  
  typedef
    TAO_Objref_Var_T<
        ValueBoxDef
      >
    ValueBoxDef_var;
  
  typedef
    TAO_Objref_Out_T<
        ValueBoxDef
      >
    ValueBoxDef_out;

#endif /* end #if !defined */
  
  // TAO_IDL - Generated from
  // be/be_visitor_interface/interface_ch.cpp:53

#if !defined (_CORBA_VALUEBOXDEF_CH_)
#define _CORBA_VALUEBOXDEF_CH_
  
  class TAO_IFR_Client_Export ValueBoxDef
    : public virtual CORBA::TypedefDef
  {
  public:
    friend class TAO::Narrow_Utils<ValueBoxDef>;
    typedef ValueBoxDef_ptr _ptr_type;
    typedef ValueBoxDef_var _var_type;
    
    // The static operations.
    static ValueBoxDef_ptr _duplicate (ValueBoxDef_ptr obj);
    
    static ValueBoxDef_ptr _narrow (
        CORBA::Object_ptr obj
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );
    
    static ValueBoxDef_ptr _nil (void)
      {
        return (ValueBoxDef_ptr)0;
      }
    
    static void _tao_any_destructor (void *);
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual ::CORBA::IDLType_ptr original_type_def (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void original_type_def (
        CORBA::IDLType_ptr original_type_def
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_interface/interface_ch.cpp:185
    
    virtual CORBA::Boolean _is_a (
        const char *type_id
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );
    
    virtual const char* _interface_repository_id (void) const;
    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
  private:
    TAO::Collocation_Proxy_Broker *the_TAO_ValueBoxDef_Proxy_Broker_;
  
  protected:
    ValueBoxDef (int collocated = 0);
    
    // These methods travese the inheritance tree and set the
    // parents piece of the given class in the right mode.
    virtual void CORBA_ValueBoxDef_setup_collocation (int collocated);
    
    ValueBoxDef (IOP::IOR *ior,
      TAO_ORB_Core *orb_core = 0);
      
    ValueBoxDef (
        TAO_Stub *objref, 
        CORBA::Boolean _tao_collocated = 0,
        TAO_Abstract_ServantBase *servant = 0,
        TAO_ORB_Core *orb_core = 0
      );
    virtual ~ValueBoxDef (void);
  
  private:
    ValueBoxDef (const ValueBoxDef &);
    void operator= (const ValueBoxDef &);
  };

#endif /* end #if !defined */
  
  // TAO_IDL - Generated from
  // be/be_visitor_typecode/typecode_decl.cpp:44
  
  TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr _tc_ValueBoxDef;

// TAO_IDL - Generated from
// be/be_visitor_module/module_ch.cpp:66

} // module CORBA

// Proxy Broker Factory function pointer declarations.

// TAO_IDL - Generated from
// be/be_visitor_root/root.cpp:121

extern TAO_IFR_Client_Export
TAO::Collocation_Proxy_Broker *
(*CORBA__TAO_FixedDef_Proxy_Broker_Factory_function_pointer) (
    CORBA::Object_ptr obj
  );

extern TAO_IFR_Client_Export
TAO::Collocation_Proxy_Broker *
(*CORBA__TAO_ValueMemberDef_Proxy_Broker_Factory_function_pointer) (
    CORBA::Object_ptr obj
  );

extern TAO_IFR_Client_Export
TAO::Collocation_Proxy_Broker *
(*CORBA__TAO_ValueDef_Proxy_Broker_Factory_function_pointer) (
    CORBA::Object_ptr obj
  );

extern TAO_IFR_Client_Export
TAO::Collocation_Proxy_Broker *
(*CORBA__TAO_ExtValueDef_Proxy_Broker_Factory_function_pointer) (
    CORBA::Object_ptr obj
  );

extern TAO_IFR_Client_Export
TAO::Collocation_Proxy_Broker *
(*CORBA__TAO_ValueBoxDef_Proxy_Broker_Factory_function_pointer) (
    CORBA::Object_ptr obj
  );

// TAO_IDL - Generated from
// be/be_visitor_traits.cpp:48

// Traits specializations.
namespace TAO
{
};

// TAO_IDL - Generated from
// be/be_visitor_interface/any_op_ch.cpp:52

TAO_IFR_Client_Export void operator<<= (CORBA::Any &, CORBA::FixedDef_ptr); // copying
TAO_IFR_Client_Export void operator<<= (CORBA::Any &, CORBA::FixedDef_ptr *); // non-copying
TAO_IFR_Client_Export CORBA::Boolean operator>>= (const CORBA::Any &, CORBA::FixedDef_ptr &);

// TAO_IDL - Generated from
// be/be_visitor_structure/any_op_ch.cpp:52

TAO_IFR_Client_Export void operator<<= (CORBA::Any &, const CORBA::ValueMember &); // copying version
TAO_IFR_Client_Export void operator<<= (CORBA::Any &, CORBA::ValueMember*); // noncopying version
TAO_IFR_Client_Export CORBA::Boolean operator>>= (const CORBA::Any &, CORBA::ValueMember *&); // deprecated
TAO_IFR_Client_Export CORBA::Boolean operator>>= (const CORBA::Any &, const CORBA::ValueMember *&);

// TAO_IDL - Generated from
// be/be_visitor_sequence/any_op_ch.cpp:52

TAO_IFR_Client_Export void operator<<= (CORBA::Any &, const CORBA::ValueMemberSeq &); // copying version
TAO_IFR_Client_Export void operator<<= (CORBA::Any &, CORBA::ValueMemberSeq*); // noncopying version
TAO_IFR_Client_Export CORBA::Boolean operator>>= (const CORBA::Any &, CORBA::ValueMemberSeq *&); // deprecated
TAO_IFR_Client_Export CORBA::Boolean operator>>= (const CORBA::Any &, const CORBA::ValueMemberSeq *&);

// TAO_IDL - Generated from
// be/be_visitor_interface/any_op_ch.cpp:52

TAO_IFR_Client_Export void operator<<= (CORBA::Any &, CORBA::ValueMemberDef_ptr); // copying
TAO_IFR_Client_Export void operator<<= (CORBA::Any &, CORBA::ValueMemberDef_ptr *); // non-copying
TAO_IFR_Client_Export CORBA::Boolean operator>>= (const CORBA::Any &, CORBA::ValueMemberDef_ptr &);

// TAO_IDL - Generated from
// be/be_visitor_interface/any_op_ch.cpp:52

TAO_IFR_Client_Export void operator<<= (CORBA::Any &, CORBA::ValueDef_ptr); // copying
TAO_IFR_Client_Export void operator<<= (CORBA::Any &, CORBA::ValueDef_ptr *); // non-copying
TAO_IFR_Client_Export CORBA::Boolean operator>>= (const CORBA::Any &, CORBA::ValueDef_ptr &);

// TAO_IDL - Generated from
// be/be_visitor_structure/any_op_ch.cpp:52

TAO_IFR_Client_Export void operator<<= (CORBA::Any &, const CORBA::ValueDef::FullValueDescription &); // copying version
TAO_IFR_Client_Export void operator<<= (CORBA::Any &, CORBA::ValueDef::FullValueDescription*); // noncopying version
TAO_IFR_Client_Export CORBA::Boolean operator>>= (const CORBA::Any &, CORBA::ValueDef::FullValueDescription *&); // deprecated
TAO_IFR_Client_Export CORBA::Boolean operator>>= (const CORBA::Any &, const CORBA::ValueDef::FullValueDescription *&);

// TAO_IDL - Generated from
// be/be_visitor_structure/any_op_ch.cpp:52

TAO_IFR_Client_Export void operator<<= (CORBA::Any &, const CORBA::ValueDescription &); // copying version
TAO_IFR_Client_Export void operator<<= (CORBA::Any &, CORBA::ValueDescription*); // noncopying version
TAO_IFR_Client_Export CORBA::Boolean operator>>= (const CORBA::Any &, CORBA::ValueDescription *&); // deprecated
TAO_IFR_Client_Export CORBA::Boolean operator>>= (const CORBA::Any &, const CORBA::ValueDescription *&);

// TAO_IDL - Generated from
// be/be_visitor_interface/any_op_ch.cpp:52

TAO_IFR_Client_Export void operator<<= (CORBA::Any &, CORBA::ExtValueDef_ptr); // copying
TAO_IFR_Client_Export void operator<<= (CORBA::Any &, CORBA::ExtValueDef_ptr *); // non-copying
TAO_IFR_Client_Export CORBA::Boolean operator>>= (const CORBA::Any &, CORBA::ExtValueDef_ptr &);

// TAO_IDL - Generated from
// be/be_visitor_structure/any_op_ch.cpp:52

TAO_IFR_Client_Export void operator<<= (CORBA::Any &, const CORBA::ExtValueDef::ExtFullValueDescription &); // copying version
TAO_IFR_Client_Export void operator<<= (CORBA::Any &, CORBA::ExtValueDef::ExtFullValueDescription*); // noncopying version
TAO_IFR_Client_Export CORBA::Boolean operator>>= (const CORBA::Any &, CORBA::ExtValueDef::ExtFullValueDescription *&); // deprecated
TAO_IFR_Client_Export CORBA::Boolean operator>>= (const CORBA::Any &, const CORBA::ExtValueDef::ExtFullValueDescription *&);

// TAO_IDL - Generated from
// be/be_visitor_interface/any_op_ch.cpp:52

TAO_IFR_Client_Export void operator<<= (CORBA::Any &, CORBA::ValueBoxDef_ptr); // copying
TAO_IFR_Client_Export void operator<<= (CORBA::Any &, CORBA::ValueBoxDef_ptr *); // non-copying
TAO_IFR_Client_Export CORBA::Boolean operator>>= (const CORBA::Any &, CORBA::ValueBoxDef_ptr &);

// TAO_IDL - Generated from
// be/be_visitor_root/cdr_op.cpp:48

#ifndef __ACE_INLINE__

// TAO_IDL - Generated from
// be/be_visitor_interface/cdr_op_ch.cpp:55

TAO_IFR_Client_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const CORBA::FixedDef_ptr );
TAO_IFR_Client_Export CORBA::Boolean operator>> (TAO_InputCDR &, CORBA::FixedDef_ptr &);

// TAO_IDL - Generated from
// be/be_visitor_structure/cdr_op_ch.cpp:53

TAO_IFR_Client_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const CORBA::ValueMember &);
TAO_IFR_Client_Export CORBA::Boolean operator>> (TAO_InputCDR &, CORBA::ValueMember &);

// TAO_IDL - Generated from
// be/be_visitor_sequence/cdr_op_ch.cpp:70

#if !defined _TAO_CDR_OP_CORBA_ValueMemberSeq_H_
#define _TAO_CDR_OP_CORBA_ValueMemberSeq_H_

TAO_IFR_Client_Export CORBA::Boolean operator<< (
    TAO_OutputCDR &,
    const CORBA::ValueMemberSeq &
  );
TAO_IFR_Client_Export CORBA::Boolean operator>> (
    TAO_InputCDR &,
    CORBA::ValueMemberSeq &
  );

#endif /* _TAO_CDR_OP_CORBA_ValueMemberSeq_H_ */

// TAO_IDL - Generated from
// be/be_visitor_interface/cdr_op_ch.cpp:55

TAO_IFR_Client_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const CORBA::ValueMemberDef_ptr );
TAO_IFR_Client_Export CORBA::Boolean operator>> (TAO_InputCDR &, CORBA::ValueMemberDef_ptr &);

// TAO_IDL - Generated from
// be/be_visitor_interface/cdr_op_ch.cpp:55

TAO_IFR_Client_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const CORBA::ValueDef_ptr );
TAO_IFR_Client_Export CORBA::Boolean operator>> (TAO_InputCDR &, CORBA::ValueDef_ptr &);

// TAO_IDL - Generated from
// be/be_visitor_structure/cdr_op_ch.cpp:53

TAO_IFR_Client_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const CORBA::ValueDef::FullValueDescription &);
TAO_IFR_Client_Export CORBA::Boolean operator>> (TAO_InputCDR &, CORBA::ValueDef::FullValueDescription &);

// TAO_IDL - Generated from
// be/be_visitor_structure/cdr_op_ch.cpp:53

TAO_IFR_Client_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const CORBA::ValueDescription &);
TAO_IFR_Client_Export CORBA::Boolean operator>> (TAO_InputCDR &, CORBA::ValueDescription &);

// TAO_IDL - Generated from
// be/be_visitor_interface/cdr_op_ch.cpp:55

TAO_IFR_Client_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const CORBA::ExtValueDef_ptr );
TAO_IFR_Client_Export CORBA::Boolean operator>> (TAO_InputCDR &, CORBA::ExtValueDef_ptr &);

// TAO_IDL - Generated from
// be/be_visitor_structure/cdr_op_ch.cpp:53

TAO_IFR_Client_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const CORBA::ExtValueDef::ExtFullValueDescription &);
TAO_IFR_Client_Export CORBA::Boolean operator>> (TAO_InputCDR &, CORBA::ExtValueDef::ExtFullValueDescription &);

// TAO_IDL - Generated from
// be/be_visitor_interface/cdr_op_ch.cpp:55

TAO_IFR_Client_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const CORBA::ValueBoxDef_ptr );
TAO_IFR_Client_Export CORBA::Boolean operator>> (TAO_InputCDR &, CORBA::ValueBoxDef_ptr &);

// TAO_IDL - Generated from
// be/be_visitor_root/cdr_op.cpp:64

#endif /* __ACE_INLINE__ */

// TAO_IDL - Generated from
// be/be_codegen.cpp:969

#if defined (__ACE_INLINE__)
#include "IFR_ExtendedC.i"
#endif /* defined INLINE */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__BORLANDC__)
#pragma option pop
#endif /* __BORLANDC__ */

#include /**/ "ace/post.h"
#endif /* ifndef */

