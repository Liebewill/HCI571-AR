//
//  ARPaddle.cpp
//  HCI571X-CollisionDetectionBasic
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/2/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//

#include "ARPaddle.h"



ARPaddle::ARPaddle(ARTagNode* trackingData, osg::Group* intersection_group, ARTagNode* worldReferenceSystem )
{
    init(trackingData);
    
    if(intersection_group != NULL)
    {
        // Create a new callback and add it to this node
        _cb = new ARPaddleCallback(intersection_group, _marker_node);
        this->addUpdateCallback(_cb);
    }
    
    if(worldReferenceSystem != NULL)
    {
		_cb->setWorldCoordinateRefSystem(&worldReferenceSystem->getMatrix());
    }
}

ARPaddle::ARPaddle(ARTagNode* trackingData, osg::Group* intersection_group, osg::Group* arbook_group ,  ARTagNode* worldReferenceSystem )
{
    init(trackingData);
    
    if(intersection_group != NULL)
    {
        // Create a new callback and add it to this node
        _cb = new ARPaddleCallback(intersection_group, _marker_node);
        this->addUpdateCallback(_cb);
    }
    
    if(worldReferenceSystem != NULL)
    {
		_cb->setWorldCoordinateRefSystem(&worldReferenceSystem->getMatrix());
        
       
    }
    if(arbook_group != NULL)
    {
        _cb->setARBookGroup(arbook_group);
    }
   
}


ARPaddle::~ARPaddle()
{
    delete _cb;
    
}


//------------------------------------
// Loads and inits the object
bool ARPaddle::init( ARTagNode* trackingData)
{
    
    // Inits an ARToolkit node
    _marker_node = trackingData;

    this->addChild(_marker_node);
    
    
    // Create a small object
    osg::Geode* geode = new osg::Geode();
    geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f,0.0f,30.0f),60.0)));
    
    osg::Material* material = new osg::Material();
    material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(80,0.0,0.0,0.3));
    geode->getOrCreateStateSet()->setAttributeAndModes(material,osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    geode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    _marker_node->addChild(geode);
    
    return true;
}