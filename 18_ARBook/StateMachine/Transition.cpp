/*
 *  Transition.cpp
 *  Untitled
 *
 *  Created by Rafael Radkowski on 22.01.09.
 *  Copyright 2009 Heinz Nixdorf Institut. All rights reserved.
 *
 */

#include "Transition.h"

using namespace mystatemachine;

int global_TransitionID = 0;


/*
 Construktor
 @param name - name of the destination state
 @param value - condition for state transition
 @param type - type of condition.
 */
Transition::Transition(std::string toState, double value, StateType type):
	_toState(toState), _value(value), _type(type)
{
	
	this->_id = global_TransitionID;
	global_TransitionID++;
	
#ifdef VERBOSE
	std::cout << "Added Transition to state " << toState << " ("<< _id <<")." << std::endl;
#endif
}

/*
 Construktor
 @param name - name of the destination state
 @param value - trigger for state transition
 @param type - type of condition.
 */
Transition::Transition(std::string toState, std::string value, StateType type):
	_toState(toState), _valueStr(value), _type(type)
{
	this->_id = global_TransitionID;
	global_TransitionID++;
#ifdef VERBOSE	
	std::cout << "Added transition to state " << toState << " ("<<_id<<"." << std::endl;
#endif
}

/*****************************************************************************
 Returns the destination state of this transition.
 @return string - The destination state.
******************************************************************************/
std::string Transition::getStateTo(void)
{
	return this->_toState;

}

/*****************************************************************************
 State transition - fires the transition; thus, the state transition gets checked.
 @param value - transition condition
 @return - if the transition is fulfilled, name of the new state. Otherwise ""
******************************************************************************/
std::string Transition::fire(double value)
{
	if(_type == Value_String) return false;
	
	
	switch(_type)
	{
		case Value_Equal:
			if(value == this->_value) return this->_toState;
			else return "";
			break;
			
	}
	
	return "";
	
}
/*****************************************************************************
 State transition - fires the transition; thus, the state transition gets checked.
 @param value - transition trigger
 @return - if the transition is fulfilled, name of the new state. Otherwise ""
******************************************************************************/
std::string Transition::fire(std::string value)
{
	if(_type != Value_String) return false;
	
	if(value.compare(this->_valueStr) == 0)
	{
		return this->_toState;
	}
	else
	{
		return "";
	}

}