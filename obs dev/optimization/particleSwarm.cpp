#include "swarmBasedOptimization.h"
#include <vector>
using namespace std;

class particle {
private:
	evaluator* evaluatorInst;
public:
	doubleVec currentPoint;
	double currentVal;
	double pastBestVal;
	doubleVec pastBestPoint;
	doubleVec velocity;
	particle(int dimX, vector<double> initPosition) {
		currentPoint = doubleVec(dimX, initPosition);
		pastBestVal = 0;
		pastBestPoint = doubleVec::fillZero(dimX);
		velocity = doubleVec::fillZero(dimX);
	}
	void gotoNextPnt(double w, double cognitiveCoeff, double socialCoeff, doubleVec swarmBest) {
		velocity = velocity * w + (pastBestPoint - currentPoint) * cognitiveCoeff + (swarmBest - currentPoint) * socialCoeff;
		currentPoint = currentPoint + velocity;
		currentVal = evaluatorInst->eval(this->currentPoint.coordinates);
		if (pastBestVal > currentVal) {
			pastBestVal = currentVal;
			pastBestPoint = currentPoint;
		}
	}
};

class swarm {
	vector<particle> swarmPoints;
	doubleVec swarmBestPoint;
	double swarmBestVal = MAX;
	vector<double> mins, maxs, increment;
	vector<double> loopKDims(int currentDim, int K, vector<double> currentVal) {
		if (currentDim >= K) {
			swarmPoints.push_back(particle(K, currentVal));
		}
		else {
			double i;
			vector<double> nextCoorVal;
			nextCoorVal.push_back(0);
			for (i = mins[currentDim]; i < maxs[currentDim]; i += increment[currentDim]) {
				nextCoorVal[currentDim] = i;
				loopKDims(currentDim + 1, K, nextCoorVal);
			}
		}
	}
public:
	swarm(vector<double> minInput, vector<double> maxInput, vector<double> incrementInput) {
		int N, M;
		int i, j;
		mins = minInput; maxs = maxInput; increment = incrementInput;
	}
	double runOneIter(double w, double cogCoeff, double socialCoeff) {
		doubleVec nextBestPnt = swarmPoints[0].currentPoint;
		double nextBestPntVal = swarmPoints[0].currentVal;
		for (auto x : swarmPoints) {
			x.gotoNextPnt(w, cogCoeff, socialCoeff, swarmBestPoint);
			if (x.currentVal < nextBestPntVal) {
				nextBestPnt = x.currentPoint;
				nextBestPntVal = x.currentVal;
			}
		}
		if (nextBestPntVal < swarmBestVal) {
			swarmBestPoint = nextBestPnt;
			swarmBestVal = nextBestPntVal;
		}
		return swarmBestVal;
	}
	doubleVec getSwarmBestPoint() {
		return swarmBestPoint;
	}
};

vector<double> particleSwarm::run(vector<double> minVals, vector<double> maxVals) {
	int i;
	double iterBest;
	vector<double> increments;
	swarm swarmInst = swarm(minVals, maxVals, increments);
	for (i = 0; i < dimensionN; i++) {
		increments.push_back(stepSize);
	}
	for (i = 0; i < maxStep; i++) {
		swarmInst.runOneIter(w, cognitiveCoefficient, socialCoefficient);
	}
	return swarmInst.getSwarmBestPoint().coordinates;
}