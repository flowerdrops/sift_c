// shijue2.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
using namespace cv;
using namespace std;

int main1()
{


	Mat img1 = cv::imread("test1.jpg",CV_LOAD_IMAGE_COLOR);
	Mat img2 = cv::imread("test2.png", CV_LOAD_IMAGE_COLOR);
	if (img1.empty()|| img2.empty())
		return 0;
//	namedWindow("Pic");imshow("Pic", img);
	std::vector<KeyPoint> key1,key2;
	Mat des1,des2;
	Ptr<ORB> orb = ORB::create(500, 1.2f, 8, 31, 0, 2, ORB::HARRIS_SCORE, 31, 20);

	orb->detect(img1, key1);
	orb->detect(img2, key2);

	orb->compute(img1, key1, des1);
	orb->compute(img2, key2, des2);

	Mat outimg1;
	drawKeypoints(img1, key1, outimg1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	imshow("ORB 特征点", outimg1);

	std::vector<DMatch> matches;
	BFMatcher matcher(NORM_HAMMING);
	matcher.match(des1, des2, matches);
	double min_dist = 10000, max_dist = 0;
	for (int i = 0; i < des1.rows; i++) {
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);

	vector<DMatch> good_matches;
	for (int i = 0; i < des1.rows; i++) {
		if (matches[i].distance <= max(2 * min_dist, 30.0)) {
			good_matches.push_back(matches[i]);
		}
	}

	Mat img_match;
	Mat img_goodmatch;
	drawMatches(img1, key1, img2, key2, matches, img_match);
	drawMatches(img1, key1, img2, key2, good_matches, img_goodmatch);
	imshow("所有匹配点对", img_match);
	imshow("优化后的匹配点对", img_goodmatch);

	waitKey(0);
    return 0;
}

