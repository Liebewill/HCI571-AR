/*
 *  MyFiniteStateMachineBuilder.cpp
 *  Untitled
 *
 *  Created by Rafael Radkowski on 26.01.09.
 *  Copyright 2009 Heinz Nixdorf Institut. All rights reserved.
 *
 */

#include "MyFiniteStateMachineBuilder.h"
#include "MyTestStateOperation.h"



MyFiniteStateMachineBuilder::MyFiniteStateMachineBuilder()
{
	
}


void MyFiniteStateMachineBuilder::setFunction(std::string stateName, std::string functionname, std::string parameter1, StateFunctionAttributeList attribute_list, StateOperationType type)
{
	switch(type)
	{
		case ON_ENTRY:
			if(functionname.compare("MyTestStateOperation") == 0)
			{
				std::string msg = "Entry";
				int size = attribute_list.size();
				if(size > 0)
				{
					msg = (*attribute_list.begin()).value;
				}
				this->_destination->setStateFunction(stateName, new MyTestStateOperation(msg), type);
			}
			break;
		case STATE_OPERATION:
			if(functionname.compare("MyTestStateOperation") == 0)
			{
				std::string msg = "Do Activity";
				int size = attribute_list.size();
				if(size > 0)
				{
					msg = (*attribute_list.begin()).value;
				}
				this->_destination->setStateFunction(stateName, new MyTestStateOperation(msg), type);
			}
			break;
		case ON_EXIT:
			if(functionname.compare("MyTestStateOperation") == 0)
			{
				std::string msg = "Exit";
				int size = attribute_list.size();
				if(size > 0)
				{
					msg = (*attribute_list.begin()).value;
				}
				
				this->_destination->setStateFunction(stateName, new MyTestStateOperation(msg), type);
			}
			break;
	}
	
}

