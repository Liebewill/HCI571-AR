//
//  ARLens.h
//  HCI571X-AR_Lens
//
//  The class ARLens implements an augmented reality lens function.
//  It renders a 3D model on a paddle and replaces the 3D model's color
//  by either a hidden 3D model or a video.
//
//  Created by Dr.-Ing. Rafael Radkowski on 3/12/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//
#pragma once

#include <iostream>

#include <iostream>
#include <iostream>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/Texture2D>
#include <osgViewer/Viewer>
#include <osgDB/ReaderWriter>
#include <osgDB/ReadFile>
#include "VideoTexCallback.h"
#include "GLSL_ShaderLoader.h"
#include "PrerenderSubgraph.h"


class ARLens : public osg::Group
{
private:
    //---------------------------------------------
    // Pointer that refers to the background video
    unsigned char*  _video;
    
    //---------------------------------------------
    // Width and height of the video
    int             _videoWidth;
    int             _videoHeight;
    
    
    
    /////////////////////////////////////////////////////////
    //
    // Attributes, if a GL quad is used to represent the AR lens
    
    //---------------------------------------------
    // Width and height of a ARlens quad in case
    // a Quad should be used to render the hidden scene
    int             _width;
    int             _height;
    
    // --------------------------------------------------
    // Attributes to generate a quad
    osg::ref_ptr<osg::Geode>            _geodeBackground;
	osg::ref_ptr<osg::Geometry>         _geometryBackground;
	osg::ref_ptr<osg::Vec3Array>        _verticesBackground;
	osg::ref_ptr<osg::DrawArrays>       _drawArrayBackground;
	osg::ref_ptr<osg::Vec4Array>        _colorBackground;
	osg::ref_ptr<osg::Vec3Array>        _background_Normal;
	std::string                         _nameRect;
    osg::Vec4                           _colorNormal;
    osg::Texture2D*                     _texture0;
	osg::ref_ptr<osg::Vec3Array>        _texCoordArray;
    
	// Start location of the quad
	osg::Vec2                           _startRect; 
    
    /////////////////////////////////////////////////////////
    //
    // Attributes, if a 3D model is loaded to represent the lens
    
    //---------------------------------------------
    // Width and height of a ARlens quad in case
    // a Quad should be used to render the hidden scene
    osg::Group*     _loadedLens;
    osg::Group*     _loadedLensFrame;
    
    std::string     _lens_center;
    std::string     _lens_frame;
    
    
    //---------------------------------------------
    // Pointer that refers to the prerender texture
    osg::Texture2D*                     _prendertexture ;

    //-------------------------------------------
    // The callback that subloads the video to the graphics hardware
    VideoTexCallback*                   _myCallback;
    
    
    /////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    ///
    /// OPERATIONS
    ///
    
    // Inits the video polygon
    void init(void);
    
    
    // This uploads the video into the graphics memory
    void initVideoUpload(void);
    
    
    /*!
     Calculate the logarithm to the basis of two.
     param double x:the input value
     return: Logartihm of basis two
     return = log2 x
     */
    double log2(double x);
    
    /*!*
     Calculates the next power of two for the passed number x.
     @param: the number, for which the value of two is seeked.
     */
    int mathNextPowerOf2(double x);

    
public:
    /*
     Create the AR Lens.
     @param video: pointer of type u char that refers to the video image;
     @param prerenderer: pointer that refers to the prerender texture
     @param width, height: the size of the GL Quad if a quad should be used to render the AR Lens
     @param videoWidth, videoHeight: the height and widht of the vidoe image ini pixels. 
     */
    ARLens(unsigned char* video, int videoWidth, int videoHeight, osg::Texture2D* prerenderer, int width, int height );
    
    /*
     Create the AR Lens.
     @param video: pointer of type u char that refers to the video image;
     @param prerenderer: pointer that refers to the prerender texture
     @param width, height: the size of the GL Quad if a quad should be used to render the AR Lens
     @param videoWidth, videoHeight: the height and widht of the vidoe image ini pixels.
     */
    ARLens(unsigned char* video, int videoWidth, int videoHeight, osg::Texture2D* prerenderer, std::string lens_center, std::string lens_frame );
    
   
    
};