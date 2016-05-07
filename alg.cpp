#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray; Mat src_gray_cp; Mat opIm; Mat element; Mat threshold_output; Mat overlay;

//Threshold Characteristics
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

//Opening Characteristics
int morph_elem = 0;
int morph_size = 5;
int operation = 2;
bool* _x1;
bool* _y1;
bool* _x2;
bool* _y2;
bool* _dp;
//int const max_elem = 2;
//int const max_kernel_size = 21;
char* window_name = "Opening";

//Function Declarations
void thresh_contour(int, void*);
void opening(int, void*);
void intToBin(bool*, int);



////////////////////////////////BEGIN MAIN/////////////////////////////
int main()
{
//Coordinate Initialization
_x1 = (bool*) calloc (10,sizeof(bool));
_y1 = (bool*) calloc (10,sizeof(bool));
_x2 = (bool*) calloc (10,sizeof(bool));
_y2 = (bool*) calloc (10,sizeof(bool));
_dp = (bool*) calloc (10,sizeof(bool));
Mat src = imread("/home/bc/Desktop/disparity.png", 1);
cvtColor( src, src_gray, CV_BGR2GRAY );

src_gray_cp= src_gray.clone();

char* source_window = "Source";
namedWindow( source_window, CV_WINDOW_AUTOSIZE);
imshow( source_window, src);

char* blur_window = "Blur";

//createTrackbar (" Threshold:", "Source", &thresh, max_thresh, thresh_contour ); //Trackbar for testing threshold setting
//createTrackbar( "Element:\n 0:Rect | 1:Cross | 2:Ellipse","Opening",&morph_elem, max_elem, opening);
//createTrackbar( "Kernel size:\n 2n+1", "Opening", &morph_size, max_kernel_size, opening );

opening(0,0);
thresh_contour(0,0);
//imshow(window_name, overlay);
waitKey(0);

return(0);
}


//////////////////BEGIN FUNCTION///////////////////
void thresh_contour(int, void*)
{
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;

//Detect Edges Using Threshold
threshold( opIm, threshold_output, thresh, 255, THRESH_BINARY );
blur(threshold_output, threshold_output, Size(10,10));
//Find Contours
findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0) );

//Approximate contours to polygons and get bouding rectangles
vector<vector<Point> > contours_poly( contours.size() );
vector<Rect> boundRect ( contours.size() );

for( int ii = 0; ii < contours.size(); ii++)
{
approxPolyDP( Mat (contours[ii]), contours_poly[ii], 3, true);
boundRect[ii] = boundingRect( Mat(contours_poly[ii]) );
}

Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3);

//Coordinate Array Initializations
int xtl [(contours.size()-1)];
int ytl [(contours.size()-1)];
int xbr [(contours.size()-1)];
int ybr [(contours.size()-1)];
int xc  [(contours.size()-1)];
int yc  [(contours.size()-1)];
int depth [(contours.size()-1)];

for( int ii = 0; ii < contours.size(); ii++)
{
Scalar color = Scalar( rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255) );
drawContours( drawing, contours_poly, ii, color, 1, 8, vector<Vec4i>(), 0, Point() );
rectangle( drawing, boundRect[ii].tl(), boundRect[ii].br(), color, 2, 8, 0);
rectangle(src_gray, boundRect[ii].tl(), boundRect[ii].br(), color, 2,8,0);

//Saving Coordinates in Array
xtl[ii] = boundRect[ii].x;
ytl[ii] = boundRect[ii].y;
xbr[ii] = boundRect[ii].x + boundRect[ii].width;
ybr[ii] = boundRect[ii].y + boundRect[ii].height;
xc[ii]  = boundRect[ii].x + (boundRect[ii].width >> 1);
yc[ii]  = boundRect[ii].y + (boundRect[ii].height >> 1);

depth[ii] = src_gray_cp.at<uchar>(xc[ii],yc[ii]); 

cout << xc[ii];
cout << "\t";
cout << yc[ii];
cout << "\t";
cout << depth[ii];
cout << "\n";
}

//Converting to BitValues
for(int ii = 0; ii <contours.size(); ii++)
{

intToBin(_x1,(xtl[ii]));
intToBin(_y1,(ytl[ii]));
intToBin(_x2,(xbr[ii]));
intToBin(_y2,(ybr[ii]));
intToBin(_dp,(depth[ii]));
	cout << xtl[ii] << '\n';
	for (int jj = 10; jj >= 0; jj--) 
		{
			cout << _x1[jj];
		}
		cout << '\n';
	cout << ytl[ii] << '\n';
	for (int jj = 10; jj >= 0; jj--) 
		{
			cout << _y1[jj];
		}
		cout << '\n';
	cout << xbr[ii] << '\n';
	for (int jj = 10; jj >= 0; jj--) 
		{
			cout << _x2[jj];
		}
		cout << '\n';
	cout << ybr[ii] << '\n';
	for (int jj = 10; jj >= 0; jj--) 
		{
			cout << _y2[jj];
		}
		cout << '\n';
	cout << depth[ii] << '\n';
		 for (int jj = 10; jj >= 0; jj--) 
		{
			cout << _dp[jj];
		}
	cout << '\n';
}


//cout << yOne;
cout << "\n";
cout << "End of Binary";
//cout << x2 << '\n' << y2 << '\n';
//cout << dp << '\n' << '\n';

//Show in window
namedWindow("Contours", CV_WINDOW_AUTOSIZE);
imshow("Contours",drawing);
namedWindow("Overlay", CV_WINDOW_AUTOSIZE);
imshow("Overlay", src_gray);
}

/////////////////////////BEGIN FUNCTION//////////////////////////////
void opening (int, void*)
{

element = getStructuringElement(morph_elem, Size(2*morph_size+1,2*morph_size+1), Point(morph_size,morph_size));

morphologyEx( src_gray, opIm, operation, element);
//imshow( window_name, opIm);
}

void intToBin(bool* bin, int num)
{
	cout << "In Function\n\n";
	for(int ii = 9; ii >= 0; ii--)
	{
		if(num >= ((pow(2,ii))))
		{
			bin[ii] = true;
			num = num - (pow(2,ii));
		}
		else
			bin[ii] = false;
	}
}