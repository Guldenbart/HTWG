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

float absolute(float f);

#endif // BEZIER_H
