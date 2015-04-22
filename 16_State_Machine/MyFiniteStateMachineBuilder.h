/*
 *  MyFiniteStateMachineBuilder.h
 *  Untitled
 *
 *  Created by Rafael Radkowski on 26.01.09.
 *  Copyright 2009 Heinz Nixdorf Institut. All rights reserved.
 *
 */
#pragma once
#include "FiniteStateMachineBuilder.h"

class MyFiniteStateMachineBuilder : public FiniteStateMachineBuilder
{
private:
	
	
	virtual void setFunction(std::string stateName, std::string functionname, std::string parameter1, StateFunctionAttributeList attribute_list, StateOperationType type);
	
public:
	MyFiniteStateMachineBuilder();
	
	
	
};
