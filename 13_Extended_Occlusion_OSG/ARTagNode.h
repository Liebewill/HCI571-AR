//
//  ARTagNode.h
//  HCI571_MarkerTracking_OSG
//
//  Created by Rafael Radkowski on 3/26/15.
//
//
#pragma once 

#include <stdio.h>

#include <osg/Group>
#include <osg/Node>
#include <osg/Switch>
#include <osg/Matrix>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osg/NodeCallback>


//The marker tracking code
#include "Marker.h"


class ARTagNode : public osg::Switch
{
private:
    
    // The transform node.
    osg::MatrixTransform*       _transform;
    
    osg::Matrix                 _matrix;
    osg::Matrix                 _offset;
    
    
    
    
    class ARTagNodeCallback : public osg::NodeCallback
    {
    private:
        
        int                     _marker_id;
        std::vector<Marker>&    _detectedMarkers;
        osg::Matrix&            _matrix;
        osg::Matrix&            _offset;
        osg::MatrixTransform*   _transform;
        
    public:
        
        ARTagNodeCallback(osg::MatrixTransform* _transform, int marker_id,  std::vector<Marker>& detectedMarkers, osg::Matrix& matrix ,osg::Matrix& offset);
        
        
        /*
         This is the update callback that copies the matrix information into the 
         node matrix.
         */
        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

        
    
    };
    
    
    ARTagNodeCallback*  _cb;
    
    
public:
    
    
    ARTagNode(int marker_id,  std::vector<Marker>& detectedMarkers, osg::Matrix offset);
    ~ARTagNode();
    
    
    
    
    virtual bool addChild(osg::Node* child );
    virtual bool addChild(osg::Group* child );
    virtual bool addChild(osg::Switch* child );
    virtual bool addChild(osg::MatrixTransform* child );


};