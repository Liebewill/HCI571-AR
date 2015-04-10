//
//  main.cpp
//  ARMarkerTracking Demo Applications
//
//  Created by Rafael Radkowski on 2/23/15.
//  Copyright (c) 2015 Rafael Radkowski. All rights reserved.
//

#include <iostream>
#include <vector>
#include <list>
#include <time.h>

#include <opencv2/opencv.hpp>

#include "Marker.h"
#include "MarkerDetector.h"

using namespace cv;
using namespace std;





int main(int argc, const char * argv[]) {

    
    /* initialize random seed: */
    srand (time(NULL));

    
    // open the video camera with id 0
    VideoCapture cap(0);
    
    // check if we succeeded
    if(!cap.isOpened())
        return -1;
    
    // TODO: Test, which camera settings are available for your camera.
    // Change camera properties
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);//1024
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);//768
    
    Mat f;
    cap >> f;
    
    // Read the camera properties width and height;
    double width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    double height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    
    
    std::cout << "Camera frame width: " << width << " and height: " << height << std::endl;
    
    
    // Create a window with name video
    namedWindow("video", 1);
    moveWindow("video", 5,0);
    
    Mat grayscale = cv::Mat(width, height, CV_32F);
    Mat bw_image = cv::Mat(width, height, CV_32F);
    
    
    
    // output Matrices
    cv::Mat intrincsicMatrix = Mat::zeros(3,3, CV_32F);
    cv::Mat distCoeffs = Mat::zeros(1, 4, CV_32F);
    
    FileStorage fs("Camera_Parameter_File.yml", FileStorage::READ);
    fs[ "intrinsic_matrix"] >> intrincsicMatrix;
    fs[ "dist_coeffs"] >> distCoeffs;
    fs.release();
    cout << intrincsicMatrix << endl;
    cout << distCoeffs << endl;
    
    
    
    
    MarkerDetector* md = new MarkerDetector(width, height, intrincsicMatrix, distCoeffs);
    md->setThreshold(150);
    
    for(;;)
    {
        Mat frame;
        
        // get new frame from camera
        cap >> frame;
        
        
        std::vector<Marker> detectedMarkers;
        md->findMarkers(frame, detectedMarkers, true);
        
        for (int i = 0; i<detectedMarkers.size(); i++) {
            cout << "Found marker with id " << detectedMarkers[i].id << endl;
        }
        
        // Render the video frame
        imshow("video", frame);
        // Wait 30 ms and break, if a kez is pressed
        if(waitKey(30) >= 0) break;
    }
    
    
    return 0;
}
