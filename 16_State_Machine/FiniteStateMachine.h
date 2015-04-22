/*
 *  FiniteStateMachine.h
 *  Untitled
 *
 *  Created by Rafael Radkowski on 22.01.09.
 *  Copyright 2009 Heinz Nixdorf Institut. All rights reserved.
 *
 */
#pragma once

#include "Transition.h"
#include "State.h"
#include "StateTypes.h"
#include "StateOperation.h"
#include "StateParameter.h"
#include <list>
#include <string>

class FiniteStateMachine
{
private:
	// List of all states
	std::list<State*> _states;
	
	// start state; is set by the user
	std::string	_startState;
	
	// The current state of the automata
	std::string	_currentState;
	State*		_currentStatePtr;
	
	/*
	 Query a state 
	 @param identifier - the state identifier
	 @return State* - a pointer that refer to that state
	 */
	State* getState(std::string identifier);
	
	/*
	 Function internally changes the variables that pose the state of the state machine
	 @param statePtr - string with the label of the new state
	 */
	void changeCurrentState( std::string state, StateParameter* param = NULL);
	
	/*
	 Fires operation when the state is realeased
	 @param ptr- Pointer that refers to the released state
	 */
	void releaseState(State* ptr, StateParameter* param = NULL);
	
	/*
	 Checks if all conditions for a state change are fulfilled.
	 @return - true, if the state change can be carried out
		false, if conditions are missing
	 */
	bool preFireOperations(void);
    
    
    /*
	 Fires the state state-function, if a function is available
	 @return - true when the function is available. 
	 */
	bool apply( StateParameter* param = NULL);
	
public:
	
	FiniteStateMachine();
	
	
	~FiniteStateMachine();
	
	/*
	 Adds a new state to the state machine. 
	 @param identifier - state identifier
	 @param statemessage - A default message, which can be submitted, if the state is swiched active.  
	 @return - int with the new state when it was added. Otherwise -1
	 */
	int addState(std::string identifier, std::string statemessage = std::string("") );
	
	/*
	 Removes a state from the automata
	 @param identifier - string id of teh state that should be removed
	 @return true when the state exits and when it got removed. 
	 */
	bool removeState(std::string identifier);
	
	/*
	 Specifies the start state of the entire automata
	 @param identifier - string id of the start state
	 @param reset - allows to reset the state in its start parameter set. 
	 @return - true, if sucessfully set as start state
	 */
	bool setStartState(std::string identifier, bool reset = true);
	
	/*
	 Add a transition between two states
	 @param fromState - start state for this transition
	 @param toState - end state with this transition
	 @param double value - transition condition. 
	 @param StateType - type of transition
	 */
	bool addTransition(std::string fromState, std::string toState, double value, StateType type = Value_Equal);
	
	/*
	 Add a transition between two states
	 @param fromState - start state for this transition
	 @param toState - end state with this transition
	 @param std::string valueStr - string value that poses a trigger
	 @param StateType - type of transition
	 */
	bool addTransition(std::string fromState, std::string toState, std::string valueStr, StateType type = Value_String);
	
	/*
	 Reset the state machine and sets the current state to the start state. 
	 @return - String with the label of the current state. "" if the string could not been set. 
	 */
	std::string reset(void);
	
	/*
	 Fires a transition
	 @param value - tranistion condition value
     @param param - additional parameter that might be passed to the 
	 @return - string with the label of the new state
	 */
	std::string fire(double value, StateParameter* param = NULL);
	
	/*
	 Fires a transition
	 @param valueStr - transition trigger
	 @return - string with the label of the new state
	 */
	std::string fire(std::string valueStr, StateParameter* param = NULL);
	
	/*
	 returns the current state
	 @return string with current state
	 */
	std::string getCurrentState(void) const;
	
	/*
	 Adds a user defined function to a state
	 @param state - identifier of the state
	 @param operation - the state operation object
	 @param type - type of operation: ON_ENTRY, ON_EXIT, or STATE_OPERATION
	 */
	bool setStateFunction(std::string state,  StateOperation* operation, StateOperationType type = STATE_OPERATION);
	
	

};