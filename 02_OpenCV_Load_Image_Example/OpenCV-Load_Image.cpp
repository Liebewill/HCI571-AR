// OpenCV-Pixel_Manipulation.cpp : Defines the entry point for the console application.
//

#include <opencv2/opencv.hpp>

using namespace cv;


int main(int argc, char* argv[])
{
    
    
    // Load an image from file
	Mat image = cv::imread("../Image.jpg");

    // Check whether the image is available
	if(!image.data)
	{
		return -1;
	}
    
    std::cout << "Press a key to continue." << std::endl;

    // Open a window and show the image on screen
    imshow("Window", image);
	cvWaitKey();

    
    
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
    // This example code browse thru all image pixels
    // and remove the color of each pixel 
    
    
	// Example image: BGR color model
	// Access the image pointer
	uchar* img_data = image.ptr<uchar>(0);

	// Create a new image
	Mat new_image = Mat(image.rows, image.cols, CV_8UC1);

    
	// Access the image pointer
	uchar* new_image_ptr = new_image.ptr<uchar>(0);

	// Get the number of channels
	int channels = image.channels();

	// Loop through all pixels in the image
	for(int i=0; i<image.rows; i++)
	{
		for(int j=0; j<image.cols; j++)
		{
			// this loop reads only the blue color component

			// calculate the read index: channels = 3
			int read_index = (i * image.cols * channels) + (j * channels);

			// calculate the write index; channel = 1
			int write_index = (i * image.cols) + j;

			// write the pixels into the new image
			new_image_ptr[write_index] = img_data[read_index];
		}
	}

	std::cout << "Press a key to continue." << std::endl;

	// show the image on screen
	// This function shows a grayscale image. 
	// - White = blue color component 
	// - Black = blue not in this pixel
	// It is shown as grayscale image because a one channel image can only be interpreted as grayscale image.
	imshow("Window", new_image);
	cvWaitKey();

	return 0;
}

