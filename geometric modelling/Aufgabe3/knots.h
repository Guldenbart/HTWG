#ifndef KNOTS_H
#define KNOTS_H
#include "points.h"

class Knots : public Points
{
public:
    Knots();
	void insertKnot(float value);
	int insertKnot(float value, int degree);
	int insertKnotX(float x, int degree);
    void setValueX(int i, float x);
    float getValue(int i);
};

#endif // KNOTS_H
