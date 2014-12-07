#ifndef BEZIER_H
#define BEZIER_H

#include <QPointF>
#include <QList>
#include "glwidget.h"

// alles zu Aufgabe 3

void drawBezierCurve(int k, QList<QPointF> ps, float epsilon_draw);
QPointF interpolate(QPointF p1, QPointF p2, double ratio);
double maxDifference(QList<QPointF> points);

#endif // BEZIER_H
