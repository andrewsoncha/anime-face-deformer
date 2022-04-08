#pragma once
#include<stdio.h>
#include<math.h>
#include<vector>
#include<math.h>
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

class evaluator {//Note to self: there might be problems if the element of direction vector is smaller than once since less than one pixel would be used as 0 pixels.
	//Must make sure that all direction vector is either bigger than one or zero
	//test with step size? digitization?

};

class optimizer {
protected:
	double alpha;
	int dimensionN;
	double alpha;
	double thresh;
	double stepSize;
	int maxStep;
public:
	optimizer(int numX, double alphaInput, double threshInput, double stepSizeInput, int maxStepInput) {
		dimensionN = numX;
		alpha = alphaInput;
		thresh = threshInput;
		stepSize = stepSizeInput;
		maxStep = maxStepInput;
	}
	virtual vector<double> run(vector<double> parameters);
};

class hillClimber : public optimizer {//actually hook-jeeves algorithm. step size is constant(=alpha).
	vector<short> intToDirection(int num);
	vector<double> findBestDir(vector<double> current);
	bool shouldGoThisDirection(vector<double> direction, vector<double> current);
};

class gradientDescent : public optimizer {//step size is proportionate to alpha value and gradient

};

vector<int> hillClimb(int numX, vector<int> values, evaluator evaluatorInst) {
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