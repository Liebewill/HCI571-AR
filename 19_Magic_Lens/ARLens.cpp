//
//  ARLens.cpp
//  HCI571X-AR_Lens
//
//  Created by Dr.-Ing. Rafael Radkowski on 3/12/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//

#include "ARLens.h"



ARLens::ARLens(unsigned char* video, int videoWidth, int videoHeight ,  osg::Texture2D* prerenderer,  int width, int height):
    _video(video), _videoWidth(videoWidth), _videoHeight(videoHeight), _prendertexture(prerenderer), _width(width), _height(height)
{
    this->_startRect.x() = - _width/2;
	this->_startRect.y() = - _height/2;
    init();
    initVideoUpload();
}


ARLens::ARLens(unsigned char* video, int videoWidth, int videoHeight, osg::Texture2D* prerenderer, std::string lens_center, std::string lens_frame ):
    _video(video), _videoWidth(videoWidth), _videoHeight(videoHeight), _prendertexture(prerenderer), _lens_center(lens_center), _lens_frame(lens_frame)
{
    _width = 1920;
	_height = 1080;
    this->_startRect.x() = - _width/2;
	this->_startRect.y() = - _height/2;
    
    _loadedLens = (osg::Group*) osgDB::readNodeFile(_lens_center);
    this->addChild(_loadedLens);
    
    _loadedLensFrame = (osg::Group*) osgDB::readNodeFile(_lens_frame);
    this->addChild(_loadedLensFrame);
    
    initVideoUpload();
}


void ARLens::init()
{
    
	this->_colorNormal = osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    
    
	//*************************************
	// background
    
	this->_geodeBackground = new osg::Geode();
	this->_geometryBackground = new osg::Geometry();
	
	// Vertices Hintergrund
	this->_verticesBackground = new osg::Vec3Array();
	_verticesBackground->push_back(osg::Vec3(_startRect.x(), _startRect.y(), 0.0));
	_verticesBackground->push_back(osg::Vec3(_startRect.x(), _startRect.y() + _height, 0.0));
	_verticesBackground->push_back(osg::Vec3(_startRect.x() + _width, _startRect.y() + _height, 0.0));
	_verticesBackground->push_back(osg::Vec3(_startRect.x() + _width, _startRect.y(), 0.0));
    
	_geometryBackground->setVertexArray(_verticesBackground.get());
    
	// Farbe f¸r den Hintergrund
	this->_colorBackground = new osg::Vec4Array;
	_colorBackground->push_back(osg::Vec4(0.9f, 0.9f, 0.9f, 0.6f));
    
	this->_drawArrayBackground = new osg::DrawArrays(GL_QUADS, 0, this->_verticesBackground->size());

	_geometryBackground->addPrimitiveSet(_drawArrayBackground.get());
	_geometryBackground->setColorArray(this->_colorBackground.get());
	_geometryBackground->setColorBinding(osg::Geometry::BIND_OVERALL);
    
	_background_Normal = new osg::Vec3Array();
	_background_Normal->push_back(osg::Vec3(0.0, 0.0, 1.0));
    
	_geometryBackground->setNormalArray(_background_Normal.get());
	_geometryBackground->setNormalBinding(osg::Geometry::BIND_OVERALL);
    
	_geodeBackground->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	_geodeBackground->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
	_geodeBackground->addDrawable(_geometryBackground.get());
    
	this->addChild(_geodeBackground.get());
    
    
    
}



/*!
 Calculate the logarithm to the basis of two.
 param double x:the input value
 return: Logartihm of basis two
 return = log2 x
 */
double ARLens::log2(double x)
{
    // Factor to convert a logarithm with basis 10 into a logarithm with basis 2.
    // Log(10)/Log(2)
    const double LG2 = 3.32192809489;
	return log10(x) * LG2;
}


/*!*
 Calculates the next power of two for the passed number x.
 @param: the number, for which the value of two is seeked.
 
 Procedure: the logarithm of the number x to the basis of 2 is calculated.
 Next, a value of 1 is added. The value z complies with the next power for x now
 The integer value is take (everythink before of the comma) and the value is converted back.
 
 */
int ARLens::mathNextPowerOf2(double x)
{
	int result = 0;
	int z = (log2(x)+1);
	result = pow(2.0,z);
	return result;
}



// This uploads the video into the graphics memory
void ARLens::initVideoUpload(void)
{
    /////////////////////////////////////////////////////////////////////
    ///// Create a texture for the video image
    _texture0 = new osg::Texture2D();
    _texture0->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR);
	_texture0->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR);
    _texture0->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_BORDER);
    _texture0->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_BORDER);
    _texture0->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_BORDER);
    
	// Set the texture size and height
	int texWidth = _videoWidth;
	int texHeight = _videoHeight;
    
	// set the texture size
	_texture0->setTextureSize(texWidth,texHeight);
    
    
	// Create the callback object that subloads the video to the graphics hardware.
	_myCallback = new VideoTexCallback(_video,
                                       _videoWidth,
                                       _videoHeight,
                                       texWidth,
                                       texHeight,
                                       GL_BGR);
    
	// Pass the callback to the texture.
	_texture0->setSubloadCallback(_myCallback);
    
    _loadedLens->getOrCreateStateSet()->setTextureAttributeAndModes(0, _texture0);
    // Create auniform variable that grants access to the texture.
	_loadedLens->getOrCreateStateSet()->addUniform( new osg::Uniform( "glSample0",0));
    
    _loadedLens->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
    _loadedLens->getOrCreateStateSet()->setRenderBinDetails(11, "RenderBin");
    
	////////////////////////////////////////////////////////////////////////////
	// Shader Program
	// This class loads a shader object and attaches is as stateset to the geode that
	// carries the video background
	GLSL_ShaderLoader* myShaderLoader = new GLSL_ShaderLoader(_loadedLens->getOrCreateStateSet(),
                                                              "arlens",
#ifdef WIN32
                                                              "../shader/arlens.vs",
                                                              "../shader/arlens.fs");
#else
    "../../shader/arlens.vs",
    "../../shader/arlens.fs");
#endif
    

	if(_prendertexture != NULL)
    {
        _loadedLens->getOrCreateStateSet()->setTextureAttributeAndModes(1, _prendertexture);
        // Create auniform variable that grants access to the texture.
        _loadedLens->getOrCreateStateSet()->addUniform( new osg::Uniform( "glSample1",1));
    }
    

    
}




