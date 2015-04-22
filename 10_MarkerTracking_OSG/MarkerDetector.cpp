//
//  MarkerDetector.cpp
//  NDESpecTrack
//
//  Created by Rafael Radkowski on 2/25/15.
//  Copyright (c) 2015 Rafael Radkowski. All rights reserved.
//

#include "MarkerDetector.h"

//#define SHOW_DEBUG_IMAGES


MarkerDetector::MarkerDetector(int image_width, int image_height, cv::Mat camMatrix, cv::Mat distCoeff):
    _image_width(image_width),_image_height(image_height), _camMatrix(camMatrix), _distCoeff(distCoeff), _threshold(127)
{
    _grayscale = cv::Mat(image_width, image_height, CV_32F);
    _threshold_image = cv::Mat(image_width, image_height, CV_32F);
    
    _markerSize.height = 100;
    _markerSize.width = 100;
    

    _minContourLengthAllowed = 100;
    
    bool centerOrigin = true;
    if (centerOrigin)
    {
        _markerCorners3d.push_back(cv::Point3f(-0.5f,-0.5f,0));
        _markerCorners3d.push_back(cv::Point3f(+0.5f,-0.5f,0));
        _markerCorners3d.push_back(cv::Point3f(+0.5f,+0.5f,0));
        _markerCorners3d.push_back(cv::Point3f(-0.5f,+0.5f,0));
    }
    else
    {
        _markerCorners3d.push_back(cv::Point3f(0,0,0));
        _markerCorners3d.push_back(cv::Point3f(1,0,0));
        _markerCorners3d.push_back(cv::Point3f(1,1,0));
        _markerCorners3d.push_back(cv::Point3f(0,1,0));
    }
    
    _markerCorners2d.push_back(cv::Point2f(0,0));
    _markerCorners2d.push_back(cv::Point2f(_markerSize.width,0));
    _markerCorners2d.push_back(cv::Point2f(_markerSize.width,_markerSize.height));
    _markerCorners2d.push_back(cv::Point2f(0,_markerSize.height));
    
    
}


MarkerDetector::~MarkerDetector()
{

}



/*!
 Perform a threshold operation on th eimage
 */
void MarkerDetector::performThreshold(const cv::Mat& grayscale, cv::Mat& thresholdImg)
{
    
    cv::threshold(grayscale, thresholdImg, _threshold, 255, cv::THRESH_BINARY_INV);
    
   /*
    cv::adaptiveThreshold(grayscale, // Input image
                          thresholdImg,// Result binary image
                          255, //
                          cv::ADAPTIVE_THRESH_GAUSSIAN_C, //
                          cv::THRESH_BINARY_INV, //
                          7, //
                          7 //
                          );
    
    */
}



std::vector<Transformation>& MarkerDetector::getTransformations() 
{
    return _transformations;
}



void MarkerDetector::findContours(cv::Mat& thresholdImg,
                                  ContoursVector& contours,
                                  int minContourPointsAllowed)
{
    ContoursVector allContours;
    cv::findContours(thresholdImg, allContours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    
    contours.clear();
    for (size_t i=0; i<allContours.size(); i++)
    {
        int contourSize = allContours[i].size();
        if (contourSize > minContourPointsAllowed)
        {
            contours.push_back(allContours[i]);
        }
    }
    
#ifdef SHOW_DEBUG_IMAGES
    {
        cv::Mat contoursImage(thresholdImg.size(), CV_8UC1);
        contoursImage = cv::Scalar(0);
        cv::drawContours(contoursImage, contours, -1, cv::Scalar(255), 2, CV_AA);
        cv::imshow("Contours", contoursImage);
        cv::waitKey();
    }
#endif
}



void MarkerDetector::drawLines(cv::Mat& thresholdImg,
               std::vector<std::vector<cv::Point> >& contours)
{
    const Scalar color(0, 0, 255 );
    
    
    int size = contours.size();
    for(int i=0; i<size; i++ )
    {
        drawContours( thresholdImg, contours, i, color, 2, 8, 1, 0, Point() );
    }
    
    //imshow("debug", thresholdImg);
    
    
}


void MarkerDetector::drawMarkers(cv::Mat& image,std::vector<Marker>& markers)
{
    const Scalar color(0.0,0.0,255);
    for(int i=0; i<markers.size(); i++)
    {
        line(image, markers[i].points[0], markers[i].points[1], color,2);
        line(image, markers[i].points[1], markers[i].points[2], color,2);
        line(image, markers[i].points[2], markers[i].points[3], color,2);
        line(image, markers[i].points[3], markers[i].points[0], color,2);
    }
    
    
}




void MarkerDetector::findCandidates (const std::vector<std::vector<cv::Point> >& contours,
                     std::vector<Marker>& detectedMarkers
                     )
{
    
    std::vector<cv::Point> approxCurve;
    std::vector<Marker> possibleMarkers;
    
    
    int num = 0;
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // For each contour, analyze if it is a parallelepiped likely to be the marker
    for (size_t i=0; i<contours.size(); i++)
    {
        // Approximate to a polygon
        double eps = contours[i].size() * 0.05;
        cv::approxPolyDP(contours[i], approxCurve, eps, true);
        
        // We interested only in polygons that contains only four points
        if (approxCurve.size() != 4)
            continue;
        
        // And they have to be convex
        if (!cv::isContourConvex(approxCurve))
            continue;
        
        // Ensure that the distance between consecutive points is large enough
        float minDist = std::numeric_limits<float>::max();
        
        for (int i = 0; i < 4; i++)
        {
            cv::Point side = approxCurve[i] - approxCurve[(i+1)%4];
            float squaredSideLength = side.dot(side);
            minDist = std::min(minDist, squaredSideLength);
        }
        
        // Check that distance is not very small
        if (minDist <_minContourLengthAllowed)
            continue;
        
        // All tests are passed. Save marker candidate:
        Marker m;
        
        for (int i = 0; i<4; i++)
            m.points.push_back( cv::Point2f(approxCurve[i].x,approxCurve[i].y) );
        
        // Sort the points in anti-clockwise order
        // Trace a line between the first and second point.
        // If the third point is at the right side, then the points are anti-clockwise
        cv::Point v1 = m.points[1] - m.points[0];
        cv::Point v2 = m.points[2] - m.points[0];
        
        double o = (v1.x * v2.y) - (v1.y * v2.x);
        
        if (o < 0.0)		 //if the third point is in the left side, then sort in anti-clockwise order
            std::swap(m.points[1], m.points[3]);
        
        possibleMarkers.push_back(m);
        
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Remove these elements which corners are too close to each other.
    // First detect candidates for removal:
    std::vector< std::pair<int,int> > tooNearCandidates;
    for (size_t i=0;i<possibleMarkers.size();i++)
    {
        const Marker& m1 = possibleMarkers[i];
        
        //calculate the average distance of each corner to the nearest corner of the other marker candidate
        for (size_t j=i+1;j<possibleMarkers.size();j++)
        {
            const Marker& m2 = possibleMarkers[j];
            
            float distSquared = 0;
            
            for (int c = 0; c < 4; c++)
            {
                cv::Point v = m1.points[c] - m2.points[c];
                distSquared += v.dot(v);
            }
            
            distSquared /= 4;
            
            if (distSquared < 100) // the marker size is 100
            {
                tooNearCandidates.push_back(std::pair<int,int>(i,j));
            }
        }				
    }
    
    
    
    // Mark for removal the element of the pair with smaller perimeter
    std::vector<bool> removalMask (possibleMarkers.size(), false);
    
    for (size_t i=0; i<tooNearCandidates.size(); i++)
    {
        float p1 = perimeter(possibleMarkers[tooNearCandidates[i].first ].points);
        float p2 = perimeter(possibleMarkers[tooNearCandidates[i].second].points);
        
        size_t removalIndex;
        if (p1 > p2)
            removalIndex = tooNearCandidates[i].second;
        else
            removalIndex = tooNearCandidates[i].first;
        
        removalMask[removalIndex] = true;
    }
    
    // Return candidates
    detectedMarkers.clear();
    for (size_t i=0;i<possibleMarkers.size();i++)
    {
        if (!removalMask[i])
            detectedMarkers.push_back(possibleMarkers[i]);
    }
    
}



/*!
 Estimates the marker position
 */
void MarkerDetector::estimatePosition(std::vector<Marker>& detectedMarkers)
{
    for (size_t i=0; i<detectedMarkers.size(); i++)
    {
        Marker& m = detectedMarkers[i];
        
        cv::Mat Rvec, RvecTemp;
        cv::Mat_<float> Tvec;
        cv::Mat raux,taux;
        cv::solvePnP(_markerCorners3d, m.points, _camMatrix, _distCoeff,raux,taux);
        raux.convertTo(Rvec,CV_32F);
        raux.convertTo(RvecTemp,CV_32F);
        taux.convertTo(Tvec ,CV_32F);
        
        cv::Mat_<float> rotMat(3,3);
        
        // Adaption to meet the coordinate system of OSG.
        RvecTemp.at<float>(0) =Rvec.at<float>(0);
        RvecTemp.at<float>(1) =Rvec.at<float>(2);
        RvecTemp.at<float>(2) =-Rvec.at<float>(1);
        
        cv::Rodrigues(RvecTemp, rotMat);
        
        // Copy to transformation matrix
        for (int col=0; col<3; col++)
        {
            for (int row=0; row<3; row++)
            {
                m.transformation.r().mat[row][col] = rotMat(row,col); // Copy rotation component
            }
            m.transformation.t().data[col] = Tvec(col); // Copy translation component
        }
        
        // Since solvePnP finds camera location, w.r.t to marker pose, to get marker pose w.r.t to the camera we invert it.
        m.transformation = m.transformation.getInverted();
    }
    
    
}


void MarkerDetector::recognizeMarkers(const cv::Mat& grayscale, std::vector<Marker>& detectedMarkers)
{
    std::vector<Marker> goodMarkers;
    
    // Identify the markers
    for (size_t i=0;i<detectedMarkers.size();i++)
    {
        Marker& marker = detectedMarkers[i];
        
        // Find the perspective transformation that brings current marker to rectangular form
        cv::Mat markerTransform = cv::getPerspectiveTransform(marker.points, _markerCorners2d);
        
        // Transform image to get a canonical marker image
        cv::warpPerspective(grayscale, _canonicalImage,  markerTransform, _markerSize);
        
#ifdef SHOW_DEBUG_IMAGES
        {
            cv::Mat markerImage = grayscale.clone();
            marker.drawContour(markerImage);
            cv::Mat markerSubImage = markerImage(cv::boundingRect(marker.points));
            
            cv::imshow("Source marker",  markerSubImage);
            cv::imshow("Marker after warp", _canonicalImage);
            cv::waitKey();
        }
#endif
        
        int nRotations;
        int id = Marker::getMarkerId(_canonicalImage, nRotations);
        if (id !=- 1)
        {
            marker.id = id;
            //sort the points so that they are always in the same order no matter the camera orientation
            std::rotate(marker.points.begin(), marker.points.begin() + 4 - nRotations, marker.points.end());
            
            goodMarkers.push_back(marker);
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Refine marker corners using sub pixel accuracy
    if (goodMarkers.size() > 0)
    {
        std::vector<cv::Point2f> preciseCorners(4 * goodMarkers.size());
        
        for (size_t i=0; i<goodMarkers.size(); i++)
        {
            const Marker& marker = goodMarkers[i];
            
            for (int c = 0; c <4; c++)
            {
                preciseCorners[i*4 + c] = marker.points[c];
            }
        }
        
        cv::TermCriteria termCriteria = cv::TermCriteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 30, 0.01);
        cv::cornerSubPix(grayscale, preciseCorners, cvSize(5,5), cvSize(-1,-1), termCriteria);
        
        // Copy refined corners position back to markers
        for (size_t i=0; i<goodMarkers.size(); i++)
        {
            Marker& marker = goodMarkers[i];
            
            for (int c=0;c<4;c++)
            {
                marker.points[c] = preciseCorners[i*4 + c];
            }
        }
    }
    
#ifdef SHOW_DEBUG_IMAGES
    {
        cv::Mat markerCornersMat(grayscale.size(), grayscale.type());
        markerCornersMat = cv::Scalar(0);
        
        for (size_t i=0; i<goodMarkers.size(); i++)
        {
            goodMarkers[i].drawContour(markerCornersMat, cv::Scalar(255));
        }
        
        cv::imshow("Markers refined edges", grayscale * 0.5 + markerCornersMat);
        cvWaitKey();
    }
#endif
    
    detectedMarkers = goodMarkers;
}


/*!
 Set the threshold value for the contour threshold.
 @param threshold: integer between 0..255;
 */
void MarkerDetector::setThreshold(int threshold)
{
    if(threshold > 0 && threshold < 255)
        _threshold = threshold;

}





/*!
 Find and return markers
 */
void MarkerDetector::findMarkers( cv::Mat& src, std::vector<Marker>& markers)
{
    
    // convert image to a grayscale image.
    cv::cvtColor(src, _grayscale, CV_BGRA2GRAY);

    
    // Make it binary
    performThreshold(_grayscale, _threshold_image);
    
    _contours.clear();
    
    // Detect contours
    findContours(_threshold_image, _contours, _grayscale.cols / 5);
    
    // Find closed contours that can be approximated with 4 points
    findCandidates(_contours, detectedMarkers);
    
    // Find is them are markers
    recognizeMarkers(_grayscale, detectedMarkers);
    
    // Calculate their poses
    estimatePosition(detectedMarkers);
    
    //sort by id
    std::sort(detectedMarkers.begin(), detectedMarkers.end());

    markers = detectedMarkers;
}



void MarkerDetector::findMarkers( cv::Mat& src, std::vector<Marker>& markers, bool draw_markers)
{
    
    std::vector<Marker> loc_markers;
    findMarkers(src, loc_markers);
    
    _transformations.clear();
    for (size_t i=0; i<loc_markers.size(); i++)
    {
        _transformations.push_back(loc_markers[i].transformation);
    }
    
    markers = loc_markers;
    
    
    if(draw_markers)
        drawMarkers(src, loc_markers);


}



////////////////////////////////////////////////////////////////////////////////////////////////


float perimeter(const std::vector<cv::Point2f> &a)
{
    float sum=0, dx, dy;
    
    for (size_t i=0;i<a.size();i++)
    {
        size_t i2=(i+1) % a.size();
        
        dx = a[i].x - a[i2].x;
        dy = a[i].y - a[i2].y;
        
        sum += sqrt(dx*dx + dy*dy);
    }
    
    return sum;
}


bool isInto(cv::Mat &contour, std::vector<cv::Point2f> &b)
{
    for (size_t i=0;i<b.size();i++)
    {
        if (cv::pointPolygonTest( contour,b[i],false)>0) return true;
    }
    return false;
}

