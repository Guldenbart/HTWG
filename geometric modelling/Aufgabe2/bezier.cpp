#include "bezier.h"
#include "glwidget.h"

void drawBezierCurve(int k, QList<QPointF> ps, float epsilon_draw)
{
	if (k == 0 || maxDifference(ps) < epsilon_draw) {
		glBegin(GL_LINE_STRIP);

		for (int i=0; i<ps.count(); i++) {
			glVertex2f(ps[i].x(),ps[i].y());
		}

		glEnd();

	} else {
		QList<QPointF> newList;
		int i;

		newList.append(ps[0]);

		for (i=0; i<(ps.count()-1); i++) {
			newList.append(interpolate(ps[i], ps[i+1], 0.5));
		}

		newList.append(ps[i]);

		drawBezierCurve(k-1, newList, epsilon_draw);

	}
}


QPointF interpolate(QPointF p1, QPointF p2, double ratio)
{
	qreal x = (1-ratio)*p1.x() + ratio*p2.x();
	qreal y = (1-ratio)*p1.y() + ratio*p2.y();

	return QPointF(x, y);
}


double maxDifference(QList<QPointF> points)
{
	double max = 0;

	for (int i=0; i<(points.count()-2); i++) {
		double deltaX = points[i+2].x() - 2*(points[i+1].x()) + points[i].x();
		if (deltaX > max) {
			max = deltaX;
		}

		double deltaY = points[i+2].y() - 2*(points[i+1].y()) + points[i].y();
		if (deltaY > max) {
			max = deltaY;
		}
	}

	return max;
}

