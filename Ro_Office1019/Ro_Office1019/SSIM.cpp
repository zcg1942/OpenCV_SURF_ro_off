#include "stdafx.h"
#include "SSIM.h"
#include <iostream>  
using namespace std;
double Ssim(IplImage* src, IplImage* key)
{
	int width = src->width;       //图像宽
	   int height = src->height;     //图像高

	CvScalar s_src;               //原图像的像素通道结构体
	CvScalar s_key;               //加密后的像素通道结构体

	double mu_src = 0.0;          //原图像均值
	double mu_key = 0.0;          //加密后的图像均值

	int row, col;
	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			s_src = cvGet2D(src, row, col);
			s_key = cvGet2D(key, row, col);
			double src_r = s_src.val[0];        //取出G通道的像素值
			double key_r = s_key.val[0];
			mu_src += src_r;
			mu_key += key_r;

		}

	}

	mu_src = mu_src / (width * height);       //原图像均值
	mu_key = mu_key / (width * height);       //加密图像均值

	//打印的中间结果信息
	//printf( "src的均值: %lf\n", mu_src );
	//printf( "key的均值: %lf\n", mu_key );

	double sigma_src2 = 0.0;                    //原图像方差，即sigma_src^2
	double sigma_key2 = 0.0;                    //加密图像方差，即sigma_key^2
	double sigma_s_k2 = 0.0;                    //原图和加密图像的方差，即sigma_s_k^2

	double sigma_src = 0.0;                     //原图像标准差
	double sigma_key = 0.0;                     //加密图像标准差
	double sigma_s_k = 0.0;                     //原图像和加密图像的标准差

	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			s_src = cvGet2D(src, row, col);
			s_key = cvGet2D(key, row, col);
			double src_r = s_src.val[0];        //取出G通道的像素值
			double key_r = s_key.val[0];
			sigma_src2 += (src_r - mu_src) * (src_r - mu_src);
			sigma_key2 += (key_r - mu_key) * (key_r - mu_key);
			sigma_s_k2 += (src_r - mu_src) * (key_r - mu_key);

		}

	}

	sigma_src2 = sigma_src2 / (width * height - 1);
	sigma_key2 = sigma_key2 / (width * height - 1);
	sigma_s_k2 = sigma_s_k2 / (width * height - 1);

	sigma_src = sqrt(sigma_src2);
	sigma_key = sqrt(sigma_key2);
	sigma_s_k = sqrt(sigma_s_k2);

	//打印的中间结果信息
	//printf( "sigma_src: %lf\n", sigma_src );
	//printf( "sigma_key: %lf\n", sigma_key );
	//printf( "sigma_s_k: %lf\n", sigma_s_k );

	//固定参数，为常量
	//c1，c2，c3是用来维持稳定的常数
	//MAX是像素值的动态范围
	const double k1 = 0.01;
	const double k2 = 0.03;
	const int MAX = 255;

	double c1 = (k1 * MAX) * (k1 * MAX);
	double c2 = (k2 * MAX) * (k2 * MAX);
	double c3 = c2 / 2;

	//亮度、对比度、结构三方面度量图像相似性
	double light = (2 * mu_src * mu_key + c1) / (mu_src * mu_src + mu_key * mu_key + c1);
	double contrast = (2 * sigma_src * sigma_key + c2) / (sigma_src2 + sigma_key2 + c2);
	double structure = (sigma_s_k2 + c3) / (sigma_src * sigma_key + c3);

	//打印的中间结果信息
	//printf( "light: %lf\n", light );
	//printf( "contrast: %lf\n", contrast );
	//printf( "structure: %lf\n", structure );

	//方法一
	//亮度 * 对比度 * 结构相似度
	double ssim = light * contrast * structure;
	//cout << "结构相似性指标SSIM是：" << ssim << endl;
	//方法二，计算结果是一样的
	//double ssim = light * ( ( 2 * sigma_s_k2 + c2 ) / (sigma_src2 + sigma_key2 + c2 ) );

	return ssim;

}