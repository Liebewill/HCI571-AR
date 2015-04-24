//
//  ARPaddleOperation.h
//  HCI571X-ARPaddle
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/3/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//
#pragma once

#include <iostream>
#include "StateOperation.h"
#include <osg/Vec3d>

using namespace mystatemachine;

class ARPaddleOperation : public StateOperation
{
private:
    
    
    std::string     _picked_object;
    
    bool            _posInit;
    osg::Vec3d       _lastPaddleLocation;
    
public:
    ARPaddleOperation();
    
    virtual void operate(std::string stateIdentifier, StateParameter* param = NULL);

};