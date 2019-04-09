#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include <iostream>
#include <conio.h>
#include <ctime>

#include "TrackedObject.h"

using namespace std;
using namespace cv;

void ProcessOneWithTrackBars();

void MultiplePages();

int main()
{
	//MultiplePages();
	ProcessOneWithTrackBars();
	return 0;
}

void MultiplePages() {

	Scalar GREEN_MIN(34, 65, 75);
	Scalar GREEN_MAX(82, 253, 184);

	Scalar YELLOW_MIN(70, 21, 134);
	Scalar YELLOW_MAX(107, 130, 254);

	Scalar BLUE_MIN(0, 144, 45);
	Scalar BLUE_MAX(20, 318, 194);


	int currentColor = 0;

	Mat source;
	Mat image;
	Mat processedImage;
	Mat drawing;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	int h_min = 30, h_max = 82;
	int s_min = 65, s_max = 253;
	int v_min = 35, v_max = 154;


	namedWindow("window", CV_WINDOW_AUTOSIZE);
	namedWindow("Parameters", CV_WINDOW_AUTOSIZE);
	VideoCapture cap(0);

	createTrackbar("h min", "Parameters", &h_min, 180);
	createTrackbar("h max", "Parameters", &h_max, 180);
	createTrackbar("s min", "Parameters", &s_min, 500);
	createTrackbar("s max", "Parameters", &s_max, 500);
	createTrackbar("v min", "Parameters", &v_min, 500);
	createTrackbar("v max", "Parameters", &v_max, 500);

	clock_t start = std::clock();
	double time = 0;

	int pressedKey = 0;
	if (cap.isOpened())
	{
		try
		{
			while (cap.read(source) && pressedKey != 27)
			{
				cvtColor(source, image, CV_RGB2HSV);
				switch (currentColor) {
				case 0:
					inRange(image, GREEN_MIN, GREEN_MAX, processedImage);
					break;
				case 1:
					inRange(image, BLUE_MIN, BLUE_MAX, processedImage);
					break;
				case 2:
					inRange(image, YELLOW_MIN, YELLOW_MAX, processedImage);
					break;
				default: break;

				}
				



				erode(processedImage, processedImage, getStructuringElement(MORPH_RECT, Size(3, 3)));
				erode(processedImage, processedImage, getStructuringElement(MORPH_RECT, Size(3, 3)));
				dilate(processedImage, processedImage, getStructuringElement(MORPH_RECT, Size(3, 3)));
				dilate(processedImage, processedImage, getStructuringElement(MORPH_RECT, Size(3, 3)));

				findContours(processedImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

				if (contours.size() > 0)
				{
					int biggest = 0;

					vector<Moments> mu(contours.size());
					for (int i = 0; i < contours.size(); i++)
					{
						mu[i] = moments(contours[i], false);
						if (contourArea(contours[i]) > contourArea(contours[biggest]))
						{
							biggest = i;
						}
					}

					Point2f mc;

					mc = Point2f(mu[biggest].m10 / mu[biggest].m00, mu[biggest].m01 / mu[biggest].m00);

					Scalar color = Scalar(50, 150, 25);
					circle(source, mc, 4, color, -1, 8, 0);
					string text = "(" + std::to_string(mc.x) + ", " + std::to_string(mc.y) + ")";
					putText(source, text, mc, FONT_HERSHEY_COMPLEX, 0.5, color);

				}

				time = (std::clock() - start) / (double) CLOCKS_PER_SEC;
				cout << time << endl;
				if (time >= 2)
				{
					// siwtch colors
					currentColor++;
					if (currentColor > 2)
					{
						currentColor = 0;
					}
					time = 0;
					start = std::clock();
				}


				imshow("window", processedImage);
				imshow("source", source);
				pressedKey = waitKey(15);
			}
		}
		catch (Exception)
		{
			cout << "Kamerka odlaczona" << endl;
			waitKey(0);
		}
	}

	source.release();
	image.release();
	processedImage.release();
	cap.release();

}

void ProcessOneWithTrackBars()
{
	Mat source;
	Mat image;
	Mat processedImage;
	Mat drawing;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	int h_min = 30, h_max = 82;
	int s_min = 65, s_max = 253;
	int v_min = 35, v_max = 154;


	namedWindow("window", CV_WINDOW_AUTOSIZE);
	namedWindow("Parameters", CV_WINDOW_AUTOSIZE);
	VideoCapture cap(0);

	createTrackbar("h min", "Parameters", &h_min, 180);
	createTrackbar("h max", "Parameters", &h_max, 180);
	createTrackbar("s min", "Parameters", &s_min, 500);
	createTrackbar("s max", "Parameters", &s_max, 500);
	createTrackbar("v min", "Parameters", &v_min, 500);
	createTrackbar("v max", "Parameters", &v_max, 500);


	int pressedKey = 0;
	if (cap.isOpened())
	{
		try
		{
			while (cap.read(source) && pressedKey != 27)
			{
				cvtColor(source, image, CV_RGB2HSV);
				inRange(image, Scalar(h_min, s_min, v_min), Scalar(h_max, s_max, v_max), processedImage);

				erode(processedImage, processedImage, getStructuringElement(MORPH_RECT, Size(3, 3)));
				erode(processedImage, processedImage, getStructuringElement(MORPH_RECT, Size(3, 3)));
				dilate(processedImage, processedImage, getStructuringElement(MORPH_RECT, Size(3, 3)));
				dilate(processedImage, processedImage, getStructuringElement(MORPH_RECT, Size(3, 3)));

				findContours(processedImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

				if (contours.size() > 0)
				{

					vector<Moments> mu;
					for (int i = 0; i < contours.size(); i++)
					{
						if (contourArea(contours[i]) > 1000)
						{
							mu.push_back(moments(contours[i], false));
						}
					}
					

					vector<TrackedObject> trackedObjects;

					for (int i = 0; i < mu.size(); i++)
					{
						trackedObjects.push_back(TrackedObject(Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00)));
					}

					Scalar color = Scalar(50, 150, 25);
					for (int i = 0; i < mu.size(); i++)
					{
						circle(source, trackedObjects[i].getPoint(), 4, color, -1, 8, 0);
						string text = "(" + std::to_string((int)trackedObjects[i].getX()) + ", " + std::to_string((int)trackedObjects[i].getY()) + ")";
						putText(source, text, trackedObjects[i].getPoint(), FONT_HERSHEY_COMPLEX, 0.5, color);
					}
					
					

				}


				imshow("window", processedImage);
				imshow("source", source);
				pressedKey = waitKey(15);
			}
		}
		catch (Exception)
		{
			cout << "Kamerka odlaczona" << endl;
			waitKey(0);
		}
	}

	source.release();
	image.release();
	processedImage.release();
	cap.release();


}