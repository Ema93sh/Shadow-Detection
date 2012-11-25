#include <iostream>
#include "ShadowDetector.h"

using namespace std;
using namespace cv;

int main( int argc, const char *argv[] )
{
	if( argc < 2 )
	{
		cout << "Error:\nUsage: shadowDetector filepath" << endl;
		return -1;
	}

	ShadowDetector sd(argv[1], true);
	sd.displayImage();
	sd.removeShadows();
	sd.displayRatioMap();
	waitKey();
	return 0;
}
