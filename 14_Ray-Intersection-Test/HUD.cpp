#include "HUD.h"


//------------------------------------StateHUD----------------------------------------------------
//------------------------------------StateHUD----------------------------------------------------
//------------------------------------StateHUD----------------------------------------------------

//CONSTRUCTOR AND DESTRUCTOR
HUD::HUD()
{

    _screenX  = 1280;
	_screenY  = 1024;

	//Set up camera
    this->setProjectionMatrix(osg::Matrix::ortho2D(0,_screenX,0,_screenY));

	// set the view matrix    
	this->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    this->setViewMatrix(osg::Matrix::identity());
		
    //_camera->setClearColor(osg::Vec4(1.0, 1.0, 0.0, 0.7));
	// only clear the depth buffer
	this->setClearMask(GL_DEPTH_BUFFER_BIT);

	// draw subgraph after main camera view.    
    this->setRenderOrder(osg::Camera::PRE_RENDER);

				
}

HUD::~HUD(void)
{			

}
		
//ADD CONMECTOTSETS TO StateHUD



bool HUD::addChild(osg::Group* node)
{
    if(node != NULL)
    {
        this->addChild(node);
        return true;
    }
    return false;
    
}


bool HUD::addChild(osg::Switch* node)
{
    if(node != NULL)
    {
        this->addChild(node);
        return true;
    }
    return false;
    
}


bool HUD::addChild(osg::MatrixTransform* node){
    if(node != NULL)
    {
        this->addChild(node);
        return true;
    }
    return false;
    
}







