//
//  main.cpp
//  HCI571X-AR_Lens
//
//  Created by Dr.-Ing. Rafael Radkowski on 4/22/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//

#include <iostream>
#include <iostream>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/Geometry>
#include <osg/ShadeModel>
#include <osgViewer/Viewer>
#include <osgDB/ReaderWriter>
#include <osgDB/ReadFile>


#include "PrerenderSubgraph.h"
#include "HUD.h"
#include "VideoScreen.h"


#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2D.hpp>


//The video background renderer
#include "VideoRenderer.h"

// Marker tracking
#include "ARTagNode.h"
#include "MarkerDetector.h"

// The ARLens include object
#include "ARLens.h"

#include "ExampleGeometry.h"

#define THRESHOLD 100
int width = 640;
int height = 480;

// vectors for all detected markers
std::vector<Marker> detectedMarkers;

// Prerender texture
osg::Texture2D* g_prendertexture = NULL;



/*!
This creates the scene and returns a group node. 
*/
osg::Group* createScene(void)
{
	// Local scene node
	osg::Group* scene =  new osg::Group();

	// transformation for the 3D models to move them away from the marker
	osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform();
	transform->setMatrix(osg::Matrix::translate(200.0, 0.0, 0.0));
	

	osg::ref_ptr<osg::Group> teapot = (osg::Group*) osgDB::readNodeFile("./../models/teapot.3ds");
	if(teapot.get() == NULL)
	{
		cout << "Problems loading the teapot model" << endl;
		system("pause"); exit(0);
	}

	 // Second model
    osg::ref_ptr<osg::Group> underground = (osg::Group*) osgDB::readNodeFile("./../models/underground.3ds");
	if(underground.get() == NULL)
	{
		cout << "Problems loading the underground model" << endl;
		system("pause"); exit(0);
	}
	underground->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON |osg::StateAttribute::OVERRIDE );

	    // Third model: the view-dependend model
    osg::ref_ptr<osg::Group> invisible_object = (osg::Group*) osgDB::readNodeFile("./../models/hidden_teapot.obj");
    invisible_object->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON |osg::StateAttribute::OVERRIDE );
	if(invisible_object.get() == NULL)
	{
		cout << "Problems loading the hidden object model" << endl;
		system("pause"); exit(0);
	}

	osg::Matrix offset;
    offset.makeRotate(1.57,osg::Vec3(1.0,0.0,0.0),
                          0.0, osg::Vec3(0.0,1.0,0.0),
                          0.0, osg::Vec3(0.0,0.0,1.0));
        
     // Here: we create an Marker node for osg
     // We fetch the marker data and tie this data to the node
     osg::ref_ptr<ARTagNode> artNode = new ARTagNode( 213, detectedMarkers, offset );

	 scene->addChild(artNode);
	 artNode->addChild(transform);
     transform->addChild(underground);
	 transform->addChild(teapot);
	 transform->addChild(invisible_object);


	 // Set the nodemasks
     teapot->setNodeMask(0x1);
     invisible_object->setNodeMask(0x2);
     underground->setNodeMask(0x3);


	 return scene;
}




int main(int argc, const char * argv[])
{
    
    osg::ref_ptr<osg::Group> root = new osg::Group();
	
       
    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 1:
    // Open a video capture device
    cv::VideoCapture* capture = new cv::VideoCapture(0);
    
    capture->set(CV_CAP_PROP_FRAME_WIDTH, (int)width);
    capture->set(CV_CAP_PROP_FRAME_HEIGHT, (int)height);
    
    // Query the frame size of the video image.
    cv::Size size = cvSize((int) capture->get(CV_CAP_PROP_FRAME_WIDTH), (int) capture->get(CV_CAP_PROP_FRAME_HEIGHT));
    
	
    // Check whether the video camera is available
	if (!capture ) {
		std::cout << "Error" << std::endl;
		return -1;
	}
    
    // Init the video by fetching the first frame
    cv::Mat videoImage;
    cv::Mat videoImageRGB;
    cv::Mat videoImageRGBFlipped;
    (*capture) >> videoImage;
    cvtColor(videoImage, videoImageRGB, CV_RGBA2RGB);
    cv::flip(videoImageRGB, videoImageRGBFlipped, 0);
    

	// Read the camera properties width and height;
    width = capture->get(CV_CAP_PROP_FRAME_WIDTH);
    height = capture->get(CV_CAP_PROP_FRAME_HEIGHT);


    ///////////////////////////////////////////////////////////////////////////////////
    // Step 2:
    // Create the video background
    osg::Group* videoCanvas = VideoRenderer::createSzene(videoImageRGB.ptr<uchar>(0), size.width, size.height);
    root->addChild(videoCanvas);


    ///////////////////////////////////////////////////////////////////////////////////
    // Step 3:
    // Load the camera calibration parameters
    // output Matrices
    cv::Mat intrincsicMatrix = Mat::zeros(3,3, CV_32F);
    cv::Mat distCoeffs = Mat::zeros(1, 4, CV_32F);
    
    FileStorage fs("../data_art/Camera_Parameter_File.yml", FileStorage::READ);
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
    // Create a scene and attach this scene to the rootßnode
	osg::Group* scene = createScene();
	root->addChild(scene);
    
    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 5:
    // Create the magic lens
    // The functions are called in main to demonstrate the steps.
    // Usually, this can be also hidden within a C++ class. 
    
	
    // Step 5.1: create a prerender texture as render target.
    g_prendertexture = new osg::Texture2D();
    g_prendertexture->setTextureSize(1920, 1080); // <- make sure, the resolution of that texture meets your screen resolution 
    g_prendertexture->setInternalFormat(GL_RGB);
    g_prendertexture->setFilter(osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR);
    g_prendertexture->setFilter(osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR);
    
    // Step 5.2: create a osg::Camera node for off-screen rendering.
    // The class PrerenderSubgraph inherits from osg::Camera; the class was introduced before ,
    PrerenderSubgraph* secondaryCamera = new PrerenderSubgraph();
    secondaryCamera->create(scene, 1920, 1080, osg::Vec4(0.0, 0.0, 0.0, 0.0), osg::Camera::FRAME_BUFFER, g_prendertexture);
    
    // !!! Set the cull mask of the off-screen render camera
    secondaryCamera->setCullMask(0x2);
    
    root->addChild(secondaryCamera);

    // Step 5.3: create an marker node for the physical object that reprents the AR lens
	ARTagNode* lensMarker = new ARTagNode( 314, detectedMarkers, osg::Matrix::identity() );
    
    // Step 5.4: create the AR lens.
    // See class notes for further information. 
    ARLens* arlens = new ARLens(videoImageRGBFlipped.ptr<uchar>(0),  width,height, g_prendertexture, "./../models/lensA.3ds", "./../models/lensB.3ds");
    root->addChild(lensMarker);
    lensMarker->addChild(arlens);
    
    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 6:
    // This step can be removed !!
    // The class VideoScreen and the HUD renders a small screen on the HUD.
    // It shows the output of the off-screen rendering process. 
    VideoScreen* lensvideo = new VideoScreen("screen", osg::Vec2(4,4), 320, 240, g_prendertexture);
    HUD* hud = new HUD();
    root->addChild(hud);
    hud->addChild(lensvideo);
    
   
    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 7:
    // Load a camera configuration and apply this configuration on the projection matrix.
     ///////////////////////////////////////////////////////////////////////////////////
    // Create a projection matrix to meet the field of view of the camera.
    Size imageSize;
    imageSize.width = width;
    imageSize.height = height;

    // physical size of the sensor.
    double apertureWidth = 1.0;
    double apertureHeight = 1.0;
    double fovx, fovy, focalLength, aspectRatio;
    Point2d principalPoint;
    cv::calibrationMatrixValues(intrincsicMatrix, imageSize, apertureWidth, apertureHeight, fovx, fovy, focalLength, principalPoint, aspectRatio);
    
    osg::Matrixd projectionmatrix;
    projectionmatrix.makePerspective(fovy, fovx/fovy, 0.1, 1000.0);
    
    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 8:
    // Create a viewer
    osgViewer::Viewer* viewer = new osgViewer::Viewer();
    viewer->setSceneData(root);
    viewer->setUpViewOnSingleScreen(0);
	//viewer->setCameraManipulator(new osgGA::TrackballManipulator());
    viewer->getCamera()->setViewMatrixAsLookAt(osg::Vec3d(0.0,-100.0, 0.0), osg::Vec3d(0.0,0.0, 0.0), osg::Vec3d(0.0,0.0, 1.0));
    viewer->getCamera()->setProjectionMatrix(projectionmatrix);

    // Set the cull mask of the main camera 
    viewer->getCamera()->setCullMask(0x1);

	
    // This function call ties the main viewer camera to the off-scrreen renderer camera.
    secondaryCamera->attachView(viewer->getCamera());
	
    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 9:
    // Run the viewer
    while(!viewer->done())
    {
		detectedMarkers.clear();

        // Fetch an image
        (*capture) >> videoImage;
        cvtColor(videoImage, videoImageRGB, CV_RGBA2RGB);
        cv::flip(videoImageRGB, videoImageRGBFlipped, 0);
        
		 // Update the marker tracking
        md->findMarkers(videoImageRGB, detectedMarkers, false);

        // Render a frame
        viewer->frame();
    }

	capture->release();
    
    
    return 0;
}

