#include <cmath>
#include <string>
#include <point.h>

using namespace std;

Vec3::Vec3(double x, double y, double z)
	:X(x), Y(y), Z(z) {
	Id = GenerateNextId();
}

Vec3::Vec3(double x, double y, double z, bool isSupportPoint)
	:X(x), Y(y), Z(z), IsSupportPoint(isSupportPoint) {
	Id = GenerateNextId();
}

Vec3::Vec3(Vec3* point, double lengthAfterProjection) {
	Id = point->Id;
	IsVisited = point->IsVisited;
	IsSupportPoint = point->IsSupportPoint;

	double length = sqrt(pow(point->X, 2) + pow(point->Y, 2) + pow(point->Z, 2));
	double scaleFactor = lengthAfterProjection / length;

	X = scaleFactor * point->X;
	Y = scaleFactor * point->Y;
	Z = scaleFactor * point->Z;
}

int Vec3::GenerateNextId() {
	static int id = 0;
	return id++;
}

bool Vec3::Equals(Vec3 point) {
	return (X == point.X && Y == point.Y && Z == point.Z);
}