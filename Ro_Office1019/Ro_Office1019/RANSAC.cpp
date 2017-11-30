#include "stdafx.h"
#include "RANSAC.h"
#include <iostream>  

#include <opencv2\opencv.hpp>
#include "opencv2/features2d/features2d.hpp"  
#include "opencv2/highgui/highgui.hpp"  
#include <opencv2/nonfree/nonfree.hpp>  
#include<opencv2/legacy/legacy.hpp>  

using namespace std;
////////////////////////////////////////////////////////////////////////////////  
//第二步:RANSAC方法剔除outliner  
//@Author:code陈  
////////////////////////////////////////////////////////////////////////////////  

void RANSAC00(std::vector<DMatch> matches, std::vector<cv::KeyPoint> firstKeypoint, std::vector<cv::KeyPoint> secondKeypoint, Mat firstImage, Mat secondImage, Mat matcheImage)
{
	//将vector转化成Mat  
	Mat firstKeypointMat(matches.size(), 2, CV_32F), secondKeypointMat(matches.size(), 2, CV_32F);
	for (int i = 0; i < matches.size(); i++)
	{
		firstKeypointMat.at<float>(i, 0) = firstKeypoint[matches[i].queryIdx].pt.x;
		firstKeypointMat.at<float>(i, 1) = firstKeypoint[matches[i].queryIdx].pt.y;
		secondKeypointMat.at<float>(i, 0) = secondKeypoint[matches[i].trainIdx].pt.x;
		secondKeypointMat.at<float>(i, 1) = secondKeypoint[matches[i].trainIdx].pt.y;
	}
	//Calculate the fundamental Mat;  
	vector<uchar> ransacStatus;
	Mat fundamentalMat = findFundamentalMat(firstKeypointMat, secondKeypointMat, ransacStatus, FM_RANSAC);
	cout << fundamentalMat << endl;
	//Calculate the number of outliner points;  
	int outlinerCount = 0;
	for (int i = 0; i < matches.size(); i++)
	{
		if (ransacStatus[i] == 0)
		{
			outlinerCount++;
		}
	}
	//Calculate inliner points;  
	vector<Point2f> firstInliner;
	vector<Point2f> secondInliner;
	vector<DMatch> inlinerMatches;
	int inlinerCount = matches.size() - outlinerCount;
	firstInliner.resize(inlinerCount);
	secondInliner.resize(inlinerCount);
	inlinerMatches.resize(inlinerCount);
	int index = 0;
	for (int i = 0; i < matches.size(); i++)
	{
		if (ransacStatus[i] != 0)
		{
			firstInliner[index].x = firstKeypointMat.at<float>(i, 0);
			firstInliner[index].y = firstKeypointMat.at<float>(i, 1);
			secondInliner[index].x = secondKeypointMat.at<float>(i, 0);
			secondInliner[index].y = secondKeypointMat.at<float>(i, 1);
			inlinerMatches[index].queryIdx = index;
			inlinerMatches[index].trainIdx = index;
			index++;
		}
	}
	vector<KeyPoint> inlinerFirstKeypoint(inlinerCount);
	vector<KeyPoint> inlinerSecondKeypoint(inlinerCount);
	KeyPoint::convert(firstInliner, inlinerFirstKeypoint);
	KeyPoint::convert(secondInliner, inlinerSecondKeypoint);
	//cout<<fundamentalMat<<endl;  
	//select 50 keypoints  
	//matches.erase(matches.begin()+50,matches.end());  
	//inlinerMatches.erase(inlinerMatches.begin()+50,inlinerMatches.end());  
	drawMatches(firstImage, inlinerFirstKeypoint, secondImage, inlinerSecondKeypoint, inlinerMatches, matcheImage);
	imshow("ransacMatches", matcheImage);
	/*drawMatches(firstImage, firstKeypoint, secondImage, secondKeypoint, matches, matcheImage);
	imshow("matches", matcheImage);*/

	//RANSAC下的配准
	
	// inlinerMatches就是筛选之后的匹配对
	//【8】特征点匹配对排序
	sort(inlinerMatches.begin(), inlinerMatches.end()); //特征点排序   
	//获取排在前N个的最优匹配特征点  
	vector<Point2f> imagePoints1, imagePoints2;
	for (int i = 0; i<3; i++)
	{
		imagePoints1.push_back(inlinerFirstKeypoint[inlinerMatches[i].queryIdx].pt);
		imagePoints2.push_back(inlinerSecondKeypoint[inlinerMatches[i].trainIdx].pt);
	}
	//【9】求得仿射变换  
	Mat warpMat(2, 3, CV_32FC1);
	warpMat = getAffineTransform(imagePoints1, imagePoints2);
	cout << "求得的仿射变换矩阵是：\n" << warpMat << endl;
	//【10】对源图像应用刚刚求得的仿射变换  
	Mat dstImage_warp;
	dstImage_warp.create(firstImage.size(), firstImage.type());
	warpAffine(firstImage, dstImage_warp, warpMat, dstImage_warp.size());
	imshow("根据RANSAC配准", dstImage_warp);
	//【11】计算两幅图像 dstImage_warp,srcImage2之间的峰值信噪比
	double psnr_RAN= PSNR(dstImage_warp, secondImage);
	
	cout << "RANSAC后opencv函数算出的psnr=" << psnr_RAN << "dB" << endl;
}