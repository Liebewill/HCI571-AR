/*
 *  FiniteStateMachine.cpp
 *  Untitled
 *
 *  Created by Rafael Radkowski on 22.01.09.
 *  Copyright 2009 Heinz Nixdorf Institut. All rights reserved.
 *
 */

#include "FiniteStateMachine.h"

using namespace std;
using namespace mystatemachine;

#define VERBOSE



FiniteStateMachine::FiniteStateMachine()
{
	
	_startState = "-1";
	
	_currentState = "-1";
	
	_currentStatePtr = NULL;
}


FiniteStateMachine::~FiniteStateMachine()
{
	
	
}

/***********************************************************************************************
 Query a state
 @param identifier - the state identifier
 @return State* - a pointer that refer to that state
************************************************************************************************/
State* FiniteStateMachine::getState(std::string identifier)
{
	if(identifier.length() == 0) return NULL;
	
	std::list<State*>::iterator itr = this->_states.begin();
	
	while(itr != this->_states.end())
	{
		std::string temp = (*itr)->getIdentifier();
		if(temp.compare(identifier) == 0)
		{
			return (*itr);
		}
	
		itr++;
	}
	
	return NULL;
	
}


/************************************************************************************************
 Function internally changes the variables that pose the state of the state machine
 @param statePtr - string with the label of the new state
 ************************************************************************************************/
void FiniteStateMachine::changeCurrentState( std::string state, StateParameter* param )
{
	if(state.length() != 0)
	{
		this->_currentStatePtr = getState(state); // Change the state
		this->_currentState = this->_currentStatePtr->getIdentifier();
		
		// Entry-Funktion is carried out
		this->_currentStatePtr->onEntry(param);
		
	}
}

/************************************************************************************************
 Fires operation when the state is realeased
 @param ptr- Pointer that refers to the released state
 ************************************************************************************************/
void FiniteStateMachine::releaseState(State* ptr, StateParameter* param )
{
	if(ptr != NULL)
	{
		// applies exit function
		ptr->onExit(param);
	}
	
}

/***********************************************************************************************
 Adds a new state to the state machine.
 @param identifier - state identifier
 @param statemessage - A default message, which can be submitted, if the state is swiched active.
 @return - int with the new state when it was added. Otherwise -1
 ***********************************************************************************************/
int FiniteStateMachine::addState(std::string identifier, std::string statemessage)
{
	if(identifier.length() == 0) return -1;
	
	State* tempState = getState(identifier);
	
	if(tempState != NULL)
	{
#ifdef VERBOSE
		std::cout << "!! FiniteStateMachine: state was not added; State with label " << identifier << " already exists." << std::endl;
#endif
		return -1;
	}
	
	this->_states.push_back(new State(identifier));
	
	std::cout << "FiniteStateMachine: added state with label " << identifier << "." << std::endl;
	
	return (*this->_states.begin())->getID();
	
}


/************************************************************************************************
 Removes a state from the automata
 @param identifier - string id of teh state that should be removed
 @return true when the state exits and when it got removed.
 ************************************************************************************************/
bool FiniteStateMachine::removeState(std::string identifier)
{
	if(identifier.length() == 0) return false;
	
	State* state = getState(identifier);
	
	if(state == 0) return false;
	
	std::list<State*>::iterator itr = _states.begin();
	
	while(itr != _states.end())
	{

		(*itr)->removeTransition(identifier);// Alle Transitionen zu dem zu löschen Zustand werden entfernt
		
		itr++;
	}
	
	return true;
}

/***********************************************************************************************
 Specifies the start state of the entire automata
 @param identifier - string id of the start state
 @param reset - allows to reset the state in its start parameter set.
 @return - true, if sucessfully set as start state
 ***********************************************************************************************/
bool FiniteStateMachine::setStartState(std::string identifier, bool reset)
{
	if(identifier.length() == 0) return false;
	
	State* ptr = getState(identifier);
	if(ptr == NULL)
	{
#ifdef VERBOSE
		std::cout << "!! FiniteStateMachine: start state not set; no state with label " << identifier << " exists. "<< std::endl;
#endif		
		return false;
	}
	
	this->_startState = identifier;
	
	if(reset == true)
	{
		this->reset();
	}
	
	return true;
}



/***********************************************************************************************
 Add a transition between two states
 @param fromState - start state for this transition
 @param toState - end state with this transition
 @param double value - transition condition.
 @param StateType - type of transition
 ***********************************************************************************************/
bool FiniteStateMachine::addTransition(std::string fromState, std::string toState, double value, StateType type)
{
	if(fromState.length() == 0) return false;
	if(toState.length() == 0) return false;
	
	State* from = this->getState(fromState);
	if(from == NULL)
	{
#ifdef VERBOSE
		std::cout << "!! FiniteStateMachine: did not add transition because state " << fromState << " unknown. "<< std::endl;
#endif
		return false;
	}
	
	State* to = this->getState(toState);
	if(to == NULL)
	{
#ifdef VERBOSE
		std::cout << "!! FiniteStateMachine: did not add transition because state " << toState << " unknown. "<< std::endl;
#endif
		return false;
	}
	
	bool ret = from->addTransition(toState, value, type);
	
	if(ret == false)
	{
#ifdef VERBOSE
		std::cout << "!! FiniteStateMachine: error while creating tansition from state " << fromState << " to state " << toState << ". "<< std::endl;
#endif
		return false;
	}

	std::cout << "FiniteStateMachine: added transition from " << fromState << " to " << toState << ". "<< std::endl;
	
	return true;
	
}

/***********************************************************************************************
 Add a transition between two states
 @param fromState - start state for this transition
 @param toState - end state with this transition
 @param std::string valueStr - string value that poses a trigger
 @param StateType - type of transition
 ***********************************************************************************************/
bool FiniteStateMachine::addTransition(std::string fromState, std::string toState, std::string valueStr, StateType type)
{
	if(fromState.length() == 0) return false;;
	if(toState.length() == 0) return false;;
	
	if(type != Value_String)
	{
#ifdef VERBOSE
		std::cout << "!! FiniteStateMachine: did not add transition from state " << fromState << " to " << toState << ": type Value_String no allowed for this transition. "<< std::endl;
#endif
		return false;
	}
	
	State* from = this->getState(fromState);
	if(from == NULL)
	{
#ifdef VERBOSE
		std::cout << "!! FiniteStateMachine: did not add transition because state " << fromState << " does not exist. "<< std::endl;
#endif
		return false;
	}
	
	State* to = this->getState(toState);
	if(to == NULL)
	{
#ifdef VERBOSE
		std::cout << "!! FiniteStateMachine: did not add transition because state " << toState << " does not exist. "<< std::endl;
#endif
		return false;
	}
	
	bool ret = from->addTransition(toState, valueStr, type);
	
	if(ret == false)
	{
#ifdef VERBOSE
		std::cout << "!!FiniteStateMachine: error during the creation of transition from state " << fromState << " to state " << toState << ". "<< std::endl;
#endif
		return false;
	}

#ifdef VERBOSE
	std::cout << "FiniteStateMachine: added transition from state " << fromState << " to " << toState << ". "<< std::endl;
#endif
	return true;
}

/***********************************************************************************************
 Reset the state machine and sets the current state to the start state.
 @return - String with the label of the current state. "" if the string could not been set.
 ***********************************************************************************************/
std::string FiniteStateMachine::reset(void)
{
	if(_startState.compare("-1") == 0)
	{
#ifdef VERBOSE
		std::cout << "!FiniteStateMachine: did not reset start state because no start state was specified; specify a start state." << std::endl;
#endif
		return "";
	}
	
	State* ptr = getState(this->_startState);
	
	if(ptr == NULL)
	{
#ifdef VERBOSE
		std::cout << "!FiniteStateMachine: did not reset start state because state "<< _startState <<" does not exist." << std::endl;
#endif	
		return "";
	}
	

	
	/****************************************************************************/
	// State change is carried out
	changeCurrentState( this->_startState);

#ifdef VERBOSE	
	std::cout << "FiniteStateMachine: set current state to " << this->_currentState << "." << std::endl;
#endif
	return this->_currentState;
	
}

/************************************************************************************************
 Checks if all conditions for a state change are fulfilled.
 @return - true, if the state change can be carried out
 false, if conditions are missing
 ************************************************************************************************/
bool FiniteStateMachine::preFireOperations(void)
{
	if(_startState.compare("-1") == 0)
	{
#ifdef VERBOSE
		std::cout << "!!FiniteStateMachine: specify start state before fire the state machine." << std::endl;
#endif
		return false;
	}
	if(_currentState.compare("-1") == 0)
	{
		reset();
	}
	if(_currentState.compare("-1") == 0)
	{
#ifdef VERBOSE
		std::cout << "!FiniteStateMachine: error during processing." << std::endl;
#endif
		return false;
	}
	
	return true;
}

/***********************************************************************************************
 Fires a transition
 @param value - tranistion condition value
 @param param - additional parameter that might be passed to the
 @return - string with the label of the new state
 ***********************************************************************************************/
std::string FiniteStateMachine::fire(double value, StateParameter* param)
{
	if(preFireOperations() == false) return "";
	
	State* stateTemp =  this->getState(_currentState);
	std::string newState = "";
	if(stateTemp != NULL)
	{
		// State conditions are checked
		newState = stateTemp->fire(value, param);
	}
	
	if(newState.compare("") != 0) // New state is set, if the transition is fulfilled
	{
		// Old state is released
		releaseState(stateTemp, param);
		
		/****************************************************************************/
		// Chang the state
		changeCurrentState(newState, param);
		/****************************************************************************/
		
#ifdef VERBOSE
		std::cout << "FiniteStateMachine: set new state -> " << _currentState << "."<< std::endl;
#endif
		return this->_currentState;
	}
	else
	{
        apply(param);
#ifdef VERBOSE
		std::cout << "FiniteStateMachine: IDLE (" <<  this->_currentState <<")."<< std::endl;
#endif
	}
	
	return newState; // If the program runs to this point, the variable is empty.
	
}

/***********************************************************************************************
 Fires a transition
 @param valueStr - transition trigger
 @return - string with the label of the new state
 ***********************************************************************************************/
std::string FiniteStateMachine::fire(std::string valueStr, StateParameter* param)
{

	if(preFireOperations() == false) return "";
	
	State* stateTemp =  this->getState(_currentState);
	std::string newState = "";
	if(stateTemp != NULL)
	{
		newState = stateTemp->fire(valueStr, param);
	}
	
	if(newState.compare("") != 0) // Set the new state
	{
		// Old state is released
		releaseState(stateTemp, param);
		
		/****************************************************************************/
		// State change is applied
		changeCurrentState(newState, param);
		/****************************************************************************/
		
#ifdef VERBOSE
		std::cout << "FiniteStateMachine: set new state -> " << _currentState << "."<< std::endl;
#endif
		return this->_currentState;
	}
	else
	{
        apply(param);
#ifdef VERBOSE
		std::cout << "FiniteStateMachine: IDLE (" <<  this->_currentState <<")."<< std::endl;
#endif
	}
	
	return newState; 
}

/***********************************************************************************************
 returns the current state
 @return string with current state
 ***********************************************************************************************/
std::string FiniteStateMachine::getCurrentState(void) const
{
	return this->_currentState;
}


/************************************************************************************************
 Adds a user defined function to a state
 @param state - identifier of the state
 @param operation - the state operation object
 @param type - type of operation: ON_ENTRY, ON_EXIT, or STATE_OPERATION
 ***********************************************************************************************/
bool FiniteStateMachine::setStateFunction(std::string state, StateOperation* operation, StateOperationType type)
{
	if(state.length() == 0) 
	{
#ifdef VERBOSE
		std::cout << "!FiniteStateMachine: did not set operation; no function was specified."<< std::endl;
#endif		
		return false;
	}
	
	State* statePtr = getState(state);
	
	if(statePtr == NULL)
	{
#ifdef VERBOSE
		std::cout << "!FiniteStateMachine: did not set function because state "<< state <<" does not exist."<< std::endl;
#endif		
		return false;
	}
	
	bool ret = statePtr->setOperation(operation, type);
	
#ifdef VERBOSE
	std::cout << "FiniteStateMachine: added function to state "<< state <<"."<< std::endl;
#endif
	
	return ret;
	
}


/*************************************************************************************************
 Fires the state state-function, if a function is available
 @return - true when the function is available.
 *************************************************************************************************/
bool FiniteStateMachine::apply(StateParameter* param )
{
	if(this->_currentStatePtr != NULL)
	{
		this->_currentStatePtr->apply(param);
		return true;
	}
	return false;	
}


























