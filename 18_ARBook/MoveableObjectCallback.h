//
//  MoveableObjectCallback.h
//  HCI571X-CollisionDetectionBasic
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/1/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//
#pragma once
#include <iostream>

#include "osg/Vec3"
#include "osg/Matrixd"
#include "osg/NodeCallback"
#include "osgText/Text"
#include <osgUtil/LineSegmentIntersector>
#include <osg/MatrixTransform>


class MoveableObjectCallback : public osg::NodeCallback
{
private:
    bool                _collision_down;
    
    osg::Vec3d          _current_Position;
    osg::Vec3d          _new_Position;
    osg::Vec3d          _final_Position;
    osg::Vec3d          _final_Up;
    
    osg::Matrixd        _moveable_object_matrix;
    
    // This contains the group of intersectable objects
    osg::Group*         _intersection_group;
    
    // A pointer that refers to the object which should be moved. 
    osg::MatrixTransform* _moveable_object;
    
    
    osg::BoundingSphere     _boundingSphere;
    
    // Enables or disables the callback function
    bool                _enabled;
    
    /*
     Invoke the ray-intersection test
     @param start: start position of the ray
     @param end: end position of the intersection ray
     @param intersection: returns the first intersection
     @param normal: returns the surface normal
     */
	bool intersect(const osg::Vec3d& start, const osg::Vec3d& end, osg::Vec3d& intersection, osg::Vec3d& normal) const;
    
    
    /*
     This function calculates the next step
     */
    bool computeIntersections(void);
    
    /*
     Traverse the colider scene graph and check for bounding sphere intersections
     */
    bool computeCollisions(osg::Group* node);
    
    
public:
    /*
     @param intersection_Group: the part of the scene graph which intersections should be checked
     @param moveable_object: a osg::MatrixTransform node that keeps the object which should be moved. 
     */
    MoveableObjectCallback(osg::Group* intersection_group, osg::MatrixTransform* moveable_object);
    
    
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);	 

    /*
     Enables or disables the callback function for intersection
     */
    void setEnableIntersection(bool value = true);
};