/*
 *  MyTestStateOperation.cpp
 *  Untitled
 *
 *  Created by Rafael Radkowski on 24.01.09.
 *  Copyright 2009 Heinz Nixdorf Institut. All rights reserved.
 *
 */

#include "MyTestStateOperation.h"

MyTestStateOperation::MyTestStateOperation(std::string str):
	_content(str)
{
	
}

void MyTestStateOperation::operate(std::string stateIdentifier, StateParameter* param )
{

	std::cout << stateIdentifier << " - Wello World! " << _content << std::endl;
}