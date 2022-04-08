#pragma once
#include<vector>
#include"optimization.h"
using namespace std;

vector<double> gradientDescent::run(vector<double> parameter) {
	vector<int> parameters;
	vector<double> gradient;
	int i, j;
	int step = 0;
	for (i = 0; i < numX; i++) {
		parameters.push_back(values[i]);
	}
	for (step = 0; step < maxStep; step++) {
		double currentVal = evaluatorInst.eval(parameters);
		double gradNorm;
		for (i = 0; i < numX; i++) {
			parameters[i] += alpha;
			gradient.push_back(evaluatorInst.eval(parameters));
			parameters[i] -= alpha;
		}
		gradNorm = norm(gradient);
		for (i = 0; i < numX; i++) {
			parameters[i] += (int)(partialDerivatives[i] * stepSize / numX);
		}
		if (evaluatorInst.eval(parameters) < thresh) {
			break;
		}
	}
	return parameters;
}