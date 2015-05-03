//
//  ARPaddle.h
//  HCI571X-CollisionDetectionBasic
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/2/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//
#pragma once

#include <iostream>


#include <osg/Group>
#include <osg/MatrixTransform>
#include <osgDB/ReaderWriter>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/material>

#include "ARPaddleCallback.h"
#include "ARTagNode.h"

#include "ARPaddleTypes.h"




class ARPaddle : public osg::Group
{
private:
    
    std::string         _filename;
    
    osg::Group*         _loadedObject;
    
    
    // The node of the ARToolkit
    ARTagNode*			_marker_node;
    
    
    // This callback computes the intersections 
    ARPaddleCallback*   _cb;
    
    
    
    ARTagNode*			_worldReferenceSystemNode;
    
    //------------------------------------
    // Loads and inits the object
    bool init(ARTagNode* trackingData);
    
    
    
public:
    /*
     Constructor
     @param trackingData: the ARToolkit marker data that belongs to that paddle
     @param intersectionGroup, the group of objects that can be manipulated by this paddle
     @param arbook_group: the ARBook group
     @param worldReferenceSystem: the ARToolkti marker data that pose the world reference coordinate system
     */
    ARPaddle(ARTagNode* trackingData, osg::Group* intersection_group = NULL, ARTagNode* worldReferenceSystem = NULL);
    ARPaddle(ARTagNode* trackingData, osg::Group* intersection_group = NULL, osg::Group* arbook_group = NULL,  ARTagNode* worldReferenceSystem = NULL);
    ~ARPaddle();
    
};