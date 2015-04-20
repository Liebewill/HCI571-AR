//
//  MoveableObject.cpp
//  HCI571X-CollisionDetectionBasic
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/1/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//

#include "MoveableObject.h"



MoveableObject::MoveableObject(MoveableObject& object, osg::CopyOp& opt):
    osg::MatrixTransform(object, osg::CopyOp::DEEP_COPY_ALL)
{
    // The intersection group node has to be copied
    _intersection_group = object._intersection_group;
    
    if(_intersection_group != NULL)
    {
        // Create a new callback and add it to this node
        _cb = new MoveableObjectCallback(_intersection_group, this);
        this->addUpdateCallback(_cb);
    }
    

    //-------------------------------------------------------
    // Material for highlighting
    _material_highlight = new osg::Material();
    _material_highlight->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.6,0.6,0.0,1.0));
    _material_highlight->ref();
    
    _material_selected = new osg::Material();
    _material_selected->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.9,0.0,0.0,1.0));
    _material_selected->ref();
    
    _selected = false;
    
}


MoveableObject::MoveableObject(std::string filename, osg::Group* intersection_group):
    _filename(filename), _intersection_group( intersection_group)
{
    init();
    
    if(intersection_group != NULL)
    {
        // Create a new callback and add it to this node
        _cb = new MoveableObjectCallback(intersection_group, this);
        this->addUpdateCallback(_cb);
    }
}


MoveableObject::~MoveableObject()
{
    
    
}


//------------------------------------
// Loads and inits the object
bool MoveableObject::init(void)
{
    if(_filename.length() == 0) return false;
    
    // Load the object
    _loadedObject = (osg::Group*)osgDB::readNodeFile(_filename);

    
    if(_loadedObject == NULL) return false;
    
    // Add the child to the scene graph
    this->addChild(_loadedObject);
    
    
    //-------------------------------------------------------
    // Material for highlighting
    _material_highlight = new osg::Material();
    _material_highlight->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.6,0.6,0.0,1.0));
    _material_highlight->ref();
    
    _material_selected = new osg::Material();
    _material_selected->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.9,0.0,0.0,1.0));
    _material_selected->ref();
    
    _selected = false;
    
    return true;
}


/*
 Highlights the object by changing the color
 @param enable = true highlights the object
 */
bool MoveableObject::highlightObject(bool enable)
{
    if(enable)
        this->_loadedObject->getOrCreateStateSet()->setAttributeAndModes(_material_highlight.get(),osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    else if(enable == false)
        this->_loadedObject->getOrCreateStateSet()->setAttributeAndModes(_material_highlight.get(),osg::StateAttribute::OFF);


	return true;
}


bool MoveableObject::selectObject(bool enable)
{
    if(_selected == false)
        _selected = true;
    else
        _selected = false;
    
    if(_selected)
        this->_loadedObject->getOrCreateStateSet()->setAttributeAndModes(_material_selected.get(),osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    else if(_selected == false)
        this->_loadedObject->getOrCreateStateSet()->setAttributeAndModes(_material_selected.get(),osg::StateAttribute::OFF);
    
    
	return true;
}

/*
 Enables or disables the callback function for intersection
 */
void MoveableObject::setEnableIntersection(bool value )
{
    if(_cb != NULL)
    {
        _cb->setEnableIntersection(value);
    }
}
