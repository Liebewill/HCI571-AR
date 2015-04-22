//
//  ExampleGeometry.cpp
//  HCI571_MarkerTracking_OSG
//
//  Created by Rafael Radkowski on 4/9/15.
//
//

#include "ExampleGeometry.h"



osg::Group* ExampleGeometry::createBox(float size, Vec4 color, Vec3 center )
{
    osg::Group* group = new osg::Group();
    
    osg::Geode* geode = new osg::Geode();
    

    osg::ShapeDrawable* unitCubeDrawable = new osg::ShapeDrawable(new osg::Box( center, size));
    
    
    osg::Material* material = new osg::Material();
    material->setDiffuse(osg::Material::FRONT_AND_BACK, color);
    
    unitCubeDrawable->getOrCreateStateSet()->setAttributeAndModes(material);
    unitCubeDrawable->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);
    
    
    geode->addDrawable(unitCubeDrawable);
    group->addChild(geode);
    
    return group;
}