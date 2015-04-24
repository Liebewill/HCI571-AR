/*
 *  MyTestStateOperation.h
 *  This is a test operation. It does not do much. But 
 *  this class is usally used to implement the interface
 *  between the state machine and the application. 
 *
 *  Created by Rafael Radkowski on 24.01.09.
 *  Copyright 2009 Heinz Nixdorf Institut. All rights reserved.
 *
 */

#pragma once

#include "StateOperation.h"
#include <iostream>
#include <string>

class MyTestStateOperation : public StateOperation
{
private:
	std::string _content;
	
public:
	MyTestStateOperation(std::string str);

	virtual void operate(std::string stateIdentifier, StateParameter* param = NULL);
};