#pragma once
#include<stdio.h>
#include<math.h>
#include<vector>
#include<math.h>
#ifndef __OPTIMIZATION_H
#define __OPTIMIZATION_H

using namespace std;


class evaluator {//Note to self: there might be problems if the element of direction vector is smaller than once since less than one pixel would be used as 0 pixels.
	//Must make sure that all direction vector is either bigger than one or zero
	//test with step size? digitization?
public:
	virtual double eval(vector<double> parameter) = 0;
};

class optimizer {
protected:
	double alpha;
	int dimensionN;
	double thresh;
	double stepSize;
	int maxStep;
	evaluator* evaluatorInst;
public:
	optimizer(int numX, double alphaInput, double threshInput, double stepSizeInput, int maxStepInput, evaluator* evaluatorInstInput) {
		dimensionN = numX;
		alpha = alphaInput;
		thresh = threshInput;
		stepSize = stepSizeInput;
		maxStep = maxStepInput;
		evaluatorInst = evaluatorInstInput;
	}
	virtual vector<double> run(vector<double> parameters) = 0;
};

class hillClimber : public optimizer {//actually hook-jeeves algorithm. step size is constant(=alpha).
	vector<short> intToDirection(int num);
	vector<double> findBestDir(vector<double> current);
	bool shouldGoThisDirection(vector<double> direction, vector<double> current);
public:
	hillClimber(int numX, double alphaInput, double threshInput, double stepSizeInput, int maxStepInput, evaluator* evaluatorInstInput);
	vector<double> run(vector<double> parameter) override;
};

class gradientDescent : public optimizer {//step size is proportionate to alpha value and gradient
public:
	gradientDescent(int numX, double alphaInput, double threshInput, double stepSizeInput, int maxStepInput, evaluator* evaluatorInstInput);
	vector<double> run(vector<double> parameter) override;
};

#endif