//
//  change_reference.cpp
//  rat_track
//
//  Created by Zun Wang on 2018-06-29.
//  Copyright © 2018 Zun Wang. All rights reserved.
//

#include "change_reference.hpp"



////global variables
static string windowNameMarkCorners = "Mark corners, press any key to confirm the current corner";
static Mat previousFrame, currentFrame;
static int keyCount;
static bool isConfirmed;

//this is to be returned
vector<Point2f> imageCorners;


//this fxn updates the selection of the 4 corners, and draws out the live feedback
static void mouseCallbackFeedback(int event, int x, int y, int flags, void*);




//it's much like a wrapper. It takes in a picture from caller and displays it.
//the actual data collecting and UI was done in the mouse callback
//it counts 4 key presses and returns the collected 4 corner coordinates.
vector<Point2f>& markCorners(const Mat& picture){

    //initialize
    namedWindow(windowNameMarkCorners, CV_WINDOW_AUTOSIZE);
    imshow(windowNameMarkCorners, picture);
    keyCount = 0;
    isConfirmed = true;
    
    previousFrame = picture.clone();    //Mat is a container, with pointers inside, so we
    currentFrame = picture.clone();     //need to have deep copy

    while (keyCount < 4){
        
        imshow(windowNameMarkCorners, previousFrame);
        
        setMouseCallback(windowNameMarkCorners, mouseCallbackFeedback);
        
        
        //to halt the program and wait for confirmation
        waitKey();
        keyCount ++;
        isConfirmed = true;
        
        previousFrame = currentFrame.clone();   //selection confirmed, update previous frame


    }
    
    destroyWindow(windowNameMarkCorners);
    
    return imageCorners;
    
}

/*
//this fxn updates the selection of the 4 corners, and draws out the live feedback
void mouseCallbackFeedback(int event, int x, int y, int flags, void*){
    
    if (event == EVENT_LBUTTONDOWN){
        
        //make a holder pair
        pair<int, int> xycoord;
        xycoord.first = x;
        xycoord.second = y;
        
        
        
        //set the return value
        //kinda hard-coded here, because it's only 4 choices. Can also use a int pointer
        
        //upper left corner
        if (keyCount == 0){
            baseCorners.upperL = xycoord;
        }
        //upper right
        else if (keyCount == 1){
            baseCorners.upperR = xycoord;
        }
        //bottom left
        else if (keyCount == 2){
            baseCorners.bottomL = xycoord;
        }
        //bottom right
        else if (keyCount == 3){
            baseCorners.bottomR = xycoord;
        }
        
        
        
        //draw the live feedback
        currentFrame = previousFrame.clone();   //in case if there's new selection for the same point, reset the frame
        for (int r = y-10; r < y+10; r++){
            for (int c = x-10; c < x+10; c++){
                currentFrame.at<Vec3b>(r, c)[0] = 0;
                currentFrame.at<Vec3b>(r, c)[1] = 0;
                currentFrame.at<Vec3b>(r, c)[2] = 255;
            }
        }
        
        imshow(windowName, currentFrame);
        
    }
    
}*/


void mouseCallbackFeedback(int event, int x, int y, int flags, void*){
    
    if (event == EVENT_LBUTTONDOWN){
        
        //holder
        Point2f xycoord;
        xycoord.x = (float) x;
        xycoord.y = (float) y;
        
        
        if (isConfirmed){
            //we are updating the new corner
            imageCorners.push_back(xycoord);
            isConfirmed = false;    //the following click would be correcting the choice
        }
        else{//we are updating the same corner
            imageCorners.pop_back();
            imageCorners.push_back(xycoord);
        }
        
        //draw the live feedback
        currentFrame = previousFrame.clone();   //if re-selecting, reset the frame
        for (int r = y-10; r < y+10; r++){
            for (int c = x-10; c < x+10; c++){
                currentFrame.at<Vec3b>(r, c)[0] = 0;
                currentFrame.at<Vec3b>(r, c)[1] = 0;
                currentFrame.at<Vec3b>(r, c)[2] = 255;
            }
        }
        
        imshow(windowNameMarkCorners, currentFrame);
        
    }
    
}


//used to transform a vector of points to another in a different frame of reference
//it takes in the original 4 points reference, the desired 4 points of reference
//and the vector of points to be transformed
vector<Point2f> getCoordNewFrame (const vector<Point2f> inputCoords, const vector<Point2f>& origBase, const vector<Point2f>& desiredFrame){
    
    vector<Point2f> outputCoords;
    
    Mat transMat = getPerspectiveTransform(origBase, desiredFrame);
    
    perspectiveTransform(inputCoords, outputCoords, transMat);
    
    return outputCoords;
    
}


//used to convert a vector of int pair to a vector of Point2f. The output vector is supposed to be empty
void vectorPairIntToPoint2f (const vector<pair<int, int>>& intVec, vector<Point2f>& point2fVec){
    //I have to use const_iterator on const input
    for (vector<pair<int, int>>::const_iterator it = intVec.begin(); it != intVec.end(); it++){
        Point2f currentCoord;
        currentCoord.x = it->first;
        currentCoord.y = it->second;
        point2fVec.push_back(currentCoord);
    }
    
}


//used to convert a vector of Point2f to a vector of int pair. The output vector is supposed to be empty
void vectorPoint2fToPairInt (const vector<Point2f>& point2fVec, vector<pair<int,int>>& intVec){
    //I have to use const_iterator on const input
    for (vector<Point2f>::const_iterator it = point2fVec.begin(); it != point2fVec.end(); it++){
        pair<int, int> currentCoord;
        currentCoord.first = (int) it->x;
        currentCoord.second = (int) it->y;
        intVec.push_back(currentCoord);
    }
}



//get the new frame size
pair<int, int> getBoundarySize(vector<Point2f> cageCorners){
    pair<int, int> boundarySize;
    
    vector<Point2f>::iterator it=cageCorners.begin();
    
    Point2f ul = *it;
    it++;
    Point2f ur = *it;
    it++;
    Point2f bl = *it;
    
    //x boundary, the width
    boundarySize.first = (int) norm(ur - ul);
    //y boundary, the height
    boundarySize.second = (int) norm(bl - ul);

    

    return boundarySize;
}


//check if the converted points exceed the boundaries, if yes, suppress them to boundary
void checkBoundary (Point2f& pt, pair<int, int> boundarySize){
    
    //check x
    if (pt.x < 0){
        cout << "pt.x < 0 at " << pt.x << " and " << pt.y << endl;
        pt.x = 0;
    }
    else if (pt.x > boundarySize.first){
        cout << "pt.x > boundaryX at " << pt.x << " and " << pt.y << endl;
        pt.x = boundarySize.first;
    }
    
    //check y
    if (pt.y < 0){
        cout << "pt.y < 0 at " << pt.x << " and " << pt.y << endl;
        pt.y = 0;
    }
    else if (pt.y > boundarySize.second){
        cout << "pt.y > boundaryY at " << pt.x << " and " << pt.y << endl;
        pt.y = boundarySize.second;
    }
    
}



vector<Point2f> getDesiredFrame (const pair<int, int>& boundarySize){
    
    vector<Point2f> desiredFrame;
    Point2f ul, ur, bl, br;
    ul.x = 0, ul.y = 0;
    ur.x = boundarySize.first, ur.y = 0;
    bl.x = 0, bl.y = boundarySize.second;
    br.x = boundarySize.first, br.y = boundarySize.second;
    
    desiredFrame.push_back(ul);
    desiredFrame.push_back(ur);
    desiredFrame.push_back(bl);
    desiredFrame.push_back(br);
    
    return desiredFrame;
    
}




void videoTransNewFrame (const string& inputPath, const string& outputPath, int marginWidth, int marginHeight){
    
    VideoCapture inputVid(inputPath);
    
    if ( !inputVid.isOpened()){
        cout << "Cannot open input file at \"" << inputPath << "\"!";
        return;
    }
    
    //I basically copied this line from the internet. it's said that static_int is safer
    int vidCode = static_cast<int>(inputVid.get(CV_CAP_PROP_FOURCC));
    
    //get the frame of reference
    Mat frame;
    inputVid.read(frame);
    
    vector<Point2f> cageCorners = markCorners(frame);
    
    pair<int, int> boundarySize = getBoundarySize(cageCorners);
    
    vector<Point2f> desiredFrame = getDesiredFrame(boundarySize);
    

    //add a bit of offset
    for (vector<Point2f>::iterator it = desiredFrame.begin(); it != desiredFrame.end(); it++){
        it->x += marginWidth;
        it->y += marginHeight;
    }
    
    Mat transMat = getPerspectiveTransform(cageCorners, desiredFrame);
    
    Mat afterWarp;
    
    Size frameSize(boundarySize.first + 2*marginWidth, boundarySize.second + 2*marginHeight);
    
    VideoWriter outputVid;
    
    outputVid.open(outputPath, vidCode, inputVid.get(CV_CAP_PROP_FPS), frameSize);
    
    if (!outputVid.isOpened()){
        cout << "Cannot open output file at \"" << outputPath << "\"!" <<endl;
        return;
    }
    
//    inputVid.release();
//    inputVid.open(inputPath);
    
    //produce the transformed and cropped video
    while (inputVid.read(frame)){
        
        warpPerspective(frame, afterWarp, transMat, frameSize);
        
        //now afterWarp has the image we want to store
        outputVid << afterWarp;
        
//        imshow("after warp", afterWarp);
//        waitKey();
        
    }
    
    outputVid.release();

}


/*void getCoordNewFrame(void){

    // Read source image.
//    Mat im_src = imread("/Users/Achilles/Desktop/2018 summer research/project files/video/snapshot9/1.png");
    // Four corners of the book in source image
    vector<Point2f> pts_src;
    pts_src.push_back(Point2f(246, 7));
    pts_src.push_back(Point2f(767, 35));
    pts_src.push_back(Point2f(214, 680));
    pts_src.push_back(Point2f(750, 679));
    
    
    // Read destination image.
//    Mat im_dst = imread("book1.jpg");
    // Four corners of the book in destination image.
    vector<Point2f> pts_dst;
    pts_dst.push_back(Point2f(0, 0));
    pts_dst.push_back(Point2f(500, 0));
    pts_dst.push_back(Point2f(0, 700));
    pts_dst.push_back(Point2f(500, 700));
    
    // Calculate Homography
    Mat h = getPerspectiveTransform(pts_src, pts_dst);
    
    // Output image
//    Mat im_out;
    // Warp source image to destination based on homography
//    warpPerspective(im_src, im_out, h, im_src.size());
    
    
    // Display images
//    imshow("Source Image", im_src);
//    imshow("Destination Image", im_dst);
//    imshow("Warped Source Image", im_out);
    
    vector<Point2f> src, dst;
    src.push_back(Point2f(577, 430));
//    dst.push_back(Point2f(100, 100));
    
    perspectiveTransform(src, dst, h);
    
    cout << dst << endl;
    
    cout << "x = " << dst.back().x << " y = " << dst.back().y <<endl;

    
//    waitKey(0);

}*/

