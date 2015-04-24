//
//  ARBook.h
//  HCI571X-ARPaddle
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/8/13.
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

#include "ARTagNode.h"
#include "MoveableObject.h"

class ARBook : public osg::Group
{
private:
    
    osg::Group* _worldRefObject;
    
    
    void init(void);
    
    
public:
    
    ARBook(osg::Group* worldRefObject);
    
    osg::Group* addARBookPattern(int pattID, std::string filename, std::string label, double x, double y, double z);
    
    



};