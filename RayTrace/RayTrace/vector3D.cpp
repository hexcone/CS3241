#include "vector3D.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>




Vector3 Vector3::operator = (Vector3 v)
{
	x[0] = v.x[0];
	x[1] = v.x[1];
	x[2] = v.x[2];
	return v;
}

Vector3 Vector3::operator - (Vector3 x2)
{
	Vector3 diff;

	diff.x[0] = x[0] - x2.x[0];
	diff.x[1] = x[1] - x2.x[1];
	diff.x[2] = x[2] - x2.x[2];

	return diff;
}

Vector3 Vector3::operator - ()
{
	Vector3 neg;
	neg.x[0] = -x[0];
	neg.x[1] = -x[1];
	neg.x[2] = -x[2];
	return neg;
}

Vector3 Vector3::operator + (Vector3 x2)
{
	Vector3 sum;

	sum.x[0] = x[0] + x2.x[0];
	sum.x[1] = x[1] + x2.x[1];
	sum.x[2] = x[2] + x2.x[2];

	return sum;
}

Vector3 Vector3::operator * (double d)
{
	Vector3 prod;

	prod.x[0] = x[0] * d;
	prod.x[1] = x[1] * d;
	prod.x[2] = x[2] * d;

	return prod;
}


Vector3 Vector3::operator / (double d)
{
	Vector3 qu;


	qu.x[0] = x[0] / d;
	qu.x[1] = x[1] / d;
	qu.x[2] = x[2] / d;

	return qu;
}





void Vector3::show()
{
	printf("(%16.12f %16.12f %16.12f) \n", x[0], x[1], x[2]);
}


void Vector3::normalize()
{
	double len;

	len = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);

	if (len != 0)
	{
		x[0] /= len;
		x[1] /= len;
		x[2] /= len;
	}
}


double dot_prod(Vector3 v1, Vector3 v2)
{
	return (v1.x[0] * v2.x[0]) + (v1.x[1] * v2.x[1]) + (v1.x[2] * v2.x[2]);
}

Vector3 cross_prod(Vector3 v1, Vector3 v2)
{
	Vector3 prod;

	prod.x[0] = v1.x[1] * v2.x[2] - v1.x[2] * v2.x[1];
	prod.x[1] = -(v1.x[0] * v2.x[2] - v1.x[2] * v2.x[0]);
	prod.x[2] = v1.x[0] * v2.x[1] - v1.x[1] * v2.x[0];

	return prod;
}
