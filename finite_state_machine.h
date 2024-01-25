/*******************************************************************************
MIT License

Copyright (c) 2024 Julian Mitchell
https://github.com/jupeos/fsm

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

#ifndef FINITE_STATE_MACHINE_H
#define FINITE_STATE_MACHINE_H

#include <stdbool.h>

#include "finite_state_machine_conf.h"

/**
 * @file finite_state_machine.h
 * @author Julian Mitchell
 * @date 25th Jan 2024
 * @brief A lightweight finite state machine.
 *
 * @details A single file intended to be included in a C source file.
 * This allows multiple state machines to be defined across different compilation units/scopes.
 * The user can modify the configuration in finite_state_machine_conf.h to suit their needs.
 *
 * Example usage:
 * @code
 *    #include "finite_state_machine.h"
 *
 *    typedef enum
 *    {
 *        STATE_OPEN,
 *        STATE_CLOSED,
 *    } STATE_ID;
 *
 *    typedef enum
 *    {
 *        EVENT_OPEN,
 *        EVENT_CLOSE,
 *    } EVENT_ID;
 *
 *    // Actions
 *    static void doorOpenedAction( data_t stateID, event_t* event )
 *    {
 *        printf( "Door opened.\r\n" );
 *    }
 *
 *    static void doorClosedAction( data_t stateID, event_t* event )
 *    {
 *        printf( "Door closed.\r\n" );
 *    }
 *
 *    // Forward declarations to be used in state definitions.
 *    static state_t doorOpenState, doorClosedState;
 *
 *    static state_t doorOpenState = {
 *        SM_STATE_ACTIONS( STATE_OPEN, SM_NO_ACTION, SM_NO_ACTION ),
 *        SM_TRANSITIONS( { EVENT_CLOSE, &doorClosedState, SM_NO_ACTION, doorClosedAction }, ),
 *    };
 *
 *    static state_t doorClosedState = {
 *        SM_STATE_ACTIONS( STATE_CLOSED, SM_NO_ACTION, SM_NO_ACTION ),
 *        SM_TRANSITIONS( { EVENT_OPEN, &doorOpenState, SM_NO_ACTION, doorOpenedAction }, ),
 *    };
 *
 *    state_machine_t fsm = { .currentState = &doorClosedState };
 *
 *    void openFunc( void )
 *    {
 *        event_t event = { .ID = EVENT_OPEN, .data = 0 };
 *        fsm_handle_event( &fsm, &event );
 *    }
 * @endcode
 */

#ifdef __cplusplus
extern "C" {
#endif

struct state;
typedef struct state state_t;

/**
 * @brief A state transition.
 */
typedef struct
{
    event_id_t eventID;                                   /*< The event that triggers this transition.*/
    state_t* nextState;                                   /*< The state to transition to.*/
    bool ( *guard )( data_t stateData, event_t* event );  /*< A function that returns true if the transition should be allowed (optional).*/
    void ( *action )( data_t stateData, event_t* event ); /*< A function to be executed on state transition (optional).*/
} transition_t;

/**
 * @brief A state.
 *
 * @note Use the helper macro SM_STATE_ACTIONS  to eliminate the need to set *data* and entry/exit actions.
 * @note Use the helper macro SM_TRANSITIONS  to eliminate the need to set *numTransitions*.
 */
struct state
{
    data_t data;                                               /*< User defined data.*/
    void ( *entryAction )( data_t stateData, event_t* event ); /*< The entry action (optional).*/
    void ( *exitAction )( data_t stateData, event_t* event );  /*< The exit action (optional).*/
    transition_t* transitions;                                 /*< An array of transition_t structs.*/
    size_t numTransitions;                                     /*< The number of transitions.*/
};

/**
 * @brief A state machine.
 * @note All the logic is defined in the state definitions.
 */
typedef struct
{
    state_t* currentState;
} state_machine_t;

// Helper macros
#define SM_TRANSITIONS( ... )                         \
    .transitions = ( transition_t[] ){ __VA_ARGS__ }, \
    .numTransitions = sizeof( ( transition_t[] ){ __VA_ARGS__ } ) / sizeof( transition_t )

#define SM_NO_ACTION NULL
#define SM_NO_GUARD  NULL

#define SM_STATE_ACTIONS( DATA, ENTRY, EXIT ) .data = DATA, .entryAction = ENTRY, .exitAction = EXIT

/**
 * @brief State machine event handler.
 *
 * @param fsm The state machine instance.
 * @param event The event to process.
 * @return true A successful transistion to another state.
 * @return false No valid transition found or the guard condition failed.
 */
static bool fsm_handle_event( state_machine_t* fsm, event_t* event )
{
    bool retVal = false;
    // Start by looking for a transition with this event for the current state.
    transition_t* transition = NULL;
    for( size_t i = 0; i < fsm->currentState->numTransitions; ++i )
    {
        if( fsm->currentState->transitions[ i ].eventID == event->ID )
        {
            // We found a match.
            transition = &fsm->currentState->transitions[ i ];
            break;
        }
    }

    if( transition )
    {
        bool guardResult = true;
        // If there is a guard function, call it.
        if( transition->guard )
        {
            guardResult = transition->guard( fsm->currentState->data, event );
        }

        if( guardResult )
        {
            // Perform the exit action (if there is one).
            if( fsm->currentState->exitAction )
            {
                fsm->currentState->exitAction( fsm->currentState->data, event );
            }

            // Perform the associated action (if there is one).
            if( transition->action )
            {
                transition->action( fsm->currentState->data, event );
            }

            // Perform the entry action (if there is one).
            if( transition->nextState->entryAction )
            {
                transition->nextState->entryAction( transition->nextState->data, event );
            }

            // Transition complete so update the state.
            fsm->currentState = transition->nextState;
            retVal = true;
        }
    }
    return retVal;
}

#ifdef __cplusplus
}
#endif

#endif
