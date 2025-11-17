#include <cmath>
#include <string>
#include <point.h>

using namespace std;

Point3::Point3(double x, double y, double z)
	:X(x), Y(y), Z(z) {
	Id = GenerateNextId();
}

Point3::Point3(double x, double y, double z, bool isSupportPoint)
	:X(x), Y(y), Z(z), IsSupportPoint(isSupportPoint) {
	Id = GenerateNextId();
}

Point3::Point3(Point3* point, double lengthAfterProjection) {
	Id = point->Id;
	IsVisited = point->IsVisited;
	IsSupportPoint = point->IsSupportPoint;

	double length = sqrt(pow(point->X, 2) + pow(point->Y, 2) + pow(point->Z, 2));
	double scaleFactor = lengthAfterProjection / length;

	X = scaleFactor * point->X;
	Y = scaleFactor * point->Y;
	Z = scaleFactor * point->Z;
}

int Point3::GenerateNextId() {
	static int id = 0;
	return id++;
}

bool Point3::Equals(Point3 point) {
	return (X == point.X && Y == point.Y && Z == point.Z);
}