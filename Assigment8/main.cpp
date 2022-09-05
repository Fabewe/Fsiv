/********************************************************
* Assigment 8:  Camera Calibration and 3D formation     *                                            
*********************************************************
*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace std;

int main(int argc,char **argv){
    //Arguments
    int rows = stoi(argv[1]);
    int cols = stoi(argv[2]);
    double size = stod(argv[3]);
    std::string parameters_path = argv[4];
    std::string pathToVideo = argv[5];
    
   vector<cv::Point2f> corners;
   vector<cv::Point3f> points3d; //Vector of points "in the object coordinate space" 
   vector<cv::Point3f> ax;

    //Setups
    for(int i=0 ; i<rows-1 ; i++){
        for(int j=0; j<cols-1 ; j++){
            points3d.push_back(cv::Point3f(j, i, 0));
        }
    }
    ax.push_back(cv::Point3f(0,0,0));
    ax.push_back(cv::Point3f(size, 0, 0));
    ax.push_back(cv::Point3f(0, size, 0));
    ax.push_back(cv::Point3f(0, 0, -size));

    //Reading yml file
    cv::Mat mtx(3, 3, CV_32F); //Intrinsic
    cv::Mat distc(5, 1, CV_32F);
    cv::FileStorage f(parameters_path, cv::FileStorage::READ); 
    f["camera_matrix"] >> mtx;
    f["distortion_coefficients"] >> distc;
    

    //Rotation and Translation vectors
    cv::Mat rvec(3, 1, CV_32F);
    cv::Mat tvec(3, 1, CV_32F);

    //Knowting chess position
    cv::Size innerC(cols-1,rows-1); //inner corners
    cv::Size winSize = cv::Size(rows*2+1, cols*2+1);
    cv::Size unreach(-1,-1);
    //Termination of the iterative process of corner refinement
    cv::TermCriteria termcrit(cv::TermCriteria::COUNT|cv::TermCriteria::EPS,20,0.03);

   cv::Mat image,grey;
   cv::VideoCapture video(pathToVideo);
   while(video.grab()){

       video.retrieve(image);


        if(cv::findChessboardCorners(image, innerC, corners)){
           
            cv::cvtColor(image, grey, cv::COLOR_BGR2GRAY);
       
            cv::cornerSubPix(grey, corners, winSize, unreach, termcrit);
  
            //Solving PnP
            cv::solvePnP(points3d, corners, mtx, distc, rvec, tvec);

            //The correct location to position the points
            cv::projectPoints(ax, rvec, tvec, mtx, distc, corners);

            //Draw lines to every axis direction
            cv::line(image, corners[0], corners[1], cv::Scalar(0,0,255), 5);
            cv::line(image, corners[0], corners[2], cv::Scalar(0,255,0), 5);
            cv::line(image, corners[0], corners[3], cv::Scalar(255,0,0), 5);

            imshow("Video frames", image);
            
        }
        

       cv::waitKey(10);
   }

}
