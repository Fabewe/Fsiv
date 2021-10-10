/***********************************************
* Francisco Javier Díaz Cantillo.              *
* Assigment 5: High boost filter		       *                                            
************************************************
*/

#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <iostream>
using namespace std;

cv::Mat input,input2,input3,boxfiltered,output,output2,normd;

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
    //creating kernel

    int c,t;
    cout<<"Introduce the C value= "<<endl;
    cin>>c;
    while(c<0){
    	cout<<"It must be in [0,∞) range"<<endl;
    	cin>>c;
    }

    //cout<<"Tamaño"<<endl;
    //cin>>t;
  	t=3;

  	cv::Mat bfKernel = cv::Mat::zeros(t,t,CV_32FC1);
  	bfKernel=1./9.;

  	


	cv::Mat hbKernel = cv::Mat::zeros(t,t,CV_32FC1);
    //giving values
	hbKernel=(float)-c;
	hbKernel.at<float>(cv::Point(t/2,t/2))=(float)(((t*t-1)*c)+1);

	for(int i=0;i<hbKernel.rows;i++){
		for(int j=0;j<hbKernel.rows;j++){
			cout<<hbKernel.at<float>(cv::Point(i,j))<<" , ";

		}
		cout<<endl;
	}


    //converting everything to floating point data

	input.convertTo(input2,CV_32FC1);
	normd.create(output.size(),CV_32FC1);

	applyKernel(input2,boxfiltered,bfKernel);

	input3=input2-boxfiltered;

	applyKernel(input3,output,hbKernel);

	output2=input2+output;




	normd.create(input2.size(),CV_32FC1);

	cv::normalize(output2,normd,255,cv::NORM_MINMAX);

    cv::namedWindow("input");
    cv::imshow("input",input);

    cv::namedWindow("Output");
	cv::imshow("Output",normd);

    cv::waitKey(0);

}
	



void applyKernel(const cv::Mat &in,cv::Mat &out,const cv::Mat &kernel){
    out.create(in.size(),in.type());

    int range=kernel.rows/2;

    for (int x=range;x<out.rows-range;x++){
        for (int y=range;y<out.cols-range;y++){
            float suma=0.0;
            for(int k2=range;k2<kernel.rows+range;k2++){
                for(int k1=range;k1<kernel.cols+range;k1++){

                        
                        suma+= in.at<float>(cv::Point(y-(range*2)+k1,x-(range*2)+k2)) * kernel.at<float>(cv::Point(k1-range,k2-range));                     
            }
            out.at<float>(cv::Point(y,x))=suma;
            
        }

    	}
	}
}
