//
//  Marker.h
//  NDESpecTrack
//
//  Created by Rafael Radkowski on 2/23/15.
//  Copyright (c) 2015 Rafael Radkowski. All rights reserved.
//
#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <list>

#include <opencv2/opencv.hpp>

#include "GeometryTypes.h"

using namespace cv;
using namespace std;


class Marker
{
public:
    
    
    
    friend bool operator<(const Marker &M1,const Marker&M2);
    friend std::ostream & operator<<(std::ostream &str,const Marker &M);
    
    static cv::Mat rotate(cv::Mat  in);
    static int hammDistMarker(cv::Mat bits);
    static int mat2id(const cv::Mat &bits);
    static int getMarkerId(cv::Mat &in,int &nRotations);
    

    
    Marker();
    ~Marker();
    
    // Id of  the marker
    int id;
    
    // Marker transformation with regards to the camera
    Transformation transformation;
    
    
     // The marker points
    std::vector<cv::Point2f> points;
    
    // Helper function to draw the marker contour over the image
    void drawContour(cv::Mat& image, cv::Scalar color = CV_RGB(0,250,0)) const;
    
    
    
    
    

};



