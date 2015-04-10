//
//  ExampleGeometry.h
//  HCI571_MarkerTracking_OSG
//
//  Created by Rafael Radkowski on 4/9/15.
//
//
#pragma once

#include <stdio.h>

#include <osg/Group>
#include <osg/Node>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Material>

using namespace osg;

class ExampleGeometry
{
    
public:
    
    static osg::Group* createBox(float size, Vec4 color, Vec3 location );


};