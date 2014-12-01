//toolset for linear algebra transformations
//copyright WillieWonka 2014

#include "LinearAlgebra.h"

//rotates given vector with a given angle in radians
Point RotateVector(float angle, Point Vector, unsigned char axis)
{
	Point v;

	double s = sin(angle);
	double c = cos(angle);

	switch (axis)
	{
	case 'x':
		v.x = Vector.x;
		v.y = Vector.y * c - Vector.z * s;
		v.z = Vector.y * s + Vector.z * c;
		break;
	case 'y':
		v.x = Vector.x * c + Vector.z * s;
		v.y = Vector.y;
		v.z = Vector.z * c - Vector.x * s;
		break;
	case 'z':
		v.x = Vector.x * c - Vector.y * s;
		v.y = Vector.x * s + Vector.y * c;
		v.z = Vector.z;
		break;
	}

	return v;
}

//rotates vector in direction of vector
Point RotateVector(Point dir, Point Vector)
{
	Point v;

	double length = sqrt(pow(Vector.x, 2) + pow(Vector.y, 2) + pow(Vector.y, 2));

	v.x = dir.x * length;
	v.y = dir.y * length;
	v.z = dir.z * length;

	return v;
}

//normalize given vector
Point Normalize(Point Vector)
{
	Point v;

	double length = sqrt(pow(Vector.x, 2) + pow(Vector.y, 2) + pow(Vector.y, 2));

	v.x = Vector.x / length;
	v.y = Vector.y / length;
	v.z = Vector.z / length;

	return v;
}

//copies a point
void CopyPoint(const Point source, Point* destination)
{
	destination->x = source.x;
	destination->y = source.y;
	destination->z = source.z;
}

//evaluates if two points are identical
bool PointIdentical(Point p1, Point p2)
{
	//round off to 1 decimal
	p1.x = floor(p1.x * 0.1) / 0.1;
	p1.y = floor(p1.y * 0.1) / 0.1;
	p1.z = floor(p1.z * 0.1) / 0.1;

	p2.x = floor(p2.x * 0.1) / 0.1;
	p2.y = floor(p2.y * 0.1) / 0.1;
	p2.z = floor(p2.z * 0.1) / 0.1;

	return (p1.x == p2.x && p1.y == p2.y && p1.z == p2.z);
}

float ToRadians(float degrees)
{
	return degrees*(M_PI / 180);
}

Point ConvertToPoint(const float vector[2])
{
	Point p;

	p.x = vector[0];
	p.y = vector[1];
	p.z = 0;

	return p;
}

float GetAngle(Point v)
{
	return atan2(v.y, v.x) * 180 / M_PI;
}

float GetLength(Point v)
{
	return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
}

Point PointMult(Point v, const float c)
{
	v.x *= c;
	v.y *= c;
	v.z *= c;
	return v;
}