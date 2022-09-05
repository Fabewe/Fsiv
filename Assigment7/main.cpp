/***********************************************
* Assigment 7: Morphological operations	       *                                            
************************************************
*/

#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <iostream>
#include <string>
using namespace std;


void noiseMedian(const cv::Mat &in,cv::Mat &out);
void applyKernel(const cv::Mat &in,cv::Mat &out,const cv::Mat &kernel);
void threshold(cv::Mat &in,int th);
void dilation(cv::Mat &in);
void erotion(cv::Mat &in);


int main(int argc,char ** argv){
    cv::Mat input,input2,output,boxfiltered;

    // ./prog in_img out_img -thres <value> -op <erode|dilate|open|close>
	if(argc!=7){

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

    //cout<<argv[6]<<endl;
    threshold(input,stoi(argv[4]));
    cv::namedWindow("input");
    cv::imshow("input",input);
    


    if((strcmp(argv[6],"erode"))==0){
        erotion(input);
        //cout<<"erotda"<<endl;
    }else if(strcmp(argv[6],"dilate")==0){
        dilation(input);
    }else if(strcmp(argv[6],"open")==0){
        erotion(input);
        dilation(input);
    }else if(strcmp(argv[6],"close")==0){
        dilation(input);
        erotion(input);
    }else{
        cout<<"ERROR WRONG OPTION"<<endl;
    }


    cv::imwrite(argv[2],input);
    cv::namedWindow("result");
    cv::imshow("result",input);

 


    cv::waitKey(0);

}

void dilation(cv::Mat &in){
    cv::Mat aux;
    aux.create(in.size(),in.type());
    in.copyTo(aux);
    int value;


    int range=3/2;

    for (int x=range;x<in.rows-range;x++){
        for (int y=range;y<in.cols-range;y++){

            for(int k2=range;k2<3+range;k2++){
                for(int k1=range;k1<3+range;k1++){
                    if(aux.at<uchar>(cv::Point(y-(range*2)+k1,x-(range*2)+k2))>aux.at<uchar>(cv::Point(y,x))){
                        in.at<uchar>(cv::Point(y,x))=in.at<uchar>(cv::Point(y-(range*2)+k1,x-(range*2)+k2));
                    }

            	}
            }
    	}
	}
}

void erotion(cv::Mat &in){
    cv::Mat aux;
    aux.create(in.size(),in.type());
    in.copyTo(aux);

    int range=3/2;

    for (int x=range;x<in.rows-range;x++){
        for (int y=range;y<in.cols-range;y++){

            for(int k2=range;k2<3+range;k2++){
                for(int k1=range;k1<3+range;k1++){
                    if(aux.at<uchar>(cv::Point(y-(range*2)+k1,x-(range*2)+k2))<aux.at<uchar>(cv::Point(y,x))){
                        in.at<uchar>(cv::Point(y,x))=in.at<uchar>(cv::Point(y-(range*2)+k1,x-(range*2)+k2));
                    }

            	}
            }
    	}
	}
}

void threshold(cv::Mat &in,int th){
    cv::Mat aux;
    aux.create(in.size(),in.type());
    in.copyTo(aux);


    int range=3/2;

    for (int x=range;x<in.rows-range;x++){
        for (int y=range;y<in.cols-range;y++){
            if(aux.at<uchar>(cv::Point(y,x))<th){
                in.at<uchar>(cv::Point(y,x))=255;
            }else{
                in.at<uchar>(cv::Point(y,x))=0;
            }
    	}
	}
}
