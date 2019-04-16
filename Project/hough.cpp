#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <iostream>
#include <conio.h>

using namespace std;
using namespace cv;

void HoughTransformation()
{

	Mat image, source;

	VideoCapture cap(0);

	namedWindow("source", CV_WINDOW_AUTOSIZE);
	namedWindow("params", CV_WINDOW_AUTOSIZE);
	namedWindow("processed", CV_WINDOW_AUTOSIZE);

	int threshold = 50;
	int minLineLength = 50;
	int maxLineGap = 10;

	createTrackbar("threshold", "params", &threshold, 255);
	createTrackbar("minLineLength", "params", &minLineLength, 255);
	createTrackbar("maxLineGap", "params", &maxLineGap, 255);

	int k = 0;
	if (cap.isOpened())
	{
		try
		{
			while (cap.read(source) && k != 27)
			{
				imshow("source", source);
				Canny(source, image, 50, 200);

				vector<Vec4i> lines;
				HoughLinesP(image, lines, 1, CV_PI / 180, threshold, minLineLength, maxLineGap);

				image = source.clone();

				for (int i = 0; i < lines.size(); i++)
				{
					Vec4i l = lines[i];
					line(image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 255, 0), 3, 4);
				}

				imshow("processed", image);

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
	destroyAllWindows();



}