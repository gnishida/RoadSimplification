/**
 * 道路網を、DilationとThinningを使って、シンプル化する。
 * 以下の論文で説明されている手法を使っている。
 * Microsoft Tech Report 2012 - Segmentaion of Urban Areas Using Road Networks
 *
 * @author Gen Nishida
 * @version 1.0
 */

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QVector2D>
#include <QHash>
#include "GraphUtil.h"
#include "BBox.h"

int main(int argc, char *argv[]) {
	int size = 10;
	float range[] = { 0, 300 };
	const float* ranges = { range };

	// create a histogram
	cv::Mat data1 = (cv::Mat_<float>(1, 10) << 20, 30, 30, 40, 80, 90, 120, 120, 130, 150);
	cv::MatND hist1;
	cv::calcHist(&data1, 1, 0, cv::Mat(), hist1, 1, &size, &ranges, true, false);

	// create a histogram
	cv::Mat data2 = (cv::Mat_<float>(1, 10) << 30, 33, 40, 70, 90, 100, 130, 135, 140, 154);
	cv::MatND hist2;
	cv::calcHist(&data2, 1, 0, cv::Mat(), hist2, 1, &size, &ranges, true, false);

	// create a histogram
	cv::Mat data3 = (cv::Mat_<float>(1, 10) << 30, 83, 140, 170, 190, 200, 200, 205, 240, 244);
	cv::MatND hist3;
	cv::calcHist(&data3, 1, 0, cv::Mat(), hist3, 1, &size, &ranges, true, false);

	// compute the similarity
	float similarity = cv::compareHist(hist1, hist2, 0);
	printf("data1 - data2: %lf\n", similarity);
	similarity = cv::compareHist(hist2, hist3, 0);
	printf("data2 - data3: %lf\n", similarity);
	similarity = cv::compareHist(hist3, hist1, 0);
	printf("data3 - data1: %lf\n", similarity);

	// Display
	/*
	cv::flip(result, result, 1);
	cv::namedWindow("segmentation", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::imshow("segmentation", result * 64);
	*/

	cv::waitKey(0);
}