#include <opencv2/opencv.hpp>
using namespace cv;
class CamShiftTracking
{
private:
	Mat hist;
	Mat mask;
	int hsize;
    float hranges[2];
public:

	CamShiftTracking()
	{

	}
	CamShiftTracking(Mat image,int vmin,int vmax,int smin, int smax,Rect selection)
	{
		Mat hsv, hue;
		getHue(image,hsv,hue);
        inRange(hsv, Scalar(0, smin, vmin),Scalar(180, smax, vmax), mask);        
        Mat roi(hue, selection), maskroi(mask, selection);
        hsize = 16;
    	hranges[0] = 0;
		hranges[1] = 180;
		const float* phranges = hranges;
        calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
        normalize(hist, hist, 0, 255, CV_MINMAX);
	}
	CamShiftTracking(Mat image,int vmin,int vmax,int smin, int smax,Rect selection,int channel)
	{
		Mat hsv, hue;
		getHue(image,hsv,hue,channel);
        inRange(hsv, Scalar(0, smin, vmin),Scalar(180, smax, vmax), mask);        
        Mat roi(hue, selection), maskroi(mask, selection);
        hsize = 16;
    	hranges[0] = 0;
		hranges[1] = 255;
		const float* phranges = hranges;
        calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
        normalize(hist, hist, 0, 255, CV_MINMAX);
	}
	Rect track(Mat image,Rect &bb,int channel)
	{
		Mat hue;
		getHue(image,hue,channel);
		Mat backproj;
		const float* phranges = hranges;
		calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
        backproj &= mask;
        RotatedRect trackBox = CamShift(backproj, bb,
                                    TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));
        return trackBox.boundingRect();

	}
	Rect track(Mat image,Rect &bb)
	{
		Mat hue;
		getHue(image,hue);
		Mat backproj;
		const float* phranges = hranges;
		calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
        backproj &= mask;
        RotatedRect trackBox = CamShift(backproj, bb,
                                    TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));
        return trackBox.boundingRect();

	}
	Rect trackPosition(Mat image,Rect &bb)
	{
		Mat hue;
		getHue(image,hue);
		Mat backproj;
		const float* phranges = hranges;
		calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
        backproj &= mask;
        meanShift(backproj, bb,
                                    TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));
        return bb;

	}
	Rect trackPosition(Mat image,Rect &bb,int channel)
	{
		Mat hue;
		getHue(image,hue,channel);
		Mat backproj;
		const float* phranges = hranges;
		calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
        backproj &= mask;
        meanShift(backproj, bb,
                                    TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));
        return bb;

	}
	void getHue(const Mat &img,Mat &hsv, Mat &hue)
	{
		cvtColor(img, hsv, COLOR_BGR2HSV);
		int ch[] = {0, 0};
        hue.create(hsv.size(), hsv.depth());//h 0-180 s v 0-255
        mixChannels(&hsv, 1, &hue, 1, ch, 1);
	}

	void getHue(const Mat &img, Mat &hue)
	{
		Mat hsv;
		cvtColor(img, hsv, COLOR_BGR2HSV);
		int ch[] = {0, 0};
        hue.create(hsv.size(), hsv.depth());//h 0-180 s v 0-255
        mixChannels(&hsv, 1, &hue, 1, ch, 1);
	}
	void getHue(const Mat &img,Mat &hsv, Mat &hue,int channel)
	{
		cvtColor(img, hsv, COLOR_BGR2HSV);
		int ch[] = {channel, 0};
        hue.create(hsv.size(), hsv.depth());//h 0-180 s v 0-255
        mixChannels(&hsv, 1, &hue, 1, ch, 1);
	}
	
	void getHue(const Mat &img, Mat &hue,int channel)
	{
		Mat hsv;
		cvtColor(img, hsv, COLOR_BGR2HSV);
		int ch[] = {channel, 0};
        hue.create(hsv.size(), hsv.depth());//h 0-180 s v 0-255
        mixChannels(&hsv, 1, &hue, 1, ch, 1);
	}

};