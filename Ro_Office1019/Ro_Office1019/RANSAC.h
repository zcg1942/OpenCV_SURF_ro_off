#include <iostream>  
#include <opencv2\opencv.hpp> 
#include<vector>
using namespace std;
using namespace cv;
//std::vector<DMatch> Matches;
//std::vector<cv::KeyPoint> FirstKeypoint;//KeyPointǰ����cv::�ᱨ��˵KeyPoint����ȷ
//std::vector<cv::KeyPoint> SecondKeypoint;
void RANSAC00(std::vector<DMatch> matches, std::vector<cv::KeyPoint> firstKeypoint, std::vector<cv::KeyPoint> secondKeypoint,Mat,Mat,Mat);

