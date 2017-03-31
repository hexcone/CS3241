#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <math.h>

class Vector3 {



public:
	double x[3];

	Vector3 operator = (Vector3);
	Vector3 operator - (Vector3);
	Vector3 operator - ();
	Vector3 operator + (Vector3);
	Vector3 operator * (double d);
	Vector3 operator / (double d);

	inline double  length() { return sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]); };
	inline double  lengthsqr() { return x[0] * x[0] + x[1] * x[1] + x[2] * x[2]; };
	void operator () (double x1, double x2, double x3) { x[0] = x1; x[1] = x2; x[2] = x3; };
	void operator () (double x_[3]) { x[0] = x_[0]; x[1] = x_[1]; x[2] = x_[2]; };;
	void show();
	void normalize();

	Vector3() { x[0] = x[1] = x[2] = 0; };
	Vector3(double x1, double x2, double x3) { x[0] = x1; x[1] = x2; x[2] = x3; };
	Vector3(double x_[3]) { x[0] = x_[0]; x[1] = x_[1]; x[2] = x_[2]; };;

};

double dot_prod(Vector3, Vector3);
Vector3 cross_prod(Vector3, Vector3);

#endif
