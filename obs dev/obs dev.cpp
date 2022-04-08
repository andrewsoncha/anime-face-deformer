#include<stdio.h>
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
#include"optimization/optimization.h"
#include"bezier.h"
#include "deformation.h"

using namespace std;
using namespace cv;

int main() {
	Mat frame;
	Mesh loadedMesh;
	meshVertexes loadedVertexes;
	frame = imread("eyebrowGreen.png", IMREAD_COLOR);
	imshow("frame", frame);
	waitKey(10);
	loadMeshFromImg("eyebrowPoints with external.png", &loadedMesh, &loadedVertexes);
	for (int i = 0; i < loadedMesh.polygonN; i++) {
		printf("%d %d %d\n", loadedMesh.polygonList[i].pointIdx[0], loadedMesh.polygonList[i].pointIdx[1], loadedMesh.polygonList[i].pointIdx[2]);
	}
	Mat meshImg = drawMeshNVertex(frame, loadedMesh, loadedVertexes);
	imshow("meshImg", meshImg);
	waitKey(1000);

	Mesh rectMesh = Mesh::rectangular(frame.size(), 4, 4);
	meshVertexes rectVertexes = meshVertexes::rectangular(frame.size(), 4,4);
	meshVertexes newVertexes = meshVertexes::copy(rectVertexes);
	Mat result;
	int i, j;
	for (i = 0; i < newVertexes.vertexN; i++) {
		newVertexes.vertexPoints[i].x /= 2;
		newVertexes.vertexPoints[i].y /= 2;
	}
	printf("newVertex set!\n");

	/*for (double alpha = 0.55; alpha < 1; alpha += 0.02) {//todo: try blinking. from alpha>0.5, mesh is inverted by the y-axis
		meshVertexes newVert = deformedMesh(loadedVertexes, alpha);
		result = drawMeshNVertex(frame, loadedMesh, newVert);
		Mat manipulationResult = meshDeform(frame, loadedMesh, loadedVertexes, newVert);
		imshow("manipulationResult", manipulationResult);
		imshow("result", result);
		waitKey(1);
	}
	for (double alpha = 1; alpha >=0.55; alpha -= 0.02) {//todo: try blinking. from alpha>0.5, mesh is inverted by the y-axis
		meshVertexes newVert = deformedMesh(loadedVertexes, alpha);
		result = drawMeshNVertex(frame, loadedMesh, newVert);
		Mat manipulationResult = meshDeform(frame, loadedMesh, loadedVertexes, newVert);
		flip(manipulationResult, manipulationResult, 0);
		imshow("manipulationResult", manipulationResult);
		imshow("result", result);
		waitKey(1);
	}*/
	for (double alpha = 0; alpha < 1; alpha += 0.02) {//todo: try blinking. from alpha>0.5, mesh is inverted by the y-axis
		meshVertexes newVert = deformedMesh(loadedVertexes, alpha);
		result = drawMeshNVertex(frame, loadedMesh, newVert);
		Mat manipulationResult = meshDeform(frame, loadedMesh, loadedVertexes, newVert);
		imshow("manipulationResult", manipulationResult);
		imshow("result", result);
		waitKey(1);
	}
	
	Mat grayFrame;
	Mat binaryImg, distanceMat;
	cvtColor(frame, grayFrame, COLOR_BGR2GRAY);
	threshold(grayFrame, binaryImg, 10,255,THRESH_BINARY_INV);
	printf("\n\n\nchannels:%d\n\n\n", binaryImg.channels());
	imshow("binaryImg", binaryImg);
	waitKey(1000);
	distanceTransform(binaryImg, distanceMat, DIST_L2,DIST_MASK_PRECISE);
	printf("distance transform done!\n");
	normalize(distanceMat, distanceMat, 0,1,NORM_MINMAX);
	imshow("distanceMat", distanceMat);
	waitKey(10000000);
	return 0;
}