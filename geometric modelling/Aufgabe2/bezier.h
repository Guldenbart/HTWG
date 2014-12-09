#ifndef BEZIER_H
#define BEZIER_H

#include "math.h"
#include <QPointF>
#include <QList>
#include <QRectF>
#include "glwidget.h"


// alles zu Aufgabe 3

void drawBezierCurve(int k, Points ps, float epsilon_draw);
Points deCasteljauTrigger(Points ps);
void deCasteljau(Points ps, int k, Points *result);
QPointF maxForwardDifference(Points points);

void drawIntersect(Points firstCurvePoints, Points secondCurvePoints, float epsilon_intersection);
drawSelfIntersect(Points points, float epsilon_intersection);
QRectF getBoundaryBox(Points p);

float absolute(float f);

#endif // BEZIER_H
