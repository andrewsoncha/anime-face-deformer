#pragma once
#include<vector>
#ifndef __OPTIMIZATION_H
#include"derivativeBasedOptimization.h"
#endif
using namespace std;

hillClimber::hillClimber(int numX, double alphaInput, double threshInput, double stepSizeInput, int maxStepInput, evaluator* evaluatorInstInput) :derivativeBasedOptimizer::derivativeBasedOptimizer(numX, alphaInput, threshInput, stepSizeInput, maxStepInput, evaluatorInstInput)
{
}

vector<short> hillClimber::intToDirection(int num) {
	vector<short> direction;
	int i, j;
	int tmp = num;
	for (i = 0; i < dimensionN; i++) {
		direction.push_back(0);
	}
	for (i = 0; i < dimensionN; i++) {
		int remainder = tmp % 3;
		tmp /= 3;
		direction[i] = remainder - 1;
	}
	return direction;
}

vector<double> hillClimber::findBestDir(vector<double> current) {
	int i, j;
	printf("findBestDir\n\n");
	int max = (int)pow(3.0, dimensionN);
	double currentEval = evaluatorInst->eval(current);
	double maxDiff = 0;
	vector<double> bestDir;
	for (i = 0; i < max; i++) {
		vector<short> shortDir = intToDirection(i);
		double vecNorm = 0;
		for (j = 0; j < dimensionN; j++) {
			vecNorm += shortDir[j] * shortDir[j];
		}
		vector<double> doubleDir;
		vector<double> currentPDir;//current+direction
		for (j = 0; j < dimensionN; j++) {
			doubleDir.push_back(shortDir[j] / vecNorm);
			currentPDir.push_back(current[j] + doubleDir[j]);
		}
		/*printf("doubleDir:");
		for (j = 0; j < dimensionN; j++) {
			printf("%lf ", doubleDir[j]);
		}
		printf("\n");*/
		double dirEval = evaluatorInst->eval(currentPDir);
		//printf("dirEval:%lf\n", dirEval);
		if (maxDiff < currentEval - dirEval) {
			maxDiff = currentEval - dirEval;
			bestDir = doubleDir;
		}
	}
	printf("\n\n\nbestDir:");
	for (j = 0; j < dimensionN; j++) {
		printf("%lf ", bestDir[j]);
	}
	printf("\n\n\n");
	return bestDir;
}

bool hillClimber::shouldGoThisDirection(vector<double> direction, vector<double> current) {
	vector<double> next;
	int i, j;
	for (i = 0; i < dimensionN; i++) {
		next.push_back(current[i] + direction[i]);
	}
	if (evaluatorInst->eval(next) < evaluatorInst->eval(current)) {
		return true;
	}
	else {
		return false;
	}
}

vector<double> hillClimber::run(vector<double> parameter) {//basically hook-jeeves. Pick a direction(among 2^n) that has the steepest ascent, keep going until starts to go down, look for new direction, repeat the process
	vector<double> current;
	vector<double> direction;
	int i, j;
	int step = 0;
	if (parameter.size() != dimensionN) {
		printf("ERROR: parameter.size():%d  doesn't match the dimensionN %d of the hillClimber instance!\n", parameter.size(), dimensionN);
	}
	printf("hillClimber run!\n");
	for (i = 0; i < dimensionN; i++) {
		current.push_back(parameter[i]);
	}
	direction = findBestDir(current);
	printf("direction:");
	for (i = 0; i < dimensionN; i++) {
		printf("%lf ", direction[i]);
	}
	printf("\n");
	for (step = 0; step < maxStep; step++) {
		printf("current:\n");
		for (i = 0; i < dimensionN; i++) {
			printf("%lf ", current[i]);
		}
		printf("\n");
		printf("eval value:%lf\n", evaluatorInst->eval(current));
		if (evaluatorInst->eval(current) < thresh) {
			printf("eval value:%lf, under thresh(=%lf), breaking from loop!\n\n", evaluatorInst->eval(current), thresh);
			break;
		}
		if (shouldGoThisDirection(direction, current)) {
			int i, j;
			for (i = 0; i < dimensionN; i++) {
				current[i] += direction[i];
			}
		}
		else {
			direction = findBestDir(current);
			printf("direction:");
			for (i = 0; i < dimensionN; i++) {
				printf("%lf ", direction[i]);
			}
			printf("\n");
		}
	}
	printf("final current:\n");
	for (i = 0; i < dimensionN; i++) {
		printf("%lf ", current[i]);
	}
	printf("\n");
	return current;
}