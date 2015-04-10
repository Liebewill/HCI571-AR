//
//  ContourRenderer.h
//  HCI571_Extended_Occlusion_OSG
//
//  Created by Rafael Radkowski on 2/18/14.
//
//
#pragma once

#include <stdio.h>

#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/Texture>
#include <osg/Switch>
#include <osg/Texture2D>
#include <osg/Vec2>
#include <osg/Vec3>
#include <osgDB/ReadFile>
#include <osg/NodeCallback>
#include <time.h>
#include <osg/Material>
#include <string>
#include <map>
#include <osg/Camera>


#include "GLSL_ShaderLoader.h"


class ContourRenderer : public osg::Camera
{
private:
    
    // Geode and geometry object for the background
    osg::ref_ptr<osg::Geode> _geodeBackground;
    osg::ref_ptr<osg::Geometry> _geometryBackground;
    
    //background vertices
    osg::ref_ptr<osg::Vec3Array> _verticesBackground;
    osg::ref_ptr<osg::DrawArrays> _drawArrayBackground;
    
    // Background colol
    osg::ref_ptr<osg::Vec4Array> _colorBackground;
    
    // and normal vector
    osg::ref_ptr<osg::Vec3Array>  _background_Normal;
    

    
    // screen size
    int _screenX;
    int _screenY;


    
protected:

    //start position and size of the quad
    osg::Vec2			_startRect;
    double				_width;
    double				_height;
    
    
    // texture to render to and to use for rendering of flag.
    osg::Texture2D*             _prendertexture ;
    
private:

    osg::Texture2D*  _texture;
    osg::ref_ptr<osg::Vec3Array>	_texCoordArray;
    
    
    
    ContourRenderer();
    
    /*!
     Init the geometry.
     */
    void init(void);
    
    /*!
     Init the camera which is required to render the texture as 2D object
     */
    void initCamera(void);
    
    /*!
     Load and init the shader programs.
     */
    void initShaderProgram(void);
    
    /*!
     Set the texture to the rendering quad
     */
    void setTextureToRect(osg::Texture2D* texture);
    
public:
    /*!
     Constructor
     @param: startRect - the start position of the renderer quad. 
     @param: height, width: the width and the height of the renderer texture. 
     @param: prerendertexture: the texture with the object, in which we would like to find contours.
     
     */
    ContourRenderer( osg::Vec2 startRect, double width, double height, osg::Texture2D* prendertexture);

    
    ~ContourRenderer(){;};
    
    


    


    
    
};