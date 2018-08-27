//
//  sub_bg_image.hpp
//  rat_track
//
//  Created by Zun Wang on 2018-07-16.
//  Copyright © 2018 Zun Wang. All rights reserved.
//

#ifndef sub_bg_image_hpp
#define sub_bg_image_hpp

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
#include <math.h>
#include "try_convex_hull.hpp"
#include <sstream>


const int GET_BG_FREQ = 20;
const int MEDIANBLUR_SIZE = 5;
const int THRESHOLD_IMG_DIFF_LOW = 35;
const int TOO_BRIGHT = 150;
const int DEMO_TIME = 10;   //in seconds



using namespace cv;
using namespace std;



//used to find the background by averaging out the frames taking at a certain frequency
Mat getRGBbg(const string& videoPath);

//the actual fxn of the algorithm, basically it subtracts the bg from each frame to find the
//raw rat image, then reduce the noise we have a better rat image. Calculate the average value
//of all the remaining pixels' coordinates we have the centroid
vector<Point2f> subBGimage (const string& videoPath, const Mat& bgAverage);

//it's a wrapper. it calls the respective fxns to obtain a vector, then store them into files
void sub_bg_img_wrapper (const string& videoPath);


#endif /* sub_bg_image_hpp */
