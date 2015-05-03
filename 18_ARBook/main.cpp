//
//  main.cpp
//  HCI571X-ARPaddle
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/2/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//

#include <iostream>

#include <osg/Group>
#include <osg/MatrixTransform>
#include <osgDB/ReaderWriter>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgText/Text>

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2D.hpp>

#include <Windows.h>

//The video background renderer
#include "VideoRenderer.h"

// Marker tracking
#include "ARTagNode.h"
#include "MarkerDetector.h"


// Geometry example; a box
#include "ExampleGeometry.h"

#include "KeyboardEventHandler.h"
#include "MoveableObject.h"
#include "ARPaddle.h"
#include "ModelHandler.h"
#include "ARBook.h"
#include "HUD.h"

#define WORLD_MARKER 213
#define PADDLE_MARKER 314
#define BOOK_MARKER 480

#define THRESHOLD 160

int IntersectTraversalMask = 0x3;


//MoveableObject* teapot;
ARPaddle*       paddle;

// An AR book which will be used on a later occasion
ARBook*         book;

MoveableObject* hidenode;



// vectors for all detected markers
std::vector<Marker> detectedMarkers;


int width = 640;
int height = 480;



extern std::string g_modelname;
extern std::string g_paddlelocation;
extern std::string g_state;
extern std::string g_direction;
extern std::string g_direction2;
extern std::string g_speed;

std::string textStr = "";
osg::ref_ptr<osgText::Text> text;

std::string textStr2 = "";
osg::ref_ptr<osgText::Text> text2;

std::string textStr3 = "";
osg::ref_ptr<osgText::Text> text3;

std::string textStr4 = "";
osg::ref_ptr<osgText::Text> text4;

std::string textStr5 = "";
osg::ref_ptr<osgText::Text> text5;

std::string textStr6 = "";
osg::ref_ptr<osgText::Text> text6;

osg::Group* createText(void)
{
    
    osg::Group* localNode = new osg::Group();
    localNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    HUD* hud = new HUD();
    localNode->addChild(hud);
    
    text = new osgText::Text();
    text->setText(textStr);
    text->setAxisAlignment(osgText::Text::SCREEN);
    text->setPosition(osg::Vec3(20.0, 1150.0, 0.0));
    text->setCharacterSize(30.0);
    text->setFont("../../models/arial.ttf");
    text->setColor(osg::Vec4(1.0, 1.0, 0.0, 0.8));
	text->setDataVariance(osgText::Text::DYNAMIC);
    
    text2 = new osgText::Text();
    text2->setText(textStr2);
    text2->setAxisAlignment(osgText::Text::SCREEN);
    text2->setPosition(osg::Vec3(20.0, 1100.0, 0.0));
    text2->setCharacterSize(30.0);
    text2->setFont("../../models/arial.ttf");
    text2->setColor(osg::Vec4(1.0, 1.0, 0.0, 0.8));
	text2->setDataVariance(osgText::Text::DYNAMIC);
    
    
    text3 = new osgText::Text();
    text3->setText(textStr3);
    text3->setAxisAlignment(osgText::Text::SCREEN);
    text3->setPosition(osg::Vec3(20.0, 1050.0, 0.0));
    text3->setCharacterSize(30.0);
    text3->setFont("../../models/arial.ttf");
    text3->setColor(osg::Vec4(1.0, 1.0, 0.0, 0.8));
	text3->setDataVariance(osgText::Text::DYNAMIC);
    
    text4 = new osgText::Text();
    text4->setText(textStr4);
    text4->setAxisAlignment(osgText::Text::SCREEN);
    text4->setPosition(osg::Vec3(20.0, 1000.0, 0.0));
    text4->setCharacterSize(30.0);
    text4->setFont("../../models/arial.ttf");
    text4->setColor(osg::Vec4(1.0, 1.0, 0.0, 0.8));
	text4->setDataVariance(osgText::Text::DYNAMIC);
    
    text5 = new osgText::Text();
    text5->setText(textStr5);
    text5->setAxisAlignment(osgText::Text::SCREEN);
    text5->setPosition(osg::Vec3(20.0, 950.0, 0.0));
    text5->setCharacterSize(30.0);
    text5->setFont("../../models/arial.ttf");
    text5->setColor(osg::Vec4(1.0, 1.0, 0.0, 0.8));
	text5->setDataVariance(osgText::Text::DYNAMIC);
    
    text6 = new osgText::Text();
    text6->setText(textStr6);
    text6->setAxisAlignment(osgText::Text::SCREEN);
    text6->setPosition(osg::Vec3(250.0, 950.0, 0.0));
    text6->setCharacterSize(30.0);
    text6->setFont("../../models/arial.ttf");
    text6->setColor(osg::Vec4(1.0, 1.0, 0.0, 0.8));
	text6->setDataVariance(osgText::Text::DYNAMIC);
    
    osg::ref_ptr<osg::Group>    textGroup = new osg::Group();
    osg::ref_ptr<osg::Geode>    textGeode = new osg::Geode();
    textGeode->addDrawable(text);
    textGeode->addDrawable(text2);
    textGeode->addDrawable(text3);
    textGeode->addDrawable(text4);
    textGeode->addDrawable(text5);
    textGeode->addDrawable(text6);
    textGroup->addChild(textGeode);
	
    hud->addChild(textGroup);

    return localNode;

}

osg::Group* loadObject(std::string label,  std::string filename, osg::Group* underground,  double x, double y, double z)
{
    osg::Group* localNode = new osg::Group();
   
    MoveableObject* mynode = new MoveableObject(filename, underground );
    
    if(mynode != NULL)
    {
		osg::Matrix offset;
        offset.makeRotate(1.57,osg::Vec3(1.0,0.0,0.0),
                          0.0, osg::Vec3(0.0,1.0,0.0),
                          0.0, osg::Vec3(0.0,0.0,1.0));
        

       	ARTagNode* artNode = new ARTagNode( 213, detectedMarkers, offset );
        mynode->setMatrix(osg::Matrix::translate(x, y, z));
        mynode->setName(label);
    
        localNode->addChild(artNode);
        artNode->addChild(mynode);
        
        mynode->setNodeMask(IntersectTraversalMask);
        artNode->setNodeMask(IntersectTraversalMask);
        
        
        int i = mynode->getNumChildren();
        if(i != 0)
        {
            for(int j=0; j<i; j++)
            {
                mynode->getChild(j)->setName(label);
            }
        }
        
        ModelHandler::addModel(label, mynode, artNode);
    }
    
    
    return localNode;
}

osg::Group* loadObject2(std::string label,  std::string filename, osg::Group* underground,  double x, double y, double z)
{
    osg::Group* localNode = new osg::Group();
    
    hidenode = new MoveableObject(filename, underground );
    
    if(hidenode != NULL)
    {
		
		ARTagNode* artNode = new ARTagNode( 213, detectedMarkers, osg::Matrix::identity() );
        hidenode->setMatrix(osg::Matrix::translate(x, y, z));
        hidenode->setName(label);
        
        localNode->addChild(artNode);
        artNode->addChild(hidenode);
        
        hidenode->setNodeMask(IntersectTraversalMask);
        artNode->setNodeMask(IntersectTraversalMask);
        
        
        int i = hidenode->getNumChildren();
        if(i != 0)
        {
            for(int j=0; j<i; j++)
            {
                hidenode->getChild(j)->setName(label);
            }
        }
        
        ModelHandler::addModel(label, hidenode, artNode);
    }
    
    
    return localNode;
}




osg::Group* createScene(void)
{
    osg::Group* helper = new osg::Group();
    
    
    helper->addChild(createText());

    osg::Group* collider_group = new osg::Group();
    collider_group->setNodeMask(IntersectTraversalMask);
    collider_group->setName("ColliderGroup");
    
    helper->addChild(collider_group);
    
	ModelHandler::setPaddleRefMarkerPtr(PADDLE_MARKER);
	ModelHandler::setWorldRefMarkerPtr(WORLD_MARKER);

    /////////////////////////////////////////////////////////
    // Load the underground
    osg::Group* underground = (osg::Group*)osgDB::readNodeFile("./../models/underground.obj");
    underground->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    if(underground != NULL)
    {

		osg::Matrix offset;
		offset.makeRotate(1.57,osg::Vec3(1.0,0.0,0.0),
                          0.0, osg::Vec3(0.0,1.0,0.0),
                          0.0, osg::Vec3(0.0,0.0,1.0));

		// Here: we create an Marker node for osg
		// We fetch the marker data and tie this data to the node
		osg::ref_ptr<ARTagNode> artNode = new ARTagNode( 213, detectedMarkers, offset );

        helper->addChild(artNode);
        artNode->addChild(underground);
        
        underground->setNodeMask(IntersectTraversalMask);
        artNode->setNodeMask(IntersectTraversalMask);
    }
    
    /////////////////////////////////////////////////////////
    // Load a moveable object
    
    
    
    
    
    book = new ARBook(underground);
    helper->addChild(book);
    
    

    /////////////////////////////////////////////////////////
    // Create the AR paddle
    ARTagNode* worldNode = new ARTagNode( 213, detectedMarkers, osg::Matrix::identity());
	ARTagNode* paddleNode = new ARTagNode( 314, detectedMarkers, osg::Matrix::identity());
	{
        paddle = new ARPaddle(	paddleNode,
								collider_group, book, 
								worldNode );

        helper->addChild(paddle);
		helper->addChild(paddleNode);
		helper->addChild(worldNode);

    }

    return helper;

}




int main(int argc, const char * argv[])
{

    // Create the root node and add a scene
    osg::Group* root = new osg::Group();  
    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 1:
    // Open a video capture device
    cv::VideoCapture* capture = new cv::VideoCapture(0);

	
    // Check whether the video camera is available
	if (!capture ) {
		std::cout << "Error" << std::endl;
		return -1;
	}
 
    capture->set(CV_CAP_PROP_FRAME_WIDTH, (int)width);
    capture->set(CV_CAP_PROP_FRAME_HEIGHT, (int)height);
    
    // Init the video by fetching the first frame
    cv::Mat videoImage;
    cv::Mat videoImageRGB;
    (*capture) >> videoImage;
    cvtColor(videoImage, videoImageRGB, CV_RGBA2RGB);
    
	// Read the camera properties width and height;
    width = capture->get(CV_CAP_PROP_FRAME_WIDTH);
    height = capture->get(CV_CAP_PROP_FRAME_HEIGHT);
     // Query the frame size of the video image.
    cv::Size size = cvSize((int) capture->get(CV_CAP_PROP_FRAME_WIDTH), (int) capture->get(CV_CAP_PROP_FRAME_HEIGHT));

    ///////////////////////////////////////////////////////////////////////////////////
    // Step 2:
    // Create the video background
    osg::Group* videoCanvas = VideoRenderer::createSzene(videoImageRGB.ptr<uchar>(0), size.width, size.height);
    root->addChild(videoCanvas);

  
	///////////////////////////////////////////////////////////////////////////////////
    // Load the camera calibration parameters
    // output Matrices
    cv::Mat intrincsicMatrix = Mat::zeros(3,3, CV_32F);
    cv::Mat distCoeffs = Mat::zeros(1, 4, CV_32F);
    
    FileStorage fs("data_art/Camera_Parameter_File.yml", FileStorage::READ);
    fs[ "intrinsic_matrix"] >> intrincsicMatrix;
    fs[ "dist_coeffs"] >> distCoeffs;
    fs.release();
    cout << intrincsicMatrix << endl;
    cout << distCoeffs << endl;
    

    ///////////////////////////////////////////////////////////////////////////////////
    // Step 3:
    // Init the Marker Tracker
    MarkerDetector* md = new MarkerDetector(width, height, intrincsicMatrix, distCoeffs);
    md->setThreshold(150);
    

    ///////////////////////////////////////////////////////////////////////////////////
    // Step 4:
    // Create Scene
    root->addChild(createScene());
    
    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 5:
    // Load a camera configuration and apply this configuration on the projection matrix. 
    Size imageSize;
    imageSize.width = width;
    imageSize.height = height;

    // physical size of the sensor.
    double apertureWidth = 3.2;
    double apertureHeight = 2.4;
    double fovx, fovy, focalLength, aspectRatio;
    Point2d principalPoint;
    cv::calibrationMatrixValues(intrincsicMatrix, imageSize, apertureWidth, apertureHeight, fovx, fovy, focalLength, principalPoint, aspectRatio);
    
    osg::Matrixd projectionmatrix;
    projectionmatrix.makePerspective(fovy, fovx/fovy, 0.1, 1000.0);
    
    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 6:
    // Create a viewer and add a manipulator
    osgViewer::Viewer* viewer = new osgViewer::Viewer();
	viewer->setSceneData( root );
	viewer->setUpViewOnSingleScreen(0);
    viewer->getCamera()->setClearColor(osg::Vec4(0.5, 0.5, 1.0, 1.0)) ;
    viewer->getCamera()->setProjectionMatrix(projectionmatrix);
    //viewer->setCameraManipulator(new osgGA::TrackballManipulator);
    viewer->getCamera()->setViewMatrixAsLookAt(osg::Vec3d(0.0,-10.0, 0.0), osg::Vec3d(0.0,0.0, 0.0), osg::Vec3d(0.0,0.0, 1.0));
    //viewer->addEventHandler(new KeyboardEventHandler(teapot));
    viewer->init();
    
    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 7:
    // Run the viewer

	// Two transform nodes are created
    while(!viewer->done())
    {
        // Fetch an image
        (*capture) >> videoImage;
        cvtColor(videoImage, videoImageRGB, CV_RGBA2RGB);
        
		 // Update the marker tracking
        md->findMarkers(videoImageRGB, detectedMarkers, false);

        // Run the viewer
        viewer->frame();
        

        textStr = "Found object "  + g_modelname;
        text->setText( textStr);
        
        textStr2 = g_paddlelocation;
        text2->setText( textStr2);

        textStr3 = g_state;
        text3->setText( textStr3);
        
        textStr4 = g_direction;
        text4->setText( textStr4);
        
        textStr5 = g_speed;
        text5->setText( textStr5);
        
        textStr6 = g_direction2;
        text6->setText( textStr6);
    }
    
	capture->release();
    
    return 0;
}

