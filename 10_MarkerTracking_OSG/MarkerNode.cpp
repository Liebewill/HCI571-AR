/*
 *  ARToolKitMatrix.cpp
 *  ArtisZee_App
 *
 *  Created by Dr.-Ing. Rafael Radkowski on 19.02.11.
 *  Copyright 2011 Heinz Nixdorf Institute. All rights reserved.
 *
 */

#include "ARToolKitNode.h"

ARToolKitNode::ARToolKitNode(ObjectInfo* trackingData)
{
    
    _trackingData = trackingData;
    

	_matrix = &trackingData->matrix;
    _cf = &trackingData->cf;

	this->setMatrix(*_matrix);
	
    this->_switch = new osg::Switch();
    this->osg::MatrixTransform::addChild(this->_switch);
    
	_cb = new ARToolKitNodeCB(this);
	this->setUpdateCallback(_cb);
    
   
}


/*
 This functions override the "standard" addChild method
 in order to ensure that the added object will be attached
 to the switch-Node of the class and not to the MatrixTransform-Node. 
 */
bool ARToolKitNode::addChild(osg::Node* node)
{
    if(node == NULL)return false;
    
    this->_switch->addChild(node);
    
    return true;
}

bool ARToolKitNode::addChild(osg::Group* node)
{
    if(node == NULL)return false;
    
    this->_switch->addChild(node);
    
    return true;
}

bool ARToolKitNode::addChild(osg::Switch* node)
{
    if(node == NULL)return false;
    
    this->_switch->addChild(node);
    
    return true;
}

bool ARToolKitNode::addChild(osg::MatrixTransform* node)
{
    if(node == NULL)return false;
    
    this->_switch->addChild(node);
    
    return true;
}


/*!
 Removes a Node from the Scenegraph
 */
bool ARToolKitNode::removeChild(osg::Node* node)
{
    if(node == NULL)return false;
    
    this->_switch->removeChild(node);
    
    return true;
}

bool ARToolKitNode::removeChild(osg::Group* node)
{
    if(node == NULL)return false;
    
    this->_switch->removeChild(node);
    
    return true;
}

bool ARToolKitNode::removeChild(osg::Switch* node)
{
    if(node == NULL)return false;
    
    this->_switch->removeChild(node);
    
    return true;
}

bool ARToolKitNode::removeChild(osg::MatrixTransform* node)
{
    if(node == NULL)return false;
    
    this->_switch->removeChild(node);
    
    return true;
}

/*!
 Returns the number of Children on this patt
 */
int ARToolKitNode::getNumChildrenOnPatt(void)
{
    return this->_switch->getNumChildren();
}
