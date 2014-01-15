/**
 * This thinning utility implements the following website code.
 * https://opencv-code.com/quick-tips/implementation-of-guo-hall-thinning-algorithm/#more-675
 * 元になる論文は、以下。
 * Z. Guo and R. Hall. Parallel thinning with two-subiteration algorithms. Communications of the ACM, 32(3):359-373, 1989.
 *
 */

#pragma once

#include <opencv/cv.h>
#include <opencv/highgui.h>

class ThinningUtil {
protected:
	ThinningUtil() {}
	~ThinningUtil() {}

private:
	static void thinningGuoHallIteration(cv::Mat& im, int iter);

public:
	static void thinningGuoHall(cv::Mat& im);
};

