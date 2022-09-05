/***********************************************
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

    int sze=2;

    while( sze%2 ==0){
        cout<<"Kernel size: ";
        cin>>sze;
        if(sze%2==0){
            cout<<"ERROR:Must be an odd number"<<endl;
        }
    }


    //cout<<range<<endl;

    cv::Mat kernel = cv::Mat::zeros(sze,sze,CV_32FC1);
    kernel=1./(sze*sze);


    applyKernel(grey,input,kernel);


    cv::namedWindow("output");
    cv::imshow("output",input);

    cv::namedWindow("input");
    cv::imshow("input",grey);

    cv::waitKey(0);
}




void applyKernel(const cv::Mat &in,cv::Mat &out,const cv::Mat &kernel){
    out.create(in.size(),in.type());

    int range=kernel.rows/2;

    cout<<range<<endl;

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
//||(y<range && k1-range>range-y && x>range) || (x<range && k2-range>=range-y && y>range)||(y<=range && x<=range && k1-range>range-y && k2-range>range-x )||(y+range>out.cols-1 && k1-range<range-(out.cols-y)) || (x+range>out.rows-1 && k2-range<range-(out.rows-x)) 
//&&(x+range<=out.rows-1 && y+range<=out.cols-1)
//(y+range>out.cols && k1-range<range-(out.cols-y)) || (x+range>out.rows && k2-range<range-(out.rows-x))
