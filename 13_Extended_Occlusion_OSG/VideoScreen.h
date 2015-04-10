//
//  VideoScreen.h
//  ARToolkit-Marker Model Teaching
//
//  Created by Dr.-Ing. Rafael Radkowski on 18.08.11.
//  Copyright 2011 Heinz Nixdorf Institute. All rights reserved.
//
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


class VideoScreen : public osg::MatrixTransform
{
private:
    
	// Geode und Geometrie f¸r den Hintergrund
	osg::ref_ptr<osg::Geode> _geodeBackground;
	osg::ref_ptr<osg::Geometry> _geometryBackground;
    
	//Eckpunkte f¸r Hintergrund
	osg::ref_ptr<osg::Vec3Array> _verticesBackground;
    
	//
	osg::ref_ptr<osg::DrawArrays> _drawArrayBackground;
    
	// Farbe f¸r den Hintergrund
	osg::ref_ptr<osg::Vec4Array> _colorBackground;
    
	osg::ref_ptr<osg::Vec3Array>  _background_Normal;
    

protected:
	// Name des Rechtecks
	std::string		_nameRect;
    
    
	//Startposition und Grˆﬂe des Rechtecks
	osg::Vec2			_startRect; // x und y Pos wird angegeben
	osg::Vec3			_startRect3Dim;
	double				_width;
	double				_height;
    
    
    // texture to render to and to use for rendering of flag.
    osg::Texture2D*             _prendertexture ;
    
    
private:
	//Farben
	osg::Vec4		_colorNormal;
	osg::Vec4		_colorSelected;
    
	// Textur
	osg::Texture2D*  _texture;
	osg::ref_ptr<osg::Vec3Array>	_texCoordArray;

    
    
    VideoScreen();
    
public:
    
	VideoScreen(std::string nameRect, osg::Vec2 startRect, double width, double height, osg::Texture2D* prendertexture);
	VideoScreen(std::string nameRect, osg::Vec3 startRect, double width, double height, osg::Texture2D* prendertexture);
    
    ~VideoScreen(){;};
    
	// Statische Funktionen
	
	void applyTextureToRect(std::string imagefile);
    
    void setTextureToRect(osg::Texture2D* texture);
    
	/*
     Funktion zum wechseln der Textur
     */
	void changeTexture(osg::Image* image);
    
	// Initialisiert und zeichnet die Rechtecke
	void init(void);
    
	// Dynamische Funktionen
	void setColor(osg::Vec4 color);
	void setNameofRect(std::string name);
    
	std::string getNameofRect(void);


};