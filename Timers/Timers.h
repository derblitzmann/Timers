#ifndef TIMERS_H
#define TIMERS_H

/************************************************************************/
/*                                                                      */
/*    Timers.h   This provides a simple interface for periodic          */
/*               functions that do not block during the delay period.   */
/*                                                                      */
/************************************************************************/
/*  Author:     Thor Steingrimsson                                      */
/*  Copyright 2014, Digilent Inc.                                       */
/************************************************************************/
/*
*
* Copyright (c) 2014, Digilent <www.digilentinc.com>
* Contact Digilent for the latest version.
*
* This program is free software; distributed under the terms of
* BSD 3-clause license ("Revised BSD License", "New BSD License", or "Modified BSD License")
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* 1.    Redistributions of source code must retain the above copyright notice, this
*        list of conditions and the following disclaimer.
* 2.    Redistributions in binary form must reproduce the above copyright notice,
*        this list of conditions and the following disclaimer in the documentation
*        and/or other materials provided with the distribution.
* 3.    Neither the name(s) of the above-listed copyright holder(s) nor the names
*        of its contributors may be used to endorse or promote products derived
*        from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/************************************************************************/
/*  Revision History:                                                   */
/*                                                                      */
/*    3/27/2014(ThorS): Created                                         */
/************************************************************************/

#include <stdlib.h>

// These constants are to select the resolution of the period
#define MICRO true
#define MILLI false

// This is a struct for our linked list in the Timers class.
typedef struct timer_node {
	// This is the pointer to the function to execute
	void (*function)(void);
	// Is this function enabled or disabled?
	bool en;
	// Are we using micros(), or millis()?
	bool micro;
	// Last time the function was executed
	int last_time;
	// The function's ID
	int id;
	// This is the minimum time between calls
	unsigned int period;
	// pointer to the next timer_node, since this is a linked list
	struct timer_node *next;
} TNODE;

class Timers {
	private:
		// This is our list of timers to run, initially empty (set to NULL).
		TNODE *list;
		// This is used to determine the id of a new timer, initialized to 0
		int ids;
	public:
		// Constructor simply initializes the private variables
		Timers();
		// This will clean up the linked list, if for some reason the 
		// instantiated class goes out of scope or is otherwise destroyed
		~Timers();
		
		// addTimer: Adds a timer to the linked list
		// bool micro: sets whether it will use millis() or micros(), default is to use millis()
		// int period: is the minimum time between calls to foo
		// void (*foo)(void): a function pointer to the function to call periodically
		// Returns the id of the timer that was created (so that individual timers can be removed, disabled, 
		// or enabled). New timers are enabled to run by default.
		int addTimer(unsigned int period, void (*foo)(void), bool micro = MILLI);
		
		// This method removes a timer with inputted id from the linked list, will fail silently
		void removeTimer(int id);
		
		// This method enables a timer with inputted id in the linked list, will fail silently
		void enableTimer(int id);
		
		// This method disables (but does not remove) a timer with inputted id in the linked list, will fail silently
		void disableTimer(int id);
		
		// This method changes the period of the timer with inputted id, it will fail silently
		void changePeriod(int id, unsigned int period);
		
		// This method changes the resolution of the timer with inputted id, it will fail silently
		void changeResolution(int id, bool res);
		
		// This function is the heart of this class, since it calls the functions
		// that are enabled and enough time has elapsed.
		void update();
};
#endif 