////
////  try_convex_hull.cpp
////  rat_track
////
////  Created by Zun Wang on 2018-07-31.
////  Copyright © 2018 Zun Wang. All rights reserved.
////
//
//#include "try_convex_hull.hpp"
//
//
//
//
////self-defined comparator
//bool contourCompareArea::operator()(const contourCompareArea& c1, const contourCompareArea& c2){
//    return c1.area < c2.area;
//}
//
////a construcutor
//contourCompareArea::contourCompareArea(vector<Point> conPoints, double conArea){
//    contourPoints = conPoints;
//    area = conArea;
//}
//
////default constructor
//contourCompareArea::contourCompareArea(){};
//
//
//
//double getMinDistance(vector<Point> contour1, vector<Point> contour2){
//    double min_dist = 99999999;
//    for (vector<Point>::iterator it1 = contour1.begin(); it1 != contour1.end(); it1 ++){
//        for (vector<Point>::iterator it2 = contour2.begin(); it2 != contour2.end(); it2 ++){
//            double distance = norm((*it1)-(*it2));
//            if ( distance < min_dist){
//                min_dist = distance;
//            }
//        }
//    }
//    
//    return min_dist;
//}
//
//
//
////it's 3 processes packed into 1: medianblur, erosion, dilation
////supposedly, it's a downstream operation after we have the difference in image, where there's quite some noise
////and gaps, so we want to get rid of the noise by medianblur/erosion and fill in the gaps after
////we only perform each of the 3 processing if the respective kernal size is greater than 0
//void frameDiffProcessing (const Mat& input, Mat& output, int medianSize, int erosionSize, int dilationSize){
//    
//    //first get rid of the noise (slim splinters along the bars)
//    Mat afterNR;
//    if (medianSize > 0){
//        medianBlur(input, afterNR, 2*medianSize+1);
//        //imshow("after median", afterNR);
//    }
//    else if (erosionSize > 0){
//        Mat erosionElement = getStructuringElement( MORPH_ELLIPSE, Size( 2*erosionSize + 1, 2*erosionSize+1 ),
//                                                   Point( erosionSize, erosionSize ) );
//        erode( input, afterNR, erosionElement );
//        //imshow("after erosion", afterNR);
//    }
//    
//    //filling int the gaps especially caused by bars
//    if (dilationSize > 0){
//        Mat dilationElement = getStructuringElement( MORPH_ELLIPSE, Size( 2*dilationSize + 1, 2*dilationSize+1), Point( dilationSize, dilationSize ) );
//        dilate( afterNR, output, dilationElement );
//        //imshow("after dilation", output);
//    }
//
//}
//
//
//
//vector<vector<Point>> selectContourArea (const vector<vector<Point>>& contour){
//    //caluculate the area of each contour and push it into a priority queue
//    priority_queue<double, vector<contourCompareArea>, contourCompareArea> findbigger;
//    for (vector<vector<Point>>::const_iterator it = contour.begin(); it != contour.end(); it++){
//        double area = contourArea(*it);
//        contourCompareArea temp(*it, area);
//        findbigger.push(temp);
//    }
//    
//    //we only want the biggest few contours
//    vector<vector<Point>> selectedContours;
//    
//    while ((findbigger.top().area >= CONTOUR_AREA_LIMIT) && (!findbigger.empty())) {
//        selectedContours.push_back(findbigger.top().contourPoints);
//        findbigger.pop();
//    }
//    
//    return selectedContours;
//
//}
//
//
//
//
//
//vector<Point> try_convex_hull_wrapper (const Mat frameDiff){
//    
//    
//    //noise reduction + gap filling
//    Mat afterProcess;
//    frameDiffProcessing(frameDiff, afterProcess, 5, 0, 6);
////    imshow("nr + filling product", afterProcess);
//    
//    //use threshold to generate binary image
//    Mat afterThresh;
//    threshold( afterProcess, afterThresh, 0, 255,THRESH_BINARY );
//    imshow("threshold", afterThresh);
//    
//    
//    //find contour
//    vector<vector<Point> > contour;
//    vector<Vec4i> hierarchy;    //just a holder, not going to use it
//    findContours(afterThresh, contour, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
//    
//    
//    //check if there's any contours with a too small area
//    vector<vector<Point>> selectedContours = selectContourArea(contour);
//    
//    
//    //combine all points to find the overall convex hull
//    vector<Point> hullPoints;   //contains the collection of all selected points
//    for (vector<vector<Point>>::iterator it = selectedContours.begin(); it != selectedContours.end(); it++){
//        for (vector<Point>::iterator it2 = it->begin(); it2 != it->end(); it2++){
//            hullPoints.push_back(*it2);
//        }
//    }
//    
//    //draw the hull
//    vector<Point> hull;
//    convexHull(hullPoints, hull);
//    Mat hullDrawing = Mat::zeros( afterThresh.size(), CV_8UC3 );
//    Scalar color = Scalar( 255, 255, 255);
//    //drawContours only takes in a nested vector
//    vector<vector<Point>> hullVec;
//    hullVec.push_back(hull);
//    drawContours( hullDrawing, hullVec, 0, color, 1, 8, vector<Vec4i>(), 0, Point() );
//    
//    imshow( "Hull demo", hullDrawing );
//    
//    return hull;
//    
//    
////    vector<Point> hullPoints;   //contains the collection of all selected points
////    for (vector<vector<Point>>::iterator it = contour.begin(); it != contour.end(); it++){
////        for (vector<Point>::iterator it2 = it->begin(); it2 != it->end(); it2++){
////            hullPoints.push_back(*it2);
////        }
////    }
////    vector<Point> hull;
////    convexHull(hullPoints, hull);
////    Mat hullDrawing = Mat::zeros( afterThresh.size(), CV_8UC3 );
////    Scalar color = Scalar( rng.uniform(50, 255), rng.uniform(50,255), rng.uniform(50,255) );
////    //drawContours only takes in a nested vector
////    vector<vector<Point>> hullVec;
////    hullVec.push_back(hull);
////    drawContours( hullDrawing, hullVec, 0, color, 1, 8, vector<Vec4i>(), 0, Point() );
////    
////    imshow( "Hull demo", hullDrawing );
////    
////    waitKey();
//    
//    
//    
//    
//    /*
//     //find the biggest area
//     vector<vector<Point>>::iterator maxAreaIt;
//     double maxArea = 0;
//     for (vector<vector<Point>>::iterator it = contour.begin(); it != contour.end(); it++){
//     double area = contourArea(*it);
//     cout << area << endl;
//     if (area > maxArea){
//     maxAreaIt = it;
//     maxArea = area;
//     }
//     }
//     cout << "a frame done, max is " << maxArea << endl;
//     
//     //find the distance of each other contours to the one with biggest area
//     //and inject it into the selected contour
//     vector<vector<Point>> selectedContours;
//     selectedContours.push_back(*maxAreaIt);
//     for (vector<vector<Point>>::iterator it = contour.begin(); it != contour.end(); it++){
//     if (it != maxAreaIt){
//     double minDist = getMinDistance(*it, *maxAreaIt);
//     if (minDist < CONTOUR_MIN_DISTANCE_LIMIT){  //then this contour is selected
//     selectedContours.push_back(*it);
//     }
//     }
//     }
//     
//     //testing, draw selected hulls
//     vector<vector<Point>> selectHull(selectedContours.size());
//     for( int i = 0; i < selectedContours.size(); i++ )
//     {  convexHull( Mat(selectedContours[i]), selectHull[i], false ); }
//     Mat selectedDrawing = Mat::zeros( afterThresh.size(), CV_8UC3 );
//     for( int i = 0; i< selectedContours.size(); i++ )
//     {
//     Scalar color = Scalar( rng.uniform(50, 255), rng.uniform(50,255), rng.uniform(50,255) );
//     drawContours( selectedDrawing, selectedContours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
//     drawContours( selectedDrawing, selectHull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
//     }
//     imshow("selected", selectedDrawing);
//     
//     
//     
//     //draw out the overall convexHull
//     vector<Point> hullPoints;   //contains the collection of all selected points
//     for (vector<vector<Point>>::iterator it = selectedContours.begin(); it != selectedContours.end(); it++){
//     for (vector<Point>::iterator it2 = it->begin(); it2 != it->end(); it2++){
//     hullPoints.push_back(*it2);
//     }
//     }
//     vector<Point> hull;
//     convexHull(hullPoints, hull);
//     Mat hullDrawing = Mat::zeros( afterThresh.size(), CV_8UC3 );
//     Scalar color = Scalar( rng.uniform(50, 255), rng.uniform(50,255), rng.uniform(50,255) );
//     //drawContours only takes in a nested vector
//     vector<vector<Point>> hullVec;
//     hullVec.push_back(hull);
//     drawContours( hullDrawing, hullVec, 0, color, 1, 8, vector<Vec4i>(), 0, Point() );
//     
//     imshow( "Hull demo", hullDrawing );
//     
//     waitKey();
//     */
//    
//    
//    //draw the contour
//    //        Mat drawing = Mat::zeros( afterThresh.size(), CV_8UC3 );
//    //        for( int i = 0; i< contour.size(); i++ )
//    //        {
//    //            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//    //            drawContours( drawing, contour, i, color, 2, 8, hierarchy, 0, Point() );
//    //        }
//    //
//    //        imshow("contours", drawing);
//    //        waitKey();
//    
//    
//    /*
//     //now we will filter out some of the contours so that only the body will emerge
//     //caluculate the area of each contour and push it into a priority queue
//     priority_queue<double, vector<contourCompareArea>, contourCompareArea> findbigger;
//     for (vector<vector<Point>>::iterator it = contour.begin(); it != contour.end(); it++){
//     double area = contourArea(*it);
//     cout << area << endl;
//     contourCompareArea temp(*it, area);
//     findbigger.push(temp);
//     }
//     cout << endl;
//     
//     //we only want the biggest few contours
//     vector<vector<Point>> selectedContours;
//     //as long as we still have quota and the queue is not empty
//     for (int i = 0; (i < CONTOUR_NUMBER_LIMIT && (!findbigger.empty())); i++){
//     if(findbigger.top().area > CONTOUR_AREA_LIMIT){ //if there's little fregmentation, avoid noise
//     selectedContours.push_back(findbigger.top().contourPoints);   //push in the max area contour
//     }
//     findbigger.pop();
//     }
//     
//     //we should have the top few contours, now we check if we missed any contour with big area
//     while ((findbigger.top().area >= CONTOUR_AREA_LIMIT) && (!findbigger.empty())) {
//     selectedContours.push_back(findbigger.top().contourPoints);
//     findbigger.pop();
//     }
//     
//     
//     
//     //testing, draw selected hulls
//     vector<vector<Point>> selectHull(selectedContours.size());
//     for( int i = 0; i < selectedContours.size(); i++ )
//     {  convexHull( Mat(selectedContours[i]), selectHull[i], false ); }
//     Mat selectedDrawing = Mat::zeros( afterThresh.size(), CV_8UC3 );
//     for( int i = 0; i< selectedContours.size(); i++ )
//     {
//     Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//     drawContours( selectedDrawing, selectedContours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
//     drawContours( selectedDrawing, selectHull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
//     }
//     imshow("selected", selectedDrawing);
//     //        waitKey();
//     
//     
//     
//     
//     
//     //draw out the overall convexHull
//     vector<Point> hullPoints;   //contains the collection of all selected points
//     for (vector<vector<Point>>::iterator it = selectedContours.begin(); it != selectedContours.end(); it++){
//     for (vector<Point>::iterator it2 = it->begin(); it2 != it->end(); it2++){
//     hullPoints.push_back(*it2);
//     }
//     }
//     vector<Point> hull;
//     convexHull(hullPoints, hull);
//     Mat hullDrawing = Mat::zeros( afterThresh.size(), CV_8UC3 );
//     Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//     //drawContours only takes in a nested vector
//     vector<vector<Point>> hullVec;
//     hullVec.push_back(hull);
//     drawContours( hullDrawing, hullVec, 0, color, 1, 8, vector<Vec4i>(), 0, Point() );
//     
//     imshow( "Hull demo", hullDrawing );
//     
//     waitKey();
//     */
//}
//
//
////something adopted from internet
///*
//void erosionDilation (const Mat& input, Mat &output, int erosionParam, int dilationParam){
//    /// Load an image
//    src = input.clone();
//    
//    if( !src.data )
//    { return; }
//    
//    
//    /// Create windows
//    namedWindow( "Erosion Demo", CV_WINDOW_AUTOSIZE );
//    namedWindow( "Dilation Demo", CV_WINDOW_AUTOSIZE );
//    //    cvMoveWindow( "Dilation Demo", src.cols, 0 );
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
//    waitKey(0);
//    output = dilation_dst;
//}
//
//
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
//    dilate( erosion_dst, dilation_dst, element );
//    imshow( "Dilation Demo", dilation_dst );
//}*/
