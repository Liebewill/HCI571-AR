//
//  MoveableObject.h
//  HCI571X-CollisionDetectionBasic
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/1/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//
#pragma once

#include <iostream>


#include <osg/Group>
#include <osg/MatrixTransform>
#include <osgDB/ReaderWriter>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/Material>

#include "MoveableObjectCallback.h"



class MoveableObject : public osg::MatrixTransform
{
protected:
    
    std::string         _filename;
    
    osg::Group*         _loadedObject;
    
    
    //---------------------------------------------
    // Material for highlighting and selecting
    osg::ref_ptr<osg::Material>                 _material_highlight;
    osg::ref_ptr<osg::Material>                 _material_selected;
    
    
    // This callback computes the intersections and
    // adapt the object's position
    MoveableObjectCallback* _cb;
    
    
    osg::Group*             _intersection_group;
    
    //------------------------------------
    // Loads and inits the object
    bool init(void);
    
    bool                    _selected;
    
public:
    MoveableObject(MoveableObject& object, osg::CopyOp& opt );
    MoveableObject(std::string filename, osg::Group* intersection_group = NULL);
    ~MoveableObject();

    /*
     Highlights the object by changing the color
     @param enable = true highlights the object
     */
    bool highlightObject(bool enable = true);
    
    
    bool selectObject(bool enable = true);
    
    
    /*
     Enables or disables the callback function for intersection
     */
    void setEnableIntersection(bool value = true);
    
    /*!
     Returns the filename of the object
     */
    std::string getFilename(void){return _filename;};
    
    osg::Group* getIntersectionGroupPtr(void){return _intersection_group;}

};