#include "ShadowDetector.h"
#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

#define PI 3.14

using namespace std;
using namespace cv;

void ShadowDetector::removeShadows()
{
}

void ShadowDetector::displayRatioMap()
{
	namedWindow("RatioMap");
	imshow("RatioMap", ratioMap);
}

void ShadowDetector::displayImage()
{
	namedWindow("Image");
	imshow("Image", image);
}

double ShadowDetector::intensityEquivalent( int Red, int Green, int Blue )
{
	double v = (Red+Green+Blue)/3;
	if( v > 255 ) v = 255;
	if( v < 0 ) v = 0;
	return v;
}

double ShadowDetector::hueEquivalent( int Red, int Green, int Blue )
{
	double v1, v2;
	double temp = sqrt(6);
	v1 = ((-temp/6) * Red) + ((-temp/6) * Green) + ((temp/3)*Blue);
	v2 = ((1/temp) * Red) + ((-2/temp) * Green );
	double v = ( atan(v2/v1) + PI ) * ( 255/ (2*PI) );
	if( v > 255 ) v = 255;
	if( v < 0 ) v = 0;
	return v;
}

void ShadowDetector::generateRatioMap()
{
	Mat rValue(image.rows, image.cols, CV_8UC1);

	for( int i = 0; i < image.rows; i++ )
	{
		for( int j = 0; j < image.cols; j++ )
		{
			int red = image.at<Vec3b>(i,j)[2];
			int green = image.at<Vec3b>(i,j)[1];
			int blue = image.at<Vec3b>(i,j)[0];

			double iValue = intensityEquivalent( red, green, blue );
			double hValue = hueEquivalent( red, green, blue );
			int r = round( hValue / (iValue+1) );

			if( r > 255 ) r = 255;
			if( r < 0 ) r = 0;
			rValue.at<uchar>(i, j) = r;
		}
	}

	if( debug )
	{
		namedWindow("rValue");
		imshow("rValue", rValue);
	}

	// find the probability of r value
	int channels[1];
	int histSize[1];
	float hranges[2];
	const float* ranges[1];
	histSize[0] = 256;
	hranges[0] = 0.0;
	hranges[1] = 255.0;
	ranges[0] = hranges;
	channels[0] = 0;
	MatND hist;
	calcHist(&rValue,
			1,  //histogram from 1 image only
			channels, // the channel used
			Mat(), // no mask used
			hist,
			1, // 1D histogram
			histSize, // number of bins
			ranges
			);
	
	if( debug )
	{
		// Get min and max bin values
		double maxVal=0;
		double minVal=0;
		minMaxLoc(hist, &minVal, &maxVal, 0, 0);
		// Image on which to display histogram
		cv::Mat histImg(histSize[0], histSize[0], CV_8U,cv::Scalar(255));
		// set highest point at 90% of nbins
		int hpt = static_cast<int>(0.9*histSize[0]);
		// Draw a vertical line for each bin
		for( int h = 0; h < histSize[0]; h++ ) {
			float binVal = hist.at<float>(h);
			int intensity = static_cast<int>(binVal*hpt/maxVal);
			// This function draws a line between 2 points
			cv::line(histImg, Point(h,histSize[0]), Point(h,histSize[0]-intensity), Scalar::all(0));
		}
		namedWindow("histogram");
		imshow("histogram", histImg);
	}
	
	normalize( hist,  hist, 1.0 );
	
	//get the Ts value
	float Ps = 0.95;
	float Pi = 0;
	int Ts;
	for( Ts=0; Pi < Ps && Ts < 256; Ts++ )
	{
		Pi += hist.at<float>(Ts);
	}
	
	if( debug )
	{
		cout << "Ts =" << Ts << endl;
	}
	// calculate sigma value
	float sigma=0;
	for( int i = 0; i < Ts - 1; i++)
	{
		sigma += hist.at<float>(i) * ( i - Ts ) * ( i - Ts );
	}

	sigma = sqrt(sigma);

	if( debug )
		cout << "Sigma =" << sigma << endl;

	ratioMap = Mat( image.rows, image.cols, CV_8UC1 );

	//sigma = 20; Ts = 200;i
	//sigma = 10;
	for( int i=0; i < image.rows; i++ )
	{
		for( int j=0; j< image.cols; j++ )
		{
			int ratio = 1000;
			if( rValue.at<uchar>(i,j) < Ts )
			{
				double temp = rValue.at<uchar>(i,j) - Ts;
				temp *= temp;
				temp /= ( 4 * sigma * sigma );
				temp = exp(-temp);
				ratio = (int)(temp * 255);

			}
			else
			{
				ratio = 255;
			}
			if( ratio == 1000 )
			{
				cout << "t";
				exit(0);
			}
			ratioMap.at<uchar>(i,j) = ratio;
		}
	}
}

