#pragma once

//#include <osgUtil/Optimizer>
//#include <osgDB/ReadFile>
//#include <osgProducer/Viewer>

#include <osg/Geometry>
#include <osg/Switch>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osg/NodeCallback>
#include <osg/Group>
#include <osg/Material>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/PolygonOffset>
#include <osg/MatrixTransform>
#include <osg/CameraNode>

#ifdef WIN32
#include <windows.h>
#endif

class HUD : public osg::Group
{
private:
	int _screenX;
	int _screenY;
    
public:	
	//CONSTRUCTOR and DESTRUCTOR
    HUD(void);
	~HUD(void);
 
	//ADDS GROUP KNOT TO CAMERA KNOT

	bool addChild(osg::Group* node);
    bool addChild(osg::Switch* node);
    bool addChild(osg::MatrixTransform* node);

private:
		//VARIABLES OF StateHUD						
	osg::ref_ptr<osg::CameraNode>	_camera;			
};