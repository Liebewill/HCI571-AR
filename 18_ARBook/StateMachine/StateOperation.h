/*
 *  StateOperation.h
 *  Untitled
 *
 *  Created by Rafael Radkowski on 23.01.09.
 *  Copyright 2009 Heinz Nixdorf Institut. All rights reserved.
 *
 */
#pragma once

#include <string>
#include "StateParameter.h"

#include "FSMExport.h"

namespace mystatemachine
{

class FSMExport StateOperation
{
private:


public:
	
	StateOperation();
	
	~StateOperation();
	
	/*
	 This virtual class must be implemented when a state operation is created. 
	 @param stateIdentifier - the indentifier of the state is passed as function
     @param param - the user definied parameter are passed to that function. 
	 */
	virtual void operate(std::string stateIdentifier, StateParameter* param) = 0;

};

}