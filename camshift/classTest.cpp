#include "camShiftTracking.h"
#include "getInitialRect.h"
int main()
{
	VideoCapture video(-1);
	Mat img;
	// select region to track by mouse click
	while(1)
	{
		video>>img;
		imshow("GetInitialRect",img);
		char c= waitKey(1000);
		if(c=='i')
			break;
	}
	InitialRect intialRect=InitialRect();
	Rect toTrack= intialRect.getInitialRect(img);
	//initial the trackor
	CamShiftTracking trackor(img,0,255,0,255,toTrack);
	//track
	while(1)
	{
		video>>img;
		Rect bb=trackor.track(img,toTrack);
		rectangle( img, bb, Scalar(0,0,255), 3, CV_AA );
		imshow("Img",img);
		char c= waitKey(10);
		if(c=='q')
			break;

	}
	return 0;
}