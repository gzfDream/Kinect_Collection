#include <iostream>
#include <OpenNI.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include "OpenNIKinect.h"
using namespace std;
using namespace openni;
using namespace cv;
Array<DeviceInfo> aDeviceList;
int showdevice(){
	// 获取设备信息  

	OpenNI::enumerateDevices(&aDeviceList);

	cout << "电脑上连接着 " << aDeviceList.getSize() << " 个体感设备." << endl;

	for (int i = 0; i < aDeviceList.getSize(); ++i)
	{
		cout << "设备 " << i << endl;
		const DeviceInfo& rDevInfo = aDeviceList[i];
		cout << "设备名： " << rDevInfo.getName() << endl;
		cout << "设备Id： " << rDevInfo.getUsbProductId() << endl;
		cout << "供应商名： " << rDevInfo.getVendor() << endl;
		cout << "供应商Id: " << rDevInfo.getUsbVendorId() << endl;
		cout << "设备URI: " << rDevInfo.getUri() << endl;

	}
	return aDeviceList.getSize();
}

void hMirrorTrans(const Mat &src, Mat &dst)
{
	dst.create(src.rows, src.cols, src.type());

	int rows = src.rows;
	int cols = src.cols;

	switch (src.channels())
	{
	case 1:   //1通道比如深度图像
		const uchar *origal;
		uchar *p;
		for (int i = 0; i < rows; i++){
			origal = src.ptr<uchar>(i);
			p = dst.ptr<uchar>(i);
			for (int j = 0; j < cols; j++){
				p[j] = origal[cols - 1 - j];
			}
		}
		break;
	case 3:   //3通道比如彩色图像
		const Vec3b *origal3;
		Vec3b *p3;
		for (int i = 0; i < rows; i++) {
			origal3 = src.ptr<Vec3b>(i);
			p3 = dst.ptr<Vec3b>(i);
			for (int j = 0; j < cols; j++){
				p3[j] = origal3[cols - 1 - j];
			}
		}
		break;
	default:
		break;
	}

}

//int main()
//{
//	Status rc = STATUS_OK;
//
//	// 初始化OpenNI环境
//	OpenNI::initialize();
//
//	showdevice();
//
//	string rgbfilepath, dfilepath;
//	char sig;
//	printf("press '1'or'2' to go on\n");
//	scanf("%c", &sig);
//
//	if (sig == '1'){
//		rgbfilepath = "..\\..\\..\\data\\color1\\";
//		dfilepath = "..\\..\\..\\data\\depth1\\";
//	}
//	else if (sig == '2'){
//		rgbfilepath = "..\\..\\..\\data\\color2\\";
//		dfilepath = "..\\..\\..\\data\\depth2\\";
//	}
//
//	// 声明并打开Device设备。
//	Device xtion;
//	const char * deviceURL = openni::ANY_DEVICE;  //设备名aDeviceList[1].getUri();
//	rc = xtion.open(deviceURL);
//
//	// 创建深度数据流
//	VideoStream streamDepth;
//	rc = streamDepth.create(xtion, SENSOR_DEPTH);
//	if (rc == STATUS_OK)
//	{
//		// 设置深度图像视频模式
//		VideoMode mModeDepth;
//		// 分辨率大小
//		mModeDepth.setResolution(640, 480);
//		// 每秒30帧
//		mModeDepth.setFps(30);
//		// 像素格式
//		mModeDepth.setPixelFormat(PIXEL_FORMAT_DEPTH_1_MM);
//
//		streamDepth.setVideoMode(mModeDepth);
//
//		// 打开深度数据流
//		rc = streamDepth.start();
//		if (rc != STATUS_OK)
//		{
//			cerr << "无法打开深度数据流：" << OpenNI::getExtendedError() << endl;
//			streamDepth.destroy();
//		}
//	}
//	else
//	{
//		cerr << "无法创建深度数据流：" << OpenNI::getExtendedError() << endl;
//	}
//
//	// 创建彩色图像数据流
//	VideoStream streamColor;
//	rc = streamColor.create(xtion, SENSOR_COLOR);
//	if (rc == STATUS_OK)
//	{
//		// 同样的设置彩色图像视频模式
//		VideoMode mModeColor;
//		mModeColor.setResolution(320, 240);
//		mModeColor.setFps(30);
//		mModeColor.setPixelFormat(PIXEL_FORMAT_RGB888);
//
//		streamColor.setVideoMode(mModeColor);
//
//		// 打开彩色图像数据流
//		rc = streamColor.start();
//		if (rc != STATUS_OK)
//		{
//			cerr << "无法打开彩色图像数据流：" << OpenNI::getExtendedError() << endl;
//			streamColor.destroy();
//		}
//	}
//	else
//	{
//		cerr << "无法创建彩色图像数据流：" << OpenNI::getExtendedError() << endl;
//	}
//
//	if (!streamColor.isValid() || !streamDepth.isValid())
//	{
//		cerr << "彩色或深度数据流不合法" << endl;
//		OpenNI::shutdown();
//		return 1;
//	}
//
//	// 图像模式注册,彩色图与深度图对齐
//	if (xtion.isImageRegistrationModeSupported(
//		IMAGE_REGISTRATION_DEPTH_TO_COLOR))
//	{
//		xtion.setImageRegistrationMode(IMAGE_REGISTRATION_DEPTH_TO_COLOR);
//	}
//
//
//	// 创建OpenCV图像窗口
//	namedWindow("Depth Image", CV_WINDOW_AUTOSIZE);
//	namedWindow("Color Image", CV_WINDOW_AUTOSIZE);
//
//	// 获得最大深度值
//	int iMaxDepth = streamDepth.getMaxPixelValue();
//
//	// 循环读取数据流信息并保存在VideoFrameRef中
//	VideoFrameRef  frameDepth;
//	VideoFrameRef  frameColor;
//	int i = 1;
//	while (true)
//	{
//		// 读取数据流
//		rc = streamDepth.readFrame(&frameDepth);
//		if (rc == STATUS_OK)
//		{
//			// 将深度数据转换成OpenCV格式
//			const Mat mImageDepth(frameDepth.getHeight(), frameDepth.getWidth(), CV_16UC1, (void*)frameDepth.getData());
//		//	imwrite(".\\depth\\"+std::to_string(i)+".png", mImageDepth);
//			// 为了让深度图像显示的更加明显一些，将CV_16UC1 ==> CV_8U格式
//			Mat mScaledDepth;
//			Mat hScaledDepth;
//			//mImageDepth.convertTo(mScaledDepth, CV_8U, 255.0 / iMaxDepth);
//			mImageDepth.convertTo(mScaledDepth, CV_16U, 5.0);
//			//水平镜像深度图
//			flip(mScaledDepth, hScaledDepth,1);
//			// 显示出深度图像
//			imshow("Depth Image", hScaledDepth);
//			imwrite(dfilepath + std::to_string(i) + ".png", hScaledDepth);
//		}
//
//		rc = streamColor.readFrame(&frameColor);
//		if (rc == STATUS_OK)
//		{
//			// 同样的将彩色图像数据转化成OpenCV格式
//			const Mat mImageRGB(frameColor.getHeight(), frameColor.getWidth(), CV_8UC3, (void*)frameColor.getData());
//		//	imwrite(".\\image\\" + std::to_string(i) + ".png", mImageRGB);
//			// 首先将RGB格式转换为BGR格式
//			Mat cImageBGR, bImageBGR, hImageBGR;
//			cvtColor(mImageRGB, cImageBGR, CV_RGB2BGR);
//
//			//水平镜像深度图
//			flip(cImageBGR, hImageBGR,1);
//			resize(hImageBGR, hImageBGR, Size(640, 480));
//			// 然后显示彩色图像
//			imshow("Color Image", hImageBGR);
//			imwrite(rgbfilepath + std::to_string(i) + ".png", hImageBGR);
//		}
//
//		// 终止快捷键
//		if (waitKey(1) == 27)
//			break;
//		i++;
//	}
//
//	// 关闭数据流
//	streamDepth.destroy();
//	streamColor.destroy();
//	// 关闭设备
//	xtion.close();
//	// 最后关闭OpenNI
//	OpenNI::shutdown();
//
//	return 0;
//}

int main()
{
	string rgbfilepath, dfilepath;
	char sig;
	uchar save_or = 0;
	double in1[4] = { 319.5, 239.5, 536.24, 536.24 };
	double in2[4] = { 319.5, 239.5, 532.49, 532.49 };
	
	Mat rgbMat, depthMat;

	string savePath1, savePath2;
	string rgbfilepath1, dfilepath1, rgbfilepath2, dfilepath2;

	OpenNIKinect m_kinect2(in2);
	OpenNIKinect m_kinect1(in1);
	printf("Open the which Kinect? Press '1'or'2' to chose\n");
	scanf_s("%c", &sig);
	printf("Opening the NO.%c Kinect?", &sig);

	if (sig == '1'){
		rgbfilepath = "..\\..\\data\\color1\\";
		dfilepath = "..\\..\\data\\depth1\\";
		m_kinect1.initEngine(sig - '1');
	}
	else if (sig == '2'){
		rgbfilepath = "..\\..\\data\\color2\\";
		dfilepath = "..\\..\\data\\depth2\\";
		m_kinect2.initEngine(sig - '1');
	}

	namedWindow("Depth Image", CV_WINDOW_AUTOSIZE);
	namedWindow("Color Image", CV_WINDOW_AUTOSIZE);

	printf("Save or not ? press [y] or [n] to go on\n");
	save_or = cv::waitKey(0);
	if (save_or == 'y')
	{
		printf("Save rgbd data and synthesis result of point cloud.\n");
	}
	else{
		printf("Don't save rgbd. Only to show.\n");
	}

	int i = 1;
	bool loop = true;
	while (loop)
	{
		if (sig == '1')
		{
			depthMat = m_kinect1.getDepthImage();
			rgbMat = m_kinect1.getRGBImage();
		}
		else if (sig == '2')
		{
			depthMat = m_kinect2.getDepthImage();
			rgbMat = m_kinect2.getRGBImage();
		}
		/*depthMat = m_kinect.getDepthImage();
		rgbMat = m_kinect.getRGBImage();*/

		imshow("Color Image", rgbMat);
		imshow("Depth Image", depthMat);

		if (save_or == 'y')
		{
			imwrite(dfilepath + std::to_string(i) + ".png", depthMat);
			imwrite(rgbfilepath + std::to_string(i) + ".png", rgbMat);
			i++;

			if (i == 6)
			{
				loop = false;
			}
		}

		if (waitKey(1) == 27)
			break;
	}

	if (save_or == 'y'){
		if (sig == '1'){
			rgbfilepath1 = "..\\..\\data\\color1\\3.png";
			dfilepath1 = "..\\..\\data\\depth1\\3.png";
			savePath1 = "..\\..\\data\\pcd1\\1.pcd";
			cout << "start to save pcd..." << endl;
			m_kinect1.PCDsynthesis(rgbfilepath1, dfilepath1, savePath1);
		}
		else if (sig == '2'){
			rgbfilepath2 = "..\\..\\data\\color2\\3.png";
			dfilepath2 = "..\\..\\data\\depth2\\3.png";
			savePath2 = "..\\..\\data\\pcd2\\1.pcd";
			cout << "start to save pcd..." << endl;
			m_kinect2.PCDsynthesis(rgbfilepath2, dfilepath2, savePath2);
		}
	}

	cout << "END" << endl;

	//system("pause");
	return 0;
}

//int main()
//{
//	string rgbfilepath, dfilepath;
//	char sig;
//	uchar save_or = 0;
//
//	printf("Open the which Kinect? Press '1'or'2' to chose\n");
//	scanf_s("%c", &sig);
//	printf("Opening the NO.%c Kinect?", &sig);
//
//	if (sig == '1'){
//		rgbfilepath = "..\\..\\data\\color1\\";
//		dfilepath = "..\\..\\data\\depth1\\";
//	}
//	else if (sig == '2'){
//		rgbfilepath = "..\\..\\data\\color2\\";
//		dfilepath = "..\\..\\data\\depth2\\";
//	}
//
//	double in1[4] = { 319.5, 239.5, 536.24, 536.24};
//	double in2[4] = { 319.5, 239.5, 532.49, 532.49};
//
//	OpenNIKinect m_kinect2(in2), m_kinect1(in1);
//	if (sig == '1')
//	{
//		m_kinect1.initEngine(sig - '1');
//	}
//	else if (sig == '2')
//	{
//		m_kinect2.initEngine(sig - '1');
//	}
//	//m_kinect.initEngine(sig - '1');
//	//m_kinect.initEngine(0);
//	
//	Mat rgbMat, depthMat;
//
//	string savePath1, savePath2;
//	string rgbfilepath1, dfilepath1, rgbfilepath2, dfilepath2;
//
//	namedWindow("Depth Image", CV_WINDOW_AUTOSIZE);
//	namedWindow("Color Image", CV_WINDOW_AUTOSIZE);
//
//	printf("Save or not ? press [y] or [n] to go on\n");
//	save_or = cv::waitKey(0);
//	if (save_or == 'y')
//	{
//		printf("Save rgbd data and synthesis result of point cloud.\n");
//	}
//	else{
//		printf("Don't save rgbd. Only to show.\n");
//	}
//
//	int i = 1;
//	bool loop = true;
//	while (loop)
//	{
//		if (sig == '1')
//		{
//			depthMat = m_kinect1.getDepthImage();
//			rgbMat = m_kinect1.getRGBImage();
//		}
//		else if (sig == '2')
//		{
//			depthMat = m_kinect2.getDepthImage();
//			rgbMat = m_kinect2.getRGBImage();
//		}
//		/*depthMat = m_kinect.getDepthImage();
//		rgbMat = m_kinect.getRGBImage();*/
//
//		imshow("Color Image", rgbMat);
//		imshow("Depth Image", depthMat);
//
//		if (save_or == 'y')
//		{
//			imwrite(dfilepath + std::to_string(i) + ".png", depthMat);
//			imwrite(rgbfilepath + std::to_string(i) + ".png", rgbMat);
//			i++;
//
//			if (i == 6)
//			{
//				loop = false;
//			}
//		}
//
//		if (waitKey(1) == 27)
//			break;
//	}
//
//	if (save_or == 'y'){
//		if (sig == '1'){
//			rgbfilepath1 = "..\\..\\data\\color1\\3.png";
//			dfilepath1 = "..\\..\\data\\depth1\\3.png";
//			savePath1 = "..\\..\\data\\pcd1\\1.pcd";
//			cout << "start to save pcd..." << endl;
//			m_kinect1.PCDsynthesis(rgbfilepath1, dfilepath1, savePath1);
//		}
//		else if (sig == '2'){
//			rgbfilepath2 = "..\\..\\data\\color2\\3.png";
//			dfilepath2 = "..\\..\\data\\depth2\\3.png";
//			savePath2 = "..\\..\\data\\pcd2\\1.pcd";
//			cout << "start to save pcd..." << endl;
//			m_kinect2.PCDsynthesis(rgbfilepath2, dfilepath2, savePath2);
//		}
//	}
//
//	cout << "END" << endl;
//
//	//system("pause");
//	return 0;
//}