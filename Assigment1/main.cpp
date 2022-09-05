/***********************************************
* Assigment 1: Foreground Highlight			   *										    
************************************************
*/

#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
using namespace std;

//Global variables declaration
cv::Mat image,cloone;
cv::Point p1,p2;


//toGrey()= turns the image passed as argument to a greyscale(turning each pixel channel to the average of all its channels values).
void toGrey(cv::Mat imgn);



//These two function are mentioned to be called in  setMouseCallback():
//getCursor() is the responsable to track correctly the mouse position and translate that to the rectparam object in order to use them correctly in the future.
//draw() is the function meant to draw on screen a rectangle, its called inside getCursor() to correctly trigger it when the first corner of the rectangle is clicked.
void getCursor(int event, int x, int y, int flags, void* param);
void draw(int event, int x, int y, int flags, void* param);


//Declaration of the class "rectparams" which contains all parametres that will be used to measure the rectangle we want to higlight 
//(i'm creating a class instead of using data from the window pixel position in order to have more control over my selection, and i get it from having a flag attached to the rectangle and postion x1 starting as an unreacheable position. More control could be added if its needed without changing the entire program logic).
class rectparams{
private:
	int x1,y1,x2,y2;
	bool flag;
public:
	inline void setx1(int newx1){x1=newx1;}
	inline void sety1(int newy1){y1=newy1;}
	inline void setx2(int newx2){x2=newx2;}
	inline void sety2(int newy2){y2=newy2;}

	inline int getx1(){return x1;}
	inline int gety1(){return y1;}
	inline int getx2(){return x2;}
	inline int gety2(){return y2;}

	inline void setflag(bool newflag){flag=newflag;}
	inline bool getflag(){return flag;}

	rectparams(){
		x1=-1;
	}
};

//filter()= receives 2 images as arguments and 1 object from the rectparam class, this function swap images' pixels inside the rectangle given by the rectparam object.
void filter(cv::Mat global,cv::Mat sust,rectparams* param);


int main(int argc, char** argv){

	

	//Error management

	if(argc!=2) {
		cerr<<"Error finding the path to the image, for more info open README.txt"<<endl;return 0;
	}
	//Image reading
	image=cv::imread(argv[1]);

   	if( image.rows==0) {
   		cerr<<"Error reading image"<<endl;return 0;
   	}

   	//Code that will be repeated everytime the key 'ESC' is pressed.

   	do{
   		//copies the image given and create a clone which is turned all grey
   		image.copyTo(cloone);
   		toGrey(cloone);
   		//create an rectparams object with the constructor which makes x1=-1
	   	rectparams* paramete=new rectparams;

	   	cv::namedWindow("Foreground higlight");
	   	cv::imshow("Foreground higlight",image);
	   	cv::setMouseCallback( "Foreground higlight", getCursor,(void*)paramete);
			

	}while(cv::waitKey(0)==27);
}


void draw(int event, int x, int y, int flags, void* param){


	rectparams* usingparam=(rectparams *) param;
	cv::Mat copia2;


	if((usingparam->getflag()==false)){

		//copies the original image and constantly draw a rectangle from the first corner to the current mouse position giving the impression of a interactive reshape of the rectangle.
		image.copyTo(copia2);
		p1=cv::Point(usingparam->getx1(),usingparam->gety1());
		p2=cv::Point(x,y);
		rectangle(copia2,p1,p2,cv::Scalar(100,0,200),2);
		cv::imshow("Foreground higlight",copia2);

	if((event==cv::EVENT_LBUTTONDOWN)&&(usingparam->getflag()==false)){
			usingparam->setx2(x);
			usingparam->sety2(y);
			usingparam->setflag(true);
			filter(cloone,image,usingparam)	;
			p1=cv::Point(usingparam->getx1(),usingparam->gety1());
			p2=cv::Point(usingparam->getx2(),usingparam->gety2());
			//rectangle(cloone,p1,p2,cv::Scalar(100,0,200),2);
			//cv::imshow("Foreground higlight",cloone);
			usingparam->setflag(true);
			}
	}

}

void getCursor(int event, int x, int y, int flags, void* param){

	rectparams* usingparam= (rectparams *) param;

	if((event==cv::EVENT_LBUTTONDOWN)&&(usingparam->getx1()==-1)){
		usingparam->setx1(x);
		usingparam->sety1(y);
		usingparam->setflag(false);
		cv::setMouseCallback( "Foreground higlight", draw ,(void*)usingparam);
		}
	
}


void filter(cv::Mat global,cv::Mat sust,rectparams* param){
	int intercambio;
	
	//takes the position of the clicked corners and swap them if its needed so it will always work with a matrix positive in (+x) and (-y) directions.

	if(param->gety2()<=param->gety1()){
		intercambio=param->gety2();
		param->sety2(param->gety1());
		param->sety1(intercambio);
	}

	if(param->getx2()<=param->getx1()){
		intercambio=param->getx2();
		param->setx2(param->getx1());
		param->setx1(intercambio);

	}

	for(int y=param->gety1();y<param->gety2();y++){
		uchar *ptrg=global.ptr<uchar>(y);
		uchar *ptrs=sust.ptr<uchar>(y);
		ptrg+=param->getx1()*3;
		ptrs+=param->getx1()*3;
		for(int x=param->getx1();x<param->getx2();x++){
			ptrg[0]=ptrs[0];
			ptrg[1]=ptrs[1];
			ptrg[2]=ptrs[2];
			ptrs+=3;
			ptrg+=3;
		}
	}
	//create a new window with the result and shows the final image once higlighted.
	cv::namedWindow("Result");
	cv::imshow("Result",global);
}

void toGrey(cv::Mat imgn){

	//turns all the pixel's channels to the average of them. 
	for(int y=0;y<imgn.rows;y++){
		uchar *ptr=imgn.ptr<uchar>(y);
  		for(int x=0;x<imgn.cols;x++){
	  	ptr[0]=ptr[1]=ptr[2]=((ptr[0]+ptr[1]+ptr[2])/3);
		ptr+=3;
  		}
 	}

}
