// HCI571X_CameraCalibrationExample.cpp : Defines the entry point for the console application.
//


#include <string>
#include <strstream>
#include <fstream>

#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char* argv[])
{
	// The input image
	Mat image;
    
    
    // These are the associated 3D chessboard <-> image coordinates.
    // These are vectors in vectors; every cell of the first vector keeps a second vector.
    // The elements of each vectors are corresponding image point - world point pairs.
    // The "Find corresponding points" section fills this vectors.
    // the points in world coordinates
    std::vector<std::vector<cv::Point3f> > objectPoints;
    // the point positions in pixels
    std::vector<std::vector<cv::Point2f> > imagePoints;
    
    // stores whether corners have been found.
    std::vector<bool> found_corners;
    

	// Create a window
	namedWindow("output", 1);
	namedWindow("input", 1);

    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////
    // Create a vector and add all the images file names for calibration
    std::vector<std::string>    sample_images;
    sample_images.push_back("../test_images/Image_01.jpg");
    sample_images.push_back("../test_images/Image_02.jpg");
    sample_images.push_back("../test_images/Image_03.jpg");
    sample_images.push_back("../test_images/Image_04.jpg");
    sample_images.push_back("../test_images/Image_05.jpg");
    sample_images.push_back("../test_images/Image_06.jpg");
    sample_images.push_back("../test_images/Image_07.jpg");
    sample_images.push_back("../test_images/Image_08.jpg");
    


    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////
    // Create the chessboard model
    
    // the points on the chessboard in image and 3D world coordinates.
    std::vector<cv::Point2f> imageCorners;
    std::vector<cv::Point3f> objectCorners;
    
    // Size of the chessboard
    // Count only the corners
    cv::Size boardSize(9,6);
    
    // 3D Scene Points:
    // Initialize the chessboard corners
    // in the chessboard reference frame
    // The corners are at 3D location (X,Y,Z)= (i,j,0)
    for (int i=0; i<boardSize.height; i++)
    {
        for (int j=0; j<boardSize.width; j++)
        {
            objectCorners.push_back(cv::Point3f(i, j, 0.0f));
        }
    }

    
    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////
    // Find corresponding points.
    
    // Keeps the number of successful processed images.
    int successes = 0;
    
    // this function size returns the number of elements (file names in our case)
    // and stores them in the variable num_images
    int num_images = sample_images.size();
    
    // This is the main loop. It runs through all images and applies the
    // looks for corresponding points.
    for (int i=0; i<num_images; i++)
    {
        
        // Open the image
        // The O (last parameter) says that the images are load as grayscale images
        image = cv::imread(sample_images[i],0);

        // Get the chessboard corners
        bool found = cv::findChessboardCorners(image, boardSize, imageCorners);
        
        // A helper to store whether corners have been found.
        // This is unimportant for the calibration, but it is a helper for the chessboard renderer; the colored lines
        found_corners.push_back(found);
        
        // Get subpixel accuracy on the corners
        cv::cornerSubPix(image, imageCorners,
                             cv::Size(5,5),
                             cv::Size(-1,-1),
                             cv::TermCriteria(cv::TermCriteria::MAX_ITER +
                                              cv::TermCriteria::EPS,
                                              30,      // max number of iterations
                                              0.1));  // min accuracy
        
        //If we have a good board, add it to our data
        if (imageCorners.size() == boardSize.area())
        {
        
            // Add image and scene points from one view
            // 2D image points from one view
            imagePoints.push_back(imageCorners);
            
            // corresponding 3D scene points
            objectPoints.push_back(objectCorners);
        
            // add +1 to remember this success.
            successes++;
        }
        
    }
    
    
        
    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////
    // Find intrinsic and extrinsic parameters
        
    // Output rotations and translation vectors
    // also known as the extrinsic parameters.
    std::vector<cv::Mat> rvecs, tvecs;
        
    // output Matrices
    cv::Mat intrincsicMatrix;
    cv::Mat distCoeffs;
        
    
    // flag to specify how calibration is done
    int flag = 0;
        
    // Size of the image in pixels
    Size imageSize(image.cols, image.rows);
        
    // Now we calculate the parameters.
    calibrateCamera(objectPoints, // the 3D points
                    imagePoints,  // the image points
                    imageSize,    // image size
                    intrincsicMatrix, // output camera matrix
                    distCoeffs,   // output distortion matrix
                    rvecs, tvecs, // Rs, Ts
                    flag);        // set options

	


	///////////////////////////////////////////////////////////
	//
	// store output into a file
	// Will be explained later in class
    FileStorage fs("Camera_Parameter_File.yml", FileStorage::WRITE);
	fs << "intrinsic_matrix" << intrincsicMatrix;
	fs << "dist_coeffs" << distCoeffs;
	fs.release();

    
    /////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////
    // Undistor all images
    
    // used in image undistortion
    cv::Mat map1,map2;
    

    
    // called once per calibration
    cv::initUndistortRectifyMap( intrincsicMatrix,  // computed camera matrix
                                distCoeffs,
                                cv::Mat(),
                                cv::Mat(),
                                // computed distortion matrix
                                // optional rectification (none)
                                // camera matrix to generate undistorted
                                image.size(),  // size of undistorted
                                CV_32FC1,      // type of output map
                                map1, map2);   // the x and y mapping functions
    

    
    for (int i=0; i<num_images; i++)
    {
    
        // The undistorted images
        cv::Mat undistorted;
        
        // Open the image
        image = cv::imread(sample_images[i]);
    
        // Apply mapping functions
        cv::remap(image, undistorted, map1, map2, cv::INTER_LINEAR); // interpolation type
       
        // Draws a chessboard
        drawChessboardCorners( image, boardSize, Mat(imagePoints[i]), found_corners[i] );
        
        // Add the text
        putText(undistorted, "Press a key to continue.", cvPoint(30,30),
                FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0,0,250), 1, CV_AA);
        
        
        // Show the images
        cvMoveWindow("input", 0, 10);
        imshow("input", image);
        cvWaitKey(1);
        cvMoveWindow("output", image.cols, 10);
        imshow("output", undistorted);
        cvWaitKey();
        
        // delete the images
        undistorted.release();
        image.release();
    }
    
    
    

	return 0;
}

