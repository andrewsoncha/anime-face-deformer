#pragma once
#include"optimization.h"

vector<double> derivativeBasedOptimizer::getGradientApprox(vector<double> parameter) {
	vector<double> gradient;
	vector<double> parameterChanged = parameter;
	double plusVal, minusVal, changeVal;
	int i;
	for (i = 0; i < dimensionN; i++) {
		parameterChanged[i] = parameter[i] + stepSize/2;
		plusVal = evaluatorInst->eval(parameterChanged);
		parameterChanged[i] = parameter[i] - stepSize/2;
		minusVal = evaluatorInst->eval(parameterChanged);
		changeVal = (plusVal - minusVal) / stepSize;
		parameterChanged[i] = parameter[i];
		gradient.push_back(changeVal);
	}
	return gradient;
}

class hillClimber : public derivativeBasedOptimizer {//actually hook-jeeves algorithm. step size is constant
	vector<short> intToDirection(int num);
	vector<double> findBestDir(vector<double> current);
	bool shouldGoThisDirection(vector<double> direction, vector<double> current);
public:
	hillClimber(int numX, double alphaInput, double threshInput, double stepSizeInput, int maxStepInput, evaluator* evaluatorInstInput);
	vector<double> run(vector<double> parameter) override;
};

class gradientDescent : public derivativeBasedOptimizer {//step size is proportionate to alpha value and gradient
public:
	gradientDescent(int numX, double alphaInput, double threshInput, double stepSizeInput, int maxStepInput, evaluator* evaluatorInstInput);
	vector<double> run(vector<double> parameter) override;
};