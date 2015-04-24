//
//  DistanceNodeVisitor.h
//  AR-Class-Projects
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/2/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//
#pragma once

#include <iostream>


#include <osg/Group>
#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/MatrixTransform>
#include <osg/Transform>
#include <osg/Switch>

#include <list>
#include <vector>

class DistanceNodeVisitor : public osg::NodeVisitor
{
private:
    
    int              _hierarchyLevel;
    
    std::string      _lastNodeName;
    
public:
    DistanceNodeVisitor();
    ~DistanceNodeVisitor();
    
    virtual const char *  className(void) const;
 	
    
   // virtual void apply(osg::Group &node);
    virtual void apply(osg::Node &node);
    virtual void apply(osg::Transform &node);
    virtual void apply(osg::Geode &node);

    
    osg::Vec3 getLocation(int idx);
    std::string getName(int idx);
    
    /*
     Returns the number of objects found.
     */
    int size(void);
    
protected:
    void enterNode(osg::Node& node);
    void leaveNode(osg::Node& node);
    
    void reset(void);
    
    typedef std::list< osg::ref_ptr<osg::RefMatrix> > MatrixStack;
    MatrixStack                 _modelStack;
    
    
    typedef std::vector< MatrixStack > ModelMatrixList;
    ModelMatrixList             _modelList;
    
    typedef std::vector< std::string > ModelNameList;
    ModelNameList               _modelNameList;
};