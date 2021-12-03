// ----------------------------------------
// seglib.cpp
// (c) FSIV, University of Cordoba
// ----------------------------------------

#include "seglib.hpp"

void fsiv_segm_by_dif(const cv::Mat & prevFrame, const cv::Mat & curFrame, cv::Mat & difimg, int thr, int r)
{
   // WRITE ME
   cv::Mat img1,img2,ej;

   prevFrame.copyTo(img1);
   curFrame.copyTo(img2);
   
   cv::absdiff(img1,img2,ej);

   cv::cvtColor(ej,ej,cv::COLOR_BGR2GRAY);

   //cv::imshow ("diff", ej);
   for(int x=0;x<ej.rows;x++){
      for(int y=0;y<ej.cols;y++){
         if(ej.at<uchar>(cv::Point(y,x))>=thr){
            difimg.at<uchar>(cv::Point(y,x))=255;
         }else{
            difimg.at<uchar>(cv::Point(y,x))=0;
         }

      }
   }

   if(r>0){
      //OPEN
      cv::Mat kernel= cv::Mat::ones(r,r,CV_32FC1);
      cv::erode(difimg,difimg,cv::Mat());
      cv::dilate(difimg,difimg,cv::Mat());

      //CLOSE
      cv::dilate(difimg,difimg,cv::Mat());
      cv::erode(difimg,difimg,cv::Mat());
}
}

void fsiv_apply_mask(const cv::Mat & frame, const cv::Mat & mask, cv::Mat & outframe)
{
   // WRITE ME

   cv::bitwise_and(frame,frame,outframe,mask);

}

// ================= OPTIONAL PART STARTS HERE =======================

void fsiv_learn_model(cv::VideoCapture & input, int maxframes, cv::Mat & MeanI, cv::Mat & I2, cv::Mat & VarI, cv::Mat & StdI)
{
   // WRITE ME

}

void fsiv_acc_model(const cv::Mat & frame, cv::Mat & MeanI, cv::Mat & I2)
{
   // WRITE ME
}

void fsiv_segm_by_model(const cv::Mat & frame, cv::Mat & theMask, const cv::Mat & mean, const cv::Mat & std, float t, int r)
{
   // WRITE ME
}

void fsiv_update_model(const cv::Mat & frame, cv::Mat & mean, cv::Mat & I2, cv::Mat &std,  float alpha, const cv::Mat & theMask)
{
   // WRITE ME
}
