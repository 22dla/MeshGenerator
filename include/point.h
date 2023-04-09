#ifndef POINT_H
#define POINT_H

/* Class Vec3 represents a 3D vector with coordinates (x, y, z)
* and provides methods for comparison with other vectors. */

class Vec3 {
public:
	Vec3(double x, double y, double z);
	Vec3(double x, double y, double z, bool isSupportPoint);
	Vec3(Vec3* point, double lengthAfterProjection);
	~Vec3() = default;

	bool Equals(Vec3 point);

	double X, Y, Z = 0.0;
	int Id = 0;
	bool IsVisited = false;
	bool IsSupportPoint = false;
private:
	int GenerateNextId();
};
#endif
