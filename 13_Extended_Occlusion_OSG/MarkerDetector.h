//
//  MarkerDetector.h
//  NDESpecTrack
//
//  Created by Rafael Radkowski on 2/25/15.
//  Copyright (c) 2015 Rafael Radkowski. All rights reserved.
//
#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <list>

#include <opencv2/opencv.hpp>


#include "Marker.h"


using namespace cv;
using namespace std;



typedef std::vector<cv::Point>    PointsVector;
typedef std::vector<PointsVector> ContoursVector;




float perimeter(const std::vector<cv::Point2f> &a);

bool isInto(cv::Mat &contour, std::vector<cv::Point2f> &b);






class MarkerDetector
{
private:
    
    // Detected markers
    std::vector<Marker>             detectedMarkers;
    
    
    // helper images
    Mat                             _grayscale;
    Mat                             _threshold_image;
    Mat                             _canonicalImage;
    
    int                             _image_width;
    int                             _image_height;
    
    
    Mat                             _camMatrix;
    Mat                             _distCoeff;
    
    
    Size                            _markerSize;
    
    // Poionts in 2D
    vector<Point2f>                 _markerCorners2d;
    
    // Transformation of the marker in 3d
    vector<Point3d>                 _markerCorners3d;
    
    // The final transformations of the markers
    std::vector<Transformation>     _transformations;
    
    
    // The minimal allowed length of a contour
    float                           _minContourLengthAllowed;
    
    // The detected contours
    ContoursVector                  _contours;
    
    
    // threshold value
    int                             _threshold;
    
    bool                            _draw_markers;
    
    //////////////////////////////////////////////////////////////////////
    
    
    /*
     Main marker detection routine
     */
    bool findMarkers(const Mat& frame, std::vector<Marker>& detectedMarkers);
    
    /*!
     Converts image to grayscale
     */
    void prepareImage(const cv::Mat& frame, cv::Mat& grayscale);
    
    /*!
     Performs binary threshold
     */
    void performThreshold(const cv::Mat& grayscale, cv::Mat& thresholdImg);
    
    /*!
     Detects appropriate contours
     */
    void findContours(cv::Mat& thresholdImg, ContoursVector& contours, int minContourPointsAllowed);
    
    /*!
     Finds marker candidates among all contours
     */
    void findCandidates(const ContoursVector& contours, std::vector<Marker>& detectedMarkers);
    
    /*!
     Tries to recognize markers by detecting marker code
     */
    void recognizeMarkers(const cv::Mat& grayscale, std::vector<Marker>& detectedMarkers);
    
    /*!
     Calculates marker poses in 3D
     */
    void estimatePosition(std::vector<Marker>& detectedMarkers);
    
    
    /*!
     Find Marker Process
     */
    void findMarkers( cv::Mat& src, std::vector<Marker>& markers);
    
    
    ////////////////////////////////////////////////////////////////////////
    // Drawing helpers
    
    /*!
     Draws the contour lines
     */
    void drawLines(cv::Mat& thresholdImg,
                   std::vector<std::vector<cv::Point> >& contours);
    
    /*!
     Draws the markers
     */
    void drawMarkers(cv::Mat& image,std::vector<Marker>& markers);

    
public:
    MarkerDetector(int image_width, int image_height, cv::Mat camMatrix, cv::Mat distCoeff);
    ~MarkerDetector();
    
    
    /*!
     Find and return markers
     */
    void findMarkers(cv::Mat& src, std::vector<Marker>& markers, bool draw_markers);
    
    
    /*!
     Returns the transformation of the object
     */
    std::vector<Transformation>& getTransformations();
    
    
    /*!
     Set the threshold value for the contour threshold. 
     @param threshold: integer between 0..255;
     */
    void setThreshold(int threshold);

};






