#pragma once
#include"optimization.h"
class doubleVec {
public:
	int dimX;
	vector<double> coordinates;
	doubleVec() {
		dimX = 0;
		coordinates = vector<double>();
	};
	doubleVec(int dimXInput, vector<double> coordinateInput) {
		dimX = dimXInput;
		coordinates = coordinateInput;
	}
	static doubleVec fillZero(int dimX) {
		int i;
		vector<double> newCoor;
		for (i = 0; i < dimX; i++) {
			newCoor.push_back(0);
		}
		return doubleVec(dimX, newCoor);
	}
	doubleVec inverse() {
		vector<double> resultCoordinates;
		int i;
		for (i = 0; i < dimX; i++) {
			resultCoordinates.push_back(coordinates[i] * -1.0);
		}
		return doubleVec(dimX, resultCoordinates);
	}
	doubleVec operator +(doubleVec const& other) {
		int i;
		vector<double> resultCoordinates;
		for (i = 0; i < dimX; i++) {
			resultCoordinates.push_back(coordinates[i] + resultCoordinates[i]);
		}
		return doubleVec(dimX, resultCoordinates);
	}
	doubleVec operator *(double other) {
		int i;
		vector<double> resultCoordinates;
		for (i = 0; i < dimX; i++) {
			resultCoordinates.push_back(coordinates[i] * other);
		}
		return doubleVec(dimX, resultCoordinates);
	}
	doubleVec operator -(doubleVec other) {
		return *this + other.inverse();
	}
	double abs() {
		int i;
		double sum = 0;
		for (i = 0; i < dimX; i++) {
			sum += coordinates[i] * coordinates[i];
		}
		return sum;
	}
};

class particleSwarm : public swarmBasedOptimizer {
public:
	double cognitiveCoefficient, socialCoefficient;
	double w;
	particleSwarm(int numX, double wInput, double threshInput, double stepSizeInput, int maxStepInput, double cognitiveCoefficientInput, double socialCoefficientInput, evaluator* evaluatorInstInput) :swarmBasedOptimizer::swarmBasedOptimizer(numX, threshInput, stepSizeInput, maxStepInput, evaluatorInstInput) {
		cognitiveCoefficient = cognitiveCoefficientInput;
		socialCoefficient = socialCoefficientInput;
		w = wInput;
	};
	vector<double> run(vector<double> minVals, vector<double> maxVals);
};