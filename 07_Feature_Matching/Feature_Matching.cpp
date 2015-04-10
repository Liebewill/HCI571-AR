//
//  main.cpp
//  HCI571X-FeatureDetector
//
//  Created by Dr.-Ing. Rafael Radkowski on 2/4/13.
//  Copyright (c) 2013 Dr.-Ing. Rafael Radkowski. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <string>
#include <strstream>

#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2D.hpp>


#include "Feature_Matching.h"


#ifdef WIN32
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#else
#include <dirent.h>
#include <unistd.h>
#endif

#define DEBUG_OUT

//-----------------------------------------------------
// pointer to the feature point detector object
cv::Ptr<cv::FeatureDetector>        _detector;

//-----------------------------------------------------
// pointer to the feature descriptor extractor object
cv::Ptr<cv::DescriptorExtractor>    _extractor;

//-----------------------------------------------------
// Parameters for the SIFT feature detector
double                              _contrast_threshold = 0.04;
double                              _edge_threshold = 16.0;
int                                 _octaves = 3;
double                              _sigma = 1.2; // The lower the sigma, the more keypoints can be found.
double                              _num_feature	= 2000;


double                              _hessianThreshold = 1800.0;
int                                 _surf_Octaves=2;
int                                 _surf_OctaveLayers=4;
bool                                _surf_extended=false;
bool                                _surf_upright=false;


//-----------------------------------------------------
// The matchers
cv::FlannBasedMatcher				_matcher;
cv::BruteForceMatcher< cv::L1<float> > _brute_force_matcher;


//-----------------------------------------------------
// The descriptor and keypoint databases
std::vector<cv::Mat>                        _descriptorsRefDB;
std::vector< std::vector<cv::KeyPoint> >    _keypointsRefDB;



//-----------------------------------------------------
// Parameters for optimization
float                               _ratio = 0.65f; // max ratio between 1st and 2nd NN

bool                                _refine = true; // if true will refine the F matrix

double                              _distance = 3.0; // min distance to epipolar

double                              _confidence = 0.99; // confidence level (probability)



int _num_ref_images = -1;
int _num_ref_features_in_db = 0;
std::string _num_ref_features_in_db_str;

//-----------------------------------------------------
// Variables helping to run the video
bool run_video = false;
int device_id = 0;



typedef enum
{
    SIFT,
    SURF,
    ORB
}FEATURETYPE;

FEATURETYPE _ftype = SIFT;


typedef enum
{
    KNN,
    BRUTEFORCE,
    
}MATCHINGTYPE;

MATCHINGTYPE _mtype = BRUTEFORCE;


/*****************************************************************************
 // MAIN
 */
int main(int argc, const char * argv[])
{
    
    
    
    //*************************************************************************
    // 1. Read the input files
    // This code reads the arguments from the input variable argv which is supposed to contain the
    // path of the input and reference database.
    std::string teachdb_folder, querydb_folder;
    if (argc > 2)
    {
        std::string command = argv[2];
        std::string type   = argv[1];
        if(type.compare("-SIFT")== 0)
        {
            _ftype = SIFT;
            std::cout << "NFT with SIFT feature detector and extractor." << std::endl;
        }
        else if(type.compare("-SURF")== 0)
        {
            _ftype = SURF;
            std::cout << "NFT with SURF feature detector and extractor." << std::endl;
        }
        else if(type.compare("-ORB")== 0)
        {
            _ftype = ORB;
            std::cout << "NFT with ORB feature detector and extractor." << std::endl;
        }
        
        
        if(command.compare("-file") == 0)
        {
            if(argc > 4)
            {
                teachdb_folder = argv[3];
                querydb_folder = argv[4];
                run_video = false;
            }
            else
            {
                std::cout << "No folder with query or reference images has been specified" << std::endl;
                std::cout << "Call: ./HCI571X_Feature_Matching -file folder_reference folder_query" << std::endl;
                system("pause");
                exit(0);
            }
            
        }
        else if(command.compare("-video") == 0)
        {
            run_video = true;
            if(argc > 4)
            {
                teachdb_folder = argv[3];
                device_id = atoi(argv[4]);
            }
        }
    }
    else
    {
        std::cout << "No command has been specified. use -file or -video" << std::endl;
        system("pause");
        exit(0);
    }
    
    
    
    // Read the filenames inside the teach database directory.
    std::vector<std::string> ref_filename;
    readDirFiles(teachdb_folder, &ref_filename);
    
    
    // Read the filenames inside the query database directory.
    std::vector<std::string> query_filename;
    readDirFiles(querydb_folder, &query_filename);
    
    
    //*************************************************************************
    // 2. Create a detector and a descriptor extractor
    // In this case, the SIFT detector and extractor are used
    
    // Corner detector
    if(_ftype == SIFT)_detector = new cv::SiftFeatureDetector(_num_feature, _octaves, _contrast_threshold, _edge_threshold, _sigma);
    else if(_ftype == SURF)_detector = new cv::SurfFeatureDetector( _hessianThreshold, _surf_Octaves, _surf_OctaveLayers, _surf_extended, _surf_upright );
    else if(_ftype == ORB)_detector = new cv::OrbFeatureDetector(1000);
    
    
    // Corner extractor
    if(_ftype == SIFT) _extractor = new cv::SiftDescriptorExtractor(_num_feature, _octaves, _contrast_threshold, _edge_threshold, _sigma);
    else if(_ftype == SURF) _extractor = new cv::SurfDescriptorExtractor( _hessianThreshold, _surf_Octaves, _surf_OctaveLayers, _surf_extended, _surf_upright );
    else if(_ftype == ORB)_extractor = new cv::OrbDescriptorExtractor(1000);
    

	// Check whether files are in the database list. 
	if(ref_filename.size() == 0)
	{
		std::cout << "STOP: no files in the reference database!!! Specify a folder or a set of files." << std::cout;
		system("pause");
		return -1;
	}

    //*************************************************************************
    // 3. Init the database
    // The code reads all the images in ref_filename, detect keypoints, extract descriptors and
    // stores them in the datbase variables.
    init_database(std::string(teachdb_folder), ref_filename);
    
    
    //*************************************************************************
    // 4. The data of the database _descriptorsRefDB is added to the featue matcher
    // and the mathcer is trained
    _matcher.add(_descriptorsRefDB);
    _matcher.train();
    
    // Read the number of reference images in the database
    _num_ref_images = _matcher.getTrainDescriptors().size();
    
    
    //*************************************************************************
    // 5. Here we run the matching.
    // for images from files
    if(!run_video)
    {
        if(_mtype == KNN)run_matching( querydb_folder, query_filename);
        else if(_mtype == BRUTEFORCE) run_bf_matching(querydb_folder, query_filename);
        else
        {
            std::cout << "No matching type specified. Specify a matching type" << std::endl;
            system("pause");
        }
        
    }
    else
        // and image from a video camera
    {
        if(_mtype == KNN)run_matching( device_id);
        else if(_mtype == BRUTEFORCE)  run_bf_matching(device_id);
        else
        {
            std::cout << "No matching type specified. Specify a matching type" << std::endl;
            system("pause");
        }
        
        
    }
    
    //*************************************************************************
    // 6. Cleanup: release the keypoint detector and feature descriptor extractor
    _extractor.release();
    _detector.release();
    
    
    return 1;
}



/*****************************************************************************
 Function that checks whether a file exists on hard disc.
 @param name: the name and path of the file.
 @return - true if the file exists, ohterwise false.
 */
bool exists_test (const std::string& name) {
    std::ifstream f(name.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }
}


/*****************************************************************************
 Read all files from a particular directory and stores the filenames in a vector
 @param directory: the search directory as string
 @param filenames: a std::vector with the filenames in directory.
 */
int readDirFiles(std::string directory, std::vector<std::string>* filenames)
{
#ifdef WIN32

	std::vector<std::string> names;
    char search_path[200];

    sprintf(search_path, "%s/*.*", directory.c_str());

    WIN32_FIND_DATA fd; 
    HANDLE hFind = ::FindFirstFile(search_path, &fd); 

    if(hFind != INVALID_HANDLE_VALUE) 
    { 
        do 
        { 

            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) 
            {
				if(fd.cFileName[0] != '.')
					filenames->push_back(fd.cFileName);
            }
        }while(::FindNextFile(hFind, &fd)); 
        ::FindClose(hFind); 
    } 
    return 1;

#else
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (directory.c_str())) != NULL)
    {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            if(ent->d_namlen > 3)
            {
                std::string dot = ".";
                if( ent->d_name[0] != '.')
                {
                    //printf ("%s\n", ent->d_name);
                    filenames->push_back(ent->d_name);
                }
            }
        }
        closedir (dir);
    }
    else
    {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
    }
	return 1;
#endif
   
}

/*****************************************************************************
 Inits the database by loading all images from a certain directory, extracts the feature
 keypoints and descriptors and saves them in the database
 -keypointsRefDB and _descriptorsRefDB
 @param directory_path: the path of all input images for query, e.g., C:/my_database
 @param files: a std::vector object with a list of filenames.
 */
void init_database(std::string directory_path, std::vector<std::string> files)
{
    
    // This code reads the image names from the argv variable, loads the image
    // detect keypoints, extract the descriptors and push everything into
    // two database objects, indicate by the ending DB.
    for (int i = 0; i<files.size(); i++)
    {
        FeatureMap fm;
        
        // Fetch the ref. image name from the input array
        fm._ref_image_str =  directory_path;
        fm._ref_image_str.append("/");
        fm._ref_image_str.append( files[i]);
        
        bool ret = exists_test(fm._ref_image_str);
        if (ret == false) continue;
        
        
        // Load the image
        fm._ref_image = cv::imread(fm._ref_image_str);
        cvtColor( fm._ref_image ,  fm._ref_image , CV_BGR2GRAY);
        
        // Detect the keypoints
        _detector->detect(fm._ref_image,fm._keypointsRef);
        
        std::cout << "For referemce image " << fm._ref_image_str << " - found " << fm._keypointsRef.size() << " keypoints" << std::endl;
        
        // If keypoints found
        if(fm._keypointsRef.size() > 0)
        {
            // extract descriptors
            _extractor->compute( fm._ref_image, fm._keypointsRef, fm._descriptorRef);
            
            // push descriptors and keypoints into database
            _keypointsRefDB.push_back(fm._keypointsRef);
            _descriptorsRefDB.push_back(fm._descriptorRef);
            
            // count the total number of feature descriptors and keypoints in the database.
            _num_ref_features_in_db += fm._keypointsRef.size();
        }
        
        // Draw the keypoints
        /*
         cv::Mat out;
         cv::drawKeypoints(fm._ref_image , fm._keypointsRef, out);
         cv::imshow("out", out );
         cv::waitKey();
         
         out.release();
         */
        
        feature_map_database.push_back(fm);
        
    }
    
    // Converting the total number of features to a string.
    std::strstream conv;
    conv << _num_ref_features_in_db;
    conv >> _num_ref_features_in_db_str;
    
}


/*****************************************************************************
 // Clear matches for which NN ratio is > than threshold
 // return the number of removed points
 // (corresponding entries being cleared,
 // i.e. size will be 0)
 */
int ratioTest(std::vector<std::vector<cv::DMatch> > &matches)
{
    int removed=0;
    // for all matches
    for (std::vector<std::vector<cv::DMatch> >::iterator matchIterator= matches.begin();
         matchIterator!= matches.end();
         ++matchIterator)
    {
        
        // if 2 NN has been identified
        if (matchIterator->size() > 1)
        {
            // check distance ratio
            if ((*matchIterator)[0].distance/(*matchIterator)[1].distance > _ratio)
            {
                matchIterator->clear(); // remove match
                removed++;
            }
        } else
        { // does not have 2 neighbours
            matchIterator->clear(); // remove match
            removed++;
        }
    }
    return removed;
}


/*****************************************************************************
 // Identify good matches using RANSAC
 // Return fundemental matrix
 */
cv::Mat ransacTest( const std::vector<cv::DMatch>& matches,
                   const std::vector<cv::KeyPoint>& keypoints1,
                   const std::vector<cv::KeyPoint>& keypoints2,
                   std::vector<cv::DMatch>& outMatches)
{
    
    // Convert keypoints into Point2f
    std::vector<cv::Point2f> points1, points2;
    for (   std::vector<cv::DMatch>::
         const_iterator it= matches.begin();
         it!= matches.end(); ++it)
        
    {
        // Get the position of left keypoints
        float x= keypoints1[it->queryIdx].pt.x;
        float y= keypoints1[it->queryIdx].pt.y;
        points1.push_back(cv::Point2f(x,y));
        
        // Get the position of right keypoints
        x= keypoints2[it->trainIdx].pt.x;
        y= keypoints2[it->trainIdx].pt.y;
        points2.push_back(cv::Point2f(x,y));
    }
    
    
    // Compute F matrix using RANSAC
    std::vector<uchar> inliers(points1.size(),0);
    cv::Mat fundemental= cv::findFundamentalMat(cv::Mat(points1),cv::Mat(points2), // matching points
                                                inliers,      // match status (inlier or outlier)
                                                CV_FM_RANSAC, // RANSAC method
                                                _distance,     // distance to epipolar line
                                                _confidence);  // confidence probability
    
    // extract the surviving (inliers) matches
    std::vector<uchar>::const_iterator
    itIn= inliers.begin();
    std::vector<cv::DMatch>::const_iterator
    itM= matches.begin();
    // for all matches
    for ( ;itIn!= inliers.end(); ++itIn, ++itM)
    {
        if (*itIn)
        { // it is a valid match
            outMatches.push_back(*itM);
        }
    }
    
    if (_refine)
    {
        // The F matrix will be recomputed with
        // all accepted matches
        // Convert keypoints into Point2f
        // for final F computation
        points1.clear();
        points2.clear();
        for (std::vector<cv::DMatch>::
             const_iterator it= outMatches.begin();
             it!= outMatches.end(); ++it)
        {
            // Get the position of left keypoints
            float x= keypoints1[it->queryIdx].pt.x;
            float y= keypoints1[it->queryIdx].pt.y;
            points1.push_back(cv::Point2f(x,y));
            
            // Get the position of right keypoints
            x= keypoints2[it->trainIdx].pt.x;
            y= keypoints2[it->trainIdx].pt.y;
            points2.push_back(cv::Point2f(x,y));
        }
        // Compute 8-point F from all accepted matches
        //fundemental = cv::findFundamentalMat(cv::Mat(points1),cv::Mat(points2), CV_FM_8POINT); // 8-point method
        
        fundemental =  cv::findHomography(cv::Mat(points1),cv::Mat(points2), cv::RANSAC/*CV_LMEDS*/, 1);
    }
    return fundemental;
}

/*****************************************************************************
 Check for symmetrical matches in matches1 and matches2 and adds them into symMatches
 @param matches1, matches2: the input matches (descriptor and keypoint index numbers) in an object of type DMatch
 @param symMatches: the output matches -
 ATTENTION: the mehtod returns only the closest match. The input of knn-match with k=2 are
 two closest solutons. At this time, we keep only the closest one in (*matchIterator1)[0].
 */
void symmetryTest(  const std::vector<std::vector<cv::DMatch> >& matches1,
                  const std::vector<std::vector<cv::DMatch> >& matches2,
                  std::vector<cv::DMatch>& symMatches)
{
    // for all matches image 1 -> image 2
    for (   std::vector<std::vector<cv::DMatch> >::
         const_iterator matchIterator1= matches1.begin();
         matchIterator1!= matches1.end(); ++matchIterator1)
    {
        // ignore deleted matches
        if (matchIterator1->size() < 2)
            continue;
        
        // for all matches image 2 -> image 1
        for (   std::vector<std::vector<cv::DMatch> >::
             const_iterator matchIterator2= matches2.begin();
             matchIterator2!= matches2.end();
             ++matchIterator2)
        {
            // ignore deleted matches
            if (matchIterator2->size() < 2)
                continue;
            
            // Match symmetry test
            if ((*matchIterator1)[0].queryIdx ==
                (*matchIterator2)[0].trainIdx  &&
                (*matchIterator2)[0].queryIdx ==
                (*matchIterator1)[0].trainIdx)
            {
                // add symmetrical match
                symMatches.push_back( cv::DMatch( (*matchIterator1)[0].trainIdx,
                                                 (*matchIterator1)[0].queryIdx,
                                                 (*matchIterator1)[0].imgIdx,
                                                 (*matchIterator1)[0].distance));
                break; // next match in image 1 -> image 2
            }
        }
    }
}


/*****************************************************************************
 // This applies a brute force match without a trained datastructure.
 // It also calculates the two nearest neigbors.
 // @paraam query_image: the input image
 // @param matches_out: a pointer that stores the output matches. It is necessary for
 //                     pose estimation.
 */
int brute_force_match(cv::Mat& query_image,  std::vector< cv::DMatch> * matches_out)
{
    
    // variabels that keep the query keypoints and query descriptors
    std::vector<cv::KeyPoint>           keypointsQuery;
    cv::Mat                             descriptorQuery;
    
    // Temporary variables for the matching results
    std::vector< std::vector< cv::DMatch> > matches1;
    std::vector< std::vector< cv::DMatch> > matches2;
    std::vector< std::vector< cv::DMatch> > matches_opt1;
    
    
    //////////////////////////////////////////////////////////////////////
    // 1. Detect the keypoints
    // This line detects keypoints in the query image
    _detector->detect(query_image, keypointsQuery);
    
    
    
    // If keypoints were found, descriptors are extracted.
    if(keypointsQuery.size() > 0)
    {
        // extract descriptors
        _extractor->compute( query_image, keypointsQuery, descriptorQuery);
        
    }
    
#ifdef DEBUG_OUT
    std::cout << "Found " << descriptorQuery.size() << " feature descriptors in the image."  << std::endl;
#endif
    
    
    //////////////////////////////////////////////////////////////////////////////
    // 2. Here we match the descriptors with all descriptors in the database
    // with k-nearest neighbors with k=2
    
    int max_removed = INT_MAX;
    int max_id = -1;
    
    for(int i=0; i<_descriptorsRefDB.size(); i++)
    {
        std::vector< std::vector< cv::DMatch> > matches_temp1;
        
        // Here we match all query descriptors agains all db descriptors and try to find
        // matching descriptors
        _brute_force_matcher.knnMatch( descriptorQuery, _descriptorsRefDB[i],  matches_temp1, 2);
        
        
        ///////////////////////////////////////////////////////
        // 3. Refinement; Ratio test
        // The ratio test only accept matches which are clear without ambiguity.
        // The best hit must be closer to the query descriptors than the second hit.
        int removed = ratioTest(matches_temp1);
        
        
        
        // We only accept the match with the highest number of hits / the vector with the minimum revmoved features
        int num_matches = matches_temp1.size();
        if(removed < max_removed)
        {
            max_removed = removed;
            max_id = i;
            matches1.clear();
            matches1 = matches_temp1;
        }
    }
    
#ifdef DEBUG_OUT
    std::cout << "Feature map number " << max_id << " has the highest hit with "<< matches1.size() -  max_removed << " descriptors." << std::endl;
#endif
    
    
    std::vector< std::vector< cv::DMatch> > matches_temp2;
    
    // Here we match all query descriptors agains all db descriptors and try to find
    // matching descriptors
    _brute_force_matcher.knnMatch(_descriptorsRefDB[max_id],  descriptorQuery,  matches_temp2, 2);
    
    // The ratio test only accept matches which are clear without ambiguity.
    // The best hit must be closer to the query descriptors than the second hit.
    int removed = ratioTest(matches_temp2);
    
    
    
    
    ///////////////////////////////////////////////////////
    // 6. Refinement; Symmetry test
    // We only accept matches which appear in both knn-matches.
    // It should not matter whether we test the database against the query desriptors
    // or the query descriptors against the database.
    // If we do not find the same solution in both directions, we toss the match.
    std::vector<cv::DMatch> symMatches;
    symmetryTest(  matches1, matches_temp2, symMatches);
#ifdef DEBUG_OUT
    std::cout << "Kept " << symMatches.size() << " matches after symetry test test."  << std::endl;
#endif
    
    ///////////////////////////////////////////////////////
    // 7. Refinement; Epipolar constraint
    // We perform a Epipolar test using the RANSAC method.
    if(symMatches.size() > 25)
    {
        matches_out->clear();
        ransacTest( symMatches,  _keypointsRefDB[max_id], keypointsQuery, *matches_out);
        
        
    }
    
#ifdef DEBUG_OUT
    std::cout << "Kept " << matches_out->size() << " matches after RANSAC test."  << std::endl;
#endif
    
    ///////////////////////////////////////////////////////
    // 8.  Draw this image on screen.
    cv::Mat out;
    cv::drawMatches(feature_map_database[max_id]._ref_image , _keypointsRefDB[max_id], query_image, keypointsQuery, *matches_out, out, cv::Scalar(255,255,255), cv::Scalar(0,0,255));
    
    std::string num_matches_str;
    std::strstream conv;
    conv << matches_out->size();
    conv >> num_matches_str;
    
    std::string text;
    text.append( num_matches_str);
    text.append("( " + _num_ref_features_in_db_str + " total)");
    text.append(" matches were found in reference image ");
    text.append( feature_map_database[max_id]._ref_image_str);
    
    putText(out, text, cvPoint(20,20),
            cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cvScalar(0,255,255), 1, CV_AA);
    
    cv::imshow("result", out);
    if (run_video) cv::waitKey(1);
    else cv::waitKey();
    
    
    
    // Delete the images
    query_image.release();
    out.release();
    
    
    return max_id;
    
}


/*****************************************************************************
 // Runs the brute force matching
 // This code reads all files from the set directory path as query images
 // and matches them against the reference images
 // @param directory_path: the path of all input images for query, e.g., C:/my_database
 // @param files: a std::vector object with a list of filenames.
 */
void run_bf_matching(std::string directory_path, std::vector<std::string> files)
{
    
    // loop through all images in the query folder
    for (int i = 0; i<files.size(); i++)
    {
        
        // Fetch the ref. image name from the input array\
        // and assemble a path.
        std::string query_image_str =  directory_path;
        query_image_str.append("/");
        query_image_str.append( files[i]);
        
        
#ifdef DEBUG_OUT
        std::cout << "*****\nTesting image " << files[i] << std::endl;
#endif
        
        // Check whether the file exists.
        bool ret = exists_test(query_image_str);
        if (ret == false) continue;
        
        
        // Load the image and convert it into a greyscale image
        cv::Mat query_image = cv::imread(query_image_str);
        cvtColor( query_image ,  query_image , CV_BGR2GRAY);
        
        std::vector< cv::DMatch > matches;
        
        // Match it against the database.
        int img_idx = brute_force_match(query_image, &matches);
        
        // release the image
        query_image.release();
    }
}



/*****************************************************************************
 // Runs the matching
 // This code opens a video camera and fetch the video stream
 // and matches the video against the database
 // @param video_device: the device number.
 // @return - false when the camera cannot be opened.
 */
bool run_bf_matching(int video_device)
{
    bool is_running = true;
    
    // Open a capture device
    cv::VideoCapture capture(video_device);
    
#ifdef WIN32
    Sleep(2000.0);
#else
    sleep(2.0);
#endif
    
    // Check whether the device is open.
    if(!capture.isOpened())
        return false;
    
    
    cv::Size video_size;
    
    double w = (double) capture.get(CV_CAP_PROP_FRAME_WIDTH);
    double h = (double) capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    video_size.width = 800;
    video_size.height = video_size.width / (w/h);
    
    cv::Mat query_image;
    // loop until the variable is_running is false;
    while (is_running)
    {
        
        // Fetch the input image
        capture >> query_image;
        
        // convert it into a greyscale image and resize the image
        cvtColor( query_image ,  query_image , CV_BGR2GRAY);
        cv::resize(query_image, query_image, video_size );
        
        std::vector< cv::DMatch > matches;
        
        // Match it against the database.
        int img_idx = brute_force_match(query_image, &matches);
        
        // wait for the key q
        int ret = cv::waitKey(1);
        if(ret == 113)is_running=false;
        
    }
    
    // release the camera
    capture.release();
    
    // release the image
    query_image.release();
}




/*****************************************************************************
 // The knn matching with k = 2
 // This code performs the matching and the refinement.
 // @paraam query_image: the input image
 // @param matches_out: a pointer that stores the output matches. It is necessary for
 //                     pose estimation.
 */
int knn_match(cv::Mat& query_image,  std::vector< cv::DMatch> * matches_out)
{
    // variabels that keep the query keypoints and query descriptors
    std::vector<cv::KeyPoint>           keypointsQuery;
    cv::Mat                             descriptorQuery;
    
    // Temporary variables for the matching results
    std::vector< std::vector< cv::DMatch> > matches1;
    std::vector< std::vector< cv::DMatch> > matches2;
    std::vector< std::vector< cv::DMatch> > matches_opt1;
    
    
    //////////////////////////////////////////////////////////////////////
    // 1. Detect the keypoints
    // This line detects keypoints in the query image
    _detector->detect(query_image, keypointsQuery);
    
    // If keypoints were found, descriptors are extracted.
    if(keypointsQuery.size() > 0)
    {
        // extract descriptors
        _extractor->compute( query_image, keypointsQuery, descriptorQuery);
        
    }
    
    //////////////////////////////////////////////////////////////////////////////
    // 2. Here we match the descriptors with the database descriptors.
    // with k-nearest neighbors with k=2
    _matcher.knnMatch(descriptorQuery , matches1, 2);
    
#ifdef DEBUG_OUT
    std::cout << "Found " << matches1.size() << " matching feature descriptors out of " << _matcher.getTrainDescriptors().size() << " database descriptors."  << std::endl;
#endif
    
    
    //////////////////////////////////////////////////////////////////////////////
    // 3 Filter the matches.
    // Accept only matches (knn with k=2) which belong ot one images
    // The database tree within _matcher contains descriptors of all input images.
    // We expect that both nearest neighbors must belong to one image.
    // Otherwise we can remove the result.
    // Along with this, we count which reference image has the highest number of matches.
    // At this time, we consinder this image as the searched image.
    
    // we init the variable hit with 0
    std::vector<int> hits(_num_ref_images);
    for (int i=0; i<_num_ref_images; i++)
    {
        hits[i] = 0;
    }
    
    // the loop runs through all matches and comparees the image indes
    // imgIdx. The must be equal otherwise the matches belong to two
    // different reference images.
    for (int i=0; i<matches1.size(); i++)
    {
        // The comparison.
        if(matches1[i].at(0).imgIdx == matches1[i].at(1).imgIdx)
        {
            // we keep it
            matches_opt1.push_back(matches1[i]);
            // and count a hit
            hits[matches1[i].at(0).imgIdx]++;
        }
    }
    
#ifdef DEBUG_OUT
    std::cout << "Optimized " << matches_opt1.size() << " feature descriptors." << std::endl;
#endif
    
    // Now we search for the highest number of hits in our hit array
    // The variable max_idx keeps the image id.
    // The variable max_value the amount of hits.
    int max_idx = -1;
    int max_value = 0;
    for (int i=0; i<_num_ref_images; i++)
    {
#ifdef DEBUG_OUT
        std::cout << "for " << i << " : " << hits[i] << std::endl;
#endif
        if(hits[i]  > max_value)
        {
            max_value = hits[i];
            max_idx = i;
        }
    }
    
    
    
    ///////////////////////////////////////////////////////
    // 4. The cross-match
    // At this time, we test the database agains the query descriptors.
    // The variable max_id stores the reference image id. Thus, we test only
    // the descriptors that belong to max_idx agains the query descriptors.
    _matcher.knnMatch(_descriptorsRefDB[max_idx], descriptorQuery, matches2, 2);
    
    
    ///////////////////////////////////////////////////////
    // 5. Refinement; Ratio test
    // The ratio test only accept matches which are clear without ambiguity.
    // The best hit must be closer to the query descriptors than the second hit.
    int removed = ratioTest(matches_opt1);
#ifdef DEBUG_OUT
    std::cout << "Removed " << removed << " matched."  << std::endl;
#endif
    
    removed = ratioTest(matches2);
#ifdef DEBUG_OUT
    std::cout << "Removed " << removed << " matched."  << std::endl;
#endif
    
    ///////////////////////////////////////////////////////
    // 6. Refinement; Symmetry test
    // We only accept matches which appear in both knn-matches.
    // It should not matter whether we test the database against the query desriptors
    // or the query descriptors against the database.
    // If we do not find the same solution in both directions, we toss the match.
    std::vector<cv::DMatch> symMatches;
    symmetryTest(  matches_opt1, matches2, symMatches);
#ifdef DEBUG_OUT
    std::cout << "Kept " << symMatches.size() << " matches after symetry test test."  << std::endl;
#endif
    
    ///////////////////////////////////////////////////////
    // 7. Refinement; Epipolar constraint
    // We perform a Epipolar test using the RANSAC method.
    if(symMatches.size() > 25)
    {
        matches_out->clear();
        ransacTest( symMatches,  _keypointsRefDB[max_idx], keypointsQuery, *matches_out);
        
        
    }
    
#ifdef DEBUG_OUT
    std::cout << "Kept " << matches_out->size() << " matches after RANSAC test."  << std::endl;
#endif
    
    ///////////////////////////////////////////////////////
    // 8.  Draw this image on screen.
    cv::Mat out;
    cv::drawMatches(feature_map_database[max_idx]._ref_image , _keypointsRefDB[max_idx], query_image, keypointsQuery, *matches_out, out, cv::Scalar(255,255,255), cv::Scalar(0,0,255));
    
    std::string num_matches_str;
    std::strstream conv;
    conv << matches_out->size();
    conv >> num_matches_str;
    
    std::string text;
    text.append( num_matches_str);
    text.append("( " + _num_ref_features_in_db_str + " total)");
    text.append(" matches were found in reference image ");
    text.append( feature_map_database[max_idx]._ref_image_str);
    
    putText(out, text, cvPoint(20,20),
            cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cvScalar(0,255,255), 1, CV_AA);
    
    cv::imshow("result", out);
    if (run_video) cv::waitKey(1);
    else cv::waitKey();
    
    
    
    // Delete the images
    query_image.release();
    out.release();
    
    
    
    return max_idx;
    
}

/*****************************************************************************
 // Runs the matching
 // This code reads all files from the set directory path as query images
 // and matches them against the database
 // @param directory_path: the path of all input images for query, e.g., C:/my_database
 // @param files: a std::vector object with a list of filenames.
 */
void run_matching(std::string directory_path, std::vector<std::string> files)
{
    
    // loop through all images in the query folder
    for (int i = 0; i<files.size(); i++)
    {
        
        // Fetch the ref. image name from the input array\
        // and assemble a path.
        std::string query_image_str =  directory_path;
        query_image_str.append("/");
        query_image_str.append( files[i]);
        
        
        // Check whether the file exists.
        bool ret = exists_test(query_image_str);
        if (ret == false) continue;
        
        
        // Load the image and convert it into a greyscale image
        cv::Mat query_image = cv::imread(query_image_str);
        cvtColor( query_image ,  query_image , CV_BGR2GRAY);
        
        std::vector< cv::DMatch > matches;
        
        // Match it against the database.
        int img_idx = knn_match(query_image, &matches);
        
        // release the image
        query_image.release();
    }
}



/*****************************************************************************
 // Runs the matching
 // This code opens a video camera and fetch the video stream
 // and matches the video against the database
 // @param video_device: the device number.
 // @return - false when the camera cannot be opened.
 */
bool run_matching(int video_device)
{
    bool is_running = true;
    
    // Open a capture device
    cv::VideoCapture capture(video_device);
    
#ifdef WIN32
    Sleep(2000.0);
#else
    sleep(2.0);
#endif
    
    // Check whether the device is open.
    if(!capture.isOpened())
        return false;
    
    
    cv::Size video_size;
    
    double w = (double) capture.get(CV_CAP_PROP_FRAME_WIDTH);
    double h = (double) capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    video_size.width = 800;
    video_size.height = video_size.width / (w/h);
    
    cv::Mat query_image;
    // loop until the variable is_running is false;
    while (is_running)
    {
        
        // Fetch the input image
        capture >> query_image;
        
        // convert it into a greyscale image and resize the image
        cvtColor( query_image ,  query_image , CV_BGR2GRAY);
        cv::resize(query_image, query_image, video_size );
        
        std::vector< cv::DMatch > matches;
        
        // Match it against the database.
        int img_idx = knn_match(query_image, &matches);
        
        // wait for the key q
        int ret = cv::waitKey(1);
        if(ret == 113)is_running=false;
        
    }
    
    // release the camera
    capture.release();
    
    // release the image
    query_image.release();
}

