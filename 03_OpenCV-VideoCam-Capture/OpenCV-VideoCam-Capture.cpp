// OpenCV-VideoCam-Capture.cpp : Defines the entry point for the console application.
//


#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char* argv[])
{

	// open the video camera with id 0
	VideoCapture cap(0);
	
	// check if we succeeded
	if(!cap.isOpened())
		return -1;
	
	// Read the camera properties width and height;
	double width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	std::cout << "Camera frame width: " << width << " and height: " << height << std::endl;


	// TODO: Test, which camera settings are available for your camera. 
	// Change camera properties
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1024);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 768);

	// Create a window with name video
	namedWindow("video", 1);

	for(;;)
	{
		Mat frame;

		// get new frame from camera
		cap >> frame;

		// Render the video frame
		imshow("video", frame);

		// Wait 30 ms and break, if a kez is pressed
		if(waitKey(30) >= 0) break;
	}

	return 0;
}

