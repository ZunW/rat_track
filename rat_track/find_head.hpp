//
//  find_head.hpp
//  rat_track
//
//  Created by Zun Wang on 2018-08-16.
//  Copyright © 2018 Zun Wang. All rights reserved.
//

#ifndef find_head_hpp
#define find_head_hpp

#include <stdio.h>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

//use black and white to represent false and true
const uchar EXAMINED_SET_TRUE = 255;    //means it 
const uchar EXAMINED_SET_FALSE = 0;

struct featureInfo{
    Point coord;
    Vec3b colors;
    int area;
};

//to take in the user input about the ears and the head cap's position, color, size
vector<featureInfo>& markEarCap(const string& videoPath);


vector<Point2f> getHeadPosition(const string& videoPath);

#endif /* find_head_hpp */
