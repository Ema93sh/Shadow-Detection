#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class ShadowDetector
{
	private:
		Mat image;
		Mat ratioMap;
		bool debug;
	public:
		ShadowDetector( const char *path, bool d = false )
		{
			image = imread( path );
			if( image.data == 0 )
			{
				cerr << "Image data not found" << endl;
				exit(0);
			}
			debug = d;
		}

		void removeShadows();
		double intensityEquivalent(int Red, int Green, int Blue ); // gives the intesity equivalent bounded by [0,255]
		double hueEquivalent(int Red, int Green, int Blue ); // gives the hue equivalent bounded by [0,255]
		void generateRatioMap();
		void displayRatioMap();
		void displayImage();
};
