#pragma once
#include<vector>
#include"optimization.h"
using namespace std;

vector<short> hillClimber::intToDirection(int num) {
	vector<short> direction;
	int i, j;
	int tmp = num;
	for (i = 0; i < numX; i++) {
		direction.push_back(0);
	}
	for (i = 0; i < numX; i++) {
		int remainder = tmp % 3;
		tmp /= 3;
		direction[i] = remainder-1;
	}
	return direction;
}

vector<double> hillClimber::findBestDir(vector<double> current) {
	int i, j;
	int max = (int)pow(3.0, numX);
	double currentEval = evaluatorInst.eval(current);
	double maxDiff=0;
	vector<double> bestDir;
	for (i = 0; i < max; i++) {
		vector<short> shortDir = intToDirection(i);
		double vecNorm=0;
		for (j = 0; j < numX; j++) {
			vecNorm += shortDir[i] * shortDir[i];
		}
		vector<double> doubleDir;
		vector<double> currentPDir;//current+direction
		for (j = 0; j < numX; j++) {
			doubleDir.push_back(shortDir[j] / vecNorm);
			currentPDir.push_back(current[j] + doubleDir[j]);
		}
		double dirEval = evaluatorInst.eval(currentPDir);
		if (maxDiff < dirEval - currentEval) {
			maxDiff = dirEval - currentEval;
			bestDir = doubleDir;
		}
	}
	return bestDir;
}

bool hillClimber::shouldGoThisDirection(vector<double> direction, vector<double> current) {
	vector<double> next;
	int i, j;
	for (i = 0; i < numX; i++) {
		next.push_back(current[i] + direction[i]);
	}
	if (evaluator.eval(next) > evaluator.eval(current)) {
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
	for (i = 0; i < numX; i++) {
		current.push_back(parameters[i]);
	}
	direction = findBestDir(current);
	for (step = 0; step < maxStep; step++) {
		if (evaluatorInst.eval(current) < thresh) {
			break;
		}
		if (shouldGoThisDirection(direction, current)) {
			int i, j;
			for (i = 0; i < numX; i++) {
				direction[i] += direction[i];
			}
		}
		else {
			direction = findBestDir(current);
		}
	}
	return current;
}