//
//  algorithm _evaluation.hpp
//  rat_track
//
//  Created by Zun Wang on 2018-06-26.
//  Copyright © 2018 Zun Wang. All rights reserved.
//

#ifndef algorithm__evaluation_hpp
#define algorithm__evaluation_hpp


/*
#include <stdio.h>
#include <utility>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>

const int SAMPLE_FREQ = 40;
const int ACCURACY_BOUND = 60;

using namespace std;
using namespace cv;

int findAccuracy(VideoCapture& vidAccuracy, vector<pair<int, int>> centre_coords);
 */

#include <stdio.h>
#include <utility>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include "change_reference.hpp"

const int SAMPLE_FREQ = 50;
const int ACCURACY_BOUND = 60;

using namespace std;
using namespace cv;


//this simply get the human input data as a vector of Point2f
vector<Point2f> getHumanData(const string& videoPath);



#endif /* algorithm__evaluation_hpp */
