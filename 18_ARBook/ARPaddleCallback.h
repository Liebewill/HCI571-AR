//
//  ARPaddleCallback.h
//  HCI571X-CollisionDetectionBasic
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/2/13.
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

#include <Windows.h>


#include "DistanceNodeVisitor.h"

#include "ARPaddleTypes.h"
#include "FiniteStateMachine.h"
#include "ARPaddleStateParameters.h"
#include "ARPaddleOperation.h"
#include "ARBookVisitor.h"

// The marker nodes
#include "ARTagNode.h"

class ARPaddleCallback : public osg::NodeCallback
{
public:
    
    
private:
    
    // This contains the group of intersectable objects
    osg::Group*         _intersection_group;
    
    ////////////////////////////////////////////////////
    // The finite stata machine
    FiniteStateMachine*     _fsm;
    
    ////////////////////////////////////////////////////
    // Data for a behaviro model
    
    bool                _collision_down;
    
    osg::Vec3d          _current_Position;
    osg::Vec3d          _new_Position;
    osg::Vec3d          _final_Position;
    osg::Vec3d          _final_Up;
    
    osg::Matrixd        _moveable_object_matrix;
    
    // The AR marker node which refers to the pointer of the paddle object
    ARTagNode*			_marker_node;
    
	ARTagNode*			_worldReferenceSystemNode;

    
    // The world coordinate reference system
    osg::Matrixf*           _worldReferenceSystem;
    
    ////////////////////////////////////////////////////
    // For drat & drop functions
    
    // The distance visitor calculates the location to all
    // Geode objects in the scene graph.
    DistanceNodeVisitor*    _distVisitor;
    
    // Keeps the distance to all other objects in the scene. 
    typedef std::map<std::string, double> DistanceMap;
    DistanceMap             _objectDistance;
    
    // The location of the paddle in world reference coordinate system
    osg::Vec3               _worldRefLocation;
    osg::Vec3               _worldRefLocation_t1;
    double                  _t0, t1;
    
    // The direction of the paddle in world reference coordinate system
    osg::Vec3               _worldRefDirection;
    osg::Vec3               _worldRefDirection_t1;
    
    // The relation between the paddle and the world ref marker
    osg::Matrixd            _world_paddle_relation;
    
    // Array to save the z-Position of that particular object
    std::vector<double>     _zPos;
    double                  _lastPos;
    double                  _pattAngle;

    osg::BoundingSphere     _boundingSphere;
    
    ////////////////////////////////////////////////////
    // ARBook functionality
    
    // Pointer that refers to the AR Book subgraph
    osg::Group*             _ARBookGroup;
    
    // Indicates that an ARBook is in use
    bool                    _withARBook;
    
    // Visits the AR book and calculates the
    // object locations
    ARBookVisitor*          _arbookvisitor;
    
    
    ////////////////////////////////////////////////////
    // Swipe
    bool                    _swipeInit;
    osg::Vec3d              _lastLocation;
    double                  _ts0;
    double                  _ts1;
    int                     _lastVel;
    
    std::vector<int>        _speedVec;
    std::vector<int>        _dirVec;
    std::vector<int>        _speedVecRef;
    std::vector<int>        _dirVecRef;
    
    int                     _swipeCount;

	////////////////////////////////////////////////////
	// sample state
	bool _collided; 
    
    ////////////////////////////////////////////////////
    // Operation
    
    
    /*
     Invoke the ray-intersection test
     @param start: start position of the ray
     @param end: end position of the intersection ray
     @param intersection: returns the first intersection
     @param normal: returns the surface normal
     */
	bool intersect(const osg::Vec3d& start, const osg::Vec3d& end, osg::Vec3d& intersection, osg::Vec3d& normal, std::string& modelname) const;
    
    
    /*
     This function calculates the next step
     */
    bool computeIntersections(void);
    
    /*
     This function computes the distance to all other objects in the intersection group
     */
    bool computeDistance(std::string& result, std::string& object);
    
    /*
     Traverse the colider scene graph and check for bounding sphere intersections
     */
    bool computeCollisions(osg::Group* node);
    
    /*
     Calculates the Euclid distance between two vectors and returns a 
     symbolic distance value
     */
    double calcEuclidDistance(osg::Vec3d distA, osg::Vec3d distB);
    
    /*
     This function calculates the location of this paddle with respect to the
     world reference system. 
     */
    bool calcPaddleWorldLocation(void);
    
    /*
     Calculates the Euler coordinates from a quaternion 
     */
    void getEulerFromQuat(osg::Quat q, double& heading, double& attitude, double& bank);
    
    /*
     Calculates the dot product of two vectors. 
     */
    double dot_product(osg::Vec3d a,osg::Vec3d b );
    
    
    /*!
     Calcuates the cross product between two vectors
     */
    osg::Vec3 cross_product(osg::Vec3d a,osg::Vec3d b );
    
    
    /*
     Computes the leviation of that paddle. 
     Make sure you have the world location of that paddle
     before you invoke this function.
     @param worldRefLoc: the world reference location
     */
    bool computeLevitation(osg::Vec3  worldRefLoc, std::string& result);
    
    
    /*!
     This calculates the angle between the object and the paddle
     @param objectLoc: the location of the object in world coordinate or world reference coordinates
     @param paddleLoc: the location of the paddle in world coordinate or world reference corrdinates. 
     @param paddleDir: this is the desired dir of the paddle in world coordinate or world reference corrdinates. 
     @param angle: the output angle between the paddle and the 
     @param speed: the approaching speed angle between the direction vector and the speed vector
     */
    bool calcObjectAngle(osg::Vec3 objectLoc, osg::Vec3 paddleLoc, double* angle, double* speed);
    
    /*!
     This function calculates the distance to all ARBook objects. 
     @param result: a state machine message
     @param object: the object that has been detected;
     */
    bool computeDistanceARBook(std::string& result, std::string& object);
    
    /*
     Calculate a symbol that represents the current direction of a paddle
     */
    PattDirection codeDirection(osg::Quat dir);
    
    /*
     This encodes the speed of the AR Paddle
     */
    int codeSpeed(osg::Vec3 location);
    
    
    int codeMovementDirection(osg::Vec3 location);
    
    
    double signed_angle(osg::Vec3d a, osg::Vec3d b);
    
    /*
     Compute the swipe.
     */
    bool computeSwipe(osg::Vec3  worldRefLoc, osg::Vec3  worldRefDir,  std::string& result);
    
    /*
     Calculates the hamming distance between two vectors
     */
    unsigned hamdist(unsigned x, unsigned y);
    
    unsigned hamdistVector(std::vector<int> a, std::vector<int> b);
    
    /*
     Function to create the state machine.
     */
    void createStateMachine(void);
    
public:
    /*
     @param intersection_Group: the part of the scene graph which intersections should be checked
     @param marker: the AR marker node
     */
    ARPaddleCallback(osg::Group* intersection_group, ARTagNode* marker);
    
    
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
    
    /*
     Set the world reference system
     @param: the osg::Matrix object that acts as the reference coordinate system for this paddle. 
     */
    void setWorldCoordinateRefSystem(  ARTagNode*  worldReferenceSystemNode);
    
    /*!
     This adds the ARBook to this NodeCallback. 
     @param ARBookGroup  -pointer to the subscene graph that keeps the arbook objects
     */
    void setARBookGroup( osg::Group* ARBookGroup);
    
};