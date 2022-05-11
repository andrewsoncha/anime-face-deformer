#pragma once
#include<vector>
#ifndef __OPTIMIZATION_H
#include"optimization.h"
#endif
using namespace std;

double norm(vector<double> vec) {
	int i;
	double sum = 0;
	for (i = 0; i < vec.size(); i++) {
		sum += vec[i] * vec[i];
	}
	return sqrt(sum);
}

vector<double> makeNormScalar(vector<double> vec, double scalar) {//normalizes the given vector and multiplies the scalar inputted, making the norm of the vector the inputted scalar
	double vecNorm;
	int i;
	vector<double> resultVec;
	vecNorm = norm(vec);
	for (i = 0; i < vec.size(); i++) {
		resultVec.push_back(vec[i] / vecNorm * scalar);
	}
	return resultVec;
}

gradientDescent::gradientDescent(int numX, double alphaInput, double threshInput, double stepSizeInput, int maxStepInput, evaluator* evaluatorInstInput) :derivativeBasedOptimizer::derivativeBasedOptimizer(numX, alphaInput, threshInput, stepSizeInput, maxStepInput, evaluatorInstInput)
{
}

vector<double> gradientDescent::run(vector<double> parameter) {
	vector<double> parameters;
	vector<double> gradient;
	int i, j;
	int step = 0;
	for (i = 0; i < dimensionN; i++) {
		parameters.push_back(parameter[i]);
	}
	for (step = 0; step < maxStep; step++) {
		double currentVal = evaluatorInst->eval(parameters);
		double gradNorm;
		for (i = 0; i < dimensionN; i++) {
			parameters[i] += alpha;
			gradient.push_back(evaluatorInst->eval(parameters));
			parameters[i] -= alpha;
		}
		gradNorm = norm(gradient);
		for (i = 0; i < dimensionN; i++) {
			parameters[i] += (gradient[i] * stepSize / gradNorm);
		}
		if (evaluatorInst->eval(parameters) < thresh) {
			break;
		}
	}
	return parameters;
}