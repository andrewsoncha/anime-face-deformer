#pragma once
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include"optimization/optimization.h"
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
		double maxKappa=0.0;
		Point maxKappaPnt;
		canvas.copyTo(result);
		printf("controlPnt 0 and 3: (%d %d) (%d %d)\n", controlPnts[0].x, controlPnts[0].y, controlPnts[3].x, controlPnts[3].y);
		printf("drawBezier start, end (%d %d) (%d %d)\n", getPnt(0.0).x, getPnt(0.0).y, getPnt(1.0).x, getPnt(1.0).y);
		for (i = 0; i < 1.0; i += BEZIER_MIN_T) {
			double kappa = getKappa(i);
			circle(result, getPnt(i), radius, color, -1);
			if (maxKappa < kappa) {
				maxKappa = kappa;
				maxKappaPnt = getPnt(i);
			}
			circle(result, getPnt(i), radius, color, -1);
			printf("t:%lf    kappa:%lf\n", i, kappa);
		}
		circle(result, maxKappaPnt, 5, Scalar(0, 0, 255), -1);
		printf("maxKappaPnt:%lf\n", maxKappa);
		return result;
	}
	Point getPnt(double t) {
		int i, j;
		double x, y;
		x = 0; y = 0;
		for (i = 0; i < 4; i++) {
			double exp = 1.0;
			//printf("i:%d\n", i);
			for (j = 0; j < i; j++) {
				//printf("1.0-%lf:%lf multiplied\n", t, 1.0 - t);
				exp *= (1.0 - t);
			}
			for (j = i; j < 3; j++) {
				//printf("%lf multiplied\n", t);
				exp *= t;
			}
			if (i == 1 || i == 2) {
				exp *= 3;
			}
			//printf("(%lf %lf)\n", controlPnts[i].x * exp, controlPnts[i].y * exp);
			x += controlPnts[i].x * exp;
			y += controlPnts[i].y * exp;
			/*if (t > 0.99) {
				printf("i:%d\n", i);
				printf("exp:%lf\n", exp);
				printf("t:%lf 1.0-t:%lf\n", t, 1.0 - t);
			}*/
		}
		//printf("t:%lf x:%lf y:%lf\n", t, x, y);
		return Point(x, y);
	}
	Point2f getPnt2f(double t) {
		int i, j;
		double x, y;
		x = 0; y = 0;
		for (i = 0; i < 4; i++) {
			double exp = 1.0;
			//printf("i:%d\n", i);
			for (j = 0; j < i; j++) {
				//printf("1.0-%lf:%lf multiplied\n", t, 1.0 - t);
				exp *= (1.0 - t);
			}
			for (j = i; j < 3; j++) {
				//printf("%lf multiplied\n", t);
				exp *= t;
			}
			if (i == 1 || i == 2) {
				exp *= 3;
			}
			//printf("(%lf %lf)\n", controlPnts[i].x * exp, controlPnts[i].y * exp);
			x += controlPnts[i].x * exp;
			y += controlPnts[i].y * exp;
			/*if (t > 0.99) {
				printf("i:%d\n", i);
				printf("exp:%lf\n", exp);
				printf("t:%lf 1.0-t:%lf\n", t, 1.0 - t);
			}*/
		}
		//printf("t:%lf x:%lf y:%lf\n", t, x, y);
		return Point2f(x, y);
	}
	void getDiffs(double *diffInput, double t) {
		double xDiff, yDiff;
		Point2f currentPnt, laterPnt;
		currentPnt = getPnt2f(t);
		laterPnt = getPnt2f(t + BEZIER_MIN_T);
		xDiff = (laterPnt.x - currentPnt.x) / BEZIER_MIN_T;
		yDiff = (laterPnt.y - currentPnt.y) / BEZIER_MIN_T;
		//printf("xDiff:%lf yDiff:%lf\n", xDiff, yDiff);
		diffInput[0] = xDiff;
		diffInput[1] = yDiff;
	}
	double getKappa(double t) {
		double currentDiffs[2], nextDiffs[2];
		double TPrime[2];
		getDiffs(currentDiffs, t);
		//printf("currentDiffs:%lf %lf\n", currentDiffs[0], currentDiffs[1]);
		getDiffs(nextDiffs, t + BEZIER_MIN_T);
		//printf("nextDiffs:%lf %lf\n", nextDiffs[0], nextDiffs[1]);
		//printf("diffDiff = %lf %lf\n", currentDiffs[0] - nextDiffs[0], currentDiffs[1] - nextDiffs[1]);
		TPrime[0] = (nextDiffs[0] - currentDiffs[0]) / BEZIER_MIN_T;
		TPrime[1] = (nextDiffs[1] - currentDiffs[1]) / BEZIER_MIN_T;
		return sqrt(TPrime[0] * TPrime[0] + TPrime[1] + TPrime[1]) / sqrt(currentDiffs[0] * currentDiffs[0] + currentDiffs[1] * currentDiffs[1]);//K=|T'|/|r'|
	}
	double evaluate(Mat img) {
		Mat originalImg, evalImg;
		Mat binaryMask = Mat::zeros(img.size(), CV_8UC3);
		double value;
		printf("bezier::evaluate!!\n");
		imshow("bazier::evaluate img", img);
		cvtColor(img, originalImg, COLOR_GRAY2BGR);
		printf("drawing binaryMask!!\n");
		binaryMask = drawBezier(binaryMask, 2, Scalar(255,255,255));
		printf("done drawing binaryMask!!\n");
		imshow("binaryMask", binaryMask);
		imshow("originalImg", originalImg);
		waitKey(1000);
		Mat compareResult;
		addWeighted(binaryMask, 0.5, originalImg, 0.5, 0, compareResult);
		imshow("evalMask&binaryMask", compareResult);
		waitKey(1000);
		evalImg = binaryMask & originalImg;
		imshow("evalImg", evalImg);
		waitKey(10);
		printf("eval result:%lf\n", (double)norm(sum(evalImg)) / (double)norm(sum(binaryMask)));
		return (double)norm(sum(evalImg)) / (double)norm(sum(binaryMask));
	}
};

class bezierEvaluator : public evaluator {
	Mat targetImg;
	double eval(vector<double> parameters) override{
		Bezier curve;
		int i;
		printf("bezierEvaluator eval:\n");
		for (i = 0; i < parameters.size(); i++) {
			printf("%lf ", parameters[i]);
		}
		printf("\n");
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
public:
	bezierEvaluator(Mat targetImgInput) {
		targetImg = targetImgInput;
	}
};
#endif