#pragma once
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#define BEZIER_MIN_T 0.0001
#ifndef __BEZIER_H
#define __BEZIER_H

using namespace cv;

class Bezier {
public:
	int controlPntN;
	Point controlPnts[4];
	Bezier() {
		controlPntN = 0;
	}
	Bezier(int controlPntNInput, Point controlPntInput[]) {
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
	Mat drawBezier(Mat canvas, int radius, Scalar color) {
		double i;
		Mat result;
		canvas.copyTo(result);
		for (i = 0; i < 1.0; i += BEZIER_MIN_T) {
			circle(result, getPnt(i), radius, color, -1);
		}
		return result;
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
		binaryMask = drawBezier(binaryMask, 2, Scalar(255));
		evalImg = binaryMask & evalImg;
		return (double)norm(sum(evalImg)) / (double)norm(sum(binaryMask));
	}
};

class bezierEvaluator : evaluator {
	Mat targetImg;
	bezierEvaluator(Mat targetImgInput) {
		targetImg = targetImgInput;
	}
	double evaluate(vector<double> parameters) {
		Bezier curve;
		int i;
		if (parameters.size() != 8) {
			printf("bezierEvaluator evaluate error! parameter size doesn't match bezier form!\n");
			printf("size must be 8 but parameter size is %d\n", parameters.size());
			return -1;
		}
		else {
			int N = 4;
			Point pntArr[4];
			for (i = 0; i < N; i++) {
				pntArr[i] = Point(parameters[i*2], parameters[i * 2 + 1]);
			}
			curve = Bezier(N, pntArr);
		}
		return curve.evaluate(targetImg);
	}
};
#endif