#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <iostream>
#include <conio.h>

using namespace std;
using namespace cv;

void TrackObjects();

int main()
{
	TrackObjects();
	return 0;
}

void TrackObjects()
{
	Mat image, source, processed, background;
	VideoCapture cap;
	Ptr<BackgroundSubtractorMOG2> bgsubtractor;
	//cap.open("bike.avi");
	cap.open(0);
	String sourceWindowName = "source";
	String processedWindowName = "processed";
	String movingObjects = "moving";

	int history = 20;
	int nmixtures = 100;

	namedWindow(sourceWindowName, CV_WINDOW_AUTOSIZE);
	namedWindow(processedWindowName, CV_WINDOW_AUTOSIZE);
	namedWindow(movingObjects, CV_WINDOW_AUTOSIZE);
	namedWindow("params", CV_WINDOW_AUTOSIZE);
	bgsubtractor = createBackgroundSubtractorMOG2();

	createTrackbar("history", "params", &history, 500);
	createTrackbar("nmixture", "params", &nmixtures, 100);

	int k = 0;
	if (cap.isOpened())
	{
		try
		{
			while (cap.read(source) && k != 27)
			{
				bgsubtractor->setHistory(history);
				bgsubtractor->setNMixtures(nmixtures);
				bgsubtractor->apply(source, processed);
				bgsubtractor->getBackgroundImage(background);

				imshow(sourceWindowName, background);

				morphologyEx(processed,processed, MORPH_OPEN, getStructuringElement(0, Size(3,3)));

				imshow(movingObjects, processed);
				vector<vector<Point> > contours;
				vector<Vec4i> hierarchy;
				findContours(processed, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

				image = source.clone();

				drawContours(image, contours, -1, Scalar(50, 150, 25));
			

				imshow(processedWindowName, image);
				k = waitKey(15);


			}
		}
		catch (Exception)
		{
			cout << "Kamerka odlaczona" << endl;
			waitKey(0);
		}
	}
	cap.release();
	image.release();
	source.release();
	bgsubtractor.release();
	processed.release();
	destroyAllWindows();
}


