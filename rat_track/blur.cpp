#include "blur.hpp"
#include <stdint.h>

//contains various blurring methods
//bluring is to reduce noise
void blur(){
    
    Mat inputImage=imread("/Users/Achilles/Desktop/2018 summer research/project files/test_images/lena.png", CV_LOAD_IMAGE_GRAYSCALE);
    
    imshow("original", inputImage);
    
    //gaussian blur
    Mat gaussianOutput_10, gaussianOutput_100;
    
    GaussianBlur(inputImage, gaussianOutput_10, Size(5, 5), 1, 1);
    
    imshow("Gaussian Blur 5", gaussianOutput_10);
    
    GaussianBlur(inputImage, gaussianOutput_100, Size(5, 5), 1000, 1000);
    
    imshow("Gaussian Blur 1000", gaussianOutput_100);
    
    
    Mat differenceGaussian10(inputImage.rows, inputImage.cols, CV_8UC1);
    Mat differenceGaussian100(inputImage.rows, inputImage.cols, CV_8UC1);
    Mat sum(inputImage.rows, inputImage.cols, CV_8UC1);
    Mat sumRemove(inputImage.rows, inputImage.cols, CV_8UC1);
    
    for (int r = 0; r < inputImage.rows; r++){
        for (int c = 0; c < inputImage.cols; c++){
            
            differenceGaussian10.at<uint8_t>(r, c) = 0;
            differenceGaussian100.at<uint8_t>(r, c) = 0;
            sum.at<uint8_t>(r, c) = 0;
            sumRemove.at<uint8_t>(r, c) = 0;
            
            
            int temp = gaussianOutput_10.at<uint8_t>(r, c) - gaussianOutput_100.at<uint8_t>(r, c);
            
            if (temp > 0){
                differenceGaussian10.at<uint8_t>(r, c)= 100 + temp;
//                cout << "Have > 0, diff is " << temp << ", row is " << r << ", col is " << c <<endl;
//                cout << "value of gauss10 is " << (int)gaussianOutput_10.at<uint8_t>(r, c);
//                cout << ", value of gausee100 is " << (int)gaussianOutput_100.at<uint8_t>(r, c) << endl;
//                cout <<endl;
                
            }
            else if (temp < 0) {
                differenceGaussian100.at<uint8_t>(r, c)= - temp + 100;
//                cout << "Have < 0, diff is " << temp << ", row is " << r << ", col is " << c <<endl;
//                cout << "value of gauss10 is " << (int)gaussianOutput_10.at<uint8_t>(r, c);
//                cout << ", value of gausee100 is " << (int)gaussianOutput_100.at<uint8_t>(r, c) << endl;
//                cout <<endl;
            }
            else{}
            
            sum.at<uint8_t>(r, c) = differenceGaussian10.at<uint8_t>(r, c) + differenceGaussian100.at<uint8_t>(r, c);
            
            if ((sum.at<uint8_t>(r, c)) > 110){
//                int ppp = sum.at<uint8_t>(r, c);
                sumRemove.at<uint8_t>(r, c) = 255;
            }
            
//            if (temp == 255){
//                cout << "Have a 255, row is " << r << ", col is " << c <<endl;
//                cout << "value of gauss10 is " << gaussianOutput_10.at<uint8_t>(r, c);
//                cout << ", value of gausee100 is " << gaussianOutput_100.at<uint8_t>(r, c) << endl;
//                cout <<endl;
//                differenceGaussian100.at<uint8_t>(r, c)= temp;
//            }
//            else if (temp == -255){
//                cout << "Have a -255, row is " << r << ", col is " << c <<endl;
//                cout << "value of gauss10 is " << gaussianOutput_10.at<uint8_t>(r, c);
//                cout << ", value of gausee100 is " << gaussianOutput_100.at<uint8_t>(r, c) << endl;
//                cout << endl;
//                differenceGaussian10.at<uint8_t>(r, c) = -temp;
//            }
//            else if(temp > 0)
//                differenceGaussian100.at<uint8_t>(r, c)= temp;
//            else
//                differenceGaussian10.at<uint8_t>(r, c) = -temp;

            
            
            /*for (int i=0; i<3; i++){
                int temp = gaussianOutput_10.at<cv::Vec3b>(r, c)[i] - gaussianOutput_100.at<cv::Vec3b>(r, c)[i];
                if (temp > 0)
                    differenceGaussian10.at<cv::Vec3b>(r, c)[i] = temp;
                else
                    differenceGaussian100.at<Vec3b>(r, c)[i] = temp;
            }*/
        }
    }
    
    imshow("difference 5", differenceGaussian10);
    imshow("difference 1000", differenceGaussian100);
    imshow("sum", sum);
    imshow("sumRemove", sumRemove);
    

//    Mat valueOne(480, 480, CV_8UC1, 1);
//    
//    imshow("valueOne", valueOne);
//    cout << differenceGaussian10<<endl;
    
    
    waitKey();



    /*
    //median blur
    //good for reducing the actual noise, like 噪点
    Mat medianOutput;
    
    medianBlur(inputImage, medianOutput, 5);
    
    imshow("Median Blur", medianOutput);
    
    waitKey();
    */
    
    
    //
    //blur(<#InputArray src#>, <#OutputArray dst#>, <#Size ksize#>);
    
    
}