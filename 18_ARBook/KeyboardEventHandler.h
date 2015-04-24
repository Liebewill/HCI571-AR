//
//  KeyboardEventHandler.h
//  HCI571X-CollisionDetectionBasic
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/1/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//
#pragma once
#include <string>

#include "osgGA/GUIEventHandler"

#include <osg/MatrixTransform>


class KeyboardEventHandler : public osgGA::GUIEventHandler
{
private:
    
    osg::MatrixTransform*   _translation;
	
public:
    
	KeyboardEventHandler(osg::MatrixTransform* translation)
	{
		_translation = translation;
	};
	

	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&)
	{
		
		switch(ea.getEventType())
		{
			case(osgGA::GUIEventAdapter::KEYDOWN):
			{
				int key = ea.getKey();
				switch(key)
				{
					case '1':
					{
						_translation->setMatrix(osg::Matrix::translate(0.0,0.0, 100.0));
						break;
					}
					
				}
				return true;
			}
                
                
			case(osgGA::GUIEventAdapter::KEYUP):
			{
            case 'a':
            case 'd':
                // Action
				break;
				return true;
			}
				
			default:
				return false;
		}
	}
	
    
};
