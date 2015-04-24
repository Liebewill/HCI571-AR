//
//  StateParameter.cpp
//  FiniteStateMachine
//
//  Created by Dr.-Ing. Rafael Radkowski on 2/4/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//

#include "StateParameter.h"

using namespace mystatemachine;

int g_StateParameterIndex = 0;

StateParameter::StateParameter()
{
    _idx = g_StateParameterIndex;
    g_StateParameterIndex++;
    
    _message = "";
}

StateParameter::StateParameter(std::string message):
    _message(message)
{
    _idx = g_StateParameterIndex;
    g_StateParameterIndex++;
}

/*!
 Returns the default message
 */
std::string StateParameter::getMessage(void)
{
    return _message;
}


/*
 Returns the index of the state parameter.
 */
int StateParameter::getIndex(void)
{
    return _idx;
}
