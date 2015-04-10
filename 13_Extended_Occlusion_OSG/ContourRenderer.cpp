//
//  ContourRenderer.cpp
//  HCI571_Extended_Occlusion_OSG
//
//  Created by Rafael Radkowski on 2/18/14.
//
//

#include "ContourRenderer.h"

#ifdef WIN32
#include <Windows.h>
#endif

ContourRenderer::ContourRenderer( osg::Vec2 startRect, double width, double height, osg::Texture2D* prendertexture):
    _startRect(startRect), _width(width), _height(height), _prendertexture(prendertexture)
{

    initCamera();
    init();
    setTextureToRect(_prendertexture);
    
    initShaderProgram();
}



ContourRenderer::ContourRenderer()
{
    
}


void ContourRenderer::initCamera(void)
{

#ifdef WIN32
    _screenX  = GetSystemMetrics(SM_CXSCREEN);
    _screenY  = GetSystemMetrics(SM_CYSCREEN);
#else
    //_screenX  = 1920;
    //_screenY  = 1200;
    _screenX  = 1280;
    _screenY  = 1024;
#endif
    //Set up camera
    this->setProjectionMatrix(osg::Matrix::ortho2D(0,_screenX,0,_screenY));
    
    // set the view matrix
    this->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    this->setViewMatrix(osg::Matrix::identity());
    
    //_camera->setClearColor(osg::Vec4(1.0, 1.0, 0.0, 0.7));
    // only clear the depth buffer
    this->setClearMask(GL_DEPTH_BUFFER_BIT);
    
    // draw subgraph after main camera view.
    this->setRenderOrder(osg::Camera::POST_RENDER);
}



void ContourRenderer::init()
{

    this->_texture = new osg::Texture2D();

    
    //*************************************
    // Background geometry
    
    this->_geodeBackground = new osg::Geode();
    this->_geodeBackground->setName("contour_renderer");
    this->_geometryBackground = new osg::Geometry();
    
    
    
    
    // Vertices Hintergrund
    this->_verticesBackground = new osg::Vec3Array();
    _verticesBackground->push_back(osg::Vec3(_startRect.x(), _startRect.y(),0.0));
    _verticesBackground->push_back(osg::Vec3(_startRect.x(), _startRect.y() + _height,0.0));
    _verticesBackground->push_back(osg::Vec3(_startRect.x() + _width, _startRect.y() + _height,0.0));
    _verticesBackground->push_back(osg::Vec3(_startRect.x() + _width, _startRect.y(),0.0));
    
    
    _geometryBackground->setVertexArray(_verticesBackground.get());
    
    
    // Background color / which does not really matter in this case.
    this->_colorBackground = new osg::Vec4Array;
    _colorBackground->push_back(osg::Vec4(0.9f, 0.9f, 0.9f, 0.6f));
    
    
    this->_drawArrayBackground = new osg::DrawArrays(GL_POLYGON, 0, this->_verticesBackground->size());
    
    
    _geometryBackground->addPrimitiveSet(_drawArrayBackground.get());
    _geometryBackground->setColorArray(this->_colorBackground.get());
    _geometryBackground->setColorBinding(osg::Geometry::BIND_OVERALL);
    
    _background_Normal = new osg::Vec3Array();
    _background_Normal->push_back(osg::Vec3(0.0, 0.0, 1.0));
    
    _geometryBackground->setNormalArray(_background_Normal.get());
    _geometryBackground->setNormalBinding(osg::Geometry::BIND_OVERALL);
    
    _geodeBackground->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    _geodeBackground->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::OFF);
    
    
    _geodeBackground->addDrawable(_geometryBackground.get());
    this->addChild(_geodeBackground.get());
    
    
}


void ContourRenderer::setTextureToRect(osg::Texture2D* texture)
{
    this->_texture = texture;
    
    this->_texCoordArray = new osg::Vec3Array();
    
    _texCoordArray->push_back(osg::Vec3(0.0, 0.0, 0.0));
    _texCoordArray->push_back(osg::Vec3(0.0, 1.0, 0.0));
    _texCoordArray->push_back(osg::Vec3(1.0, 1.0, 0.0));
    _texCoordArray->push_back(osg::Vec3(1.0, 0.0, 0.0));
    
    this->_geometryBackground->setTexCoordArray(0,_texCoordArray.get());
    _texture->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR);
    _texture->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR);
    
    this->_geometryBackground->getOrCreateStateSet()->setTextureAttributeAndModes(0,_prendertexture);
    
    _geometryBackground->dirtyDisplayList();
    
}



void ContourRenderer::initShaderProgram(void)
{

    
    // This class loads a shader object and attaches is as stateset to the geode that
    // carries the edge rendering
    GLSL_ShaderLoader* myShaderLoader2 = new GLSL_ShaderLoader(this->getOrCreateStateSet(),
                                                               "edge_shader",
#ifdef WIN32
                                                               "../shader/contour.vs",
                                                               "../shader/contour.fs");
#else
    "../shader/contour.vs",
    "../shader/contour.fs");
#endif
    
    osg::Uniform* radius2 = new osg::Uniform("radius", 2);
    this->getOrCreateStateSet()->addUniform(radius2);
    
    osg::Uniform* renderwidth2 = new osg::Uniform("renderwidth", 1024);
    this->getOrCreateStateSet()->addUniform(renderwidth2);
    
    
    
}



