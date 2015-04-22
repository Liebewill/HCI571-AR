//
//  StateParameter.h
//  FiniteStateMachine
//
//  Created by Dr.-Ing. Rafael Radkowski on 2/4/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//
#pragma once
#include <iostream>



class StateParameter
{
private:
    
    int             _idx;
    
    std::string     _message;
    
public:
    
    StateParameter();
    StateParameter(std::string message = "");
    
    /*!
     Returns the default message
     */
    std::string getMessage(void);
    
    
    /*
     Returns the index of the state parameter. 
     */
    int getIndex(void);
    

};
