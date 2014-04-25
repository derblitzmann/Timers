// This should make this library work for Arduino and chipKIT.
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// License information is in the header
#include "Timers.h"

// This will be used to detect timer overflows
const unsigned int MAX_INT = 0 - 1;

// We don't need to do much in here, but initialize our private variables
Timers::Timers()
{
	list = NULL;
	ids = 0;
}

// This deconstructor is is important as it frees the memory we allocated 
// when the instantiated class goes out of scope.
Timers::~Timers()
{	
	// If there isn't anything, we don't have to do anything
	if(list == NULL){
		return;
	}
	TNODE *next_item;
	// We know there is at least one item in the list.
	do {
		// Since we are removing the item that list points to;
		// preserve the pointer to the next item
		next_item = list->next;
		free(list);
		list = next_item;
		// continue this process until the list is empty
	}while(next_item != NULL);
	// all memory has been freed
}

// This method is how we add new timers to the linked list
int Timers::addTimer(int period, void (*foo)(void), bool micro)
{
	// allocate memory for the timer
	TNODE *new_tnode = (TNODE*) malloc(sizeof(TNODE));
	// If there was an error (not enough memory) return 0
	if(new_tnode == NULL){
		return 0;
	}
	// enabled by default
	new_tnode->en = true;
	// set the resolution to the input
	new_tnode->micro = micro;
	// Set the period to the input
	new_tnode->period = period;
	// This is the function to execute after at least the period
	new_tnode->function = foo;
	// We place new timers "on top" of previous timers
	new_tnode->next = list;
	// set the id, but increment it first
	new_tnode->id = ++ids; // ids increments and THEN sets the tnode id
	// set the node pointer list to this new timer
	list = new_tnode;
	// Return the id of the timer we made.
	return list->id;
}

void Timers::removeTimer(int id)
{
	// This is the special case block where we check some special conditions before
	// trying out the general case, since that could lead to SEGFAULTs
	if(list == NULL){ // if there is nothing in the list
		return;
	}else if(list->next == NULL){ // if there is only one item in the list
		if(list->id == id){
			free(list);
			list = NULL;
			return;
		}
	}else if(list->id == id){ // if the first item is the one we want to remove
		TNODE *tmp = list->next;
		free(list);
		list = tmp;
		return;
	}
	// We need the previous node so we don't lose the the rest of the list, which is why
	// if list is the timer to remove, it has a special case since it is the beginning.
	TNODE *prev_node = list; 
	TNODE *curr_node = list->next; // Our "current" node starts with the second
	TNODE *next_node;
	while(curr_node != NULL){
		next_node = curr_node->next;
		if(curr_node->id == id){
			free(curr_node);
			// make the previous node point to the next node
			prev_node->next = next_node;
			return;
		}
		prev_node = curr_node;
		curr_node = next_node;
	}
}

void Timers::enableTimer(int id)
{
	// if the list is empty, get out
	if(list == NULL){
		return;
	}
	TNODE *curr_node = list;
	TNODE *next_node;
	while(curr_node != NULL){
		next_node = curr_node->next;
		if(curr_node->id == id){
			curr_node->en = true;
			return;
		}
		curr_node = next_node;
	}
}

void Timers::disableTimer(int id)
{
	// if the list is empty, get out
	if(list == NULL){
		return;
	}
	TNODE *curr_node = list;
	TNODE *next_node;
	while(curr_node != NULL){
		next_node = curr_node->next;
		if(curr_node->id == id){
			curr_node->en = false;
			return;
		}
		curr_node = next_node;
	}
}

void Timers::changePeriod()
{
	if(list == NULL){
		return;
	}
	TNODE *curr_node = list;
	TNODE *next_node;
	while(curr_node != NULL){
		next_node = curr_node->next;
		if(curr_node->id == id){
			curr_node->period = period;
			return;
		}
		curr_node = next_node;
	}
}

void Timers::update()
{
	// if the list is empty, then abort
	if(list == NULL){
		return;
	}
	TNODE *curr_node = list;
	TNODE *next_node;
	while(curr_node != NULL){
		next_node = curr_node->next;
		// If this timer is enabled
		if(curr_node->en){
			int curr_time;
			// select the right time scale
			if(curr_node->micro){
				curr_time = micros();
			}else{
				curr_time = millis();				
			}
			// has enough time passed? And have we overflowed?
			unsigned int future_time = curr_node->last_time + curr_node->period;
			if((curr_time >= future_time) || ((future_time < curr_node->last_time) && (curr_time >= curr_node->period - (MAX_INT - curr_node->last_time)))){
				curr_node->function();
				curr_node->last_time = curr_time;
			}
		}
		curr_node = next_node;
	}
}




