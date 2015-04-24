//
//  ARPaddleStateParameters.h
//  HCI571X-ARPaddle
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/3/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//
#pragma once

#include <iostream>
#include "StateParameter.h"

#include <osg/Vec3>

using namespace mystatemachine;

class ARPaddleStateParameters : public mystatemachine::StateParameter
{
private:
    
    
    // The name of an object
    std::string     _object_name;
    
    // The location of an object
    osg::Vec3       _object_position;
    
    // The movement direction of an object
    osg::Vec3       _movement_direction;

    
public:
    
    ARPaddleStateParameters(std::string object_name, osg::Vec3 position, osg::Vec3 direction):
        StateParameter(object_name), _object_name(object_name), _object_position(position), _movement_direction(direction){};
    
    
    /*!
     Get the current location of an object. 
     @return osg::Vec3 with the object location
     */
    osg::Vec3 object_position(void) {return _object_position;}
    
    /*!
     Get the current movement direction of an object.
     @return osg::Vec3 with the current movement direction.
     */
    osg::Vec3 movement_direction(void) {return _movement_direction;}
    
    /*!
     The name of the object
     @return: a string with the name of the object. 
     */
    std::string object_name(void){return _object_name;}
    
};