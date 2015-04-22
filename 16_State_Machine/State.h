/*
 *  State.h
 *  Untitled
 *
 *  Created by Rafael Radkowski on 22.01.09.
 *  Copyright 2009 Heinz Nixdorf Institut. All rights reserved.
 *
 */
#pragma once

#include <list>
#include <string>
#include "Transition.h"
#include "StateTypes.h"
#include "StateOperation.h"
#include "StateParameter.h"

class State
{

	
private:
	
	// ID of the state
	int			_id;
	
	// Label of the state
	std::string	_stateLabel;
	
	// Outgoing transitions of this state
	std::list<Transition*> _transitions;
	
	// A default message that can be send, if the state got active. 
	std::string _message;
	
	/*
	Pointer that refer to the state operations. 
	 */
	StateOperation*	_entryOperation;
	StateOperation*	_stateOperation;
	StateOperation*	_exitOperation;
	
	/*
	 Returns a transitions. Only for internal usage. 
	 @param toState - Name of the state to which the transition might lead. 
	 @return - pointer that refers to that transition object, if it exists. Otherwise = NULL
	 */
	Transition* getTransition(std::string toState);
	
	/*
	Init function
	 */
	void init(void);
	
	
public:

	
	/*
	 Construktor
	 @param name - Label of the state
	 @param statemessage - Default message that can be submitted if necessary. 
	 */
	State(std::string name, std::string statemessage = "");
	
	~State();
	
	/*
	 Returns the id index of the state
	 @return - int with id
	 */
	int getID(void);
	
	/*
	Returns the label of that state
	 @return - string with label
	 */
	std::string	getIdentifier(void);
	
	/*
	 Adds a transition to that particular state
	 @param toState - destination transition
	 @param value - double as transition condition 
	 @param type - StateType, 
	 @return - bool when the transition got added. Otherwise NULL
	*/
	bool addTransition(std::string toState, double value, StateType type = Value_Equal );
	
	/*
	 Adds a transition to that particular state
	 @param toState - destination transition
	 @param value - a transition trigger
	 @param type - StateType,
	 @return - bool when the transition got added. Otherwise NULL
	 */
	bool addTransition(std::string toState, std::string value, StateType type = Value_String );
	
	/*
	 Removes all transitions that are linked with one particular state. 
	 @param toState - the destination state. All transitions to that state get removed. 
	 @return - number of removed transitions. 
	 */
	int removeTransition(std::string toState);
	
	/*
	 Removes all transitions of the state
	 @return - number of removed transitions. 
	 */
	int removeAllTransitions(void);
	
	/*
	 Fire - applies a state transition. 
	 @param value - double value with fire condtion.
	 @return - string with the label of the new state. 
	 */
	std::string fire(double value, StateParameter* param = NULL);
	
	/*
	 Fire - applies a state transition.
	 @param value - string trigger to fire the state transition. 
	 @return - string with the label of the new state.
	 */
	std::string fire(std::string value, StateParameter* param = NULL);
	
	/*
	 Returns the default message of the state
	 @return - string mit der Nachricht
	 */
	std::string getMessageStr(void);
	
	/*
	 Applies the state operation of the state
	 */
	void apply(StateParameter* param = NULL);
	
	/*
	Applies the entry operation of that state
	 */
	void onEntry(StateParameter* param = NULL);
	
	/*
	 Applies an exit operation of that state
	 */
	void onExit(StateParameter* param = NULL);
	
	
	/*
	 Set a particular operation to that state. 
	 @param operation - pointer that refers to that operation 
	 @param type - specifies the operation slot. Three are available: ON_ENTRY, ON_EXIT und
		STATE_OPERATION. Default is STATE_OPERATION
	 @return - true, if the operation was added, otherwise false
	 */
	bool setOperation(StateOperation* operation, StateOperationType type = STATE_OPERATION);

	/*
	 Returns the number of transition that this state has to a destination state
	 @param toState - the destination state
	 @return - int value with the amount of transitions. 
	 */
	int hasTransitionTo(std::string toState);
};

















