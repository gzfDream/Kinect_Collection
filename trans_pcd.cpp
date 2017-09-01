#include <pcl/visualization/cloud_viewer.h>
#include <iostream>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <opencv2/opencv.hpp>
#include <pcl/common/transforms.h>   
#include <Eigen/Core>
#include <fstream>  
#include <string>

using namespace std;
using namespace cv;
using namespace Eigen;

Matrix4f readf(string str){
	Matrix4f mat;
	
	ifstream  file(str);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			file >> mat(i, j);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << mat(i, j) << ' ';
		}
		cout << endl;
	}
	file.close();

	return mat;
}

int main(){

	Matrix4f mat1;
	Matrix4f mat2;

	mat1 = readf("..//..//data//cal1.txt");
	mat2 = readf("..//..//data//cal2.txt");
	

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud1(new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_t1(new pcl::PointCloud<pcl::PointXYZRGB>);

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud2(new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_t2(new pcl::PointCloud<pcl::PointXYZRGB>);

	if (pcl::io::loadPCDFile<pcl::PointXYZRGB>("..//..//data//pcd1//1.pcd", *cloud1) == -1) 
	{
		PCL_ERROR("Couldn't read file .pcd \n"); 
		return (-1);
	}

	if (pcl::io::loadPCDFile<pcl::PointXYZRGB>("..//..//data//pcd2//1.pcd", *cloud2) == -1) 
	{
		PCL_ERROR("Couldn't read file .pcd \n"); 
		return (-1);
	}
	pcl::transformPointCloud(*cloud1, *cloud_t1, mat1);
	pcl::transformPointCloud(*cloud2, *cloud_t2, mat2);

	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
	viewer = boost::shared_ptr<pcl::visualization::PCLVisualizer>(new pcl::visualization::PCLVisualizer("Cloud Viewer"));
	viewer->setBackgroundColor(1.0, 1.0, 1.0);
	
	string outputpath = "..//..//data//";
	viewer->addPointCloud(cloud_t1, "transform1");
	pcl::io::savePCDFile(outputpath + to_string(1)+".pcd", *cloud_t1);
	viewer->addPointCloud(cloud_t2, "transform2");
	pcl::io::savePCDFile(outputpath + to_string(2) + ".pcd", *cloud_t2);

	boost::shared_ptr<pcl::visualization::PCLVisualizer> ori_viewer;
	ori_viewer = boost::shared_ptr<pcl::visualization::PCLVisualizer>(new pcl::visualization::PCLVisualizer("Origin Cloud Viewer"));
	ori_viewer->setBackgroundColor(1.0, 1.0, 1.0);

	ori_viewer->addPointCloud(cloud1, "origin1");
	ori_viewer->addPointCloud(cloud2, "origin2");

	while (!viewer->wasStopped() && !ori_viewer->wasStopped())
	{
		viewer->spinOnce();
		ori_viewer->spinOnce();
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}

	return 0;
}