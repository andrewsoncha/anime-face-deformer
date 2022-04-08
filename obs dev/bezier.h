#pragma once
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#define BEZIER_MIN_T 0.0001
using namespace cv;

class Bezier {
public:
	int controlPntN;
	Point controlPnts[4];
	Bezier(int controlPntNInput, Point controlPntInput[6]) {
		controlPntN = controlPntNInput;
		for (int i = 0; i < 4; i++) {
			controlPnts[i] = controlPntInput[i];
		}
	}
	Bezier(Point controlPntOne, Point controlPntTwo, Point controlPntThree) {
		controlPntN = 3;
		controlPnts[0] = controlPntOne;
		controlPnts[1] = controlPntTwo;
		controlPnts[2] = controlPntThree;
	}
	Bezier(Point controlPntOne, Point controlPntTwo, Point controlPntThree, Point controlPntFour) {
		controlPntN = 4;
		controlPnts[0] = controlPntOne;
		controlPnts[1] = controlPntTwo;
		controlPnts[2] = controlPntThree;
		controlPnts[3] = controlPntFour;
	}
	Point getPnt(double t) {
		int i, j;
		double x, y;
		x = 0; y = 0;
		for (i = 0; i < 4; i++) {
			double exp = 1.0;
			for (j = 0; j < i; j++) {
				exp *= t;
			}
			for (j = i; j < 4; j++) {
				exp *= (1 - t);
			}
			x += controlPnts[i].x * exp;
			y += controlPnts[i].y * exp;
		}
		return Point(x, y);
	}
	double evaluate(Mat img) {
		Mat grayImg, evalImg;
		Mat binaryMask = Mat::zeros(img.size(), CV_8UC1);
		double value;
		cvtColor(img, grayImg, COLOR_BGR2GRAY);
		binaryMask = drawBezier(binaryMask, *this, 2, Scalar(255));
		evalImg = binaryMask & evalImg;
		return (double)norm(sum(evalImg)) / (double)norm(sum(binaryMask));
	}
};

Mat drawBezier(Mat canvas, Bezier curve, int radius, Scalar color) {
	double i;
	Mat result;
	canvas.copyTo(result);
	for (i = 0; i < 1.0; i += BEZIER_MIN_T) {
		circle(result, curve.getPnt(i), radius, color, -1);
	}
	return result;
}