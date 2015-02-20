#include "bezier.h"
#include "glwidget.h"


/*
 * Funktion, um eine Kurve in Bezier-Repräsentation zu zeichnen

 * Parameter:
 * k - Höchstanzahl an Rekursionsaufrufen
 * ps - Bezier-Punkte
 * epsilon_draw - geforderte Genauigkeit
 */

void drawBezierCurve(int k, Points ps, float epsilon_draw)
{
	// Abbruchbedinung erreicht: entweder das Höchstmaß an Durchläufen oder eine hinreichende Genauigkeit
	if (k == 0 || (maxForwardDifference(ps).x() < epsilon_draw && maxForwardDifference(ps).y() < epsilon_draw) ) {

		// Liniensegmente zeichnen
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


/*
 * Startet den deCasteljau-Algorithmus und gibt das Ergebnis zurück.

 * Parameter:
 * ps - Bezier-Punkte
 * ratio - Verhältnis, mit dem die Kurve geteilt wird

 * Rückgabewert: durch den deCasteljau-Algorithmus errechneten Punkte
 */
Points deCasteljauTrigger(Points ps, float ratio)
{
	Points result;
	deCasteljau(ps, ps.getCount(), ratio, &result); //Punkte, Anzahl Punkte des Polygons, Ergebnisfeld
	return result;
}


/*
 * Rekursions-Funktion für deCasteljau-Berechnung

 * Parameter:
 * ps - Bezier-Punkte
 * k - Anzahl der Rekursionsaufrufe bis gesuchter Punkt berechnet ist.
 * ratio - Verhältnis, mit dem die Kurve geteilt wird
 * result - gesuchte Punkte
 */
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


/*
 * berechnet die maximale Vorwärtsdifferenz

 * Parameter:
 * p - Punkte, zwischen denen die maximale Vorwärtsdifferenz berechnet werden soll.

 * Rückgabewert: x- und y-Wert der maximalen Vorwärtsdifferenz in einem QPointF
 */
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


/*
 * Betrags-Funktion

 * Parameter:
 * f - Zahl, deren Betrag berechnet werden soll

 * Rückgabewert: |f|
 */
float absolute(float f)
{
	if (f < 0) {
		f *= -1;
	}
	return f;
}
