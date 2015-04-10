//
//  VideoRenderer.h
//  HCi571X-ARToolkit
//
//  Created by Dr.-Ing. Rafael Radkowski on 2/20/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//
#pragma once

#include <iostream>


#include <iostream>
#include <math.h>

#include <osg/Group>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osg/Vec3>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>


#ifdef WIN32
#include <windows.h>
#endif

//---------------------------------------------
// Includes the video callback device
#include "VideoTexCallback.h"

//---------------------------------------------
// A shader loader object
#include "GLSL_ShaderLoader.h"



class VideoRenderer
{
public:
    
    static osg::Geode* createBackgroundObject(void);
    
    
    static double getRadius(double x, double y);
    
    
    static bool radialCalibration(double k0, double k1);
    
#ifdef COCOA // Only for Mac OS X Cocoa users 
    static void getScreenResolution (int& screenX, int& screenY);
#endif
    
    
    /*!
     This function adapts the texture coordinates to meet the field of view and the
     apsect ratio of the screen.
     */
    static double adaptTexCoordToFieldOfView(double fov_display_h, double fov_display_v,
                                      double fov_camera_h, double fov_camera_v,
                                      int& video_w, int& video_h,
                                      double& tex_size_x, double& tex_size_y,
                                      double& offset_x, double& offset_y);
    
    
    
    /*!
     This function creates the texture coordinates for the quads.
     The texture coordinates pose the undistorted set, which
     maps a texture either streched nor compressed onto the quad.
     */
    static void setTextureToRect(osg::Texture2D* texture, int videoWidth, int videoHeight, int textureWidth, int textureHeight);
    
        
    
    /*!
     This function sets a new texture coordinate.
     The index cannot be larger than the specified number of coordinates.
     @param i the column (width);
     @param j the row (height)
     @param tx the x coordinate (width)
     @param ty the y coordinate (height)
     */
    static bool setTextureCoordinateAt(int i, int j, double tx, double ty);
    
    
    static osg::Vec2 getTextureCoordinateAt(int i, int j);
    
    
    static osg::Group *createSphere(int radius);
    
    
    static osg::Group* createSzene(unsigned char* video, int videoWidth, int videoHeight );
    
};