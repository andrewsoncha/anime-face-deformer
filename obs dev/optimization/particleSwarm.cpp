#include "optimization.h"
#include <vector>
using namespace std;

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

class particle{
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
		velocity = velocity * w + (pastBestPoint-currentPoint) * cognitiveCoeff + (swarmBest-currentPoint) * socialCoeff;
		currentPoint = currentPoint + velocity;
		currentVal = evaluatorInst->eval(this->currentPoint.coordinates);
		if (pastBestVal > currentVal) {
			pastBestVal = currentVal;
			pastBestPoint = currentPoint;
		}
	}
};

class swarm{
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
		for (auto x:swarmPoints) {
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
};

particleSwarm:: particleSwarm(int numX, double alphaInput, double threshInput, double stepSizeInput, int maxStepInput, double cognitiveCoeffInput, double socialCoeffInput, evaluator* evaluatorInstInput) :optimizer::optimizer(numX, alphaInput, threshInput, stepSizeInput, maxStepInput, evaluatorInstInput){
	cognitiveCoefficient = cognitiveCoeffInput;
	socialCoefficient = socialCoeffInput;
	swarmInst = //TODO: divide optimizer classes into derivative using ones and derivative-free ones.
}

vector<double> particleSwarm::run(vector<double> parameters) {//in this case the parameters are NOT INITIAL VALUES. they represent the maximum values each coordinate of particles can take(minimum is 0).
															//TODO: This is not a very user-friendly way to implement this. Find way to remove the vector<double> parameters from the parameter and replace it with something of a name and type that fits the purpose&function
																	//maybe use Templates to define the run method?
	int i;
	double iterBest;
	for (i = 0; i < maxStep; i++) {
		swarmInst.runOneIter(w, cognitiveCoefficient, socialCoefficient);
	}
}