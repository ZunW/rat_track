//
//  change_reference.hpp
//  rat_track
//
//  Created by Zun Wang on 2018-06-29.
//  Copyright © 2018 Zun Wang. All rights reserved.
//

#ifndef change_reference_hpp
#define change_reference_hpp

#include <stdio.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include <string>
#include <utility>
#include "opencv2/opencv.hpp" 
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;





//used to convert a vector of int pair to a vector of Point2f. The output vector is supposed to be empty
void vectorPairIntToPoint2f (const vector<pair<int, int>>& intVec, vector<Point2f>& point2fVec);


//used to convert a vector of Point2f to a vector of int pair. The output vector is supposed to be empty
void vectorPoint2fToPairInt (const vector<Point2f>& point2fVec, vector<pair<int,int>>& intVec);


//get the new frame size
pair<int, int> getBoundarySize(vector<Point2f> cageCorners);


//check if the converted points exceed the boundaries, if yes, suppress them to boundary
void checkBoundary (Point2f& pt, pair<int, int> boundarySize);


//it's much like a wrapper. It takes in a picture from caller and displays it.
//the actual data collecting and UI was done in the mouse callback
//it counts 4 key presses and returns the collected 4 corner coordinates.
vector<Point2f>& markCorners(const Mat& picture);


//used to transform a vector of points to another in a different frame of reference
//it takes in the original 4 points reference, the desired 4 points of reference
//and the vector of points to be transformed
vector<Point2f> getCoordNewFrame (const vector<Point2f> inputCoords, const vector<Point2f>& origBase, const vector<Point2f>& desiredFrame);




void videoTransNewFrame (const string& inputPath, const string& outputPath, int newWidth, int newHeight);
vector<Point2f> getDesiredFrame (const pair<int, int>& boundarySize);





#endif /* change_reference_hpp */
