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
	if (argc < 3) {
		std::cerr << "Usage: RoadSimplification <road GSM file> <road type>" << std::endl;
		return 1;
	}
	
	int roadType = atoi(argv[2]);

	// AvenueとLocal streetsのみを読み込む
	RoadGraph r;
	GraphUtil::loadRoads(r, argv[1], roadType);

	RoadGraph r2;
	GraphUtil::copyRoads(r, r2);
	GraphUtil::simplify2(r2, 10.0f);


	/*
	GraphUtil::translate(r, QVector2D(500, 500));

	// Create a matrix from the road object
	cv::Mat_<uchar> mat;
	GraphUtil::convertToMat(r, mat, cv::Size(1000, 1000), false);

	// Dilation
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));//, cv::Point(10, 10));
	cv::dilate(mat, result, kernel);

	// Thinning
	ThinningUtil::thinningGuoHall(result, result);

	// simplify the roads
	cv::Mat result2;
	RoadGraph r2;
	ThinningUtil::simplifyRoad(result, r, r2);
	*/

	GraphUtil::saveRoads(r2, "result.gsm");




	cv::waitKey(0);
}