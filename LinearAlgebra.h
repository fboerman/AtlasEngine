//header for toolset for linear algebra transformations
//copyright WillieWonka 2014

#ifndef _LINALG_
#define _LINALG_
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>


typedef struct _Point_
{
	float x, y, z;
}Point;

//rotates given vector with a given angle
Point RotateVector(float angle, Point Vector, unsigned char axis);

//rotates vector in direction of vector
Point RotateVector(Point dir, Point Vector);

//normalize given vector
Point Normalize(Point Vector);

//copies a point
void CopyPoint(const Point source, Point* destination);

//evaluates if two points are identical
bool PointIdentical(Point p1, Point p2);

//converts array to point struct
Point ConvertToPoint(const float vector[2]);

//converts degrees to radians
float ToRadians(float degrees);

//calculates the angle of a vector in degrees
float GetAngle(Point v);

//calculates the length of the vector
float GetLength(Point v);

//multiplies vector with constant
Point PointMult(Point v, const float c);
#endif