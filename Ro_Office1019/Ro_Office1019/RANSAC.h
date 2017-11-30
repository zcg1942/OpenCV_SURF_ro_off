#include <iostream>  
#include <opencv2\opencv.hpp> 
#include<vector>
using namespace std;
using namespace cv;
//std::vector<DMatch> Matches;
//std::vector<cv::KeyPoint> FirstKeypoint;//KeyPoint前不加cv::会报错说KeyPoint不明确
//std::vector<cv::KeyPoint> SecondKeypoint;
void RANSAC00(std::vector<DMatch> matches, std::vector<cv::KeyPoint> firstKeypoint, std::vector<cv::KeyPoint> secondKeypoint,Mat,Mat,Mat);

