#include "stdafx.h"
#include "SSIM.h"
#include <iostream>  
using namespace std;
double Ssim(IplImage* src, IplImage* key)
{
	int width = src->width;       //ͼ���
	   int height = src->height;     //ͼ���

	CvScalar s_src;               //ԭͼ�������ͨ���ṹ��
	CvScalar s_key;               //���ܺ������ͨ���ṹ��

	double mu_src = 0.0;          //ԭͼ���ֵ
	double mu_key = 0.0;          //���ܺ��ͼ���ֵ

	int row, col;
	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			s_src = cvGet2D(src, row, col);
			s_key = cvGet2D(key, row, col);
			double src_r = s_src.val[0];        //ȡ��Gͨ��������ֵ
			double key_r = s_key.val[0];
			mu_src += src_r;
			mu_key += key_r;

		}

	}

	mu_src = mu_src / (width * height);       //ԭͼ���ֵ
	mu_key = mu_key / (width * height);       //����ͼ���ֵ

	//��ӡ���м�����Ϣ
	//printf( "src�ľ�ֵ: %lf\n", mu_src );
	//printf( "key�ľ�ֵ: %lf\n", mu_key );

	double sigma_src2 = 0.0;                    //ԭͼ�񷽲��sigma_src^2
	double sigma_key2 = 0.0;                    //����ͼ�񷽲��sigma_key^2
	double sigma_s_k2 = 0.0;                    //ԭͼ�ͼ���ͼ��ķ����sigma_s_k^2

	double sigma_src = 0.0;                     //ԭͼ���׼��
	double sigma_key = 0.0;                     //����ͼ���׼��
	double sigma_s_k = 0.0;                     //ԭͼ��ͼ���ͼ��ı�׼��

	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			s_src = cvGet2D(src, row, col);
			s_key = cvGet2D(key, row, col);
			double src_r = s_src.val[0];        //ȡ��Gͨ��������ֵ
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

	//��ӡ���м�����Ϣ
	//printf( "sigma_src: %lf\n", sigma_src );
	//printf( "sigma_key: %lf\n", sigma_key );
	//printf( "sigma_s_k: %lf\n", sigma_s_k );

	//�̶�������Ϊ����
	//c1��c2��c3������ά���ȶ��ĳ���
	//MAX������ֵ�Ķ�̬��Χ
	const double k1 = 0.01;
	const double k2 = 0.03;
	const int MAX = 255;

	double c1 = (k1 * MAX) * (k1 * MAX);
	double c2 = (k2 * MAX) * (k2 * MAX);
	double c3 = c2 / 2;

	//���ȡ��Աȶȡ��ṹ���������ͼ��������
	double light = (2 * mu_src * mu_key + c1) / (mu_src * mu_src + mu_key * mu_key + c1);
	double contrast = (2 * sigma_src * sigma_key + c2) / (sigma_src2 + sigma_key2 + c2);
	double structure = (sigma_s_k2 + c3) / (sigma_src * sigma_key + c3);

	//��ӡ���м�����Ϣ
	//printf( "light: %lf\n", light );
	//printf( "contrast: %lf\n", contrast );
	//printf( "structure: %lf\n", structure );

	//����һ
	//���� * �Աȶ� * �ṹ���ƶ�
	double ssim = light * contrast * structure;
	//cout << "�ṹ������ָ��SSIM�ǣ�" << ssim << endl;
	//����������������һ����
	//double ssim = light * ( ( 2 * sigma_s_k2 + c2 ) / (sigma_src2 + sigma_key2 + c2 ) );

	return ssim;

}