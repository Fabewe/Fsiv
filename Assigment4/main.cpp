/***********************************************
* Assigment 4: Magnitude of Derivative         *                                            
************************************************
*/

#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <iostream>
using namespace std;

cv::Mat input,input2,output,derivX,derivY,derivXf,derivYf;

void applyKernel(const cv::Mat &in,cv::Mat &out,const cv::Mat &kernel);

void vec(cv::Mat &x,cv::Mat &y,cv::Mat &out );

int main(int argc,char** argv){
	if(argc!=2){

		cout<<"ERROR:Wrong arguments given"<<endl;

		return 0;
	}

	input=cv::imread(argv[1],cv::IMREAD_GRAYSCALE);

	if(input.rows==0){

		cout<<"ERROR:Failed to open the image"<<endl;

		return 0;

	}

	cv::Mat kernelx = cv::Mat::zeros(3,3,CV_32FC1);

	kernelx.at<float>(cv::Point(0,0))=-1.0;
	kernelx.at<float>(cv::Point(1,0))=-2.0;
	kernelx.at<float>(cv::Point(2,0))=-1.0;

	kernelx.at<float>(cv::Point(0,2))=1.0;
	kernelx.at<float>(cv::Point(1,2))=2.0;
	kernelx.at<float>(cv::Point(2,2))=1.0;


	cv::Mat kernely = cv::Mat::zeros(3,3,CV_32FC1);

	kernely.at<float>(cv::Point(0,0))=-1.0;
	kernely.at<float>(cv::Point(0,1))=-2.0;
	kernely.at<float>(cv::Point(0,2))=-1.0;

	kernely.at<float>(cv::Point(2,0))=1.0;
	kernely.at<float>(cv::Point(2,1))=2.0;
	kernely.at<float>(cv::Point(2,2))=1.0;


	input.convertTo(input2,CV_32FC1);


	derivX.convertTo(derivXf,CV_32FC1);
	derivY.convertTo(derivYf,CV_32FC1);

	applyKernel(input2,derivXf,kernelx);
	applyKernel(input2,derivYf,kernely);


	
	//input.convertTo(input2,CV_32FC1);



	//input2.create(input.size(),CV_32FC1);

	

	vec(derivXf,derivYf,input2);
	output.create(input2.size(),CV_32FC1);
	cv::normalize(input2,output,255,cv::NORM_MINMAX);


	cv::namedWindow("derixf");
	cv::imshow("derixf",derivXf);
	cv::namedWindow("derivyf");
	cv::imshow("derivyf",derivYf);

    cv::namedWindow("input");
    cv::imshow("input",input2);


    cv::namedWindow("Output");
	cv:imshow("Output",output);

    cv::waitKey(0);

}



void vec(cv::Mat &derix,cv::Mat &deriy,cv::Mat &out ){
	out.create(derix.size(),CV_32FC1);
	//cout<<" dX rows: "<< derix.rows<<endl;
	//cout<<" dX cols: "<< derix.cols<<endl;
	//cout<<" dY rows: "<< deriy.rows<<endl;
	//cout<<" dY cols: "<< derix.cols<<endl;
	//cout<<" out rows: "<<out.rows<<endl;
	//cout<<" out cols: "<<out.cols<<endl;

	for(int x=0;x<out.rows;x++){
		for(int y=0;y<out.cols;y++){

			float x2=derix.at<float>(cv::Point(y,x));
			float y2=deriy.at<float>(cv::Point(y,x));


			float sqrv =sqrt((x2*x2) + (y2*y2));

			out.at<float>(cv::Point(y,x))=sqrv;
			

		}


	}
	

}


void applyKernel(const cv::Mat &in,cv::Mat &out,const cv::Mat &kernel){
    out.create(in.size(),in.type());

    int range=kernel.rows/2;

    //cout<<range<<endl;

    for (int x=range;x<out.rows-range;x++){
        for (int y=range;y<out.cols-range;y++){
            float suma=0.0;
            for(int k2=range;k2<kernel.rows+range;k2++){
                for(int k1=range;k1<kernel.cols+range;k1++){

                        
                        suma+= in.at<float>(cv::Point(y-(range*2)+k1,x-(range*2)+k2)) * kernel.at<float>(cv::Point(k1-range,k2-range));                     
            }
            out.at<float>(cv::Point(y,x))=suma;
            
        }
        //cout<<endl;
    	}
	}
}
