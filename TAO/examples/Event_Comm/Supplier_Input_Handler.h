/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    EventComm
//
// = FILENAME
//    Supplier_Input_Handler.h
//
// = DESCRIPTION
//    Handle input from the keyboard.
//
// = AUTHOR
//    Douglas C. Schmidt (schmidt@cs.wustl.edu) and Pradeep Gore (pradeep@cs.wistl.edu)
//
// ============================================================================

#ifndef SUPPLIER_INPUT_HANDLER_H
#define SUPPLIER_INPUT_HANDLER_H

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration.
class Notifier_Handler;

class Supplier_Input_Handler : public ACE_Service_Object
{
  // = TITLE
  //   Handles input events generated from a keyboard.
  //
  // = DESCRIPTION
  //   The events are currently framed and forwarded to all Consumers.
  //   In the future, we will need to be more selective and only send
  //   to those Consumers whose filtering criteria matches!
public:
  // = Initialization and termination methods.
  Supplier_Input_Handler (void);
  // Constructor.

  ~Supplier_Input_Handler (void);
  // Destructor.

  int initialize (Notifier_Handler *, ACE_HANDLE = ACE_STDIN);
  // Initialization, uses stdin by default.

  virtual int handle_input (ACE_HANDLE);
  // Frame input events and notify <Consumers>.

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::NULL_MASK);
  // Close down the handler.

protected:
  virtual ACE_HANDLE get_handle (void) const;
  // Return the input handle.

  ACE_HANDLE handle_;
  // ACE_HANDLE where the input comes from.

  Notifier_Handler *notifier_;
  // Pointer to a <Notifier_Handler> that's used to inform Consumers
  // that events of interest have occurred.

  FILE *fp_;
  // Pointer to an input FILE.
};

#endif /* SUPPLIER_INPUT_HANDLER_H */
