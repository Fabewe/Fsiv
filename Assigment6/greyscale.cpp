/***********************************************
* Francisco Javier Díaz Cantillo.              *
* Assigment 6: Median noise remove	       *                                            
************************************************
*/

#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <iostream>
using namespace std;


void noiseMedian(const cv::Mat &in,cv::Mat &out);
void applyKernel(const cv::Mat &in,cv::Mat &out,const cv::Mat &kernel);


int main(int argc,char ** argv){
    cv::Mat input,input2,output,boxfiltered;
	if(argc!=2){

		cout<<"ERROR:Wrong arguments given"<<endl;

		return 0;
	}
    cv::Mat kernel = cv::Mat::zeros(3,3,CV_32FC1);
    kernel=1./9.;
	input=cv::imread(argv[1],cv::IMREAD_GRAYSCALE);

	if(input.rows==0){

		cout<<"ERROR:Failed to open the image"<<endl;

		return 0;
	}

	noiseMedian(input,output);
    applyKernel(input,boxfiltered,kernel);


    cv::namedWindow("input");
    cv::imshow("input",input);

    cv::namedWindow("Boxfiltered");
	cv::imshow("Boxfiltered",boxfiltered);

    cv::namedWindow("Median");
	cv::imshow("Median",output);

    cv::waitKey(0);

}

void noiseMedian(const cv::Mat &in,cv::Mat &out){
    out.create(in.size(),in.type());


    int range=3/2;

    for (int x=range;x<out.rows-range;x++){
        for (int y=range;y<out.cols-range;y++){
            vector<uchar> vect (9,0);
        	int p=0;
            for(int k2=range;k2<3+range;k2++){
                for(int k1=range;k1<3+range;k1++){
                vect[p]=in.at<uchar>(cv::Point(y-(range*2)+k1,x-(range*2)+k2));                     
                p++;
            	}
            }
            sort(vect.begin(),vect.end());
            out.at<uchar>(cv::Point(y,x))=vect[vect.size()/2];
    	}
	}
}

void applyKernel(const cv::Mat &in,cv::Mat &out,const cv::Mat &kernel){
    out.create(in.size(),in.type());

    int range=kernel.rows/2;

    for (int x=0;x<out.rows;x++){
        for (int y=0;y<out.cols;y++){
            float suma=0.0;
            //cout<<"["<<x<<","<<y<<"] ";
            for(int k2=range;k2<kernel.rows+range;k2++){
                for(int k1=range;k1<kernel.cols+range;k1++){

                    
                    if(((y>=range && x>=range)&&(x+range<=in.rows-1 && y+range<=in.cols-1))                                         ||

                        (y<range && k1-range>range-y && x>range && x+range<in.rows-1)                                               || //Left border
                        (y+range>in.cols-1 && k1-range>(range-(out.cols-y)) && x>range && x+range<in.rows-1)                        || //Right border

                        (x<range && k2-range>range-x && y>range && y+range<out.cols)                                                || //Top border
                        (x+range>in.rows-1 && k2-range>(range-(in.rows-x)) && y>range && y+range<in.cols-1)                         || //Bottom border

                        (y<=range && k1-range>range-y && x<=range &&  k2-range>range-x)                                             || //Top-Left corner
                        (y<=range && k1-range>range-y && x+range>=in.rows-1 && k2-range>(range-(in.rows-x)))                        || //Bottom-Left corner

                        (x<=range && k2-range>range-x && y+range>=in.cols-1 && k1-range>(range-(out.cols-y)))                       || //Top-Right corner
                        (x+range>=in.rows-1 && k2-range>(range-(in.rows-x)) && y+range>=in.cols-1 && k1-range>(range-(out.cols-y)))    //Bottom-Right corner

                        ){
                        suma+= in.at<uchar>(cv::Point(y-(range*2)+k1,x-(range*2)+k2)) * kernel.at<float>(cv::Point(k1-range,k2-range));     
                        //out.at<uchar>(cv::Point(y-(range*2)+k1,x-(range*2)+k2))=255;

                    }else{
                        suma+=0;
                    }
                    
                    
                }        
            }
            out.at<uchar>(cv::Point(y,x))=suma;
            
        }
        //cout<<endl;
    }
}  