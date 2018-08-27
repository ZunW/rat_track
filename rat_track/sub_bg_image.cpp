//
//  sub_bg_image.cpp
//  rat_track
//
//  Created by Zun Wang on 2018-07-16.
//  Copyright © 2018 Zun Wang. All rights reserved.
//

#include "sub_bg_image.hpp"




//it's a wrapper. it calls the respective fxns to obtain a vector, then store them into files
void sub_bg_img_wrapper (const string& videoPath){
    
//    Mat bgAverage = getRGBbg(videoPath);

    Mat bgAverage = imread("/Users/Achilles/Desktop/2018 summer research/project files/opencv trial/rat_track/rgb_bg_warp.png", CV_LOAD_IMAGE_GRAYSCALE);
    
    //change to desired frame
    Mat frame = imread("/Users/Achilles/Desktop/2018 summer research/project files/opencv trial/rat_track/rgb_bg_warp.png", CV_LOAD_IMAGE_COLOR);
    
    //get the coordinates of the base of the cage
    vector<Point2f> baseCoord = markCorners(frame);
    pair<int, int> boundarySize = getBoundarySize(baseCoord);
    cout << "boundary size is " << boundarySize.first << "   " << boundarySize.second << endl;
    vector<Point2f> desiredFrame = getDesiredFrame (boundarySize);


    
    
    //get algorithm data
    vector<Point2f> method2AlgData = subBGimage(videoPath, bgAverage);
    
    vector<Point2f> algoDataNewFrame = getCoordNewFrame(method2AlgData, baseCoord, desiredFrame);

    for(vector<Point2f>::iterator it = algoDataNewFrame.begin(); it != algoDataNewFrame.end(); it++){
        checkBoundary(*it, boundarySize);
    }
    
    //output to file
    ofstream algoData;
    algoData.open("/Users/Achilles/Desktop/2018 summer research/project files/opencv trial/rat_track/clip9_warp_algo.txt");
    
    if (!algoData.is_open()){
        cout << "Can't open algo output file!" << endl;
    }
    else{
        for (vector<Point2f>::iterator it = algoDataNewFrame.begin(); it != algoDataNewFrame.end(); it++){
            int x = (int) it->x, y = boundarySize.second - ((int) it->y);   //it's more intuitive to have left bottom as origin
            algoData << x << "  " << y << endl;
        }
    }
    
    
    
    
    

    //get the human input for the cap position
    vector<Point2f> method2HumanData = getHumanData(videoPath);
    
    vector<Point2f> humanDataNewFrame = getCoordNewFrame(method2HumanData, baseCoord, desiredFrame);
    
    for(vector<Point2f>::iterator it = humanDataNewFrame.begin(); it != humanDataNewFrame.end(); it++){
        checkBoundary(*it, boundarySize);
    }
    
    //output to file
    ofstream humanData;
    humanData.open("/Users/Achilles/Desktop/2018 summer research/project files/opencv trial/rat_track/clip9_warp_human_centroid.txt");
    
    if (!humanData.is_open()){
        cout << "Can't open human output file!" << endl;
    }
    else{
        for (vector<Point2f>::iterator it = humanDataNewFrame.begin(); it != humanDataNewFrame.end(); it++){
            int x = (int) it->x, y = boundarySize.second - ((int) it->y);
            humanData << x << "  " << y << endl;
        }
    }

}




//the actual fxn of the algorithm, basically it subtracts the bg from each frame to find the
//raw rat image, then reduce the noise we have a better rat image. Calculate the average value
//of all the remaining pixels' coordinates we have the centroid
vector<Point2f> subBGimage (const string& videoPath, const Mat& bgAverage){
    
    VideoCapture capVid(videoPath);
    int width = (int) capVid.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = (int) capVid.get(CV_CAP_PROP_FRAME_HEIGHT);
    
    //to generate a marked video
    int vidCode = static_cast<int>(capVid.get(CV_CAP_PROP_FOURCC));

    Size origFrameSize(width, height);
    Size procFrameSize(2*width, 2*height);
    
    VideoWriter origVid, processedVid;
    
    double fps = 10;

    origVid.open("/Users/Achilles/Desktop/2018 summer research/project files/opencv trial/rat_track/orig_vid.avi", vidCode, fps, origFrameSize);
    
    if (!origVid.isOpened()){
        cout << "Cannot open output file!" <<endl;
    }
    
    processedVid.open("/Users/Achilles/Desktop/2018 summer research/project files/opencv trial/rat_track/processed_vid.avi", vidCode, fps, procFrameSize);
    
    if (!processedVid.isOpened()){
        cout << "Cannot open output file!" <<endl;
    }

    
    //will keep a ~10s rat route for drawing purpose
    int listSize = fps * DEMO_TIME;
    list<Point> ratRoute;
    int frameCount = 0;
    Mat grayFrame = Mat::zeros(height, width, CV_8UC1);
    Mat frameDiff = Mat::zeros(height, width, CV_8UC1);
    Mat frame;
    vector<Point2f> method2AlgData;
    
    while (capVid.read(frame)){
        
        //keep a copy of the frame
        Mat frameCopy = frame.clone();
        
        cvtColor(frame, grayFrame, CV_BGRA2GRAY);
        
        for (int r = 0; r < frame.rows; r++){
            for (int c = 0; c < frame.cols; c++){
                
                int absDiff = abs( grayFrame.at<uchar>(r, c) - bgAverage.at<uchar>(r, c));
                
                //if there's not a big difference in content, it's background
                if ( (absDiff < THRESHOLD_IMG_DIFF_LOW) || (frame.at<Vec3b>(r, c)[0] > TOO_BRIGHT) )
                {
                    frameDiff.at<uchar>(r, c) = 0;
                }
                else{   //it's a moving object
                    frameDiff.at<uchar>(r, c) = grayFrame.at<uchar>(r, c);
                    
                }
            }
        }
        
//        imshow("before mb", frameDiff);
        

        Mat afterMB;
        medianBlur(frameDiff, afterMB, MEDIANBLUR_SIZE);
//        imshow("after mb", afterMB);
        
        double xsum = 0, ysum = 0, ratPixCount = 0;
        
        //traverse again to add up the coordinates, it has to be after the median blur
        for (int r = 0; r < afterMB.rows; r++){
            for (int c = 0; c < afterMB.cols; c++) {
                if (afterMB.at<uchar>(r, c) != 0){   //if it's non-zero, it's a rat pixel
                    xsum = xsum + c;
                    ysum = ysum + r;
                    ratPixCount ++;
                }
            }
        }
        
        Point2f currentCentroid;
        currentCentroid.x = xsum / ratPixCount;
        currentCentroid.y = ysum / ratPixCount;
        
        
        method2AlgData.push_back(currentCentroid);
        
        
        int xavg = (int) currentCentroid.x, yavg = (int) currentCentroid.y;
        
        
        //store it in the rat route
        Point centroidInt(xavg, yavg);
        if (ratRoute.size() < listSize){   //we only keep certain amount
            ratRoute.push_back(centroidInt);
        }
        else{   //then we will pop and push
            ratRoute.pop_front(); //pop the first
            ratRoute.push_back(centroidInt);
        }

        
        
        //draw out the centroid
        for (int r = yavg - 10; r < yavg + 10; r++){
            for (int c = xavg - 10; c < xavg + 10; c++){
                frame.at<Vec3b>(r, c)[0] = 0;
                frame.at<Vec3b>(r, c)[1] = 0;
                frame.at<Vec3b>(r, c)[2] = 255;
            }
        }
        
        //draw out the path
        for (list<Point>::iterator it = ratRoute.begin(); it != ratRoute.end(); it++) {
            list<Point>::iterator nx = next(it);
            if(nx != ratRoute.end()){
                line(frame, *it, *nx, Scalar(0, 0, 255));
            }
        }
        
        
        //save videos for presentation purpose
        if ((frameCount > 600) && (frameCount < 800)){
            //the original video
            origVid << frameCopy;
            
            //nned to convert all to 3 channels
            vector<Mat> frameDiffHolder, afterMBholder;
            Mat frameDiff3Channel, afterMB3Channel;
            for (int i=0; i<3; i++){
                frameDiffHolder.push_back(frameDiff);
                afterMBholder.push_back(afterMB);
            }
            merge(frameDiffHolder, frameDiff3Channel);
            merge(afterMBholder, afterMB3Channel);
            
            //concatenate all 4 mats
            Mat hMat1, hMat2, processedFrame;
            hconcat(frameCopy, frameDiff3Channel, hMat1);
            hconcat(afterMB3Channel, frame, hMat2);
            vconcat(hMat1, hMat2, processedFrame);
            
            processedVid << processedFrame;
        }
        
        frameCount++;
        
        
    }
    
    capVid.release();
    origVid.release();
    processedVid.release();
    
    return method2AlgData;
    
}





//used to find the background by averaging out the frames taking at a certain frequency
Mat getRGBbg(const string& videoPath){
    
    VideoCapture capVid(videoPath);
    
    Mat frame;
    
    capVid.read(frame);

    Mat bgAverage(frame.rows, frame.cols, CV_64FC1, 0.0);

    int frameCount = 0, count = 0;
    
    
    while (capVid.read(frame)) {
        
        if (! (frameCount % GET_BG_FREQ)){
            
            cvtColor(frame, frame, CV_BGR2GRAY);
                        
            frame.convertTo(frame, CV_64FC1);
            
            bgAverage = bgAverage + frame;
            
            
            count ++;
        }
        
        frameCount ++;
    }
    
    capVid.release();
    
    bgAverage = bgAverage / count;
    
    bgAverage.convertTo(bgAverage, CV_8UC1);
    
    imshow("bgAverage", bgAverage);
    
    imwrite("/Users/Achilles/Desktop/2018 summer research/project files/opencv trial/rat_track/rgb_bg_warp.png", bgAverage);
    
    waitKey();
    
    return bgAverage;
    
}











