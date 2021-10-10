/***********************************************
* Francisco Javier Díaz Cantillo.              *
* Assigment 3: Box filter.                     *                                            
************************************************
*/

#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace std;

cv::Mat input,grey;

void applyKernel(const cv::Mat &in,cv::Mat &out,const cv::Mat &kernel);

int main(int argc,char** argv){

    if (argc!=2){
        cout<<"Wrong arguments given"<<endl;
        return 0;
    } 
    
    input=cv::imread(argv[1],cv::IMREAD_GRAYSCALE);

    if(input.rows==0){

        cout<<"Error trying to read the image"<<endl;
        return 0;
    }

    input.copyTo(grey);

    cv::Mat kernel = cv::Mat::zeros(3,3,CV_32FC1);
    kernel=1./9.;


    applyKernel(grey,input,kernel);


    cv::namedWindow("output");
    cv::imshow("output",input);

    cv::namedWindow("input");
    cv::imshow("input",grey);

    cv::waitKey(0);
}


void applyKernel(const cv::Mat &in,cv::Mat &out,const cv::Mat &kernel){
    //out.create(in.size(),in.type());

    for (int x=0;x<out.rows;x++){
        for (int y=0;y<out.cols;y++){
            float suma=0.0;
            for(int k2=1;k2<kernel.rows+1;k2++){
                for(int k1=1;k1<kernel.cols+1;k1++){

                    
                    if(((y!=0 && x!=0)&&(y+1<out.cols-1 && x+1<out.rows-1))||(y==0 && k1>0) || (x==0 && k2>0) || (y+1==out.cols && k1<2) || (x+1==out.rows && k2<2)){
                        suma+= input.at<uchar>(cv::Point(y-1+k1,x-1+k2)) * kernel.at<float>(cv::Point(k1-1,k2-1));                   

                    }
                    
                    
                }        
            }
            out.at<uchar>(cv::Point(y,x))=suma;
        }
    }

}
