#pragma once


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <conio.h>
#include <ctime>

#ifndef Tracked_h
#define Tracked_h

using namespace std;
using namespace cv;

class TrackedObject
{
private:

	Point2f point;

public:

	TrackedObject(Point2f point);

	Point2f getPoint();
	int getX();
	int getY();
	
	

};

#endif