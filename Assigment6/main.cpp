/***********************************************
* Francisco Javier Díaz Cantillo.              *
* Assigment 6: Median noise remove(colour)     *                                            
************************************************
*/

#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <iostream>
using namespace std;


void noiseMedian(cv::Mat &in,cv::Mat &out);
void applyKernel(const cv::Mat &in,cv::Mat &out,const cv::Mat &kernel);


int main(int argc,char ** argv){
    cv::Mat input,output,boxfiltered;
    vector<cv::Mat> colors(3);
    vector<cv::Mat> outs(3);
    vector<cv::Mat> boxs(3);
	if(argc!=2){

		cout<<"ERROR:Wrong arguments given"<<endl;

		return 0;
	}

	input=cv::imread(argv[1]);
    cv::Mat kernel = cv::Mat::zeros(3,3,CV_32FC1);
    kernel=1./9.;
    

	if(input.rows==0){

		cout<<"ERROR:Failed to open the image"<<endl;

		return 0;
	}
    input.convertTo(input,CV_8UC3);




    cv::split(input,colors);

    for(int i=0;i<3;i++){
        noiseMedian(colors[i],outs[i]);
        applyKernel(colors[i],boxs[i],kernel);
    }
    output.create(input.size(),input.type());
    boxfiltered.create(input.size(),input.type());
    cv::merge(outs,output);
    cv::merge(boxs,boxfiltered);




    cv::namedWindow("input");
    cv::imshow("input",input);

    cv::namedWindow("Output");
	cv::imshow("Output",output);

    cv::namedWindow("Box");
	cv::imshow("Box",boxfiltered);


    cv::waitKey(0);

}

void noiseMedian(cv::Mat &in,cv::Mat &out){
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

    for (int x=range;x<out.rows;x++){
        for (int y=range;y<out.cols;y++){
            float suma=0.0;
            //cout<<"["<<x<<","<<y<<"] ";
            for(int k2=range;k2<kernel.rows+range;k2++){
                for(int k1=range;k1<kernel.cols+range;k1++){

                    
                        suma+= in.at<uchar>(cv::Point(y-(range*2)+k1,x-(range*2)+k2)) * kernel.at<float>(cv::Point(k1-range,k2-range));     
                        //out.at<uchar>(cv::Point(y-(range*2)+k1,x-(range*2)+k2))=255;


                    
                    
                }        
            }
            out.at<uchar>(cv::Point(y,x))=suma;
            
        }
        //cout<<endl;
    }
}