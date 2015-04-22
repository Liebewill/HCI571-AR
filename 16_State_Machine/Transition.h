/*
 *  Transition.h
 *  Untitled
 *
 *  Created by Rafael Radkowski on 22.01.09.
 *  Copyright 2009 Heinz Nixdorf Institut. All rights reserved.
 *
 */
#pragma once

#include "StateTypes.h"
#include <string>
#include <iostream>

class Transition
{
private:
		
	// ID of the transition
	int			_id;
		
	// ID of the destination  state
	std::string	_toState;
		
	// Type of state condition. 
	StateType	_type;
	
	double		_value;
	std::string	_valueStr;
		
public:
		
	/*
	Construktor
	@param name - name of the destination state
	@param value - condition for state transition
	@param type - type of condition. 
	*/
	Transition(std::string toState, double value, StateType type);
	
	/*
	 Construktor
     @param name - name of the destination state
     @param value - trigger for state transition
     @param type - type of condition.
	 */
	Transition(std::string toState, std::string value, StateType type);
		
	/*
	 Returns the destination state of this transition. 
	 @return string - The destination state. 
	 */
	std::string getStateTo(void);
	
	/*
	 State transition - fires the transition; thus, the state transition gets checked. 
	 @param value - transition condition 
	 @return - if the transition is fulfilled, name of the new state. Otherwise ""
	 */
	std::string fire(double value);
	
	/*
     State transition - fires the transition; thus, the state transition gets checked.
	 @param value - transition trigger
	 @return - if the transition is fulfilled, name of the new state. Otherwise ""
	 */
	std::string fire(std::string value);
		
};