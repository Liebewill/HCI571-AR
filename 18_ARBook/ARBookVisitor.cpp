//
//  ARBookVisitor.cpp
//  HCI571X-ARPaddle
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/10/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//

#include "ARBookVisitor.h"



ARBookVisitor::ARBookVisitor(osg::Matrixf* worldReferenceSystem):
    _worldReferenceSystem(worldReferenceSystem)
{
    setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
    _hierarchyLevel = 0;
    reset();
}


ARBookVisitor::~ARBookVisitor()
{
    // delete _modelStack;
}

//virtual
const char*  ARBookVisitor::className(void) const
{
    return "ARBookVisitor";
}


//virtual
/*
 void ARBookVisitor::apply(osg::Group &node)
 {
 enterNode(node);
 
 std::cout << "find group with name " << node.getName()  << " at leve " << _hierarchyLevel << std::endl;
 
 traverse(node);
 
 leaveNode(node);
 }*/

//virtual
void ARBookVisitor::apply(osg::Node &node)
{
    enterNode(node);
    
#ifdef VERBOSE
    std::cout << "find node with name " << node.getName()  << " at level " << _hierarchyLevel << std::endl;
#endif
    traverse(node);
    
    leaveNode(node);
    
}



//virtual
void ARBookVisitor::apply(osg::Transform &node)
{
    enterNode(node);
    
    std::string classname = node.className();
    // An der Geode wird die Traversierung abgebrochen,
    // da sie keine Elemente mehr enth‰lt, die wichtig sind.
    //if(_hierarchyLevel != 3)
    {
        
        osg::ref_ptr<osg::RefMatrix> matrix =  new osg::RefMatrix();
        node.computeLocalToWorldMatrix(*matrix,this);
        _modelStack.push_back(matrix);
        
#ifdef VERBOSE
        std::cout << "found transform node with name " << node.getName()  << " at level " << _hierarchyLevel << std::endl;
#endif
    }
   // else
    {
#ifdef VERBOSE
    //    std::cout << "found ARToolkitNode node with name " << node.getName()  << " at level " << _hierarchyLevel << std::endl;
#endif
    }
    traverse(node);
    
    leaveNode(node);
}


//virtual

void ARBookVisitor::apply(osg::Geode &node)
{
    
#ifdef VERBOSE
    std::cout << "find geode of node " << std::endl;
#endif
    
}



void ARBookVisitor::enterNode(osg::Node& node)
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

void ARBookVisitor::leaveNode(osg::Node& node)
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



void ARBookVisitor::reset(void)
{
    
    if(!_modelList.empty())
    {
        _lastNodeName = "";
        _modelList.clear();
        _modelNameList.clear();
    }
}


osg::Vec3 ARBookVisitor::getLocation(int idx)
{
    if(idx < _modelList.size() && idx >= 0 )
    {
        _tempMatrix.identity();
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
        _tempMatrix = result;
        location = result.getTrans();
        return location;
    }
    return osg::Vec3d(0.0,0.0,0.0);
}

osg::Matrixd ARBookVisitor::getWorldMatrix(int idx)
{
    if(idx < _modelList.size() && idx >= 0 )
    {
        _tempMatrix.identity();
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

        return result;
        
    }
    return osg::Matrix::identity();
}


std::string ARBookVisitor::getName(int idx)
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
int ARBookVisitor::size(void)
{
    return _modelList.size();
}

