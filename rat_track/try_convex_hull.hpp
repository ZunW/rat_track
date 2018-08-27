////
////  try_convex_hull.hpp
////  rat_track
////
////  Created by Zun Wang on 2018-07-31.
////  Copyright © 2018 Zun Wang. All rights reserved.
////
//
//#ifndef try_convex_hull_hpp
//#define try_convex_hull_hpp
//
//#include <stdio.h>
//#include "opencv2/opencv.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "change_reference.hpp"
//
//
//using namespace cv;
//using namespace std;
//
//const double CONTOUR_AREA_LIMIT = 4000;
////const int CONTOUR_NUMBER_LIMIT = 3;
////const double CONTOUR_MIN_DISTANCE_LIMIT = 20;
//
//class contourCompareArea{
//public:
//    vector<Point> contourPoints;
//    double area;
//    bool operator()(const contourCompareArea& c1, const contourCompareArea& c2);
//    contourCompareArea(vector<Point> conPoints, double conArea);
//    contourCompareArea();
//};
//
//
//
//void frameDiffProcessing (const Mat& input, Mat& output, int medianSize, int erosionSize, int dilationSize);
//double getMinDistance(vector<Point> contour1, vector<Point> contour2);
//vector<Point> try_convex_hull_wrapper (const Mat frameDiff);
//
//#endif /* try_convex_hull_hpp */
