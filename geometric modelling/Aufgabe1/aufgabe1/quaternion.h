////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////
#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "vec.h"
//#include <iostream>

class Quaternion {
private:
	float x[4];

public:
	// constructors/destructors
	Quaternion();									// default constructor
	Quaternion(const float _x[4]);					// simple constructor
	Quaternion(const float s, const float a, const float b, const float c); // another simple constructor
	Quaternion(const float s, const CVec4f vec);	// constructor with scale and vector
	Quaternion(const Quaternion &quat);				// copy constructor
	~Quaternion();

	// assignment operator
	Quaternion &operator = (const Quaternion &quat);

	// index operators
	float  &operator () (unsigned i);
	float	operator () (unsigned i) const;

	// arithmetic operators
	void		operator +=	(const Quaternion &quat);
	void		operator -=	(const Quaternion &quat);
	void		operator *=	(const Quaternion &quat);
	Quaternion	operator +	(const Quaternion &quat);
	Quaternion	operator -	(const Quaternion &quat);
	Quaternion	operator *	(const Quaternion &quat);
	Quaternion	operator *	(const float f);

	// other methods
	float norm();
	void normalize();
	float dot(const Quaternion &quat);
	Quaternion Inverse();
	void print();
	CVec4f getVector(int i);

};


#endif