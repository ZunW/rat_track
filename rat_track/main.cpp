//
//  main.cpp
//  rat_track
//
//  Created by Zun Wang on 2018-06-13.
//  Copyright © 2018 Zun Wang. All rights reserved.
//
//  files are named using the "_" and so does the wrapper fxn
//  other fxns are named using capitalized letter convention
//



#include <stdio.h>
#include "blur.hpp"
#include "sub_bg.hpp"
#include "change_reference.hpp"
#include "sub_bg_image.hpp"
#include "find_head.hpp"


using namespace std;
using namespace cv;



int main( void ){
    
//    RGBtest();

    
//    blur();
    
//    Mat sample;
//    return getBackground(sample);
//    sub_bg_wrapper("/Users/Achilles/Desktop/2018 summer research/project files/video/Videoclip7.avi");

//    sub_bg_img_wrapper("/Users/Achilles/Desktop/2018 summer research/project files/video/clip7afterTransWarp.avi");

//    markEarCap("/Users/Achilles/Desktop/2018 summer research/project files/video/clip7afterTransWarp.avi");

    vector<Point2f> hold = getHeadPosition("/Users/Achilles/Desktop/2018 summer research/project files/video/clip7afterTransWarp.avi");

    
//    videoTransNewFrame("/Users/Achilles/Desktop/2018 summer research/project files/video/Videoclip7.avi", "/Users/Achilles/Desktop/2018 summer research/project files/video/clip7afterTransWarp.avi", 100, 5);
    
    
//    Mat someMat=imread("/Users/Achilles/Desktop/2018 summer research/project files/video/snapshot9/1.png");
//    fourCorners myCorner = markCorners(someMat);
//    
//    cout << myCorner.upperL.first << " " << myCorner.upperL.second << endl;
//    cout << myCorner.upperR.first << " " << myCorner.upperR.second << endl;
//    cout << myCorner.bottomL.first << " " << myCorner.bottomL.second << endl;
//    cout << myCorner.bottomR.first << " " << myCorner.bottomR.second << endl;

//    getCoordNewFrame();
    
    
    
    
    
//    imread("/Users/Achilles/Desktop/2018 summer research/project files/video/snapshot9/1.png");

//    setMouseCallback("", )
    
    
    
    
}








////test findContour
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
//
//using namespace cv;
//using namespace std;
//
//Mat src; Mat src_gray;
//int thresh = 100;
//int max_thresh = 255;
//RNG rng(12345);
//
///// Function header
//void thresh_callback(int, void* );
//
///** @function main */
//int main( int argc, char** argv )
//{
//    /// Load source image and convert it to gray
//    src = imread( "/Users/Achilles/Desktop/lena.png" );
//    
//    /// Convert image to gray and blur it
//    cvtColor( src, src_gray, CV_BGR2GRAY );
//    blur( src_gray, src_gray, Size(3,3) );
//    
//    /// Create Window
//    string source_window = "Source";
//    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
//    imshow( source_window, src );
//    
//    createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
//    thresh_callback( 0, 0 );
//    
//    waitKey(0);
//    return(0);
//}
//
///** @function thresh_callback */
//void thresh_callback(int, void* )
//{
//    Mat canny_output;
//    vector<vector<Point> > contours;
//    vector<Vec4i> hierarchy;
//    
//    /// Detect edges using canny
//    Canny( src_gray, canny_output, thresh, thresh*2, 3 );
//    /// Find contours
//    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
//    
//    /// Draw contours
//    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
//    for( int i = 0; i< contours.size(); i++ )
//    {
//        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
//    }
//    
//    /// Show in a window
//    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
//    imshow( "Contours", drawing );
//}






//void RGBtest(void){
//    Mat orig = imread("/Users/Achilles/Desktop/2018 summer research/project files/video/snapshot9/1.png");
//    vector<Mat> vecMat;
//
//    split(orig, vecMat);
//
//    Mat Rgrey = vecMat[2];
//    Mat Ggrey = vecMat[1];
//    Mat Bgrey = vecMat[0];
//
//    imshow("R grey", Rgrey);
//    imshow("G grey", Ggrey);
//    imshow("B grey", Bgrey);
//
//
//    GaussianBlur(Rgrey, Rgrey, Size(5, 5), 50);
//    Canny(Rgrey, Rgrey, cannyLowThreshold, cannyHighThreshold);
//    imshow("R grey edge", Rgrey);
//
//
//    GaussianBlur(Ggrey, Ggrey, Size(5, 5), 50);
//    Canny(Ggrey, Ggrey, cannyLowThreshold, cannyHighThreshold);
//    imshow("G grey edge", Ggrey);
//
//
//    GaussianBlur(Bgrey, Bgrey, Size(5, 5), 50);
//    Canny(Bgrey, Bgrey, cannyLowThreshold, cannyHighThreshold);
//    imshow("B grey edge", Bgrey);
//
//
//    waitKey();
//
//    destroyAllWindows();
//
//    Mat origR = orig.clone();
//    Mat origG = orig.clone();
//    Mat origB = orig.clone();
//
//    for (int r = 0; r < orig.rows; r++) {
//        for (int c =0; c < orig.cols; c++){
//            origR.at<Vec3b>(r, c)[0] = 0;
//            origR.at<Vec3b>(r, c)[1] = 0;
//            origG.at<Vec3b>(r, c)[0] = 0;
//            origG.at<Vec3b>(r, c)[2] = 0;
//            origB.at<Vec3b>(r, c)[1] = 0;
//            origB.at<Vec3b>(r, c)[2] = 0;
//
//        }
//    }
//
//    imshow("R", origR);
//    imshow("G", origG);
//    imshow("B", origB);
//
//    waitKey();
//    destroyAllWindows();
//
//}






//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv/highgui.h"
//#include <stdlib.h>
//#include <stdio.h>
//
//using namespace cv;
//using namespace std;
//
///// Global variables
//Mat src, erosion_dst, dilation_dst;
//
//int erosion_elem = 0;
//int erosion_size = 0;
//int dilation_elem = 0;
//int dilation_size = 0;
//int const max_elem = 2;
//int const max_kernel_size = 21;
//
///** Function Headers */
//void Erosion( int, void* );
//void Dilation( int, void* );
//
///** @function main */
//int main( int argc, char** argv )
//{
//    /// Load an image
//    src = imread("/Users/Achilles/Desktop/2018 summer research/project files/opencv trial/rat_track/diff.png", CV_LOAD_IMAGE_GRAYSCALE);
//    
//    if( !src.data )
//    { return -1; }
//    
////    imshow("grayscale orig image", src);
////    
////    
////    //perform gaussian blur and edge detection
////    GaussianBlur(src, src, Size(5, 5), 50);
////    imshow("gaussian blur", src);
////    
////    Canny(src, src, 30, 90);
////    imshow("edge map", src);
////    
////    
//
//
//    
//    
//    
//    
//    
//    /// Create windows
//    namedWindow( "Erosion Demo", CV_WINDOW_AUTOSIZE );
//    namedWindow( "Dilation Demo", CV_WINDOW_AUTOSIZE );
//    cvMoveWindow( "Dilation Demo", src.cols, 0 );
//    
//    /// Create Erosion Trackbar
//    createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Erosion Demo",
//                   &erosion_elem, max_elem,
//                   Erosion );
//    
//    createTrackbar( "Kernel size:\n 2n +1", "Erosion Demo",
//                   &erosion_size, max_kernel_size,
//                   Erosion );
//    
//    /// Create Dilation Trackbar
//    createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Dilation Demo",
//                   &dilation_elem, max_elem,
//                   Dilation );
//    
//    createTrackbar( "Kernel size:\n 2n +1", "Dilation Demo",
//                   &dilation_size, max_kernel_size,
//                   Dilation );
//    
//    /// Default start
//    Erosion( 0, 0 );
//    Dilation( 0, 0 );
//    
//       waitKey(0);
//    return 0;
//} 
//
///**  @function Erosion  */
//void Erosion( int, void* )
//{
//    int erosion_type;
//    if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
//    else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
//    else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }
//    
//    Mat element = getStructuringElement( erosion_type,
//                                        Size( 2*erosion_size + 1, 2*erosion_size+1 ),
//                                        Point( erosion_size, erosion_size ) );
//    
//    /// Apply the erosion operation
//    erode( src, erosion_dst, element );
//    imshow( "Erosion Demo", erosion_dst );
//}
//
///** @function Dilation */
//void Dilation( int, void* )
//{
//    int dilation_type;
//    if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
//    else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
//    else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }
//    
//    Mat element = getStructuringElement( dilation_type,
//                                        Size( 2*dilation_size + 1, 2*dilation_size+1 ),
//                                        Point( dilation_size, dilation_size ) );
//    /// Apply the dilation operation
//    dilate( src, dilation_dst, element );
//    imshow( "Dilation Demo", dilation_dst );
//}



//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include <stdlib.h>
//#include <stdio.h>
//
//using namespace cv;
//
///// Global variables
//
//Mat src, src_gray;
//Mat dst, detected_edges;
//
//int edgeThresh = 1;
//int lowThreshold;
//int const max_lowThreshold = 100;
//int ratio = 3;
//int kernel_size = 3;
//char* window_name = "Edge Map";
//
///**
// * @function CannyThreshold
// * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
// */
//void CannyThreshold(int, void*)
//{
//    /// Reduce noise with a kernel 3x3
//    blur( src_gray, detected_edges, Size(3,3) );
//    
//    /// Canny detector
//    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
//    
//    /// Using Canny's output as a mask, we display our result
//    dst = Scalar::all(0);
//    
//    src.copyTo( dst, detected_edges);
//    imshow( window_name, dst );
//}
//
//
///** @function main */
//int main( void )
//{
//    /// Load an image
//    src = imread("/Users/Achilles/Desktop/2018 summer research/project files/video/snapshot9/1.png");
//    
//    if( !src.data )
//    { return -1; }
//    
//    /// Create a matrix of the same type and size as src (for dst)
//    dst.create( src.size(), src.type() );
//    
//    /// Convert the image to grayscale
//    cvtColor( src, src_gray, CV_BGR2GRAY );
//    
//    /// Create a window
//    namedWindow( window_name, CV_WINDOW_AUTOSIZE );
//    
//    /// Create a Trackbar for user to enter threshold
//    createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
//    
//    /// Show the image
//    CannyThreshold(0, 0);
//    
//    /// Wait until user exit program by pressing a key
//    waitKey(0);
//    
//    return 0;
//}
