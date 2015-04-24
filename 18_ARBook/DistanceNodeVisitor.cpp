//
//  DistanceNodeVisitor.cpp
//  AR-Class-Projects
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/2/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//

#include "DistanceNodeVisitor.h"


//#define VERBOSE

DistanceNodeVisitor::DistanceNodeVisitor()
{
    setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
    
    reset();
}


DistanceNodeVisitor::~DistanceNodeVisitor()
{
   // delete _modelStack;
}

//virtual
const char*  DistanceNodeVisitor::className(void) const
{
    return "DistanceNodeVisitor";
}


//virtual
/*
void DistanceNodeVisitor::apply(osg::Group &node)
{
    enterNode(node);
    
    std::cout << "find group with name " << node.getName()  << " at leve " << _hierarchyLevel << std::endl;
    
    traverse(node);
    
    leaveNode(node);
}*/

//virtual
void DistanceNodeVisitor::apply(osg::Node &node)
{
    enterNode(node);
    
#ifdef VERBOSE
    std::cout << "find node with name " << node.getName()  << " at level " << _hierarchyLevel << std::endl;
#endif
    traverse(node);
    
    leaveNode(node);
    
}



//virtual
void DistanceNodeVisitor::apply(osg::Transform &node)
{
    enterNode(node);
    
    std::string classname = node.className();
    // An der Geode wird die Traversierung abgebrochen,
    // da sie keine Elemente mehr enth‰lt, die wichtig sind.
    if(_hierarchyLevel != 3)
    {
    
        osg::ref_ptr<osg::RefMatrix> matrix =  new osg::RefMatrix();
        node.computeLocalToWorldMatrix(*matrix,this);
        _modelStack.push_back(matrix);

#ifdef VERBOSE
    std::cout << "found transform node with name " << node.getName()  << " at level " << _hierarchyLevel << std::endl;
#endif
    }
    else
    {
#ifdef VERBOSE
         std::cout << "found ARToolkitNode node with name " << node.getName()  << " at level " << _hierarchyLevel << std::endl;
#endif
    }
    traverse(node);
    
    leaveNode(node);
}


//virtual

void DistanceNodeVisitor::apply(osg::Geode &node)
{
   
#ifdef VERBOSE    
    std::cout << "find geode of node " << std::endl;
#endif

}



void DistanceNodeVisitor::enterNode(osg::Node& node)
{
    if(_hierarchyLevel == 0)
    {
        reset();
    }
    
    if(_hierarchyLevel == 1) // create new path list
    {
        if (!_modelStack.empty())
        {
            _modelStack.clear();
        }
#ifdef VERBOSE
        std::cout << "Create new list" << std::endl;
#endif
    }
    
    _lastNodeName = node.getName();
    
    _hierarchyLevel ++;
}

void DistanceNodeVisitor::leaveNode(osg::Node& node)
{

    _hierarchyLevel --;
    
    if(_hierarchyLevel == 0) 
    {
#ifdef VERBOSE
        std::cout << "Found " << _modelList.size() << " branches" << std::endl;
#endif
    }
    else if(_hierarchyLevel == 1)
    {
        // Save the old list
        if(_modelStack.size() > 0)
        {
            _modelList.push_back(_modelStack);
            _modelNameList.push_back(_lastNodeName);
        }
#ifdef VERBOSE
        std::cout << "Save list" << std::endl;
#endif
    }
}



void DistanceNodeVisitor::reset(void)
{
    
   if(!_modelList.empty())
   {
       _lastNodeName = "";
       _modelList.clear();
       _modelNameList.clear();
   }
}


osg::Vec3 DistanceNodeVisitor::getLocation(int idx)
{
    if(idx < _modelList.size() && idx >= 0 )
    {
        osg::Vec3d location = osg::Vec3d(0.0,0.0,0.0);
        
        MatrixStack modelStack = _modelList[idx];
        
        MatrixStack::iterator itr = modelStack.begin();
        osg::Matrixd matrix, result;
        matrix.identity();
        result.identity();
        while (itr != modelStack.end())
        {
            matrix = (*(*itr).get());
            result = matrix * result;
            itr++;
        }
        location = result.getTrans();
        return location;
    }
    return osg::Vec3d(0.0,0.0,0.0);
}

std::string DistanceNodeVisitor::getName(int idx)
{
    if(idx < _modelNameList.size() && idx >= 0 )
    {
        return _modelNameList[idx];
    }
    
    return "";

}


/*
 Returns the number of objects found.
 */
int DistanceNodeVisitor::size(void)
{
    return _modelList.size();
}

