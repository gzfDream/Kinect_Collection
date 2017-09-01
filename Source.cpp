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
	// ��ȡ�豸��Ϣ  

	OpenNI::enumerateDevices(&aDeviceList);

	cout << "������������ " << aDeviceList.getSize() << " ������豸." << endl;

	for (int i = 0; i < aDeviceList.getSize(); ++i)
	{
		cout << "�豸 " << i << endl;
		const DeviceInfo& rDevInfo = aDeviceList[i];
		cout << "�豸���� " << rDevInfo.getName() << endl;
		cout << "�豸Id�� " << rDevInfo.getUsbProductId() << endl;
		cout << "��Ӧ������ " << rDevInfo.getVendor() << endl;
		cout << "��Ӧ��Id: " << rDevInfo.getUsbVendorId() << endl;
		cout << "�豸URI: " << rDevInfo.getUri() << endl;

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
	case 1:   //1ͨ���������ͼ��
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
	case 3:   //3ͨ�������ɫͼ��
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
//	// ��ʼ��OpenNI����
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
//	// ��������Device�豸��
//	Device xtion;
//	const char * deviceURL = openni::ANY_DEVICE;  //�豸��aDeviceList[1].getUri();
//	rc = xtion.open(deviceURL);
//
//	// �������������
//	VideoStream streamDepth;
//	rc = streamDepth.create(xtion, SENSOR_DEPTH);
//	if (rc == STATUS_OK)
//	{
//		// �������ͼ����Ƶģʽ
//		VideoMode mModeDepth;
//		// �ֱ��ʴ�С
//		mModeDepth.setResolution(640, 480);
//		// ÿ��30֡
//		mModeDepth.setFps(30);
//		// ���ظ�ʽ
//		mModeDepth.setPixelFormat(PIXEL_FORMAT_DEPTH_1_MM);
//
//		streamDepth.setVideoMode(mModeDepth);
//
//		// �����������
//		rc = streamDepth.start();
//		if (rc != STATUS_OK)
//		{
//			cerr << "�޷��������������" << OpenNI::getExtendedError() << endl;
//			streamDepth.destroy();
//		}
//	}
//	else
//	{
//		cerr << "�޷����������������" << OpenNI::getExtendedError() << endl;
//	}
//
//	// ������ɫͼ��������
//	VideoStream streamColor;
//	rc = streamColor.create(xtion, SENSOR_COLOR);
//	if (rc == STATUS_OK)
//	{
//		// ͬ�������ò�ɫͼ����Ƶģʽ
//		VideoMode mModeColor;
//		mModeColor.setResolution(320, 240);
//		mModeColor.setFps(30);
//		mModeColor.setPixelFormat(PIXEL_FORMAT_RGB888);
//
//		streamColor.setVideoMode(mModeColor);
//
//		// �򿪲�ɫͼ��������
//		rc = streamColor.start();
//		if (rc != STATUS_OK)
//		{
//			cerr << "�޷��򿪲�ɫͼ����������" << OpenNI::getExtendedError() << endl;
//			streamColor.destroy();
//		}
//	}
//	else
//	{
//		cerr << "�޷�������ɫͼ����������" << OpenNI::getExtendedError() << endl;
//	}
//
//	if (!streamColor.isValid() || !streamDepth.isValid())
//	{
//		cerr << "��ɫ��������������Ϸ�" << endl;
//		OpenNI::shutdown();
//		return 1;
//	}
//
//	// ͼ��ģʽע��,��ɫͼ�����ͼ����
//	if (xtion.isImageRegistrationModeSupported(
//		IMAGE_REGISTRATION_DEPTH_TO_COLOR))
//	{
//		xtion.setImageRegistrationMode(IMAGE_REGISTRATION_DEPTH_TO_COLOR);
//	}
//
//
//	// ����OpenCVͼ�񴰿�
//	namedWindow("Depth Image", CV_WINDOW_AUTOSIZE);
//	namedWindow("Color Image", CV_WINDOW_AUTOSIZE);
//
//	// ���������ֵ
//	int iMaxDepth = streamDepth.getMaxPixelValue();
//
//	// ѭ����ȡ��������Ϣ��������VideoFrameRef��
//	VideoFrameRef  frameDepth;
//	VideoFrameRef  frameColor;
//	int i = 1;
//	while (true)
//	{
//		// ��ȡ������
//		rc = streamDepth.readFrame(&frameDepth);
//		if (rc == STATUS_OK)
//		{
//			// ���������ת����OpenCV��ʽ
//			const Mat mImageDepth(frameDepth.getHeight(), frameDepth.getWidth(), CV_16UC1, (void*)frameDepth.getData());
//		//	imwrite(".\\depth\\"+std::to_string(i)+".png", mImageDepth);
//			// Ϊ�������ͼ����ʾ�ĸ�������һЩ����CV_16UC1 ==> CV_8U��ʽ
//			Mat mScaledDepth;
//			Mat hScaledDepth;
//			//mImageDepth.convertTo(mScaledDepth, CV_8U, 255.0 / iMaxDepth);
//			mImageDepth.convertTo(mScaledDepth, CV_16U, 5.0);
//			//ˮƽ�������ͼ
//			flip(mScaledDepth, hScaledDepth,1);
//			// ��ʾ�����ͼ��
//			imshow("Depth Image", hScaledDepth);
//			imwrite(dfilepath + std::to_string(i) + ".png", hScaledDepth);
//		}
//
//		rc = streamColor.readFrame(&frameColor);
//		if (rc == STATUS_OK)
//		{
//			// ͬ���Ľ���ɫͼ������ת����OpenCV��ʽ
//			const Mat mImageRGB(frameColor.getHeight(), frameColor.getWidth(), CV_8UC3, (void*)frameColor.getData());
//		//	imwrite(".\\image\\" + std::to_string(i) + ".png", mImageRGB);
//			// ���Ƚ�RGB��ʽת��ΪBGR��ʽ
//			Mat cImageBGR, bImageBGR, hImageBGR;
//			cvtColor(mImageRGB, cImageBGR, CV_RGB2BGR);
//
//			//ˮƽ�������ͼ
//			flip(cImageBGR, hImageBGR,1);
//			resize(hImageBGR, hImageBGR, Size(640, 480));
//			// Ȼ����ʾ��ɫͼ��
//			imshow("Color Image", hImageBGR);
//			imwrite(rgbfilepath + std::to_string(i) + ".png", hImageBGR);
//		}
//
//		// ��ֹ��ݼ�
//		if (waitKey(1) == 27)
//			break;
//		i++;
//	}
//
//	// �ر�������
//	streamDepth.destroy();
//	streamColor.destroy();
//	// �ر��豸
//	xtion.close();
//	// ���ر�OpenNI
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