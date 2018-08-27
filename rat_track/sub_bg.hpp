//
//  localization.hpp
//  rat_track
//
//  Created by Zun Wang on 2018-06-19.
//  Copyright © 2018 Zun Wang. All rights reserved.
//

#ifndef localization_hpp
#define localization_hpp

#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include "algorithm _evaluation.hpp"
#include "change_reference.hpp"


using namespace cv;
using namespace std;

const double cannyLowThreshold = 30.0;
const double cannyHighThreshold = 90.0;
const int BACKGROUND_THRESHOLD = 30;

//wrapper
void sub_bg_wrapper (const string& videoPath);

//to extract the background edge map, it's the average of all the sampled frames
//return true if successful and bg edge map stored in argument
//return false if unsuccessful
Mat getBackground (VideoCapture& capVid);

//to get the centre of the rat
//takes in the background edge map and the video, and prints out the location
//it gets the edge map of every frame, remove the bg edge from it, and take the average of the remaining
vector<pair<int, int>> findLocation(Mat& bgEdgeMap, VideoCapture& findRat);


#endif /* localization_hpp */
