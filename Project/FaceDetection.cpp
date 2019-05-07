#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <conio.h>

using namespace std;
using namespace cv;

void detectFaceWithCircles();


int main()
{
	detectFaceWithCircles();

	return 0;
}

void detectFaceWithCircles()
{
	Mat image, source;

	VideoCapture cap;
	cap.open(0);
	namedWindow("window", CV_WINDOW_AUTOSIZE);
	namedWindow("params", CV_WINDOW_AUTOSIZE);
	int minNeighbors = 2, minSize = 15;
	float scaleFactor = 1.1;
	int scaleInt = 11;
	createTrackbar("scale factor", "params", &scaleInt, 200);
	createTrackbar("min Neigh", "params", &minNeighbors, 10);
	createTrackbar("min Size", "params", &minSize, 100);
	CascadeClassifier cas, eyesCas;

	cas.load("C:/opencv/build/etc/haarcascades/haarcascade_frontalface_alt.xml");
	eyesCas.load("C:/opencv/build/etc/haarcascades/haarcascade_eye_tree_eyeglasses.xml");

	int mode = 1;
	int pressedKey = 0;
	if (cap.isOpened())
	{
		try
		{
			while (cap.read(source) && mode != -1)
			{
				scaleFactor = (float) scaleInt / 100;

				if (scaleFactor < 1.01)
					scaleFactor = 1.1;
				if (minNeighbors == 0)
					minNeighbors = 1;
				image = source.clone();

				vector<Rect> faces;
				cas.detectMultiScale(image, faces, scaleFactor, minNeighbors, 0, Size(minSize, minSize));
			
				//Circles
				if (mode == 1)
				{ 
					for (size_t i = 0; i < faces.size(); i++)
					{
						Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
						circle(image, center, (faces[i].width + faces[i].height)*0.25, Scalar(255, 255, 255),2);

						vector<Rect> eyes;

						eyesCas.detectMultiScale(image(faces[i]), eyes);

						for (size_t j = 0; j < eyes.size(); j++)
						{
							Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
							int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
							circle(image, center, radius, Scalar(255, 255, 0), 2);
						}
					}
				} else if (mode == 2)
				{ 
					//Gauss Blur
					for (size_t i = 0; i < faces.size(); i++)
					{
						Rect r = faces[i];
						blur(image(r), image(r), Size(17, 17));
					
					}
				}
				else if (mode == 3)
				{
					//Black rectangle
					for (size_t i = 0; i < faces.size(); i++)
					{
						Point center;
						center.x = faces[i].x;
						center.y = 0;
						vector<Rect> eyes;

						eyesCas.detectMultiScale(image(faces[i]), eyes, scaleFactor, minNeighbors, 0, Size(minSize, minSize));

						int rectHeight = 0;

						for (size_t j = 0; j < eyes.size(); j++)
						{
							rectHeight = eyes[j].height;
							center.y = faces[i].y + eyes[j].y;
						}
						rectangle(image, Rect(center.x, center.y, faces[i].width, rectHeight),Scalar(0,0,0), CV_FILLED);
					}
				}
				else if (mode == 4)
				{
					//Copying Face
					Mat face;
					for (size_t i = 0; i < faces.size(); i++)
					{
						Rect r = faces[i];
						if (!face.empty())
						{
							resize(face, face, faces[i].size());

							face.copyTo(image(faces[i]));

						}
						face = image(faces[i]);
					}
					face.release();
				}
				imshow("window", image);
				pressedKey = waitKey(15);
				switch (pressedKey)
				{
				case '0':
					mode = 0;
					break;
				case '1':
					mode = 1;
					break;
				case '2':
					mode = 2;
					break;
				case '3':
					mode = 3;
					break;
				case '4':
					mode = 4;
					break;
				case 27:
					mode = -1;
					break;
				default:
					break;
				}

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




}
