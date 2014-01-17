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
#include "GraphUtil.h"
#include "ThinningUtil.h"

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: RoadSimplification <road GSM file> <road type>" << std::endl;
		return 1;
	}
	
	cv::Mat result;

	if (argc == 3) {
		int roadType = atoi(argv[2]);

		// AvenueとLocal streetsのみを読み込む
		RoadGraph r;
		GraphUtil::loadRoads(r, argv[1], roadType);
		GraphUtil::translate(r, QVector2D(500, 500));

		// Create a matrix from the road object
		cv::Mat_<uchar> mat;
		GraphUtil::convertToMat(r, mat, cv::Size(1000, 1000), false);

		// Dilation
		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(21, 21));//, cv::Point(10, 10));
		cv::dilate(mat, result, kernel);

		// Thinning
		ThinningUtil::thinningGuoHall(result, result);

		// simplify the roads
		cv::Mat result2;
		RoadGraph r2;
		ThinningUtil::simplifyRoad(result, r, r2);

		GraphUtil::saveRoads(r2, "result.gsm");

		// 画面に表示するために、上下反転する
		cv::flip(result, result, 0);
	} else {
		result = cv::imread(argv[1], 0);
		result.convertTo(result, CV_8UC1);

		// Thinning
		//ThinningUtil::thinning(result, result);
		ThinningUtil::thinningGuoHall(result, result);
		//ThinningUtil::thinningGen(result, result);
	}
	
	cv::imwrite("result.bmp", result);

	// Display
	cv::namedWindow("result", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::imshow("result", result);

	cv::waitKey(0);
}