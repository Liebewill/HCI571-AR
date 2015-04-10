#include "HUD.h"


//------------------------------------StateHUD----------------------------------------------------
//------------------------------------StateHUD----------------------------------------------------
//------------------------------------StateHUD----------------------------------------------------

//CONSTRUCTOR AND DESTRUCTOR
HUD::HUD()
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
		this->_camera = new osg::Camera();
		_camera->setProjectionMatrix(osg::Matrix::ortho2D(0,_screenX,0,_screenY));

	// set the view matrix    
		_camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		_camera->setViewMatrix(osg::Matrix::identity());	
		
		//_camera->setClearColor(osg::Vec4(1.0, 1.0, 0.0, 0.7));
	// only clear the depth buffer
		_camera->setClearMask(GL_DEPTH_BUFFER_BIT);

	// draw subgraph after main camera view.    
		_camera->setRenderOrder(osg::Camera::POST_RENDER);			

	//Add Camera to group
    this->osg::Group::addChild(_camera.get());					
}

HUD::~HUD(void)
{			
	_camera.~ref_ptr();
}
		
//ADD CONMECTOTSETS TO StateHUD



bool HUD::addChild(osg::Group* node)
{
    if(node != NULL)
    {
        this->_camera->addChild(node);
        return true;
    }
    return false;
    
}


bool HUD::addChild(osg::Switch* node)
{
    if(node != NULL)
    {
        this->_camera->addChild(node);
        return true;
    }
    return false;
    
}


bool HUD::addChild(osg::MatrixTransform* node){
    if(node != NULL)
    {
        this->_camera->addChild(node);
        return true;
    }
    return false;
    
}







