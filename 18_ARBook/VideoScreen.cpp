//
//  VideoScreen.cpp
//  ARToolkit-Marker Model Teaching
//
//  Created by Dr.-Ing. Rafael Radkowski on 18.08.11.
//  Copyright 2011 Heinz Nixdorf Institute. All rights reserved.
//

#include "VideoScreen.h"






class  MyVideoDrawCallback : public osg::Camera::DrawCallback
{
public:
    MyVideoDrawCallback(osg::Image* image):
    _image(image)
    {
    }
    
    virtual void operator () (const osg::Camera& /*camera*/) const
    {
        if(_image != NULL)
        {
            
            // _image->readPixels( 0,0,1280, 1024, GL_RGBA, GL_RGB);
                   }
        
    }
    
    
    osg::Image* _image;
};



VideoScreen::VideoScreen(std::string nameRect, osg::Vec2 startRect, double width, double height,
                         osg::Texture2D* prendertexture):
_nameRect(nameRect), _startRect(startRect), _width(width), _height(height), _prendertexture(prendertexture)
{
	this->_startRect3Dim = osg::Vec3(0,0,0);
	init();
    setTextureToRect(_prendertexture);
	
}

VideoScreen::VideoScreen(std::string nameRect, osg::Vec3 startRect, double width, double height,
                         osg::Texture2D* prendertexture):
_nameRect(nameRect), _startRect3Dim(startRect), _width(width), _height(height), _prendertexture(prendertexture)
{
	this->_startRect.x() = this->_startRect3Dim.x();
	this->_startRect.y() = this->_startRect3Dim.y();
	init();
    setTextureToRect(_prendertexture);
    
}

VideoScreen::VideoScreen()
{
    
}


void VideoScreen::init()
{
	//this->setAllChildrenOn();
	this->_texture = new osg::Texture2D();
    
	this->_colorNormal = osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->_colorSelected = osg::Vec4(0.9f, 0.3f, 0.0f, 1.0f);
    
    
	//*************************************
	// Hintergrund
    
	this->_geodeBackground = new osg::Geode();
	if(_nameRect.length() != 0)
	{
		this->_geodeBackground->setName(_nameRect);
	}
	this->_geometryBackground = new osg::Geometry();
	
    
	
    
	// Vertices Hintergrund
	this->_verticesBackground = new osg::Vec3Array();
	_verticesBackground->push_back(osg::Vec3(_startRect.x(), _startRect.y(),_startRect3Dim.z()));
	_verticesBackground->push_back(osg::Vec3(_startRect.x(), _startRect.y() + _height,_startRect3Dim.z()));
	_verticesBackground->push_back(osg::Vec3(_startRect.x() + _width, _startRect.y() + _height,_startRect3Dim.z()));
	_verticesBackground->push_back(osg::Vec3(_startRect.x() + _width, _startRect.y(),_startRect3Dim.z()));
    
    
	_geometryBackground->setVertexArray(_verticesBackground.get());
    
    
	// Farbe f¸r den Hintergrund
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

void VideoScreen::setColor(osg::Vec4 color)
{
	this->_colorSelected = color;
	this->_colorBackground->at(0) = color;
	this->_geometryBackground->dirtyDisplayList();
}

void VideoScreen::setNameofRect(std::string name)
{
	this->_nameRect = name;
	this->_geodeBackground->setName(_nameRect);
}

void VideoScreen::applyTextureToRect(std::string imagefile)
{
    
	
	this->_texture->setImage(osgDB::readImageFile(imagefile));
    
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

void VideoScreen::setTextureToRect(osg::Texture2D* texture)
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

std::string VideoScreen::getNameofRect(void)
{
	return this->_nameRect;
}




/*
 Funktion zum wechseln der Textur
 */
void VideoScreen::changeTexture(osg::Image* image)
{
	this->_texture->setImage(image);
    
	_geometryBackground->dirtyDisplayList();
}

