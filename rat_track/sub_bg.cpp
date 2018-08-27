//
//  localization.cpp
//  rat_track
//
//  Created by Zun Wang on 2018-06-19.
//  Copyright © 2018 Zun Wang. All rights reserved.
//

#include "sub_bg.hpp"


void sub_bg_wrapper (const string& videoPath){
    
    VideoCapture capVid(videoPath);
    
    if (!capVid.isOpened()){
        cout << "Video can't be opened!" <<endl;
    }
    else{   //if the video can be opened, we will do the actual work
        /*
        //convert frame of reference
        Mat cornerMat = imread("/Users/Achilles/Desktop/2018 summer research/project files/video/snapshot9/1.png");
        vector<Point2f> imageCorners = markCorners(cornerMat);
        pair<int, int> boundarySize = getBoundarySize(imageCorners);
        
        cout << boundarySize.first << "  " << boundarySize.second << endl;
        
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
        */
        
        

        //get the background edge map
        Mat bgEdgeMap = getBackground(capVid);
        imshow("background", bgEdgeMap);
        
        //close the object for the next reading
        capVid.release();   

        VideoCapture findRat(videoPath);
        
        //get a vector of the centre points and plot them on screen
        vector<pair<int, int>> centreCoords = findLocation(bgEdgeMap, findRat);
        findRat.release();
        
        
        
        
        
        
        /*
        //convert int to Point2f for centre_coords
        vector<Point2f> centreCoordsPoint2f;
        vectorPairIntToPoint2f(centreCoords, centreCoordsPoint2f);
        
        

        //get the transformed algorithm generated coord vector
        vector<Point2f> algCoordNewFrameVec = getCoordNewFrame(centreCoordsPoint2f, imageCorners, desiredFrame);
        
        //check for boundary and change back to int
        vector<pair<int, int>> algCoordNewFrameVecPairInt;
        for (vector<Point2f>::iterator it = algCoordNewFrameVec.begin(); it != algCoordNewFrameVec.end(); it++){
            
            Point2f pt = *it;
            checkBoundary(pt, boundarySize);
            
            pair<int, int> currentCoord;
            currentCoord.first = (int) pt.x;
            currentCoord.second = (int) pt.y;
            algCoordNewFrameVecPairInt.push_back(currentCoord);
        }

        
        
        //output to file
        ofstream algoCoordNewFrameFile;
        algoCoordNewFrameFile.open("/Users/Achilles/Desktop/2018 summer research/project files/opencv trial/rat_track/alg_coord_new_frame.txt");
        
        if (!algoCoordNewFrameFile.is_open()){
            cout << "Algorithm Data: File can't be opened!" << endl;
        }
        else {
            
            for (vector<pair<int, int>>::iterator it = algCoordNewFrameVecPairInt.begin(); it != algCoordNewFrameVecPairInt.end(); it ++){
                algoCoordNewFrameFile << it->first << " " << it->second << endl;
            }
            
            algoCoordNewFrameFile.close();
            
        }
        
        
        
        //get the human input for the cap position in desired frame of reference
        VideoCapture vidHumanData(videoPath);
        vector<Point2f> humanData = getHumanData(vidHumanData);
        vidHumanData.release();

        
        
        vector<Point2f> humanDataNewFrameVec = getCoordNewFrame(humanData, imageCorners, desiredFrame);
        
        //check boundaries and change back to int pair
        vector<pair<int, int>> humanDataNewFrameVecPairInt;
        for (vector<Point2f>::iterator it = humanDataNewFrameVec.begin(); it != humanDataNewFrameVec.end(); it++){
            
            Point2f pt = *it;
            checkBoundary(pt, boundarySize);
            
            pair<int, int> currentCoord;
            currentCoord.first = (int) pt.x;
            currentCoord.second = (int) pt.y;
            humanDataNewFrameVecPairInt.push_back(currentCoord);
        }
        
        
        //write to file
        std::ofstream humanDataNewFrameVecFile;
        humanDataNewFrameVecFile.open("/Users/Achilles/Desktop/2018 summer research/project files/opencv trial/rat_track/human_coord_new_frame.txt");
        
        if (!humanDataNewFrameVecFile.is_open()){
            cout << "Human Data: File can't be opened!" << endl;
        }
        else{
            
            for (vector<pair<int, int>>::iterator it = humanDataNewFrameVecPairInt.begin(); it != humanDataNewFrameVecPairInt.end(); it ++){
                humanDataNewFrameVecFile << it->first << " " << it->second << endl;
            }
            
            humanDataNewFrameVecFile.close();
        }*/
    }
    
}


Mat getBackground (VideoCapture& capVid){
    
    //the loop will take in frame one by one. we will take a frame every 2 seconds for now.
    Mat frame;
    capVid.read(frame);
    
    Mat bgAverage(frame.rows, frame.cols, CV_64FC1, 0.0);
        
    int count = 0, numFrame = 0;
    while (capVid.read(frame)) {
        
        if (! (numFrame%20) ){ //if the count can be divided by 20, we will perform the canny edge.
            
            cvtColor(frame, frame, CV_BGR2GRAY);    //convert to grayscale for easy processing
            
            GaussianBlur(frame, frame, Size(5, 5), 50);
//            Mat frameCopy = frame.clone();
//            bilateralFilter(frameCopy, frame, 8, 2000, 2000);
//            imshow("blf bg", frame);
//            waitKey();
//            cvtColor(frame, frame, CV_BGR2GRAY);
            
            Canny(frame, frame, cannyLowThreshold, cannyHighThreshold);
            
            frame.convertTo(frame, CV_64FC1);
            
            bgAverage = bgAverage + frame;
            
            count ++;
        }
        numFrame ++;
    }   //we should have a summed-up edge map in bgAverage
    bgAverage = bgAverage / count;  //take the average
    
    bgAverage.convertTo(bgAverage, CV_8UC3);
    
    return bgAverage;

}




vector<pair<int, int>> findLocation(Mat& bgEdgeMap, VideoCapture& findRat){

    //now read frame by frame and process each frame to get the center point of the rat
    Mat grayFrame, frame;
    vector<pair<int, int>> centre_coords;
    
    //the output file
    ofstream outFile;
    outFile.open("/Users/Achilles/Desktop/2018 summer research/project files/opencv trial/rat_track/alg_centre_points.txt");
    
    
    while (findRat.read(frame)) {
        
        cvtColor(frame, grayFrame, CV_BGR2GRAY);    //convert to grayscale for easy processing
        
        GaussianBlur(grayFrame, grayFrame, Size(5, 5), 50);
//        Mat frameCopy = frame.clone();
//        bilateralFilter(frameCopy, frame, 8, 2000, 2000);
//        cvtColor(frame, grayFrame, CV_BGR2GRAY);
        
        
        Canny(grayFrame, grayFrame, cannyLowThreshold, cannyHighThreshold);
        
        imshow("frame edge map", grayFrame);
        
        //extract the rat edge and calculate the centre
        int xavg = 0, yavg = 0, coordCount = 0; //need to reset for every loop
        Mat ratContour(grayFrame.rows, grayFrame.cols, CV_8UC1, Scalar(0)); //the contour image, need to reset
        
        for (int r = 0; r < frame.rows; r++){
            for (int c = 200; c < (frame.cols - 300); c++){ //limiting the area reduces the noise from sides
                if (bgEdgeMap.at<uchar>(r, c) < BACKGROUND_THRESHOLD ){  //if the pixel on bg is low, probably it's not background
                    if (grayFrame.at<uchar>(r, c) != 0){    //is there any vale at this coordinate
                        xavg += c;
                        yavg += r;
                        coordCount ++;
                        ratContour.at<uchar>(r, c) = 255;
                    }
                }
            }
        }
        
        
//        imshow("rat contour", ratContour);
//        imwrite("/Users/Achilles/Desktop/2018 summer research/project files/opencv trial/rat_track/diff.png", ratContour);
//        waitKey();
        
        xavg = xavg/coordCount;
        yavg = yavg/coordCount;
        
        //add the current centre coordinates to the vector
        centre_coords.push_back(make_pair(xavg, yavg));
        
        outFile << xavg << "    " << yavg << endl;
        

        
        //draw out the location marker
        for (int r = yavg -15; r < yavg + 15; r++){
            for (int c = xavg -15; c < xavg + 15; c++){
                frame.at<Vec3b>(r, c)[0] = 0;
                frame.at<Vec3b>(r, c)[1] = 0;
                frame.at<Vec3b>(r, c)[2] = 255;
                ratContour.at<uchar>(r, c) = 255;
            }
        }
        
        //to draw out both video images (RGB), and the edge map (binary)
        imshow("rat contour", ratContour);
        imshow("rat tract", frame);
        
        waitKey();
        
    }
    
    
    //close the output file
    outFile.close();
    
    return centre_coords;
    
}