////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "quaternion.h"
#include "math.h"
#include <iostream>


// default constructor
Quaternion::Quaternion()
{
	for (int i=0; i<4; i++) x[i]=0;	
}

// simple constructor
Quaternion::Quaternion(const float _x[4])
{
	for (int i = 0; i < 4; i++) {
		x[i] = _x[i];
	}
}


Quaternion::Quaternion(const float s, const float a, const float b, const float c)
{
	x[0] = s;
	x[1] = a;
	x[2] = b;
	x[3] = c;
}


// constructor with scale and vector
Quaternion::Quaternion(const float s, const CVec4f vec)
{
	x[0] = s;
	x[1] = vec(0);
	x[2] = vec(1);
	x[3] = vec(2);
}


// copy constructor
Quaternion::Quaternion(const Quaternion &quat)
{
	if (this==&quat) {
		return; // nothing to do, it's me
	}

	*this = quat;
}


// destuctor
Quaternion::~Quaternion()
{
}


// assignment operator
Quaternion &Quaternion::operator = (const Quaternion &quat)
{
	if (this != &quat) {
		for (int i = 0; i < 4; i++) {
			x[i] = quat(i);
		}
	}	

	return (*this);
}


// index operators
float &Quaternion::operator () (unsigned i)
{
	return x[i%4];
}

float Quaternion::operator () (unsigned i) const
{
	return x[i%4];
}


// arithmetic operators
void Quaternion::operator+=	(const Quaternion &quat)
{
	for (int i = 0; i < 4; i++) {
		x[i] += quat(i);
	}
}


void Quaternion::operator-=	(const Quaternion &quat)
{
	for (int i = 0; i < 4; i++) {
		x[i] -= quat(i);
	}
}


void Quaternion::operator*=	(const Quaternion &quat)
{
	Quaternion qTemp;

	qTemp(0) = x[0]*quat(0) - x[1]*quat(1) - x[2]*quat(2) - x[3]*quat(3);
	qTemp(1) = x[0]*quat(1) + x[1]*quat(0) + x[2]*quat(3) - x[3]*quat(2);
	qTemp(2) = x[0]*quat(2) + x[2]*quat(0) + x[3]*quat(1) - x[1]*quat(3);
	qTemp(3) = x[0]*quat(3) + x[3]*quat(0) + x[1]*quat(2) - x[2]*quat(1);

	*this = qTemp;

}

Quaternion Quaternion::operator+ (const Quaternion& quat)
{
	Quaternion retVal(x);
	retVal += quat;
	return quat;
}


Quaternion Quaternion::operator- (const Quaternion &quat)
{
	Quaternion retVal(x);
	retVal -= quat;
	return quat;
}


Quaternion Quaternion::operator*(const Quaternion& quat)
{
	Quaternion retVal(x);
	retVal *= quat;
	return retVal;
}


Quaternion Quaternion::operator*(const float f)
{
	Quaternion retVal(x);
	
	for (int i = 0; i < 4; i++) {
		retVal(i) *= f;
	}

	return retVal;
}


// other methods
float Quaternion::norm()
{
	return sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2] + x[3]*x[3]);
}


void Quaternion::normalize()
{
	float norm = this->norm();

	for (int i = 0; i < 4; i++) {
		x[i] /= norm;
	}
}


float Quaternion::dot(const Quaternion &quat)
{
	float result =  x[0]*quat(0) + x[1]*quat(1) + x[2]*quat(2) + x[3]*quat(3);
	return result;
}


Quaternion Quaternion::Inverse()
{
	Quaternion inverse;

	inverse(0) = x[0];
	for (int i = 1; i < 4; i++) {
		inverse(i) = -(x[i]);
	}

	float norm2 = norm()*norm();

	for (int i = 0; i < 4; i++) {
		inverse(i) /= norm2;
	}

	return inverse;
}


void Quaternion::print()
{
	std::cout << "(" << x[0] << " +" << x[1] << "i +" << x[2] << "j +" << x[3] << "k)\n";
}


CVec4f Quaternion::getVector(int i)
{
	return CVec4f(x[1], x[2], x[3], i);
}
