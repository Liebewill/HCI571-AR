//
//  CreatePrerenderSubgraph.cpp
//  HCi571X-ARToolkit
//
//  Created by Dr.-Ing. Rafael Radkowski on 3/6/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//

#include "PrerenderSubgraph.h"




void  PrerenderSubgraph::create(osg::Group *subgraph,
                                            int width, int height,
                                            const osg::Vec4 &clearColor,
                                            osg::Camera::RenderTargetImplementation renderImplementation,
                                            osg::Texture2D* destination)
{
    
    // Set the clear mask; it determines what memories get cleared every frame
	this->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set the clear color
	this->setClearColor(clearColor);
    
    // Set a default projection matrix and a default view matrix.
    // Note, this settings are replaced  
    this->setProjectionMatrixAsPerspective(1.57, 0.7, 0.01, 10000000.0);
    this->setViewMatrixAsLookAt(osg::Vec3d(1.0f, 0.0f, 0.0f), osg::Vec3d(0.0f, 0.0f, 0.0f), osg::Vec3d(0.0f, 0.0f, 1.0f));
    
    // Set the coordinate reference frame
    this->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    
    // Set the viewport for the texture; it determines the region of the texture in which the
    // prerenderer can render the image
	this->setViewport(0,0,width, height);
    
    // The sequence of rendering
	this->setRenderOrder(osg::Camera::PRE_RENDER);
    
    // The target implementation specifies how the target texture is realized on the graphics card.
    // It can be implemented as frame buffer memory or as a frame buffer object etc.
    this->setRenderTargetImplementation(renderImplementation);
    
    // Attach the target texture to a memory. 
	this->attach(osg::Camera::COLOR_BUFFER, destination);
    
    // Culling is deactivated. You can activate it. Sometimes, the culling cause some problems.
    this->setCullingActive(false);
    
    // Here we add the subgraph to the second camera
    this->addChild(subgraph);
    
}


void PrerenderSubgraph::attachView(osg::Camera* camera)
{
    _cb = new PrerenderSubgraphCallback(camera, this); 
    this->setProjectionMatrix(camera->getProjectionMatrix());
    this->addUpdateCallback(_cb);
}