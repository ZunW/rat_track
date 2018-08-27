//
//  find_head.cpp
//  rat_track
//
//  Created by Zun Wang on 2018-08-16.
//  Copyright © 2018 Zun Wang. All rights reserved.
//

#include "find_head.hpp"


//I will need a fxn to take in initial data from user by clicking on the image: the color, the starting position of the ear and head cap, the shapes & areas of the above.
//another fxn to process every frame, the major fxn
//a fxn to process and reduce the noise

static int keyCount;
static bool isConfirmed;
static Mat previousFrame, currentFrame;
static string windowName = "mark the ears by first two clicks and then head cap";
static vector<featureInfo> featureVec;



//the mouse callback function
void markEarCapMouse(int event, int x, int y, int flags, void*){
    if (event == EVENT_LBUTTONDOWN){
        
        //holder
        
        featureInfo currentFeature;
        currentFeature.coord.x = x;
        currentFeature.coord.y = y;
        currentFeature.area = 300;
        currentFeature.colors = currentFrame.at<Vec3b>(y, x);
        
        
        if (isConfirmed){
            //we are updating
            featureVec.push_back(currentFeature);
            isConfirmed = false;    //the following click would be correcting the choice
        }
        else{//we are updating the same info
            featureVec.pop_back();
            featureVec.push_back(currentFeature);
        }
        
        //draw the live feedback
        currentFrame = previousFrame.clone();   //if re-selecting, reset the frame
        circle(currentFrame, currentFeature.coord, 10, Scalar(255, 255, 155), -1);
        
        imshow(windowName, currentFrame);
    }
}

//to take in the user input about the ears and the head cap's position, color, size
vector<featureInfo>& markEarCap(const string& videoPath){
    
    VideoCapture capVid(videoPath);
    
    Mat frame;
    capVid.read(frame);
    capVid.release();
    
    imshow(windowName, frame);
    
    keyCount = 0;
    isConfirmed = true;
    currentFrame = frame.clone();
    previousFrame = frame.clone();
    
    setMouseCallback(windowName, markEarCapMouse);
    
    while (keyCount < 3){
        waitKey();
        keyCount ++;
        isConfirmed = true;
        previousFrame = currentFrame.clone();   //selection confirmed, update previous frame

    }
    
    destroyWindow(windowName);

    return featureVec;
    
}




vector<Point2f> getHeadPosition(const string& videoPath){
    VideoCapture capVid(videoPath);
    
    vector<Point2f> headPositions;
    
    Mat frame;
    while (capVid.read(frame)) {
        //for now will search for entire frame, later will search within only the rat image
        
//        Mat grayF;
//        cvtColor(frame, grayF, CV_BGRA2GRAY);
//        imshow("grayF", grayF);
//
//        SimpleBlobDetector::Params params;
//
////        Vec3b color = colors[0];
//
//        // Change thresholds
//        params.minThreshold = 0;
//        params.maxThreshold = 255;
//        params.thresholdStep = 10;
//
//        // Filter by Area.
//        params.filterByArea = true;
//        params.minArea = 50;
//        params.maxArea = 500;
//
//        // Filter by Circularity
//        params.filterByCircularity = true;
//        params.minCircularity = 0.5;
//
//        // Filter by Convexity
//        params.filterByConvexity = true;
//        params.minConvexity = 0.5;
//
//        // Filter by Inertia
//        params.filterByInertia = true;
//        params.minInertiaRatio = 0.5;
//
//        Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
//
//        // Storage for blobs
//        std::vector<KeyPoint> keypoints;
//
//        // Detect blobs
//        detector->detect(frame, keypoints);
//
//        Mat im_with_keypoints;
//        drawKeypoints( frame, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
//        imshow("keypoints", im_with_keypoints );
        
//        imshow("frame", frame);
        waitKey();
//
    }
    
    return headPositions;
}


struct detectorParameter{
    Scalar blobColor;
    double colorAccuracy;   //max is 1
    Point prevBlobCentroid; //will search from this point
    double rangeLimit;  //won't search pixels beyond this range limit from the previous blob centroid
    double areaApprox;
    double minIntertia; //we will look for round blob, so set a min inertia. max is 1
    double circularity; //max is 1
    double convexity;   //max is 1
};


//will look for only one blob that is close in color and close to the previous blob,
//within the range,
void customBlobDetector(detectorParameter inputParam, Mat inputImg, vector<Point> blobContour){
    
//    double confidence;
    double colorAccuracy = inputParam.colorAccuracy;
    Scalar blobColor = inputParam.blobColor;
    Point prevBlobCentroid = inputParam.prevBlobCentroid;
    double rangeLimit = inputParam.rangeLimit;
    double areaApprox = inputParam.areaApprox;
    double minIntertia = inputParam.minIntertia;
    double circularity = inputParam.circularity;
    double convexity = inputParam.convexity;
    
    
    //will search within the range limit, centered at the previous centroid
    //the idea is very similar to Dijkstra's algorithm
    
    //three sets, each holds different set of coordinates of pixels
    vector<Point> discovered_set;
    vector<Point> examining_set;
    //decide to use Mat of uchar (a byte), white 255 = true, black 0 = false
    //this way we can visualize the process. Initially we have all true, non has been examined
    int imgRow = inputImg.rows, imgCol = inputImg.cols;
    Mat examined_set(imgRow, imgCol, CV_8UC1, EXAMINED_SET_TRUE);
    
    
    //initialize the sets
    examining_set.push_back(prevBlobCentroid);
    
    
    //as long as there are pixels to be examined
    while(!examining_set.empty()){
        
        //go over every element in the examining_set, and put every legal neighbor
        //of that pixel into the discovered_set
        for (vector<Point>::iterator it = examining_set.begin(); it != examining_set.end(); it++){
            int pixRow = it->y, pixCol = it->x;

            //before we do anything, we want to make sure this pixel is a fresh one
            //see discussion below
            if (examined_set.at<uchar>(pixRow, pixCol)){
                
                //first turn them off in the boolean table: if we don't, there'll be duplicates
                //e.g. if we don't turn off at first, and there's two pixels A, B stacking up.
                //we will add B to A's neighbors and A to B's neighbors, generating duplicates.
                examined_set.at<uchar>(pixRow, pixCol) = EXAMINED_SET_FALSE;
                //it also raises a concern in discovering the new pixels (neighbors of examining pixels)
                //that similar situation can happen in that process. The same pixel can
                //be inserted into the discovered_set multiple times if duplicates aren't checked
                //there's two option: 1. check it when we are inserting new elements into the
                //discovered_set, and the fastest way to do so is using hash table (unordered_map).
                //2. we insert all the neighbors disregarding duplicates, and when the next for loop
                //starts (when we change the discovered_set to examining_set), we check for duplicates
                //by checking if the pixel has already been examined: if it's a unique pixel, then
                //nothing happened, but if it's a duplicate, it would have been examined so will be
                //skipoed. Method 1 would take in total c*n time using .find function of unordered_map
                //which is constant time, and there's N elements. Method 2 is also c*n, but I prefer it
                //since it's much simpler structure (just vectors)
                
                
                //to build the discovered list
                //for every neighbor of the current pixel, add it to the discovered_set
                //this would contain: 1. the new fresh pixels, what we truely want
                //2. duplicates of the new fresh pixels, because the reasons above
                //3. the pixels that have been examined from previous iterations
                //and the discovered_set will become the examining_set in the
                //next iteration, and we perform a eligibility check (if in examined_set)
                //2 and 3 would be filtered out because:
                //2: when dupliates are been considered, program will found it's already
                //evaluated. 3: it's just already evaluated
                //below discovers the box area around the current pixel.
                for (int row = pixRow-1; row <=pixRow+1; row ++){
                    for (int col = pixCol-1; col <= pixCol+1; col++){
                        //I will just use a box instead of a circlue to set the search range
                        //to save time
                        if ((abs(col - prevBlobCentroid.x) <= rangeLimit) && (abs(row - prevBlobCentroid.y) <= rangeLimit)){
                            //check if beyond image boundary
                            if ((row >= 0) && (row <= imgRow) && (col >= 0) && (col <= imgCol)){
                                discovered_set.push_back(Point(col, row));
                            }
                        }
                    }
                }
                
                
                
                if ()
                
                

                    //vector.swap
                
                
                
                
            }
        }
        
        
    }
    
    
    
    
    
    
//    return confidence;
}



//fxn to evaluate if two colors are close to each other. It takes in two RGB color
//and an accuracy percentage, and returns true or false
//it calculates the weight of each channel and based on this calulcates a score for each
//we compare if they are close enough within the accuracy range
//I've checked online, ppl mostly recommend using HSV color space and compare distance in
//the hue wheel. But I don't find the source code of cvtColor fxn and I don't know how
//expensive it is in terms of computation. So I decide to implement my own RGB based algorithm
//the idea is that we will focus more on the more significant channel (hihger value)
bool pixColorMatch(const Scalar& refColor, const Scalar& pixColor, double colorAccuracy){
    int refB = refColor[0], refG = refColor[1], refR = refColor[2];
    int pixB = pixColor[0], pixG = pixColor[1], pixR = pixColor[2];
    int refSum = refB + refG + refR;
    double percentR = refR / refSum, percentG = refG / refSum, percentB = refB / refSum;
    
    
    
    
    
}
