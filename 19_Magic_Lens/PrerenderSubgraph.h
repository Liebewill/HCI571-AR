//
//  CreatePrerenderSubgraph.h
//  HCi571X-ARToolkit
//
//  Created by Dr.-Ing. Rafael Radkowski on 3/6/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//
#pragma once
#include <iostream>

#include <osg/Group>
#include <osg/Node>
#include <osg/TexGen>
#include <osg/Texture2D>
#include <osg/Camera>

#include <osg/GLExtensions>
#include <osg/Node>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osg/Stencil>
#include <osg/ColorMask>

#include <osg/NodeCallback>




class PrerenderSubgraph : public osg::Camera
{

    ////////////////////////////////////////////////////
    // Update Callback
    // The task of this callback is to update the camera position
    // of th
    class PrerenderSubgraphCallback : public osg::NodeCallback
    {
    private:
        // The main camera that controls the view onto the hidden scene
        osg::Camera*    _mainCamera;
        
        // A reference to the super class
        PrerenderSubgraph*   _parent;
        
        
    public:
        
        PrerenderSubgraphCallback(osg::Camera* mainCamera, PrerenderSubgraph* parent):
            _mainCamera(mainCamera), _parent(parent)
        {
        
        }
        
        /*
         The  update function.
         */
        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
        {
            // This line updates the view
			_parent->setViewMatrix(_mainCamera->getViewMatrix());
            
            nv->traverse(*node);
        };
    };
    
    
    PrerenderSubgraphCallback* _cb;
    
public:
    
   /*
    Create a prerender subgraph.
    @subgraph:  the subgraph branch
    @width      the texture width
    @heigt      the texture height.
    @clearColor the clearcolor that cleans the rendering
    @targetImplementation   determines the method that is used to implement the texture in memory.
    @destination    the render target texture
    */
   void  create(osg::Group *subgraph,int width, int height,
                          const osg::Vec4 &clearColor,
                          osg::Camera::RenderTargetImplementation renderImplementation,
                          osg::Texture2D* destination);


    /*!
     For continuous update of the camera location, attach a camera to the offscreen render cam. 
     @camera       a camera that poses the main view to the scene
     */
    void attachView(osg::Camera* camera);
    
};