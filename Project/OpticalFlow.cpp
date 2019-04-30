#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include<iostream>
#include<conio.h>

using namespace std;
using namespace cv;

void Track(int choice);



int main()
{
	cout << "1 - bike.avi" << endl;
	cout << "2 - kamerka" << endl;
	int choice = 0;
	cin >> choice;
	Track(choice);

}

void Track(int choice)
{
	Mat source, source_gray, image, linesImage, processed;


	VideoCapture cap;
	if (choice == 1)
	cap.open("bike.avi");
	else cap.open(0);
	vector<Point2f> source_points;
	int maxCorners = 50;
	double qualityLevel = 0.01;
	int minDistance = 10;

	cap.read(source);
	cvtColor(source, source_gray, CV_RGB2GRAY);

	goodFeaturesToTrack(source_gray, source_points, maxCorners, qualityLevel, minDistance);

	namedWindow("source", CV_WINDOW_AUTOSIZE);
	namedWindow("processed", CV_WINDOW_AUTOSIZE);
	namedWindow("params", 1);
	createTrackbar("maxCorners", "params", &maxCorners, 200);
	createTrackbar("minDistance", "params", &minDistance, 20);

	linesImage = Mat::zeros(Size(source.cols, source.rows), source.type());
	int k = 0;
	if (cap.isOpened())
	{
		try
		{
			while (cap.read(source) || k != 27)
			{
				imshow("source", source);
				processed = source.clone();
				cvtColor(source, image, CV_RGB2GRAY);
				vector<Point2f> points;
				vector<uchar> status;
				vector<float> err;
				calcOpticalFlowPyrLK(source_gray, image, source_points, points, status, err);
				int tmp = 0;
				for (int i = 0; i < points.size(); i++)
				{
					/* Different method of finding difference between points
					if (status[i] && (norm(points[i] - source_points[i]) > 2))
					{
						
							line(linesImage, source_points[i], points[i], Scalar(255, 255, 0));
							points[tmp++] = points[i];
							circle(processed, points[i], 3, Scalar(0, 255, 0), -1, 8);
						
					}
					*/
					if (status[i] && (abs(points[i].x - source_points[i].x) + abs(points[i].y - source_points[i].y)) > 2)
					{

						line(linesImage, source_points[i], points[i], Scalar(255, 255, 0));
						points[tmp++] = points[i];
						circle(processed, points[i], 3, Scalar(0, 255, 0), -1, 8);

					}
				}
				if (tmp == 0) {
					points.clear();
					goodFeaturesToTrack(image, source_points, maxCorners, qualityLevel, minDistance);
					linesImage = Mat::zeros(Size(source.cols, source.rows), source.type());
					cv::swap(source_gray, image);

				}
				else {
					points.resize(tmp);

					for (int i = 0; i < processed.rows; i++)
					{
						for (int j = 0; j < processed.cols; j++)
						{
							if (linesImage.at<Vec3b>(i, j)[1] == 255 )
							{
								processed.at<Vec3b>(i, j)[0] = 255;
								processed.at<Vec3b>(i, j)[1] = 255;
							}
						}
					}


					std::swap(points, source_points);
					cv::swap(source_gray, image);
				}

				imshow("processed", processed);
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

}
