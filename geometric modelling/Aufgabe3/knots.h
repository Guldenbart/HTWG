#ifndef KNOTS_H
#define KNOTS_H
#include "points.h"

class Knots : public Points
{
public:
    Knots();
	int insertKnot(float value);
	int insertKnotX(float x);
    void setValueX(int i, float x);
    float getValue(int i);
};

#endif // KNOTS_H
