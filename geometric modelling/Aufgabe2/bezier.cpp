#include "bezier.h"
#include "glwidget.h"

void drawBezierCurve(int k, Points ps, float epsilon_draw)
{
	// Abbruchbedinung erreicht: entweder das H�chstma� an Durchl�ufen oder eine hinreichende Genauigkeit
	if (k == 0 || (maxForwardDifference(ps).x() < epsilon_draw && maxForwardDifference(ps).y() < epsilon_draw) ) {

		glBegin(GL_LINE_STRIP);
		for (int i = 0; i<ps.getCount(); i++) {
			glVertex2f(ps.getPointX(i), ps.getPointY(i));
		}
		glEnd();

		return;
	}
	else
	{
		// neue Punkte mit deCasteljau-Algorithmus errechnen
		Points newPoints = deCasteljauTrigger(ps, 0.5);
		Points leftHalf, rightHalf;
		int halfSize = newPoints.getCount() / 2;

		// neue Punkte aufteilen
		for (int i=0; i<halfSize; i++) {
			leftHalf.addPoint(newPoints.getPointX(i), newPoints.getPointY(i));
		}
		for (int i=halfSize; i<newPoints.getCount(); i++) {
			rightHalf.addPoint(newPoints.getPointX(i), newPoints.getPointY(i));
		}

		// eine Rekursion tiefer gehen
		drawBezierCurve(k - 1, leftHalf, epsilon_draw);
		drawBezierCurve(k - 1, rightHalf, epsilon_draw);

	}
}


Points deCasteljauTrigger(Points ps, float ratio)
{
	Points result;
	deCasteljau(ps, ps.getCount(), ratio, &result); //Punkte, Anzahl Punkte des Polygons, Ergebnisfeld
	return result;
}


void deCasteljau(Points ps, int k, float ratio, Points *result)
{
	Points tmp;
	QPointF pf;

	// Abbruchbedingung; Ende der Rekursion erreicht.
	if (k == 0) {
		return;
	}

	result->addPoint(ps.getPointX(0), ps.getPointY(0));

	for (int i = 0; i < k - 1; i++) {
		pf.setX(ps.getPointX(i)*(1-ratio) + ps.getPointX(i + 1)*ratio);
		pf.setY(ps.getPointY(i)*(1-ratio) + ps.getPointY(i + 1)*ratio);
		tmp.addPoint(pf.rx(),pf.ry());
	}

	// rekursiver Aufruf
	deCasteljau(tmp, k - 1, ratio, result);

	result->addPoint(ps.getPointX(ps.getCount() - 1), ps.getPointY(ps.getCount() - 1));
}


QPointF maxForwardDifference(Points p)
{
	float xMax = 0.0;
	float yMax = 0.0;
	float xdiff;
	float ydiff;

	for (int i = 0; i < p.getCount() - 2; i++) {
		xdiff = p.getPointX(i + 2) - 2 * p.getPointX(i + 1) + p.getPointX(i);
		ydiff = p.getPointY(i + 2) - 2 * p.getPointY(i + 1) + p.getPointY(i);
		xdiff = absolute(xdiff);
		ydiff = absolute(ydiff);

		xMax = std::max(xMax, xdiff);
		yMax = std::max(yMax, ydiff);
	}

	return QPointF(xMax, yMax);
}


void drawIntersect(Points bPoints, Points cPoints, float epsilon_intersection)
{
	QRectF bBox_b = getBoundaryBox(bPoints);
	QRectF bBox_c = getBoundaryBox(cPoints);

	/*Sonderfall Gerade*/
	QLineF::IntersectType  lineintersect = QLineF::NoIntersection;
	if (bBox_b.height() == 0 || bBox_c.height() == 0){
		QLineF line1(bBox_b.bottomLeft(), bBox_b.topRight());
		QLineF line2(bBox_c.bottomLeft(), bBox_c.topRight());
		QPointF* intersectionPoint = new QPointF;
		lineintersect = line1.intersect(line2, intersectionPoint);
	}

	if (bBox_b.intersects(bBox_c) || lineintersect != QLineF::NoIntersection) {

		QPointF maxdiff_firstCurve = maxForwardDifference(bPoints);
		QPointF maxdiff_secondCurve = maxForwardDifference(cPoints);
		float m = bPoints.getCount()-1;
		float n = cPoints.getCount()-1;

		if (  m * (m - 1) * maxdiff_firstCurve.x() > epsilon_intersection && m * (m - 1) * maxdiff_firstCurve.y() > epsilon_intersection  )
		{
			/* Berechne a0, ..., a2m �ber[0, 1/2, 1] mit dem deCasteljau - Algorithmus*/
			Points newPoints = deCasteljauTrigger(bPoints, 0.5);
			Points leftHalf, rightHalf;
			int halfSize = newPoints.getCount() / 2;

			for (int i = 0; i < newPoints.getCount(); i++) {
				if (i < halfSize) {
					leftHalf.addPoint(newPoints.getPointX(i), newPoints.getPointY(i));
				}
				else {
					rightHalf.addPoint(newPoints.getPointX(i), newPoints.getPointY(i));
				}
			}

			drawIntersect(leftHalf, cPoints, epsilon_intersection);
			drawIntersect(rightHalf, cPoints, epsilon_intersection);
		} else if (n * (n - 1) * maxdiff_secondCurve.x() > epsilon_intersection && n * (n - 1) * maxdiff_secondCurve.y() > epsilon_intersection) {

			// Berechne d0, ..., d2n �ber[0, 1/2, 1] mit dem deCasteljau - Algorithmus;
			Points newPoints = deCasteljauTrigger(cPoints, 0.5);
			Points leftHalf, rightHalf;
			int halfSize = newPoints.getCount() / 2;

			for (int i = 0; i < newPoints.getCount(); i++) {
				if (i < halfSize) {
					leftHalf.addPoint(newPoints.getPointX(i), newPoints.getPointY(i));
				}
				else {
					rightHalf.addPoint(newPoints.getPointX(i), newPoints.getPointY(i));
				}
			}
			drawIntersect(bPoints, leftHalf, epsilon_intersection);
			drawIntersect(bPoints, rightHalf, epsilon_intersection);
		} else {

			// Berechne Schnittpunkt
			QPointF intersection;
			QLineF line1 = QLineF(bPoints.getPointX(0), bPoints.getPointY(0), bPoints.getPointX(bPoints.getCount() - 1), bPoints.getPointY(bPoints.getCount() - 1));
			QLineF line2 = QLineF(cPoints.getPointX(0), cPoints.getPointY(0), cPoints.getPointX(cPoints.getCount() - 1), cPoints.getPointX(cPoints.getCount() - 1));
			QLineF::IntersectType ret = line1.intersect(line2, &intersection);

			if (ret == QLineF::BoundedIntersection)
			{
				glPointSize(7.0);
				glBegin(GL_POINTS);
				glVertex2f(intersection.x(), intersection.y());
				glEnd();
			}

			return;
		}
	} // bounding boxes schneiden sich
}


void drawSelfIntersect(Points points, float epsilon_intersection)
{
	if (tangentsAngle(points) <= 180.0) {
			// no intersection possible. nothing to do here...
			return;
	}

	// zwei H�lften mit deCasteljau-Algorithmus errechnen
	Points newPoints = deCasteljauTrigger(points, 0.5);
	Points leftHalf, rightHalf;
	int halfSize = newPoints.getCount() / 2;

	// neue Punkte aufteilen
	for (int i=0; i<halfSize; i++) {
		leftHalf.addPoint(newPoints.getPointX(i), newPoints.getPointY(i));
	}
	for (int i=halfSize; i<newPoints.getCount(); i++) {
		rightHalf.addPoint(newPoints.getPointX(i), newPoints.getPointY(i));
	}

	drawIntersect(leftHalf, rightHalf, epsilon_intersection);
}


QRectF getBoundaryBox(Points p)
{
	float xMin = p.getPointX(0);
	float xMax = p.getPointX(0);
	float yMin = p.getPointY(0);
	float yMax = p.getPointY(0);

	for (int i=1; i<p.getCount(); i++) {
		xMin = std::min(xMin, p.getPointX(i));
		xMax = std::max(xMax, p.getPointX(i));
		yMin = std::min(yMin, p.getPointY(i));
		yMax = std::max(yMax, p.getPointY(i));
	}

	QPointF tl1(xMin, yMax);
	QPointF br1(xMax, yMin);

	/*
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xMin, yMin);
	glVertex2f(xMin, yMax);

	//glVertex2f(xMin, yMax);
	glVertex2f(xMax, yMax);

	//glVertex2f(xMax, yMax);
	glVertex2f(xMax, yMin);

	//glVertex2f(xMax, yMin);
	//glVertex2f(xMin, yMin);

	glEnd();
	*/

	return QRectF(tl1, br1);
}


float tangentsAngle(Points p)
{
	if (p.getCount() < 2) {
		return 0.0;
	}

	float totalAngle = 0.0;
	int n = p.getCount();
	QPointF lastP(p.getPointX(1)-p.getPointX(0), p.getPointY(1)-p.getPointX(0));

	for (int i=1; i<(n-1); i++)	{
			QPointF newP(p.getPointX(i+1)-p.getPointX(i), p.getPointY(i+1)-p.getPointY(i));
			double angle = (lastP.x()*newP.x() + lastP.y()*newP.y()) / (sqrt(lastP.x()*lastP.x()+newP.y()*newP.y()) * sqrt(lastP.y()*lastP.y()+newP.y()*newP.y()));
			totalAngle += acos(angle);
	}

	return totalAngle;
}


float absolute(float f)
{
	if (f < 0) {
		f *= -1;
	}
	return f;
}


Points createSegment(Points p, QPointF newPoint)
{
	Points temp = deCasteljauTrigger(p, 1.5);
	Points segmentPoints;

	/*
	 * Die zweite H�lfte der erhaltenen Punkte sind die des neuen Segments.
	 * Der letzte Punkt wird jedoch nicht ben�tigt, weil stattdessen 'newPoint' eingef�gt wird
	 */
	for (int i=temp.getCount()-1; i>p.getCount(); i--) {
		segmentPoints.addPoint(temp.getPointX(i), temp.getPointY(i));
	}
	segmentPoints.addPoint(newPoint.x(), newPoint.y());

	return segmentPoints;
}

