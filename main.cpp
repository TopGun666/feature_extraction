#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;


int main(int argc, char *argv[]){
    if ( argc != 3 ){
        std::cout << "usage:feature_extraction img1 img2" << std::endl;
        return 1;
    }

    //Read image
    cv::Mat img_1 = cv::imread ( argv[1],CV_LOAD_IMAGE_COLOR );
    cv::Mat img_2 = cv::imread ( argv[2],CV_LOAD_IMAGE_COLOR );

    //Initialization
    std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
    cv::Mat descriptors_1, descriptors_2;
    cv::Ptr<cv::ORB> orb = cv::ORB::create ( 500, 1.2f, 8, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31, 20);

    //Step1: Detects Oriented FAST corner locations.
    orb->detect ( img_1, keypoints_1);
    orb->detect ( img_2, keypoints_2);

    //Step2: Calculate the BRIEF descriptor from the position of the corner points.
    orb->compute ( img_1, keypoints_1, descriptors_1);
    orb->compute ( img_2, keypoints_2, descriptors_2 );

    cv::Mat outimg1;
    cv::drawKeypoints( img_1, keypoints_1, outimg1, cv::Scalar::all( -1 ), cv::DrawMatchesFlags::DEFAULT);
    cv::imshow( "ORB Feature points ", outimg1 );

    //Step3: Match the BRIEF descriptor in both images, using the Hamming distance.
    std::vector<cv::DMatch> matches;
    cv::BFMatcher matcher ( cv::NORM_HAMMING );
    matcher.match ( descriptors_1, descriptors_2, matches );

    //Step4: Filter matching points.
    double min_dist = 10000, max_dist = 0;

    //Find the minimum and maximum distance between all matches, ie the distance between two groups of points that are the most similar and the least wanted
    for (int i = 0; i < descriptors_1.rows; i++){
        double dist = matches[i].distance;
        if ( dist < min_dist ) min_dist = dist;
        if ( dist > max_dist ) max_dist = dist;
    }

    std::cout << "--Max dist :" << max_dist << std::endl;
    std::cout << "--Min dist :" << min_dist << std::endl;

    //Declare the match incorrect when the distance between descriptors is more than twice the minimum distance.
    //The minium distance may be very little, We will set a experience value as a lower limit.
    std::vector<cv::DMatch> good_matches;
    for ( int i = 0; i < descriptors_1.rows; i++ ){
        if ( matches[i].distance <= max (2 * min_dist, 30.0) ){
            good_matches.push_back( matches[i] );
        }
    }

    //Step5: Draw matching results
    cv::Mat img_match;
    cv::Mat img_goodmatch;
    cv::drawMatches( img_1, keypoints_1, img_2, keypoints_2, matches, img_match );
    cv::drawMatches( img_1, keypoints_1, img_2, keypoints_2, good_matches, img_goodmatch );
    cv::imshow ("All match point pairs ", img_match );
    cv::imshow ( "Match piont pairs after filter ", img_goodmatch );
    cv::waitKey(0);

    return 0;
}
