/*
 *  State.cpp
 *  Untitled
 *
 *  Created by Rafael Radkowski on 22.01.09.
 *  Copyright 2009 Heinz Nixdorf Institut. All rights reserved.
 *
 */

#include "State.h"

int	global_StateID = 0;

using namespace mystatemachine;


/******************************************************************************************
 Construktor
 @param name - Label of the state
 @param statemessage - Default message that can be submitted if necessary.
***************************************************************************************** */
State::State(std::string name, std::string statemessage):
	_stateLabel(name), _message(statemessage)
{
	init();
}




State::~State()
{
	
}

/******************************************************************************************
 Initialisierungsfunktion
 ******************************************************************************************/
void State::init(void)
{
	this->_id = global_StateID;
	global_StateID++;
	
#ifdef VERBOSE	
	std::cout << "Created state " << name << "." << std::endl;
#endif
	
	_entryOperation = NULL;
	_stateOperation = NULL;
	_exitOperation = NULL;
}

/*****************************************************************************************
 Liefert eine Transition zurück
 @param toState - Name des Zustandes, zu dem die Transition führt
 @return - Pointer der Transition wenn sie vorhanden ist, sonst NULL
 *****************************************************************************************/
Transition* State::getTransition(std::string toState)
{
	
	if(toState.length() == 0) return NULL;
	
	std::list<Transition*>::iterator itr = this->_transitions.begin();
	
	while(itr != this->_transitions.end())
	{
		std::string tempStateTo = (*itr)->getStateTo();
		
		if(tempStateTo.compare(toState) == 0)
		{
			return (*itr);
		}
		
		itr++;
	}
	
	return NULL;
}


/*****************************************************************************************
 Returns the id index of the state
 @return - int with id
 *****************************************************************************************/
int State::getID(void)
{
	return this->_id;
}

/*****************************************************************************************
 Returns the label of that state
 @return - string with label
 *****************************************************************************************/
std::string	State::getIdentifier(void)
{
	return this->_stateLabel;
}


/*****************************************************************************************
 Adds a transition to that particular state
 @param toState - destination transition
 @param value - double as transition condition
 @param type - StateType,
 @return - bool when the transition got added. Otherwise NULL
 *****************************************************************************************/
bool State::addTransition(std::string toState, double value, StateType type )
{
	if(toState.length() == 0) return false;
	
	// Checks whether the same transition already exists to that state
	Transition* temp = this->getTransition(toState);
	if(temp != NULL) return false; 
	
	// New transition is created
	this->_transitions.push_back(new Transition(toState, value, type ));
	
	return true;
}


/*****************************************************************************************
 Adds a transition to that particular state
 @param toState - destination transition
 @param value - a transition trigger
 @param type - StateType,
 @return - bool when the transition got added. Otherwise NULL
 *****************************************************************************************/
bool State::addTransition(std::string toState, std::string value, StateType type )
{
	if(toState.length() == 0) return false;
	
	// Checks whether the same transition already exists to that state
	Transition* temp = this->getTransition(toState);
	if(temp != NULL) return false; 
	
	//  New transition is created
	this->_transitions.push_back(new Transition(toState, value, Value_String ));
	
	return true;
}


/*****************************************************************************************
 Removes all transitions that are linked with one particular state.
 @param toState - the destination state. All transitions to that state get removed.
 @return - number of removed transitions.
 *****************************************************************************************/
int State::removeTransition(std::string toState)
{

	if(toState.length() == 0) return false;

	int delcounter = 0;
	
	// Chechs it the transition exists
	Transition* temp = this->getTransition(toState);
	while(temp != NULL)
	{ 
	
		//  Transition is deleted
		this->_transitions.remove(temp);
	
		delete temp;
		
		delcounter++;
		
#ifdef VERBOSE		
		std::cout << "State " << toState << " was deleted." << std::endl;
#endif
		temp = this->getTransition(toState);
	}
	
	return delcounter;
	
}

/******************************************************************************************
 Removes all transitions of the state
 @return - number of removed transitions.
 ******************************************************************************************/
int State::removeAllTransitions(void)
{
	
	std::list<Transition*>::iterator itr = _transitions.begin();
	int counter = 0;
	
	while(itr != this->_transitions.end())
	{
		delete (*itr);
		
		counter++;
		itr++;
	}
	
	this->_transitions.clear();
	
	return counter;
}


/*****************************************************************************************
 Fire - applies a state transition.
 @param value - string trigger to fire the state transition.
 @return - string with the label of the new state.
 *****************************************************************************************/
std::string State::fire(double value, StateParameter* param)
{

	std::list<Transition*>::iterator itr = this->_transitions.begin();
	
	while(itr != this->_transitions.end())
	{
		std::string tempStateTo = (*itr)->fire(value);

		if(tempStateTo.compare("") != 0)
		{
			return tempStateTo;
		}
		itr++;
	}
	
	return "";
}


/*****************************************************************************************
 Fire - applies a state transition.
 @param value - double value with fire condtion.
 @return - string with the label of the new state.
 *****************************************************************************************/
std::string State::fire(std::string value, StateParameter* param)
{
	std::list<Transition*>::iterator itr = this->_transitions.begin();
	
	while(itr != this->_transitions.end())
	{
		std::string tempStateTo = (*itr)->fire(value);
		
		if(tempStateTo.compare("") != 0)
		{
			return tempStateTo;
		}
		itr++;
	}
		
	return "";
}


/******************************************************************************************
 Returns the default message of the state
 @return - string mit der Nachricht
 ******************************************************************************************/
std::string State::getMessageStr(void)
{
	return this->_message;
	
}


/******************************************************************************************
Applies the state operation of the state
 ******************************************************************************************/
void State::apply(StateParameter* param)
{
	if(this->_stateOperation != NULL)
	{
		this->_stateOperation->operate(_stateLabel);
	}
	
}

/*******************************************************************************************
Applies the entry operation of that state
 *******************************************************************************************/
void State::onEntry(StateParameter* param)
{
	if(this->_entryOperation != NULL)
	{
		this->_entryOperation->operate(_stateLabel);
	}
}

/***************************************************************
 Applies an exit operation of that state
 *******************************************************************************************/
void State::onExit(StateParameter* param)
{
	if(this->_exitOperation != NULL)
	{
		this->_exitOperation->operate(_stateLabel);
	}
}

/*******************************************************************************************
 Set a particular operation to that state.
 @param operation - pointer that refers to that operation
 @param type - specifies the operation slot. Three are available: ON_ENTRY, ON_EXIT und
 STATE_OPERATION. Default is STATE_OPERATION
 @return - true, if the operation was added, otherwise false
 *******************************************************************************************/
bool State::setOperation(StateOperation* operation, StateOperationType type )
{

	if(operation != NULL)
	{
		switch(type)
		{
			case ON_ENTRY:
				this->_entryOperation = operation;
				return true;
			case ON_EXIT:
				this->_exitOperation = operation;
				return true;
			case STATE_OPERATION:
				this->_stateOperation = operation;
				return true;
		}
		
	}
	
	return false;

}



/********************************************************************************************
 Returns the number of transition that this state has to a destination state
 @param toState - the destination state
 @return - int value with the amount of transitions.
*********************************************************************************************/
int State::hasTransitionTo(std::string toState)
{
	if(toState.length() == 0) return 0;

	std::list<Transition*>::iterator itr = _transitions.begin();
	int counter = 0;
	
	while(itr != this->_transitions.end())
	{
		std::string n = (*itr)->getStateTo();
		if(n.compare(toState) == 0)
		{
			counter++;
		}
		
	
		itr++;
	}
		
	return counter;
}

























