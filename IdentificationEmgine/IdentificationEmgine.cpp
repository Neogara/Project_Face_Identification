// IdentificationEmgine.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include "opencv2\opencv.hpp"
#include "opencv2\highgui.hpp"

#include "face_identification.h"
#include "common.h"

#include "math.h"
#include "time.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <stdlib.h>
#include <stdio.h>

#include "ctime"

using namespace std;
using namespace seeta;

#define EXPECT_NE(a, b) if ((a) == (b)) std::cout << "ERROR: "
#define EXPECT_EQ(a, b) if ((a) != (b)) std::cout << "ERROR: "
int main()
{
	FaceIdentification face_recognizer("seeta_fr_v1.0.bin");
	std::string test_dir = "test_face_recognizer/";

	int feat_size = face_recognizer.feature_size();
	EXPECT_EQ(feat_size, 2048);

	FILE* feat_file = NULL;

	// Load features extract from caffe
	fopen_s(&feat_file, (test_dir + "feats.dat").c_str(), "rb");
	int n, c, h, w;
	EXPECT_EQ(fread(&n, sizeof(int), 1, feat_file), (unsigned int)1);
	EXPECT_EQ(fread(&c, sizeof(int), 1, feat_file), (unsigned int)1);
	EXPECT_EQ(fread(&h, sizeof(int), 1, feat_file), (unsigned int)1);
	EXPECT_EQ(fread(&w, sizeof(int), 1, feat_file), (unsigned int)1);
	float* feat_caffe = new float[n * c * h * w];
	float* feat_sdk = new float[n * c * h * w];
	EXPECT_EQ(fread(feat_caffe, sizeof(float), n * c * h * w, feat_file),
		n * c * h * w);
	EXPECT_EQ(feat_size, c * h * w);

	int cnt = 0;

	/* Data initialize */
	std::ifstream ifs(test_dir + "test_file_list.txt");
	std::string img_name;
	FacialLandmark pt5[5];

	clock_t start, count = 0;
	int img_num = 0;
	double average_sim = 0.0;
	while (ifs >> img_name) {
		// read image
		cv::Mat src_img = cv::imread(test_dir + img_name, 1);
		EXPECT_NE(src_img.data, nullptr) << "Load image error!";

		// ImageData store data of an image without memory alignment.
		ImageData src_img_data(src_img.cols, src_img.rows, src_img.channels());
		src_img_data.data = src_img.data;

		// 5 located landmark points (left eye, right eye, nose, left and right 
		// corner of mouse).
		for (int i = 0; i < 5; ++i) {
			ifs >> pt5[i].x >> pt5[i].y;
		}

		/* Extract feature: ExtractFeatureWithCrop */
		start = clock();
		face_recognizer.ExtractFeatureWithCrop(src_img_data, pt5,
			feat_sdk + img_num * feat_size);
		count += clock() - start;

		/* Caculate similarity*/
		float* feat1 = feat_caffe + img_num * feat_size;
		float* feat2 = feat_sdk + img_num * feat_size;
		float sim = face_recognizer.CalcSimilarity(feat1, feat2);
		average_sim += sim;
		img_num++;
	}
	ifs.close();
	average_sim /= img_num;
	if (1.0 - average_sim >  0.02) {
		std::cout << "average similarity: " << average_sim << std::endl;
	}
	else {
		std::cout << "Test successful!\nAverage extract feature time: "
			<< 1000.0 * count / CLOCKS_PER_SEC / img_num << "ms" << std::endl;
	}
	delete[]feat_caffe;
	delete[]feat_sdk;

    return 0;
}

