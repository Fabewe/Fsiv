/****************************************
 * Author: Francisco Javier Perez Castillo
 * Universidad de Córdoba (Spain)
 * 
 * The program must have the following command-line arguments:
 * 
 * ./augReal rows cols size intrinsics.yml video.mp4
 * 
 * **************************************/

/*
 * Documentation:

    - findChessboardCorners: https://docs.opencv.org/4.5.3/d9/d0c/group__calib3d.html#ga93efa9b0aa890de240ca32b11253dd4a
    - solvePnP: https://docs.opencv.org/3.4.15/d9/d0c/group__calib3d.html#ga549c2075fac14829ff4a58bc931c033d
    - cornerSubPix: https://docs.opencv.org/4.5.3/dd/d1a/group__imgproc__feature.html#ga354e0d7c86d0d9da75de9b9701a9a87e
    - projectPoints: https://docs.opencv.org/4.5.3/d9/d0c/group__calib3d.html#ga1019495a2c8d1743ed5cc23fa0daff8c

*/

#include <opencv2/core/core.hpp> //core routines
#include <opencv2/highgui/highgui.hpp>//imread,imshow,namedWindow,waitKey
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv){
try {

    vector<Point2f> pattern_centers; //Points for the center of the pattern
    vector<Point3f> board_points3D; //Vector of points "in the object coordinate space" 
    vector<Point3f> axis;

    //Arguments of the program
    int rows = stoi(argv[1]);
    int cols = stoi(argv[2]);
    double size = stod(argv[3]);
    String parameters_path = argv[4];
    String video_path = argv[5];

    for(int i=0 ; i<rows-1 ; i++){
        for(int j=0; j<cols-1 ; j++){
            board_points3D.push_back(Point3f(j, i, 0));
        }
    }

    //We create the axis
    axis.push_back(Point3f(0,0,0));
    axis.push_back(Point3f(size, 0, 0));
    axis.push_back(Point3f(0, size, 0));
    axis.push_back(Point3f(0, 0, -size));

    //We read the yml file and we get the parameters
    Mat cameraMatrix(3, 3, CV_32F); //We store the intrinsic parameters of the camera in this matrix
    Mat distorsionCoeffs(5, 1, CV_32F);
    FileStorage f(parameters_path, FileStorage::READ); //We open the file and read its content
    f["camera_matrix"] >> cameraMatrix;
    f["distortion_coefficients"] >> distorsionCoeffs;
    //Necessary vectors to solvePnP
    Mat rodriguesVec(3, 1, CV_32F);
    Mat translationVec(3, 1, CV_32F);

    //Necesary to know where the chess pattern is
    Size nSize(cols-1,rows-1); //Number of corners inside of the pattern, not counting edges of the pattern
    Size winSize = Size(rows*2+1, cols*2+1);
    Size zeroZone(-1,-1);
    //Termination of the iterative process of corner refinement
    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
        
    //Images that will store the frames of the video
    Mat video_frame;
    Mat grey_video_frame;

    //We have created the object video, now we want to get every frame of the video and read it as an image
    VideoCapture video(video_path); //We use the clas 'VideoCapture' to use the video for the 3D Cube.

    while(video.grab()){ //We get the video frames, returns true in that case, false if there is no frames to get

        video.retrieve(video_frame); //We put the grabbed frame in the while() into the video_frame image object.

        //Returns different than 0 if it find the cornosers of the chessboard pattern.
        if(findChessboardCorners(video_frame, nSize, pattern_centers)){
           
            cvtColor(video_frame, grey_video_frame, COLOR_BGR2GRAY); //We change the color of the frame
       
            cornerSubPix(grey_video_frame, pattern_centers, winSize, zeroZone, termcrit); //"Half of the side length of the search window"
  
            //We stimate the camera with respect the board by solving the PnP
            //To do this, we need the intrisic parameters of the camera.  
       
            solvePnP(board_points3D, pattern_centers, cameraMatrix, distorsionCoeffs, rodriguesVec, translationVec);

            //Now we project information into the chess board pattern
            projectPoints(axis, rodriguesVec, translationVec, cameraMatrix, distorsionCoeffs, pattern_centers);

            //Finally we can draw lines for each axis thanks to the points
            line(video_frame, pattern_centers[0], pattern_centers[1], Scalar(0,0,255), 5);
            line(video_frame, pattern_centers[0], pattern_centers[2], Scalar(0,255,0), 5);
            line(video_frame, pattern_centers[0], pattern_centers[3], Scalar(255,0,0), 5);
    
            imshow("Video frames", video_frame);
        }
        waitKey(60);
    }


} catch (exception &ex) {
    cout<< "Error in function: " << ex.what()<<endl;
}
}