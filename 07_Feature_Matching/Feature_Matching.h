//
//  Feature_Matching.h
//  HCI571X_Feature_Matching
//
//  Created by Rafael Radkowski on 2/13/14.
//
//
#pragma once


/*****************************************************************************
 A struct that can be used as template datatype for a feature map.
 */
typedef struct
{
    std::vector<cv::KeyPoint>           _keypointsRef;
    
    cv::Mat                             _descriptorRef;
    
    std::string                         _ref_image_str;
    
    cv::Mat                             _ref_image;
    
}FeatureMap;

// Create a datatype FeatureMapDB
typedef std::vector<FeatureMap > FeatureMapDB;

//*****************************************************************************
// A feature map database.
FeatureMapDB feature_map_database;


/*****************************************************************************
 Function that checks whether a file exists on hard disc.
 @param name: the name and path of the file.
 @return - true if the file exists, ohterwise false.
 */
bool exists_test (const std::string& name);


/*****************************************************************************
 Read all files from a particular directory and stores the filenames in a vector
 @param directory: the search directory as string
 @param filenames: a std::vector with the filenames in directory.
 */
int readDirFiles(std::string directory, std::vector<std::string>* filenames);


/*****************************************************************************
 Inits the database by loading all images from a certain directory, extracts the feature
 keypoints and descriptors and saves them in the database
 -keypointsRefDB and _descriptorsRefDB
 @param directory_path: the path of all input images for query, e.g., C:/my_database
 @param files: a std::vector object with a list of filenames.
 */
void init_database(std::string directory_path, std::vector<std::string> files);



/*****************************************************************************
 // Clear matches for which NN ratio is > than threshold
 // return the number of removed points
 // (corresponding entries being cleared,
 // i.e. size will be 0)
 */
int ratioTest(std::vector<std::vector<cv::DMatch> > &matches);


/*****************************************************************************
 // Identify good matches using RANSAC
 // Return fundemental matrix
 */
cv::Mat ransacTest( const std::vector<cv::DMatch>& matches,
                   const std::vector<cv::KeyPoint>& keypoints1,
                   const std::vector<cv::KeyPoint>& keypoints2,
                   std::vector<cv::DMatch>& outMatches);


/*****************************************************************************
 Check for symmetrical matches in matches1 and matches2 and adds them into symMatches
 @param matches1, matches2: the input matches (descriptor and keypoint index numbers) in an object of type DMatch
 @param symMatches: the output matches -
 ATTENTION: the mehtod returns only the closest match. The input of knn-match with k=2 are
 two closest solutons. At this time, we keep only the closest one in (*matchIterator1)[0].
 */
void symmetryTest(  const std::vector<std::vector<cv::DMatch> >& matches1,
                  const std::vector<std::vector<cv::DMatch> >& matches2,
                  std::vector<cv::DMatch>& symMatches);


/*****************************************************************************
 // The knn matching with k = 2
 // This code performs the matching and the refinement.
 // @paraam query_image: the input image
 // @param matches_out: a pointer that stores the output matches. It is necessary for
 //                     pose estimation.
 */
int knn_match(cv::Mat& query_image,  std::vector< cv::DMatch> * matches_out);


/*****************************************************************************
 // Runs the matching
 // This code reads all files from the set directory path as query images
 // and matches them against the database
 // @param directory_path: the path of all input images for query, e.g., C:/my_database
 // @param files: a std::vector object with a list of filenames.
 */
void run_matching(std::string directory_path, std::vector<std::string> files);


/*****************************************************************************
 // Runs the matching
 // This code opens a video camera and fetch the video stream
 // and matches the video against the database
 // @param video_device: the device number.
 // @return - false when the camera cannot be opened.
 */
bool run_matching(int video_device);

/*****************************************************************************
 // This applies a brute force match without a trained datastructure.
 // It also calculates the two nearest neigbors.
 // @paraam query_image: the input image
 // @param matches_out: a pointer that stores the output matches. It is necessary for
 //                     pose estimation.
 */
int brute_force_match(cv::Mat& query_image,  std::vector< cv::DMatch> * matches_out);


/*****************************************************************************
 // Runs the brute force matching
 // This code reads all files from the set directory path as query images
 // and matches them against the reference images
 // @param directory_path: the path of all input images for query, e.g., C:/my_database
 // @param files: a std::vector object with a list of filenames.
 */
void run_bf_matching(std::string directory_path, std::vector<std::string> files);


/*****************************************************************************
 // Runs the matching
 // This code opens a video camera and fetch the video stream
 // and matches the video against the database
 // @param video_device: the device number.
 // @return - false when the camera cannot be opened.
 */
bool run_bf_matching(int video_device);
