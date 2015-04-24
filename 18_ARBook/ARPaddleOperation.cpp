//
//  ARPaddleOperation.cpp
//  HCI571X-ARPaddle
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/3/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//

#include "ARPaddleOperation.h"
#include "ARPaddleStateParameters.h"
#include "ModelHandler.h"


std::string g_state = "";

ARPaddleOperation::ARPaddleOperation()
{
    _picked_object = "";
    _posInit = false;
    _lastPaddleLocation = osg::Vec3d(-1.0, -1.0, -1.0);

}


//virtual
void ARPaddleOperation::operate(std::string stateIdentifier, StateParameter* param )
{

    
    
    if(stateIdentifier.compare("Pick") == 0)
    {
        if(param != NULL)
        {
            ARPaddleStateParameters* p = (ARPaddleStateParameters* )param;
            _picked_object = p->object_name();
            ModelHandler::getModel(_picked_object)->highlightObject();
            delete param;
        }
        
        g_state = "State: Pick";
    }
    else if(stateIdentifier.compare("Drag") == 0)
    {
        if(_picked_object.compare("") != 0)
        {
            ModelHandler::setObjectToPaddle(_picked_object);
        }
        g_state = "State: Drag";
    }
    else if(stateIdentifier.compare("Drop") == 0)
    {
        if(_picked_object.compare("") != 0)
        {
            if(param != NULL)
            {
                ARPaddleStateParameters* p = (ARPaddleStateParameters* )param;
                ModelHandler::setObjectToWorld(_picked_object, p->object_position());
                delete param;
            }
           
        }
        g_state = "State: Drop";
        
    }
    else if(stateIdentifier.compare("IDLE") == 0)
    {
        if(_picked_object.compare("") != 0)
            ModelHandler::getModel(_picked_object)->highlightObject(false);
        
        _picked_object = "";
        
        g_state = "State: IDLE";
    }
    else if(stateIdentifier.compare("Push") == 0)
    {
        
         if(param != NULL)
        {
            
            
            ARPaddleStateParameters* p = (ARPaddleStateParameters* )param;
            _picked_object = p->object_name();
            
            if(_posInit == false)
            {
                _lastPaddleLocation = p->object_position();
                _posInit = true;
            }
            
            osg::Vec3 currentPosition = p->object_position();
            osg::Vec3 delta = currentPosition - _lastPaddleLocation;
            
            ModelHandler::getModel(p->object_name())->highlightObject(true);
            ModelHandler::moveObject(_picked_object, p->movement_direction(), 2.0);
            
            
            _lastPaddleLocation = p->object_position();
        }
        g_state = "State: Push";
    }
    else if(stateIdentifier.compare("MagicBookPick") == 0)
    {
        if(param != NULL)
        {
            ARPaddleStateParameters* p = (ARPaddleStateParameters* )param;
            std::string new_object = p->object_name();
        
            _picked_object = ModelHandler::copyObjectToPaddle(new_object);
            
            g_state = "State: ARBook-get Model ";
            g_state.append(new_object);
        }
    }
    else if(stateIdentifier.compare("Swipe") == 0)
    {

        ModelHandler::selectObject("Teapot2");
            
        g_state = "State: Swipe";
 
    }

}