//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include<opencv2/opencv.hpp>
//#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
//#include <vector>
//using namespace cv;
//using namespace std;
//
//
//
///** @function main */
//int main(int argc, char** argv)
//{
//
//	double camD[9] = { 618.526381968738, 0, 310.8963715614199,
//		0, 619.4548980786033, 248.6374860176724,
//		0, 0, 1 };
//	double distCoeffD[5] = { 0.09367405350511771, -0.08731677320554751, 0.002823563134787144, -1.246739177460954e-005, -0.0469061739387372 };
//	Mat camera_matrix = Mat(3, 3, CV_64FC1, camD);
//	Mat distortion_coefficients = Mat(5, 1, CV_64FC1, distCoeffD);
//
//	cv::Mat image_color = cv::imread("..\\..\\data\\color2\\4.png", cv::IMREAD_COLOR);
//
//	cv::Mat image_gray;
//	cv::cvtColor(image_color, image_gray, cv::COLOR_BGR2GRAY);
//
//	std::vector<cv::Point2f> corners, corner_t;
//
//	bool ret = cv::findChessboardCorners(image_gray,
//		cv::Size(6, 9),
//		corners,
//		cv::CALIB_CB_ADAPTIVE_THRESH |
//		cv::CALIB_CB_NORMALIZE_IMAGE);
//
//
//	//指定亚像素计算迭代标注  
//	cv::TermCriteria criteria = cv::TermCriteria(
//		cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS,
//		40,
//		0.1);
//
//	//亚像素检测  
//	cv::cornerSubPix(image_gray, corners, cv::Size(5, 5), cv::Size(-1, -1), criteria);
//
//	cout << corners << endl;
//
//	//角点绘制  
//	//cv::drawChessboardCorners(image_color, cv::Size(9, 9), corners, ret);
//	for (int i = 0; i < 54; i++)
//	{
//		cv::circle(image_color, corners[i], 5, cv::Scalar(255, 0, 255), 2);
//	}
//
//	vector<Point3f>objP;
//	objP.push_back(Point3f(0, 0, 0));
//	objP.push_back(Point3f(15, 0, 0));
//	objP.push_back(Point3f(10, 10, 0));
//	objP.push_back(Point3f(0, 5, 0));
//	//objP.push_back(Point3f(5, 5, 0));
//	//objP.push_back(Point3f(10, 5, 0));
//
//	Mat objPM;
//	Mat(objP).convertTo(objPM, CV_32F);
//
//	corner_t.push_back(corners[5]);
//	corner_t.push_back(corners[0]);
//	corner_t.push_back(corners[48]);
//	corner_t.push_back(corners[53]);
//	//corner_t.push_back(corners[40]);
//	//corner_t.push_back(corners[44]);
//	for (int i = 0; i < corner_t.size(); i++)
//	{
//		cv::circle(image_color, corner_t[i], 5, cv::Scalar(255, 255, 255), 2);
//	}
//	cv::circle(image_color, corners[0], 5, cv::Scalar(0, 0, 255), 2);
//
//	cv::imshow("chessboard corners", image_color);
//	cv::waitKey(0);
//
//	Mat rvec, tvec, rotM;
//
//	solvePnP(objPM, Mat(corner_t), camera_matrix, distortion_coefficients, rvec, tvec);
//	Rodrigues(rvec, rotM);
//
//	cout << "\nrvec" << rotM << endl;
//	cout << "\ntvec" << tvec << endl;
//	system("pause");
//	return(0);
//}

#include "CalChessboard.h"

int main(){
	double camD1[9] = { 546.49, 0, 319.5,
						0, 546.49, 239.5,
				       0, 0, 1 };
	double distCoeffD1[5] = { /*1.3365431359795218e-001, -8.8064028564220309e-001, 0., 0.,
							  1.8548424360444218e+000*/ -3.7375092947566635e-002, 1.5210414013521094e+000, 0., 0.,
							  - 7.3090578619797810e+000 };
	double camD2[9] = { 532.49, 0, 319.5,
		0, 532.49, 239.5,
		0, 0, 1 };
	double distCoeffD2[5] = { 4.7049745556333566e-002, -1.7143637680137072e-001, 0., 0.,
		-2.0746566883707779e-001 };
	double markerRealSize = 3;

	CalChessboard cal1 = CalChessboard(camD1,distCoeffD1,markerRealSize);
	CalChessboard cal2 = CalChessboard(camD2, distCoeffD2, markerRealSize);
	std::string imgpath, calibFile;
	imgpath = "..\\..\\data\\color1\\4.png";
	calibFile = "..\\..\\data\\cal1.txt";
	cal1.calprocess(imgpath, calibFile, 1);

	imgpath = "..\\..\\data\\color2\\4.png";
	calibFile = "..\\..\\data\\cal2.txt";
	cal2.calprocess(imgpath, calibFile, 2);

	system("pause");
	return 0;
}


/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

/// Global variables
Mat src, src_gray;
int thresh = 200;
int max_thresh = 255;

char* source_window = "Source image";
char* corners_window = "Corners detected";

/// Function header
void cornerHarris_demo(int, void*);

/// @function main
int main(int argc, char** argv)
{
/// Load source image and convert it to gray
src = imread("marker.jpg", 1);
cvtColor(src, src_gray, CV_BGR2GRAY);

/// Create a window and a trackbar
namedWindow(source_window, CV_WINDOW_AUTOSIZE);
createTrackbar("Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo);
imshow(source_window, src);

cornerHarris_demo(0, 0);

waitKey(0);
return(0);
}

/// @function cornerHarris_demo
void cornerHarris_demo(int, void*)
{

Mat dst, dst_norm, dst_norm_scaled;
dst = Mat::zeros(src.size(), CV_32FC1);

/// Detector parameters
int blockSize = 2;
int apertureSize = 3;
double k = 0.04;

/// Detecting corners
cornerHarris(src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT);

/// Normalizing
normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
convertScaleAbs(dst_norm, dst_norm_scaled);

/// Drawing a circle around corners
for (int j = 0; j < dst_norm.rows; j++)
{
for (int i = 0; i < dst_norm.cols; i++)
{
if ((int)dst_norm.at<float>(j, i) > thresh)
{
circle(dst_norm_scaled, Point(i, j), 5, Scalar(255, 0, 255), 2, 8, 0);
}
}
}
/// Showing the result
namedWindow(corners_window, CV_WINDOW_AUTOSIZE);
imshow(corners_window, dst_norm_scaled);
}*/