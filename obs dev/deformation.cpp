#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<vector>
using namespace std;
using namespace cv;
class Polygon {
	Point trianglePoints[3];
};
class Mesh {
	vector<Polygon> polygonList;
public:
	Mesh(vector<Point> pointList, int generationMode=1) {//generationMode 1: delauney triangulation

	}
};

Mat deformPolygon(Mat img, Polygon initialPolygon, Polygon goalPolygon) {
	Mat polygonMap, onlyPolygon;
	Mat affineMatrix, deformationResult;

	polygonMap = Mat::zeros(img.size(), CV_8UC1);
	Point polyList[1][3];
	polyList[0][0] = initialPolygon.trianglePoints[0];
	polyList[0][1] = initialPolygon.trianglePoints[1];
	polyList[0][2] = initialPolygon.trianglePoints[2];
	drawPoly()
}

Mat meshDeform(Mat inputImg, Mesh initialMesh, Mesh goalMesh) {

}