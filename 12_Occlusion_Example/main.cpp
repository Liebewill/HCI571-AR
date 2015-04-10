//
//  main.cpp
//  HCi571X-Marker Tracking with OSG
//
//  Created by Dr.-Ing. Rafael Radkowski on 2/20/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//


#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2D.hpp>


#include <osg/Group>
#include <osg/Node>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>



//The video background renderer
#include "VideoRenderer.h"

//The marker tracking code
#include "Marker.h"
#include "MarkerDetector.h"

// Node to attach a matrix to the object
#include "ARTagNode.h"

int width = 640;
int height = 480;

// vectors for all detected markers
std::vector<Marker> detectedMarkers;


int main(int argc, const char * argv[])
{
    
    // Create the root node and add a scene
    osg::Group* root = new osg::Group();
    

    ///////////////////////////////////////////////////////////////////////////////////
    // Step 1:
    // Open a video capture device
    cv::VideoCapture* capture = new cv::VideoCapture(0);

    capture->set(CV_CAP_PROP_FRAME_WIDTH, (int)width);
    capture->set(CV_CAP_PROP_FRAME_HEIGHT, (int)height);

	
    // Check whether the video camera is available
	if (!capture ) {
		std::cout << "Error" << std::endl;
		return -1;
	}
    
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
    
    
    std::cout << "Camera frame width: " << width << " and height: " << height << std::endl;
    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 2:
    // Create the video background
    osg::Group* videoCanvas = VideoRenderer::createSzene(videoImageRGB.ptr<uchar>(0), size.width, size.height);
    root->addChild(videoCanvas);
    

    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 2:
    // Load the camera calibration parameters
    // output Matrices
    cv::Mat intrincsicMatrix = Mat::zeros(3,3, CV_32F);
    cv::Mat distCoeffs = Mat::zeros(1, 4, CV_32F);
    
    FileStorage fs("Camera_Parameter_File.yml", FileStorage::READ);
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
    // Here: we create an Marker node for osg
   
    // An offset for the teapot
    osg::Matrix offset;
    offset.makeRotate(1.57,osg::Vec3(1.0,0.0,0.0),
                      0.0, osg::Vec3(0.0,1.0,0.0),
                      0.0, osg::Vec3(0.0,0.0,1.0));

    ARTagNode* artNode = new ARTagNode( 213, detectedMarkers, offset );
    root->addChild(artNode);
    
    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 5:
    // Load the phantom object
#ifdef WIN32
    osg::Group* phantomModel = (osg::Group*) osgDB::readNodeFile("../data_art/box.3ds");
#else
	osg::Group* phantomModel = (osg::Group*) osgDB::readNodeFile("../data_art/box.3ds");
#endif
    

    
    if(phantomModel)
    {
        
        /////////////////////////////////
        // This switches of the rendering process.
        phantomModel->getOrCreateStateSet()->setAttributeAndModes(new osg::ColorMask(false,false,false,true));
        phantomModel->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
        
        artNode->addChild(phantomModel);
    }
	else
	{
		cout << "Object not loaded" << endl;
	}
    
    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 6:
    // Load an object an link this object with the marker tracking code
#ifdef WIN32
    osg::Group* loadedModel = (osg::Group*) osgDB::readNodeFile("../data_art/teapot.3ds");
#else
    osg::Group* loadedModel = (osg::Group*) osgDB::readNodeFile("../data_art/teapot.3ds");
#endif
    
    osg::MatrixTransform* animation_transform = new osg::MatrixTransform();
    
    if(loadedModel)
    {
        animation_transform->addChild(loadedModel);
        artNode->addChild(animation_transform);
        
        loadedModel->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::OFF);
    }
    else
    {
        cout << "Object not loaded" << endl;
    }
    
    
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
    // Step 7:
    // Create a viewer and add a manipulator
    osgViewer::Viewer* viewer = new osgViewer::Viewer();
	viewer->setSceneData( root );
	viewer->setUpViewOnSingleScreen(0);
    viewer->setCameraManipulator(new osgGA::TrackballManipulator());
    viewer->getCamera()->setClearColor(osg::Vec4(0.5, 0.5, 1.0, 1.0)) ;
    viewer->getCamera()->setProjectionMatrix(projectionmatrix);

    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 8:
    // Run the viewer
    while(!viewer->done())
    {
        
        detectedMarkers.clear();
        
        // Fetch an image
        (*capture) >> videoImage;
        cvtColor(videoImage, videoImageRGB, CV_RGBA2RGB);
        
        // Update the marker tracking
        md->findMarkers(videoImage, detectedMarkers, true);
        
        
        // This animates the teapot
        // It is not a real good solution for creating animations. But it works in this simple example
        double delta = 1.0 * sin((double)clock()/CLOCKS_PER_SEC  * 4.0 );
        animation_transform->setMatrix(osg::Matrix::translate( delta, 0.0 , 0.0));

        
        // Run the viewer
        viewer->frame();
        
    }
    
	capture->release();
    
    return 0;
}

