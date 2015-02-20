#ifndef KNOTS_H
#define KNOTS_H
#include "points.h"

class Knots : public Points
{
public:
	// constructors
    Knots();
	Knots(const Knots &ks);			// copy constructor

	// assignment operator
	Knots &operator = (const Knots &ks);

	void insertKnot(float value);
	int insertKnot(float value, int degree);
	int insertKnotX(float x, int degree);
    void setValueX(int i, float x);
    float getValue(int i);
};

#endif // KNOTS_H
