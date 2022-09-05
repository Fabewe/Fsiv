/***********************************************
* Assigment 2: Image equalization			   *										    
************************************************
*/

#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>
using namespace std;

cv::Mat image,greyy,equalized;

int main(int  argc,char** argv){
	


	if (argc=!2){
		cout<<"First argument must be the image path"<<endl;
	}

	image=cv::imread(argv[1]);

	if(image.rows==0){
		cout<<"Error reading the image"<<endl;
	}
	//Histogram
	float H[256];
	//H[]->0
	for(int x=0;x<256;x++){
		H[x]=0;
	}
	//Cumulative histogram
	float C[256];

	//Normalized histogram
	float N[256];

	cv::cvtColor(image,greyy,cv::COLOR_BGR2GRAY);
	cv::cvtColor(image,equalized,cv::COLOR_BGR2GRAY);
	cv::namedWindow("Grey");
	cv::imshow("Grey",greyy);


	//Fill histogram
	for(int y=0;y<image.rows;y++){
		uchar *ptr=greyy.ptr<uchar>(y);
  		for(int x=0;x<image.cols;x++){
  		H[ptr[x]]++;
  		}
 	}

 	//C[]->0
 	for(int x=0;x<256;x++){
		C[x]=0;
	}

 	//Fill cumulative histogram
 	C[0]=H[0];
 	for (int x=1;x<256;x++){
 		C[x]=H[x]+C[x-1];
 	}

 	//N[]->0
 	for(int x=0;x<256;x++){
		N[x]=0;
	}

	//N[]->Normalized C[]
	for(int x=0;x<256;x++){
		N[x]=(C[x]/C[255])*255;
	}

	//N[x]->image[x]
	for(int y=0;y<image.rows;y++){
		uchar *ptre=equalized.ptr<uchar>(y);
		uchar *ptrg=greyy.ptr<uchar>(y);
  		for(int x=0;x<image.cols;x++){
  			ptre[x]=N[ptrg[x]];
  		}
 	}

 	cv::namedWindow("Equalized");
	cv::imshow("Equalized",equalized);

/*	 Debug
 	cout<<"Pixel totales: "<<greyy.rows*greyy.cols<<endl;

 	cout<<"C[200]: "<<C[200]<<endl;
 	cout<<"C[255]: "<<C[255]<<endl;

 	cout<<"N[254]: "<<N[254]<<endl;

	ºskjjdhasj
 	cout<<"N[255]: "<<N[255]<<endl;
*/


	cv::waitKey(0);

}
