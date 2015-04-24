//
//  ModelHandler.h
//  HCI571X-ARPaddle
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/3/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//

#pragma once

#include <iostream>
#include <map>
#include <string>
#include <strstream>
#include <osg/MatrixTransform>
#include "MoveableObject.h"

#include "ARTagNode.h"

class ModelHandler
{
private:
    
    
    
    
    
public:
    
    /*
     Returns the model reference
     */
    static MoveableObject* getModel(std::string model_identifier);
    
    /*
     Adds a model to the static model list;
     */
    static bool addModel(std::string model_identifier, MoveableObject* model, ARTagNode* pattern);
    
    
    static bool setObjectToWorld(std::string model_identifier, osg::Vec3 location);
    
    
    static bool setObjectToPaddle(std::string model_identifier);
    
    static std::string copyObjectToPaddle(std::string model_identifier);

    static bool moveObject(std::string model_identifier, osg::Vec3 direction, double distance);
    
    static bool selectObject(std::string model_identifier);

	/*!
	Set the id of the world reference marker
	*/
	static void setWorldRefMarkerPtr(int pattern);

		/*!
	Set the id of the paddle marker
	*/
	static void setPaddleRefMarkerPtr(int pattern);
};