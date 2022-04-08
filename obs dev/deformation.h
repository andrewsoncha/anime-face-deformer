#pragma once
#include<vector>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<math.h>
#ifndef __DEFORMATION_H
#define __DEFORMATION_H

using namespace cv;
using namespace std;

class Polygon {
public:
	int pointIdx[3];
	Polygon(int a, int b, int c) {
		pointIdx[0] = a;
		pointIdx[1] = b;
		pointIdx[2] = c;
		printf("polygon: %d %d %d\n", a, b, c);
	}
};

class Mesh {
public:
	int polygonN;
	vector<Polygon> polygonList;
	Mesh() {}
	Mesh(vector<Point> pointList, int generationMode = 1) {//generationMode 1: delauney triangulation
		polygonN = pointList.size();
	}
	Mesh(vector<Polygon> inputPolygonList) {
		polygonList = inputPolygonList;
		polygonN = polygonList.size();
	}
	static Mesh rectangular(Size imgSize, int horiN, int vertiN) {
		int i, j;
		int widthUnit, heightUnit;
		Mesh newMesh = Mesh();
		newMesh.polygonN = horiN * vertiN + 2;
		for (i = 0; i < horiN; i++) {
			if (i > 0) {
				for (j = 0; j < vertiN - 1; j++) {
					printf("i:%d j:%d\n", i, j);
					newMesh.polygonList.push_back(Polygon((i - 1) * vertiN + j, (i - 1) * vertiN + j + 1, i * vertiN + j));
					newMesh.polygonList.push_back(Polygon((i - 1) * vertiN + j + 1, i * vertiN + j, i * vertiN + j + 1));
				}
			}
		}
		return newMesh;
	}
	Mesh loadMesh(char* path) {
		/*FILE* fInput = fopen(path, "r");
		int N;
		int tmp[3];
		*/
	}
};

class meshVertexes {
public:
	int vertexN;
	vector<Point> vertexPoints;
	meshVertexes() {}
	meshVertexes(vector<Point> inputPointList) {
		vertexPoints = inputPointList;
		vertexN = vertexPoints.size();
	}
	static meshVertexes copy(meshVertexes original) {//copy
		meshVertexes newCopy = meshVertexes();
		newCopy.vertexPoints = original.vertexPoints;
		newCopy.vertexN = original.vertexN;
		return newCopy;
	}
	static meshVertexes rectangular(Size imgSize, int horiN, int vertiN) {
		int i, j;
		int widthUnit, heightUnit;
		meshVertexes newVertex;
		newVertex.vertexN = horiN * vertiN;
		widthUnit = imgSize.width / (horiN - 1);
		heightUnit = imgSize.height / (vertiN - 1);
		for (i = 0; i < horiN; i++) {
			for (j = 0; j < vertiN; j++) {
				newVertex.vertexPoints.push_back(Point(i * (widthUnit), j * (heightUnit)));
			}
		}
		return newVertex;
	}
};

Mat deformPolygon(Mat img, Polygon polygon, meshVertexes initVertexes, meshVertexes goalVertexes) {
	Mat polygonMap, onlyPolygon;
	Mat affineMatrix, deformationResult;
	printf("deform polygon!\n");

	polygonMap = Mat::zeros(img.size(), CV_8UC3);
	Point polyList[1][3];
	polyList[0][0] = initVertexes.vertexPoints[polygon.pointIdx[0]];
	polyList[0][1] = initVertexes.vertexPoints[polygon.pointIdx[1]];
	polyList[0][2] = initVertexes.vertexPoints[polygon.pointIdx[2]];
	const Point* ppt[1] = { polyList[0] };
	int npt[] = { 3 };
	fillPoly(polygonMap, ppt, npt, 1, Scalar(255, 255, 255), 8);
	//imshow("polygonMap", polygonMap);
	//waitKey(1);
	bitwise_and(img, polygonMap, onlyPolygon);
	//imshow("onlyPolygon", onlyPolygon);
	//waitKey(1);

	vector<Point2f> initialTri, goalTri;
	cout << initVertexes.vertexPoints[polygon.pointIdx[0]];
	cout << initVertexes.vertexPoints[polygon.pointIdx[1]];
	cout << initVertexes.vertexPoints[polygon.pointIdx[2]];

	cout << goalVertexes.vertexPoints[polygon.pointIdx[0]];
	cout << goalVertexes.vertexPoints[polygon.pointIdx[1]];
	cout << goalVertexes.vertexPoints[polygon.pointIdx[2]];
	initialTri.push_back(initVertexes.vertexPoints[polygon.pointIdx[0]]);
	initialTri.push_back(initVertexes.vertexPoints[polygon.pointIdx[1]]);
	initialTri.push_back(initVertexes.vertexPoints[polygon.pointIdx[2]]);
	goalTri.push_back(goalVertexes.vertexPoints[polygon.pointIdx[0]]);
	goalTri.push_back(goalVertexes.vertexPoints[polygon.pointIdx[1]]);
	goalTri.push_back(goalVertexes.vertexPoints[polygon.pointIdx[2]]);
	printf("initialTri:%d   goalTri:%d\n", initialTri.size(), goalTri.size());

	affineMatrix = getAffineTransform(initialTri, goalTri);
	cout << affineMatrix;
	warpAffine(onlyPolygon, deformationResult, affineMatrix, img.size(), INTER_LINEAR);
	//imshow("deformationResult", deformationResult);
	//waitKey(1);
	return deformationResult;
}

Mat meshDeform(Mat inputImg, Mesh meshNet, meshVertexes initVertexes, meshVertexes goalVertexes) {
	int i;
	int polygonN = meshNet.polygonN;
	Mat result = Mat::zeros(inputImg.size(), inputImg.type());
	printf("mesh Deform!\n");
	for (i = 0; i < polygonN; i++) {
		Mat polygonDeformed = deformPolygon(inputImg, meshNet.polygonList[i], initVertexes, goalVertexes);
		//imshow("polygonDeformed", polygonDeformed);
		//waitKey(10);
		add(result, polygonDeformed, result);//TODO: find the way to get rid of the bright lines. probably have to do with overlapping lines in the binary maps.
		//maybe change the interpolation constants?
	}
	//imshow("meshDeformed result", result);
	//waitKey(1);
	return result;
}

Mat drawMeshNVertex(Mat inputImg, Mesh meshNet, meshVertexes vertex) {
	int i;
	Mat resultImg = Mat::zeros(inputImg.size(), inputImg.channels());
	inputImg.copyTo(resultImg);
	for (i = 0; i < meshNet.polygonN; i++) {
		line(resultImg, vertex.vertexPoints[meshNet.polygonList[i].pointIdx[0]], vertex.vertexPoints[meshNet.polygonList[i].pointIdx[1]], Scalar(0, 255, 0));
		line(resultImg, vertex.vertexPoints[meshNet.polygonList[i].pointIdx[0]], vertex.vertexPoints[meshNet.polygonList[i].pointIdx[2]], Scalar(0, 255, 0));
		line(resultImg, vertex.vertexPoints[meshNet.polygonList[i].pointIdx[1]], vertex.vertexPoints[meshNet.polygonList[i].pointIdx[2]], Scalar(0, 255, 0));
		circle(resultImg, vertex.vertexPoints[meshNet.polygonList[i].pointIdx[0]], 3, Scalar(255, 0, 0));
		circle(resultImg, vertex.vertexPoints[meshNet.polygonList[i].pointIdx[2]], 3, Scalar(255, 0, 0));
		circle(resultImg, vertex.vertexPoints[meshNet.polygonList[i].pointIdx[1]], 3, Scalar(255, 0, 0));
	}
	/*for (i = 0; i < vertex.vertexN; i++) {
		circle(resultImg, vertex.vertexPoints[i], 5, Scalar(255, 0, 0), -1);
	}*/
	return resultImg;
}

void loadMeshFromImg(String imgPath, Mesh* resultMesh, meshVertexes* initialVertexes) {
	Mat pointImg = imread(imgPath, IMREAD_GRAYSCALE), binaryPointImg;
	Mat locations;
	Subdiv2D subdiv(Rect(0, 0, pointImg.cols, pointImg.rows));
	vector<Point> pointList;
	int i, j;

	threshold(pointImg, binaryPointImg, 125, 255, THRESH_BINARY);
	findNonZero(binaryPointImg, locations);
	for (i = 0; i < locations.rows; i++) {
		pointList.push_back(Point2f(locations.at<int>(i, 0), locations.at<int>(i, 1)));
		subdiv.insert(pointList[i]);
	}
	vector<Vec6f> triangleList;
	subdiv.getTriangleList(triangleList);

	vector<Polygon> polygonList;

	printf("triangleList size:%d\n", triangleList.size());
	for (i = 0; i < triangleList.size(); i++) {
		int x, y;
		int a, b, c;

		x = triangleList[i][0]; y = triangleList[i][1];
		for (j = 0; j < pointList.size(); j++) {
			if (pointList[j].x == x && pointList[j].y == y) {
				a = j;
				break;
			}
		}

		x = triangleList[i][2]; y = triangleList[i][3];
		for (j = 0; j < pointList.size(); j++) {
			if (pointList[j].x == x && pointList[j].y == y) {
				b = j;
				break;
			}
		}

		x = triangleList[i][4]; y = triangleList[i][5];
		for (j = 0; j < pointList.size(); j++) {
			if (pointList[j].x == x && pointList[j].y == y) {
				c = j;
				break;
			}
		}

		Polygon newPoly = Polygon(a, b, c);
		polygonList.push_back(newPoly);
	}
	Mesh newMesh = Mesh(polygonList);
	meshVertexes newVertexes = meshVertexes(pointList);

	*resultMesh = newMesh;
	*initialVertexes = newVertexes;
}

meshVertexes deformedMesh(meshVertexes originalMesh, double a) {
	int i;
	int sumX, sumY, avgX, avgY;
	sumX = 0; sumY = 0;
	for (i = 0; i < originalMesh.vertexN; i++) {
		sumX += originalMesh.vertexPoints[i].x;
		sumY += originalMesh.vertexPoints[i].y;
	}
	avgX = sumX / originalMesh.vertexN;
	avgY = sumY / originalMesh.vertexN;

	meshVertexes newVertex(originalMesh);
	for (i = 0; i < originalMesh.vertexN; i++) {
		//newVertex.vertexPoints[i].y += (newVertex.vertexPoints[i].x > avgX) ? a*(avgX - newVertex.vertexPoints[i].x) : a*(newVertex.vertexPoints[i].x - avgX);
		newVertex.vertexPoints[i].y = (avgY - newVertex.vertexPoints[i].y) * 2 * (a - 0.5) + avgY;
	}
	return newVertex;
}

#endif