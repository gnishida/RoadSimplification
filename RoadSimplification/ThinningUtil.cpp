#include "ThinningUtil.h"

/**
 * Perform one thinning iteration.
 * Normally you wouldn't call this function directly from your code.
 *
 * @param  im    Binary image with range = 0-1
 * @param  iter  0=even, 1=odd
 */
void ThinningUtil::thinningIteration(cv::Mat& im, int iter)
{
    cv::Mat marker = cv::Mat::zeros(im.size(), CV_8UC1);

    for (int i = 1; i < im.rows - 1; i++) {
        for (int j = 1; j < im.cols - 1; j++) {
            uchar p1 = im.at<uchar>(i-1, j-1);
            uchar p2 = im.at<uchar>(i-1, j);
            uchar p3 = im.at<uchar>(i-1, j+1);
            uchar p4 = im.at<uchar>(i, j+1);
            uchar p5 = im.at<uchar>(i+1, j+1);
            uchar p6 = im.at<uchar>(i+1, j);
            uchar p7 = im.at<uchar>(i+1, j-1);
            uchar p8 = im.at<uchar>(i, j-1);

            int A  = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) + 
                     (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) + 
                     (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                     (p8 == 0 && p1 == 1) + (p1 == 0 && p2 == 1);
            int B  = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p1;
            int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
            int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

            if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
                marker.at<uchar>(i,j) = 1;
        }
    }

    im &= ~marker;
}

/**
* Perform one thinning iteration.
* Normally you wouldn't call this function directly from your code.
*
* Parameters:
*                 im Binary image with range = [0,1]
*                 iter 0=even, 1=odd
*/
void ThinningUtil::thinningGuoHallIteration(cv::Mat& im, int iter) {
    cv::Mat marker = cv::Mat::zeros(im.size(), CV_8UC1); 

    for (int i = 1; i < im.rows - 1; i++) {
        for (int j = 1; j < im.cols - 1; j++) {
            uchar p1 = im.at<uchar>(i-1, j-1);
            uchar p2 = im.at<uchar>(i-1, j);
            uchar p3 = im.at<uchar>(i-1, j+1);
            uchar p4 = im.at<uchar>(i, j+1);
            uchar p5 = im.at<uchar>(i+1, j+1);
            uchar p6 = im.at<uchar>(i+1, j);
            uchar p7 = im.at<uchar>(i+1, j-1);
            uchar p8 = im.at<uchar>(i, j-1);

            int C  = (!p2 & (p3 | p4)) + (!p4 & (p5 | p6)) +
                     (!p6 & (p7 | p8)) + (!p8 & (p1 | p2));
            int N1 = (p1 | p2) + (p3 | p4) + (p5 | p6) + (p7 | p8);
            int N2 = (p2 | p3) + (p4 | p5) + (p6 | p7) + (p8 | p1);
            int N  = N1 < N2 ? N1 : N2;
            int m  = iter == 0 ? ((p6 | p7 | !p1) & p8) : ((p2 | p3 | !p5) & p4);

            if (C == 1 && (N >= 2 && N <= 3) & m == 0)
                marker.at<uchar>(i,j) = 1;
        }
    }

    im &= ~marker;
}

/**
* Perform one thinning iteration.
* Parameters:
*                 im Binary image with range = [0,1]
*                 iter 0=even, 1=odd
*/
void ThinningUtil::thinningGenIteration(cv::Mat& im, int iter) {
    cv::Mat marker = cv::Mat::zeros(im.size(), CV_8UC1); 

    for (int i = 1; i < im.rows - 1; i++) {
        for (int j = 1; j < im.cols - 1; j++) {
            uchar p1 = im.at<uchar>(i-1, j-1);
            uchar p2 = im.at<uchar>(i-1, j);
            uchar p3 = im.at<uchar>(i-1, j+1);
            uchar p4 = im.at<uchar>(i, j+1);
            uchar p5 = im.at<uchar>(i+1, j+1);
            uchar p6 = im.at<uchar>(i+1, j);
            uchar p7 = im.at<uchar>(i+1, j-1);
            uchar p8 = im.at<uchar>(i, j-1);

			int C  = (!p1 & (p2 | p3)) + (p1 & !p2 & p3) + (!p3 & (p4 | p5)) + (p3 & !p4 & p5) + (!p5 & (p6 | p7)) + (p5 & !p6 & p7) + (!p7 & (p8 | p1)) + (p7 & !p8 & p1);
            int N1 = (p1 | p2) + (p3 | p4) + (p5 | p6) + (p7 | p8);
            int N2 = (p2 | p3) + (p4 | p5) + (p6 | p7) + (p8 | p1);
            int N  = N1 < N2 ? N1 : N2;
			int m;
			if (iter == 0) {
				m = p2;
			} else if (iter == 1) {
				m = p4;
			} else if (iter == 2) {
				m = p6;
			} else if (iter == 3) {
				m = p8;
			}

            if (C == 1 && N >= 2 && N <= 3 && m == 0) {
                marker.at<uchar>(i,j) = 1;
			}
        }
    }

    im &= ~marker;
}

/**
 * Function for thinning the given binary image
 *
 * @param  im  Binary image with range = 0-255
 */
void ThinningUtil::thinning(const cv::Mat& src, cv::Mat& dst) {
	dst = src.clone();
	dst /= 255;

    cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
    cv::Mat diff;

    do {
        thinningGuoHallIteration(dst, 0);
        thinningGuoHallIteration(dst, 1);
        cv::absdiff(dst, prev, diff);
        dst.copyTo(prev);
    } 
    while (cv::countNonZero(diff) > 0);

    dst *= 255;
}

/**
* Function for thinning the given binary image
*
* Parameters:
*                 src The source image, binary with range = [0,255]
*                 dst The destination image
*/
void ThinningUtil::thinningGuoHall(const cv::Mat& src, cv::Mat& dst) {
	dst = src.clone();
	dst /= 255;

    cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
    cv::Mat diff;

    do {
        thinningGuoHallIteration(dst, 0);
        thinningGuoHallIteration(dst, 1);
        cv::absdiff(dst, prev, diff);
        dst.copyTo(prev);
    } 
    while (cv::countNonZero(diff) > 0);

    dst *= 255;
}

/**
* Function for thinning the given binary image
*
* Parameters:
*                 src The source image, binary with range = [0,255]
*                 dst The destination image
*/
void ThinningUtil::thinningGen(const cv::Mat& src, cv::Mat& dst) {
	dst = src.clone();
	dst /= 255;

    cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
    cv::Mat diff;

    do {
        thinningGenIteration(dst, 0);
        thinningGenIteration(dst, 1);
        thinningGenIteration(dst, 2);
        thinningGenIteration(dst, 3);
        cv::absdiff(dst, prev, diff);
        dst.copyTo(prev);
    } 
    while (cv::countNonZero(diff) > 0);

    dst *= 255;
}

void ThinningUtil::findIntersection(const cv::Mat& src, cv::Mat& dst) {
	src.convertTo(dst, CV_8UC3);
	src /= 255;

    for (int i = 1; i < src.rows - 1; i++) {
        for (int j = 1; j < src.cols - 1; j++) {
			if (src.at<uchar>(i, j) == 0) continue;

			int count = 0;
			for (int v = -1; v <= 1; v++) {
				for (int u = -1; u <= 1; u++) {
					if (u == 0 && v == 0) continue;

					count += src.at<uchar>(i + v, j + u);
				}
			}

			if (count >= 3 || count == 1) {
				cv::circle(dst, cv::Point(j, i), 5, cv::Scalar(255, 255, 0));
				// this is an inersection
				//dst.at<cv::Vec3b>(i, j)[0] = 255;//= cv::Vec3b(255, 255, 0);
				//dst.at<cv::Vec3b>(i, j)[1] = 255;//= cv::Vec3b(255, 255, 0);
				//dst.at<cv::Vec3b>(i, j)[2] = 0;//= cv::Vec3b(255, 255, 0);
			}
		}
	}
}
