#pragma once
#include<stdio.h>
#include<math.h>
#include<vector>
#include<math.h>
#ifndef __OPTIMIZATION_H
#define __OPTIMIZATION_H
#define MAX 99999999

using namespace std;


class evaluator {//Note to self: there might be problems if the element of direction vector is smaller than once since less than one pixel would be used as 0 pixels.
	//Must make sure that all direction vector is either bigger than one or zero
	//test with step size? digitization?
public:
	virtual double eval(vector<double> parameter) = 0;
};

class optimizer {
protected:
	int dimensionN;
	double thresh;
	double stepSize;
	int maxStep;
	evaluator* evaluatorInst;
public:
	optimizer(int numX, double threshInput, double stepSizeInput, int maxStepInput, evaluator* evaluatorInstInput) {
		dimensionN = numX;
		thresh = threshInput;
		stepSize = stepSizeInput;
		maxStep = maxStepInput;
		evaluatorInst = evaluatorInstInput;
	}
	virtual vector<double> run(vector<double> parameters) = 0;
};

class derivativeBasedOptimizer : public optimizer {
	double alpha;
	vector<double> getGradientApprox(vector<double> parameter);//get approximation of gradient by changing each parameter by stepsize and dividing the change in y value with stepsize
public:
	derivativeBasedOptimizer(int numX, double threshInput, double alphaInput, double stepSizeInput, int maxStepInput, evaluator* evaluatorInstInput) :optimizer::optimizer(numX, threshInput, stepSizeInput, maxStepInput, evaluatorInstInput) {
		alpha = alphaInput;
	}
};

class swarmBasedOptimizer : public optimizer {
public:
	swarmBasedOptimizer(int numX, double threshInput, double stepSizeInput, int maxStepInput, evaluator* evaluatorInstInput) :optimizer::optimizer(numX, threshInput, stepSizeInput, maxStepInput, evaluatorInstInput) {}
};

#endif