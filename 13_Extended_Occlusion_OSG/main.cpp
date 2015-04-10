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

#include "ExampleGeometry.h"

#include "PrerenderSubgraph.h"
#include "HUD.h"
#include "VideoScreen.h"


//The video background renderer
#include "VideoRenderer.h"

//The marker tracking code
#include "Marker.h"
#include "MarkerDetector.h"

// The node that links the Marker with OSG
#include "MarkerNode.h"

// Node to attach a matrix to the object
#include "ARTagNode.h"

#include "ContourRenderer.h"

int width = 640;
int height = 480;

// vectors for all detected markers
std::vector<Marker> detectedMarkers;



//----------------------------------------------
// The prerender subgraph object
PrerenderSubgraph* g_prerenderer = NULL;
osg::MatrixTransform* g_prerenderModel_transform = NULL;



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
    // Step 3:
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
    // Step 4:
    // Init the Marker Tracker
    MarkerDetector* md = new MarkerDetector(width, height, intrincsicMatrix, distCoeffs);
    md->setThreshold(150);
    

    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 5:
    // Create the objects for your visible scene


    // A box geometry
    osg::Group* example = ExampleGeometry::createBox(100.0, Vec4(1.0,0.0,0.0,1.0), Vec3(0.0,0.0,50.0));
    
    
    osg::MatrixTransform* nodeTransform = new osg::MatrixTransform();
    nodeTransform->setMatrix(osg::Matrix::identity());
    
    // Link the loaded model to an ARTag node
    if(example)
    {
        // An offset for the teapot
        osg::Matrix offset;
        offset.makeRotate(1.57,osg::Vec3(1.0,0.0,0.0),
                          0.0, osg::Vec3(0.0,1.0,0.0),
                          0.0, osg::Vec3(0.0,0.0,1.0));
        
        // Here: we create an Marker node for osg
        // We fetch the marker data and tie this data to the node
        osg::ref_ptr<ARTagNode> artNode = new ARTagNode( 213, detectedMarkers, offset );
        artNode->addChild(example);
        root->addChild(artNode);
    }
	else
	{
		cout << "Object not loaded" << endl;
	}
    
    
    
    
    ///////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 6:
    // The prerender subgraph
    
    
#ifdef WIN32
    osg::Group* loadedModel = (osg::Group*) osgDB::readNodeFile("../data_art/teapot.3ds");
#else
    osg::Group* loadedModel = (osg::Group*) osgDB::readNodeFile("../data_art/teapot.3ds");
#endif
    
    if(!loadedModel)
    {
        cout << "Object not loaded" << endl;
    }
    
    
    // Our texture target for the prerender subgraph.
    // The renderer renders the image into this texture.
    osg::Texture2D* g_prendertexture;
    g_prendertexture = new osg::Texture2D();
    g_prendertexture->setTextureSize(1680, 1050);
    g_prendertexture->setInternalFormat(GL_RGB);
    g_prendertexture->setFilter(osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR);
    g_prendertexture->setFilter(osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR);
    
    // Am ARTag node for the prerender object
    osg::ref_ptr<ARTagNode> prereder_art_node = new ARTagNode( 213, detectedMarkers, osg::Matrix::rotate(1.57,osg::Vec3(1.0,0.0,0.0),
                                                                                                         0.0, osg::Vec3(0.0,1.0,0.0),
                                                                                                         0.0, osg::Vec3(0.0,0.0,1.0)) );
    prereder_art_node->addChild(loadedModel);
    
    // Here, the prerender subgraph is created
    g_prerenderer = new PrerenderSubgraph();
    g_prerenderer->create(prereder_art_node, 1680, 1050, osg::Vec4(0.0, 0.0, 0.0, 1.0), osg::Camera::FRAME_BUFFER, g_prendertexture);
    root->addChild(g_prerenderer);
    
    
    // Creates a head up display
    HUD* hud = new HUD();
    
    //-----------------------------------------------------------------------------------------------------------------------------
    // ONLY FOR DEBUG PURPOSES TO CREAT AN OUTPUT ON SCREEN. NOT REQUIRED FOR THR EDGE RENDERING PROCESS
    // These lines creates a hud and a video screen on this hud
    
    VideoScreen* prerender_output_display = new VideoScreen("virtual_content", osg::Vec2(10,10), 240, 180, g_prendertexture);
    hud->addChild(prerender_output_display);
    root->addChild(hud);
    
    
    

    
    ///////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 7:
    // The output edge image which is rendered on top of the image stack.
    
    ContourRenderer* endge_detection_renderer_2 = new ContourRenderer( osg::Vec2(0,0), 1280, 1024, g_prendertexture);
    root->addChild(endge_detection_renderer_2);
   


    
    
    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 8:
    // Create a viewer and add a manipulator
    osgViewer::Viewer* viewer = new osgViewer::Viewer();
	viewer->setSceneData( root );
	viewer->setUpViewOnSingleScreen(0);
    viewer->setCameraManipulator(new osgGA::TrackballManipulator());
    viewer->getCamera()->setClearColor(osg::Vec4(0.5, 0.5, 1.0, 1.0)) ;
    
    
    // copy the camera location of your viewer to the camera location of the
    // prerender object. This is necessary to align both virtual cameras.
    if( g_prerenderer != NULL)
    {
        g_prerenderer->attachView(viewer->getCamera());
    }
    

    
    ///////////////////////////////////////////////////////////////////////////////////
    // Step 9:
    // Run the viewer
    while(!viewer->done())
    {
        
        detectedMarkers.clear();
        
        // Fetch an image
        (*capture) >> videoImage;
        cvtColor(videoImage, videoImageRGB, CV_RGBA2RGB);
        
        // Update the marker tracking
        md->findMarkers(videoImage, detectedMarkers, true);

        
        // Run the viewer
        viewer->frame();
        
    }
    
	capture->release();
    
    return 0;
}

