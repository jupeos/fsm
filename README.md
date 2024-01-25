# fsm
A lightweight finite state machine.

Designed to be easy to use covering the basic needs of state machine functionality. A single file intended to be included in a C source file.
This allows multiple state machines to be implemented across different compilation units/scopes.
The user may modify the configuration in *finite_state_machine_conf.h* to suit their needs.

Example usage:
```
   #include "finite_state_machine.h"

   typedef enum
   {
       STATE_OPEN,
       STATE_CLOSED,
   } STATE_ID;

   typedef enum
   {
       EVENT_OPEN,
       EVENT_CLOSE,
   } EVENT_ID;

   // Actions
   static void doorOpenedAction( data_t stateID, event_t* event )
   {
       printf( "Door opened.\r\n" );
   }
   static void doorClosedAction( data_t stateID, event_t* event )
   {
       printf( "Door closed.\r\n" );
   }

   // Forward declarations to be used in state definitions.
   static state_t doorOpenState, doorClosedState;

   static state_t doorOpenState = {
       SM_STATE_ACTIONS( STATE_OPEN, SM_NO_ACTION, SM_NO_ACTION ),
       SM_TRANSITIONS( { EVENT_CLOSE, &doorClosedState, SM_NO_ACTION, doorClosedAction }, ),
   };
   static state_t doorClosedState = {
       SM_STATE_ACTIONS( STATE_CLOSED, SM_NO_ACTION, SM_NO_ACTION ),
       SM_TRANSITIONS( { EVENT_OPEN, &doorOpenState, SM_NO_ACTION, doorOpenedAction }, ),
   };

   state_machine_t fsm = { .currentState = &doorClosedState };

   void openFunc( void )
   {
       event_t event = { .ID = EVENT_OPEN, .data = 0 };
       fsm_handle_event( &fsm, &event );
   }
```
