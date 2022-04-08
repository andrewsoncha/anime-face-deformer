#pragma once
#include<vector>
#include"optimization.h"
using namespace std;

vector<double> gradientDescent::run(vector<double> parameter) {
	vector<double> parameters;
	vector<double> gradient;
	int i, j;
	int step = 0;
	for (i = 0; i < dimensionN; i++) {
		parameters.push_back(parameter[i]);
	}
	for (step = 0; step < maxStep; step++) {
		double currentVal = evaluatorInst.eval(parameters);
		double gradNorm;
		for (i = 0; i < dimensionN; i++) {
			parameters[i] += alpha;
			gradient.push_back(evaluatorInst.eval(parameters));
			parameters[i] -= alpha;
		}
		gradNorm = norm(gradient);
		for (i = 0; i < dimensionN; i++) {
			parameters[i] += (gradient[i] * stepSize / gradNorm);
		}
		if (evaluatorInst.eval(parameters) < thresh) {
			break;
		}
	}
	return parameters;
}