#ifndef POINT_H
#define POINT_H

/* Class Point3 represents a 3D vector with coordinates (x, y, z)
* and provides methods for comparison with other vectors. */

class Point3 {
public:
	Point3(double x, double y, double z);
	Point3(double x, double y, double z, bool isSupportPoint);
	Point3(Point3* point, double lengthAfterProjection);
	~Point3() = default;

	bool Equals(Point3 point);

	double X, Y, Z = 0.0;
	int Id = 0;
	bool IsVisited = false;
	bool IsSupportPoint = false;
private:
	int GenerateNextId();
};
#endif
