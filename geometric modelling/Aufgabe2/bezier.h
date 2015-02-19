#ifndef BEZIER_H
#define BEZIER_H

#include "math.h"
#include <QPointF>
#include <QList>
#include <QRectF>
#include "glwidget.h"


// alles zu Aufgabe 3
void drawBezierCurve(int k, Points ps, float epsilon_draw);
Points deCasteljauTrigger(Points ps, float ratio);
void deCasteljau(Points ps, int k, float ratio, Points *result);
QPointF maxForwardDifference(Points points);

void drawIntersect(Points firstCurvePoints, Points secondCurvePoints, float epsilon_intersection);
void drawSelfIntersect(Points points, float epsilon_intersection);
QRectF getBoundaryBox(Points p);

float tangentsAngle(Points p);
float absolute(float f);

// alles zu Aufgabe 4
Points createSegment(Points p, QPointF newPoint);

#endif // BEZIER_H
