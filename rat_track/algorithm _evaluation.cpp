//
//  algorithm _evaluation.cpp
//  rat_track
//
//  Created by Zun Wang on 2018-06-26.
//  Copyright © 2018 Zun Wang. All rights reserved.
//


#include "algorithm _evaluation.hpp"


//static global variables
static string windowNameHumanData = "Click on the mouse's head cap, press any key for next frame";
static bool isConfirmed;


//this callback function is only visible within this file, and it updates the mouse coordinate vector
//when the left button is clicked
static void mouseLocation(int event, int x, int y, int flags, void* mouseCoordVec);



//this simply get the human input data as a vector of Point2f
vector<Point2f> getHumanData(const string& videoPath){
    
    VideoCapture capVid(videoPath);
    
    //initialize parameters
    int frameCount = 0;
    isConfirmed = true;
    
    
    Mat frame;
    vector<Point2f> mouseCoordVec;
    

    
    namedWindow(windowNameHumanData);
    
    //we keep reading the frames until we hit the maximum times
    while (capVid.read(frame)){
        
        //we sample at a freq
        if ((frameCount%SAMPLE_FREQ) == 0){
            
            imshow(windowNameHumanData, frame);
            
            setMouseCallback(windowNameHumanData, mouseLocation, &mouseCoordVec);
            
            waitKey();
            
            isConfirmed = true;
        }
        
        frameCount ++;
        
    }
    
    
    destroyWindow(windowNameHumanData);
    capVid.release();
    
    return mouseCoordVec;
    
}


void mouseLocation(int event, int x, int y, int flags, void* mouseCoordVec){
    //we only care about left button
    if  ( event == EVENT_LBUTTONDOWN )
    {
        Point2f currentPt;
        currentPt.x = (float) x;
        currentPt.y = (float) y;
        
        
        //we need to cast void* to the actual type
        vector<Point2f>* vecPtr = (vector<Point2f>*) mouseCoordVec;
        
        if (isConfirmed){
            //we are adding new entry
            vecPtr->push_back(currentPt);
            isConfirmed = false;    //the following click will be to update on the same frame
        }
        else{
            //we are updating on the same frame
            vecPtr->pop_back();
            vecPtr->push_back(currentPt);
        }
        
    }
    
}


//below is the previous version
/*
#include "algorithm _evaluation.hpp"

//this callback function is only visible within this file, and it updates the mouse coordinate vector
//when the left button is clicked
static void mouseLocation(int event, int x, int y, int flags, void* mouseCoordVec);

static int sampleCount;
static int frameCount;

//callback fxn will create a vector of this struct
struct mouseCoordPkg{
    int frameNum;
    int xcoord, ycoord;
};


int findAccuracy(VideoCapture& vidAccuracy, vector<pair<int, int>> centre_coords){
    
    //make a output file
    ofstream humanData;
    humanData.open("/Users/Achilles/Desktop/2018 summer research/project files/opencv trial/rat_track/human_centre_points.txt");
    
    
    //initialize parameters
    int accuracy= 0;
    frameCount = 0;
    sampleCount = 0;
    
    vector<mouseCoordPkg> mouseCoordVec;
    
    Mat frame;
    
    namedWindow("current frame");
    
    //we keep reading the frames until we hit the maximum times
    while (vidAccuracy.read(frame)){
        
        //we sample at a freq
        if ((frameCount%SAMPLE_FREQ) == 0){
            
            imshow("current frame", frame);
            
            setMouseCallback("current frame", mouseLocation, &mouseCoordVec);
        
            waitKey();
        }
        
        frameCount ++;
        
    }
    
    
    destroyWindow("current frame");
    
    //at this point, we should have finished collecting human data
    //we calculate the accuracy
    int correctNum = 0;
    
    //iterate over the human data
    for (vector<mouseCoordPkg>::iterator it = mouseCoordVec.begin(); it != mouseCoordVec.end(); it++){
        
        int humanX = (*it).xcoord;
        int humanY = (*it).ycoord;
        int frameNum = (*it).frameNum;
        
        //write to output file
        humanData << frameNum << "  " << humanX << "    " << humanY << endl;
        
        
        //calculate a binary accuracy
        int algX = centre_coords[frameNum].first;
        int algY = centre_coords[frameNum].second;
        
        int diffX = humanX - algX;
        int diffY = humanY - algY;
        
        //for now I will use int division
        int error = sqrt(diffX*diffX + diffY*diffY);
        
        cout << "frameNum=" << frameNum << ", humanX=" << humanX << ", humanY=" << humanY << ", algX=";
        cout << algX << ", algY=" << algY << ", diffX=" << diffX << ", diffY=" << diffY << ", error=";
        cout << error <<endl;
        
        //we count how many are within acceptable error range
        if (error <= ACCURACY_BOUND){
            correctNum ++;
        }
        
    }
    
    
    accuracy = 100 * correctNum / sampleCount;
    
    cout << "correctNum = " << correctNum << ", accuracy = " << accuracy << endl;
    
    return accuracy;
    
}



void mouseLocation(int event, int x, int y, int flags, void* mouseCoordVec){
    //we only care about left button
    if  ( event == EVENT_LBUTTONDOWN )
    {
        cout << "sampleCount = " << sampleCount;
        
        
        //prepare the input package to the vector
        mouseCoordPkg currentPkg;
        currentPkg.frameNum = frameCount + 1;   //we want to start from 1 instead of 0
        currentPkg.xcoord = x;
        currentPkg.ycoord = y;
        
        //we need to cast void* to the actual type
        vector<mouseCoordPkg>* vecPtr = (vector<mouseCoordPkg>*) mouseCoordVec;
        
        
        //so we only want to either update the human data on the same frame
        //or we add a new entry for a new frame
        if (sampleCount > 0){
            int previousEntryframeNum = vecPtr->back().frameNum;
            
            //if the frame stays the same
            if (previousEntryframeNum == (frameCount + 1)){
                //we pop the end struct and add this new struct
                vecPtr->pop_back();
                vecPtr->push_back(currentPkg);
            }
            else{//else it's a new frame, we add new stuff
                vecPtr->push_back(currentPkg);
                sampleCount ++;
            }
        }
        else {  //it's the first entry
            vecPtr->push_back(currentPkg);
            sampleCount ++;
        }
        
        //update sample count
        
        cout << ", after fxn = " << sampleCount << endl << endl;
        
    }
    
}*/