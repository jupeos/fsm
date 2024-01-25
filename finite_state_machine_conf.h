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

#ifndef FINITE_STATE_MACHINE_CONF_H
#define FINITE_STATE_MACHINE_CONF_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* User defined data types used by the state machine in finite_state_machine.h. */

typedef int32_t data_t;     /*< Generic data passed to guards and actions (could be an enumeration for a state identifier). */
typedef int32_t event_id_t; /*< User defined type for an event identifier. */

typedef struct
{
    event_id_t ID; /*< A unique event identifier (could be an enumeration) */
    data_t data;   /*< User defined data to be included with each event. */
} event_t;

#ifdef __cplusplus
}
#endif

#endif  // FINITE_STATE_MACHINE_CONF_H
