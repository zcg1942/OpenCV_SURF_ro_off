// OpenCV_surf.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include"SSIM.h"


//-----------------------------------��ͷ�ļ��������֡�---------------------------------------  
//      ����������������������ͷ�ļ�  
//----------------------------------------------------------------------------------------------  
#include "opencv2/core/core.hpp"  
#include "opencv2/features2d/features2d.hpp"  
#include "opencv2/highgui/highgui.hpp"  
#include <opencv2/nonfree/nonfree.hpp>  
#include<opencv2/legacy/legacy.hpp>  
#include <iostream>  
#include "highgui.h"  
#include "cv.h"  
#include "vector"  
#include "opencv\cxcore.hpp"  
#include "iostream"  
#include "opencv.hpp"  
#include "opencv2/nonfree/features2d.hpp"
#include <Windows.h>
#include"stdio.h"


//-----------------------------------�������ռ��������֡�--------------------------------------  
//      ����������������ʹ�õ������ռ�  
//-----------------------------------------------------------------------------------------------  
using namespace cv;
using namespace std;
//-----------------------------------���궨�岿�֡�--------------------------------------------   
//      ����������һЩ������ 
//------------------------------------------------------------------------------------------------   
#define WINDOW_NAME1 "��ͼ1��"                  //Ϊ���ڱ��ⶨ��ĺ�   
#define WINDOW_NAME2 "��ͼ2��"        //Ϊ���ڱ��ⶨ��ĺ�   
#define WINDOW_NAME3 "��ֱ����׼���ͼ��"        //Ϊ���ڱ��ⶨ��ĺ�   
#define WINDOW_NAME4 "��ƽ��������׼���ͼ��"        //Ϊ���ڱ��ⶨ��ĺ�   


//-----------------------------------��ȫ�ֺ����������֡�--------------------------------------  
//      ������ȫ�ֺ���������  
//-----------------------------------------------------------------------------------------------  
static void ShowHelpText();//����������� 
void on_Rotation(int, void *);//�ص�����  
void PSNR_count(Mat, Mat);
double Ssim(IplImage* src, IplImage* key);
//void on_Rotation_scale(int, void *);//�ص����� ����ÿһ��barҪһ���ص��������ο��ı����ȶԱȵ�
void onMouse(int event, int x, int y, int flags, void* param);
Mat srcImage2, dstImage2,dstImage_warp;
Mat srcImage1, dstImage1;
Mat Rotation_matrix;
Point2f Point2fcenter=float((200.0,150.0));
int g_n_Rotation_angle = 11;
int g_n_Rotation_scale=11;
int g_nminHessian=111;

void onMouse(int event, int x, int y, int flags, void* param)
{
	Mat *im = reinterpret_cast<Mat*>(param);
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:     //������������Ӧ����������ͻҶ�  
		std::cout << "at(" << x << "," << y << ")value is:"
			<< static_cast<int>(im->at<uchar>(cv::Point(x, y))) << std::endl;
	// Point2fcenter = Point(x, y);
		break;
	//case CV_EVENT_RBUTTONDOWN:    //����Ҽ�������Ӧ���������겢���ظ�����ĻҶ�  
	//	std::cout << "input(x,y)" << endl;
	//	std::cout << "x =" << endl;
	//	cin >> x;
	//	std::cout << "y =" << endl;
	//	cin >> y;
	//	std::cout << "at(" << x << "," << y << ")value is:"
	//		<< static_cast<int>(im->at<uchar>(cv::Point(x, y))) << std::endl;
	//	break;
	}
}

//-----------------------------------��main( )������--------------------------------------------  
//      ����������̨Ӧ�ó������ں��������ǵĳ�������￪ʼִ��  
//-----------------------------------------------------------------------------------------------  
int main()
{
	//��0���ı�console������ɫ  
	system("color 3B");

	

	//��0�������ز�ͼ  
     srcImage1 = imread("D://1.jpg", 1);
	 imshow(WINDOW_NAME1, srcImage1);
	 //��0����ʾ��ӭ�Ͱ�������  
	 ShowHelpText();
	 srcImage2 = Mat::zeros(srcImage1.rows, srcImage1.cols, srcImage1.type());
	//Mat srcImage2 = imread("D://2.jpg", 1);
	
	//imshow(WINDOW_NAME2, srcImage2);
	// ����Ŀ��ͼ��Ĵ�С��������Դͼ��һ��  
	
	Mat dstImage1;
	// ����Ŀ��ͼ��Ĵ�С��������Դͼ��һ��  
	dstImage1 = Mat::zeros(srcImage1.rows, srcImage1.cols, srcImage1.type());
	Mat dstImage2;
	// ����Ŀ��ͼ��Ĵ�С��������Դͼ��һ��  
	dstImage2 = Mat::zeros(srcImage2.rows, srcImage2.cols, srcImage2.type());
	Mat dstImage_warp;
	// ����Ŀ��ͼ��Ĵ�С��������Դͼ��һ��  
	dstImage_warp = Mat::zeros(srcImage1.rows, srcImage1.cols, srcImage1.type());
	Mat dstImage4;
	// ����Ŀ��ͼ��Ĵ�С��������Դͼ��һ��  
	dstImage4 = Mat::zeros(srcImage1.rows, srcImage1.cols, srcImage1.type());
	if (!srcImage1.data)
	{
		printf("��ȡͼƬ������ȷ��Ŀ¼���Ƿ���imread����ָ����ͼƬ����~�� \n"); return false;
	}

	//�����켣�� �ͻ�ȡ������λ�� 
	namedWindow(WINDOW_NAME2, 1);
	cv::setMouseCallback(WINDOW_NAME1, onMouse, reinterpret_cast<void*> (&srcImage1));
	createTrackbar("�Ƕ�", WINDOW_NAME2, &g_n_Rotation_angle, 300, on_Rotation);
	createTrackbar("�ߴ�", WINDOW_NAME2, &g_n_Rotation_scale, 13, on_Rotation);
	createTrackbar("H��ֵ", WINDOW_NAME2, &g_nminHessian, 999, on_Rotation);//����bar����ͬһ������
	//���ûص�����
	on_Rotation(g_n_Rotation_angle, 0);
	on_Rotation(g_n_Rotation_scale, 0);
	on_Rotation(g_nminHessian, 0);
	

	//��2��ʹ��SURF���Ӽ��ؼ���  
	//int minHessian = 700;//SURF�㷨�е�hessian��ֵ  
	/*static int minHessian;
	cout << "��Ӽ������뺣��������ֵ";
	cin >> minHessian;*/
	


	////double adjustValue1 = srcImage1.cols;
	//double adjustValue1 = 400.0;
	////double adjustValue2 = srcImage1.rows;
	//double adjustValue2 = 300.0;
	//Mat adjustMat = (Mat_<double>(3, 3) << 1.0, 0, adjustValue1, 0, 1.0, adjustValue2, 0, 0, 1.0);
	//cout << "��������Ϊ��\n" << adjustMat << endl << endl;
	//cout << "������任����Ϊ��\n" << adjustMat*warpMat<< endl;
	//warpAffine(srcImage1, dstImage4, warpMat, dstImage4.size());
	//imshow(WINDOW_NAME4, dstImage4);

	waitKey(0);
	return 0;
}

//-----------------------------------��ShowHelpText( )������----------------------------------    
//      ���������һЩ������Ϣ    
//----------------------------------------------------------------------------------------------    
static void ShowHelpText()
{
	//���һЩ������Ϣ    
	printf("\n\n\n\t��ӭ������SURF����������ʾ������~\n\n");
	printf("\t��ǰʹ�õ�OpenCV�汾Ϊ OpenCV CV_VERSION\n\n");
	//printf("\t��Ӽ�������SURF�㷨�е�hessian��ֵ��\n\n");
	printf("\t��������˳�\n\n\n\n\t\t\t\t\t\t\t\t by����\n\n\n");
}
//-----------------------------��Process( )������------------------------------------  
//            �����������Զ������ת����׼����  
//-----------------------------------------------------------------------------------------  
void Process()
{//����trackbar��ֵ������ת����
	DWORD start_time = GetTickCount();
	double angle = (double)(g_n_Rotation_angle / 10);
	double scale = (double)(g_n_Rotation_scale / 10);
	int minHessian = g_nminHessian;
	Rotation_matrix = getRotationMatrix2D(Point2fcenter, angle, scale);
	cout << "��ת����Ϊ��" << Rotation_matrix;
	warpAffine(srcImage1, srcImage2, Rotation_matrix, srcImage2.size());
	//��ʾЧ��ͼ 
	imshow(WINDOW_NAME2, srcImage2);
	
	SurfFeatureDetector detector(minHessian);//����һ��SurfFeatureDetector��SURF�� ������������    
	std::vector<KeyPoint> keyPoints1, keyPoints2;//vectorģ���࣬����������͵Ķ�̬����  

	//��3������detect��������SURF�����ؼ��㣬������vector������  
	detector.detect(srcImage1, keyPoints1);
	detector.detect(srcImage2, keyPoints2);
	drawKeypoints(srcImage1, keyPoints1, dstImage1, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(srcImage2, keyPoints2, dstImage2, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cout << "size of description of Img1: " << keyPoints1.size() << endl;
	cout << "size of description of Img2: " << keyPoints2.size() << endl;

	namedWindow("KeyPoints of image1", 1);
	namedWindow("KeyPoints of image2", 1);

	//����������
	CvFont font;
	double hScale = 1;
	double vScale = 1;
	int lineWidth = 2;// �൱��д�ֵ����� 
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 0, lineWidth);//��ʼ�����壬׼��д��ͼƬ�ϵ�   
	// cvPoint Ϊ��ʵ�x��y����     
	IplImage* transimg1 = cvCloneImage(&(IplImage)dstImage1);
	IplImage* transimg2 = cvCloneImage(&(IplImage)dstImage2);

	char str1[20], str2[20];
	sprintf(str1, "%d", keyPoints1.size());
	sprintf(str2, "%d", keyPoints2.size());


	const char* str = str1;
	cvPutText(transimg1, str1, cvPoint(280, 230), &font, CV_RGB(255, 0, 0));//��ͼƬ������ַ�   

	str = str2;
	cvPutText(transimg2, str2, cvPoint(280, 230), &font, CV_RGB(255, 0, 0));//��ͼƬ������ַ�   
	//imshow("Description 1",res1);  
	cvShowImage("KeyPoints of image1", transimg1);
	cvShowImage("KeyPoints of image2", transimg2);

	//��4������������������������  
	SurfDescriptorExtractor extractor;
	Mat descriptors1, descriptors2;
	extractor.compute(srcImage1, keyPoints1, descriptors1);
	extractor.compute(srcImage2, keyPoints2, descriptors2);

	//��5��ʹ��BruteForce����ƥ��  
	// ʵ����һ��ƥ����  
	BruteForceMatcher< L2<float> > matcher;
	std::vector< DMatch > matches;
	//ƥ������ͼ�е������ӣ�descriptors��  
	matcher.match(descriptors1, descriptors2, matches);

	//��6�����ƴ�����ͼ����ƥ����Ĺؼ���  
	Mat imgMatches;
	drawMatches(srcImage1, keyPoints1, srcImage2, keyPoints2, matches, imgMatches);//���л���  
	cout << "number of matched points: " << matches.size() << endl;

	//��7����ʾƥ��Ч��ͼ  
	imshow("ƥ��ͼ", imgMatches);
	//��8��������ƥ�������
	sort(matches.begin(), matches.end()); //����������   
	//��ȡ����ǰN��������ƥ��������  
	vector<Point2f> imagePoints1, imagePoints2;
	for (int i = 0; i<3; i++)
	{
		imagePoints1.push_back(keyPoints1[matches[i].queryIdx].pt);
		imagePoints2.push_back(keyPoints2[matches[i].trainIdx].pt);
	}
	//��9����÷���任  
	Mat warpMat(2, 3, CV_32FC1);
	warpMat = getAffineTransform(imagePoints1, imagePoints2);
	cout << "��õķ���任�����ǣ�\n" << warpMat << endl;
	//��10����Դͼ��Ӧ�øո���õķ���任  
	warpAffine(srcImage1, dstImage_warp, warpMat, dstImage_warp.size());
	imshow(WINDOW_NAME3, dstImage_warp);
	//��11����������ͼ�� dstImage_warp,srcImage2֮��ķ�ֵ�����
	PSNR_count(dstImage_warp, srcImage2);
	DWORD end_time = GetTickCount();
	cout << "The run time is:" << (end_time - start_time) << "ms!" << endl;
	//��12����������ͼ�� dstImage_warp,srcImage2֮��Ľṹ������
	IplImage* src;
	char* strPath1 = "D://1.jpg";  //ע���׺�����Ƿ���ȷ  
	src = cvLoadImage(strPath1, 1);//��ʼ��ָ��
	*src = IplImage(dstImage_warp);

	IplImage* key;
	

	key = cvLoadImage(strPath1, 1);
	*key = IplImage(srcImage2);//����������Matת��ΪIplImage

	double ssim=Ssim( src, key);
	cout << "ssim is :" << ssim << endl;
	printf("\t\t\t\t\n\n\n\n");
	

}
//-----------------------------��on_�ص�( )������------------------------------------  
//            ������ʵ����ת�����ű任�ص�����  
//------------------------------------------------------------------------------------------------
void on_Rotation(int, void *)//�ص�����  
{
	Process();
}
//-----------------------------������PSNR������------------------------------------  
//            ������ת��Ϊ�Ҷ�ͼ֮�󣬶Ա����ز���
//--------------------------------------------------------------------------------------
void PSNR_count(Mat x,Mat y)
{
	double MSE,PSNR0;
	double s = 0;
	int PSNR_dB;
	Mat gray1, gray2;
	
	cvtColor(x, gray1, CV_BGR2GRAY);
	cvtColor(y, gray2, CV_BGR2GRAY);
	for (int i = 0; i < gray1.rows; i++)//������С�ڵ��ڣ����������
	{
		for (int j = 0; j < gray1.cols; j++)
		{
			s = s + int((gray1.at<uchar>(i, j) - gray2.at<uchar>(i, j)) ^ 2);//ע��ѭ��˳�������˳���Ƚ�����ѭ������Ӧy���꣬atʱ����ǰ��
		}
	}
	MSE = s / ((gray1.rows)*(gray1.cols));
	PSNR0 = (255 ^ 2 )/ MSE;
	PSNR_dB = 10 * log10(PSNR0);
	cout << "��ԭǰ���ֵ������ǣ�" << PSNR_dB << "dB"<<endl;
	
}
