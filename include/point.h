#ifndef POINT_H
#define POINT_H

// Struct Point3 represents a 3D point with coordinates (x, y, z)
// and provides methods for comparison with other points.

struct Point3 {
	double X = 0.0;
	double Y = 0.0;
	double Z = 0.0;
	int Id = 0;
	bool IsVisited = false;
	bool IsSupportPoint = false;

	// Default constructor
	Point3() = default;

	// Constructors
	Point3(double x, double y, double z);
	Point3(double x, double y, double z, bool isSupportPoint);
	Point3(const Point3& point, double lengthAfterProjection);

	// Methods
	bool Equals(const Point3& point) const;

private:
	static int GenerateNextId();
};

#endif
