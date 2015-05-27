#include "glwidget.h"
#include <QtGui>
#include "mainwindow.h"
#include "bezier.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);

    epsilon_draw = 0.05;
	drawCurve = false;
	degree = 4;

    // Hier Punkte hinzufügen: Schönere Startpositionen und anderer Grad!
    points.addPoint(-1.00,  0.5);
    points.addPoint(-0.30, -0.25);
	points.addPoint( 0.00,  0.5);
    points.addPoint( 0.30, -0.25);
    points.addPoint( 1.00,  0.5);

	knots.insertKnot(0.05);
    knots.insertKnot(0.1);
    knots.insertKnot(0.3);
    knots.insertKnot(0.4);
    knots.insertKnot(0.5);
    knots.insertKnot(0.7);
	knots.insertKnot(0.9);
	knots.insertKnot(0.95);
}

GLWidget::~GLWidget()
{
}

void GLWidget::paintGL()
{
    // clear
    glClear(GL_COLOR_BUFFER_BIT);

    // Koordinatensystem
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_LINES);
    glVertex2f(-1.0, 0.0);
    glVertex2f( 1.0, 0.0);
    glVertex2f( 0.0,-1.0);
    glVertex2f( 0.0, 1.0);
    glEnd();

    // Kontrollunkte zeichnen
    glPointSize(7.0);
    glBegin(GL_POINTS);
    glColor3f(1.0,0.0,0.0);
    for (int i=0; i<points.getCount(); i++) {
        glVertex2f(points.getPointX(i),points.getPointY(i));
    }
    glEnd();

    // Kontrollpolygon zeichnen
    glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINE_STRIP);
    for (int i=0; i<points.getCount(); i++) {
        glVertex2f(points.getPointX(i),points.getPointY(i));
    }
    glEnd();

    // Knoten zeichnen
    glColor3f(0.0,1.0,1.0);
    glBegin(GL_LINE_STRIP);
    for (int i=0; i<knots.getCount(); i++) {
        glVertex2f(knots.getPointX(i),knots.getPointY(i));
    }
    glEnd();
    glColor3f(1.0,0.0,1.0);
    glBegin(GL_POINTS);
    for (int i=0; i<knots.getCount(); i++) {
        glVertex2f(knots.getPointX(i),knots.getPointY(i));
    }
    glEnd();

    // AUFGABE: Hier Kurve zeichnen
    // dabei epsilon_draw benutzen

	if (drawCurve) {

		// Bézier-Punkte zeichnen
		glColor3f(1.0,0.7,0.0);
		glBegin(GL_POINTS);
		for (int i=0; i<bezierPoints.getCount(); i++) {
			glVertex2f(bezierPoints.getPointX(i), bezierPoints.getPointY(i));
		}
		glEnd();

		// Hüllpolygone zeichnen
		glBegin(GL_LINE_STRIP);
		for (int i=0; i<bezierPoints.getCount(); i++) {
			glVertex2f(bezierPoints.getPointX(i),bezierPoints.getPointY(i));
		}
		glEnd();


		int s = points.getCount()-degree;
		glColor3f(1.0,1.0,1.0);
		// Bézier-Kurve zeichnen
		if (bezierPoints.getCount() < degree+1) {
			qDebug("zu wenig Punkte zum Zeichnen!!!");
		} else {
			for (int i=0; i<s; i++) {
				Points* pointsArray = new Points[degree+1];
				int startIndex = i*degree;

				// immer genügend Punkte für eine Bézier-Kurve sammeln
				for (int j=0; j<degree+1; j++) {
					pointsArray->addPoint(bezierPoints.getPointX(startIndex+j), bezierPoints.getPointY(startIndex+j));
				}

				drawBezierCurve(degree+1, *pointsArray, this->epsilon_draw);
			}
		}
	}
}


void GLWidget::initializeGL()
{
    resizeGL(width(),height());
}


void GLWidget::resetPoints()
{
	epsilon_draw = 0.05;
	drawCurve = false;
	degree = 4;

	Points defaultPoints;
	defaultPoints.addPoint(-1.00,  0.5);
	defaultPoints.addPoint(-0.30, -0.25);
	defaultPoints.addPoint( 0.00,  0.5);
	defaultPoints.addPoint( 0.30, -0.25);
	defaultPoints.addPoint( 1.00,  0.5);
	this->points = defaultPoints;

	Knots defaultKnots;
	defaultKnots.insertKnot(0.05);
	defaultKnots.insertKnot(0.1);
	defaultKnots.insertKnot(0.3);
	defaultKnots.insertKnot(0.4);
	defaultKnots.insertKnot(0.5);
	defaultKnots.insertKnot(0.7);
	defaultKnots.insertKnot(0.9);
	defaultKnots.insertKnot(0.95);
	this->knots = defaultKnots;
}

void GLWidget::setupThree()
{
	epsilon_draw = 0.05;
	drawCurve = false;
	degree = 3;

	Points defaultPoints;
	defaultPoints.addPoint(-1.00,  0.0);
	defaultPoints.addPoint(-0.90,  0.3);
	defaultPoints.addPoint( 0.10,  0.5);
	defaultPoints.addPoint( 1.00,  0.15);
	defaultPoints.addPoint( 0.40, -0.4);
	defaultPoints.addPoint( 0.10, -0.4);
	this->points = defaultPoints;

	Knots defaultKnots;
	defaultKnots.insertKnot(0.05);
	defaultKnots.insertKnot(0.1);
	defaultKnots.insertKnot(0.2);
	defaultKnots.insertKnot(0.33);
	defaultKnots.insertKnot(0.66);
	defaultKnots.insertKnot(0.8);
	defaultKnots.insertKnot(0.9);
	defaultKnots.insertKnot(0.95);
	this->knots = defaultKnots;
}
void GLWidget::setupFour()
{
	epsilon_draw = 0.05;
	drawCurve = false;
	degree = 4;

	Points defaultPoints;
	defaultPoints.addPoint( 0.00,  0.9);
	defaultPoints.addPoint(-0.90,  0.3);
	defaultPoints.addPoint(-0.50, -0.8);
	defaultPoints.addPoint(-0.30,  0.0);
	defaultPoints.addPoint( 1.00,  0.5);
	defaultPoints.addPoint( 1.00, -0.8);
	this->points = defaultPoints;

	Knots defaultKnots;
	defaultKnots.insertKnot(0.05);
	defaultKnots.insertKnot(0.1);
	defaultKnots.insertKnot(0.2);
	defaultKnots.insertKnot(0.3);
	defaultKnots.insertKnot(0.5);
	defaultKnots.insertKnot(0.7);
	defaultKnots.insertKnot(0.8);
	defaultKnots.insertKnot(0.9);
	defaultKnots.insertKnot(0.95);
	this->knots = defaultKnots;
}


void GLWidget::resizeGL(int width, int height)
{
    aspectx=1.0;
    aspecty=1.0;
    if (width>height) {
        aspectx=float(width)/height;
    }
    else {
        aspecty=float(height)/width;
    }
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-aspectx,aspectx,-aspecty,aspecty);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

QPointF GLWidget::transformPosition(QPoint p)
{
    return QPointF((2.0*p.x()/width() - 1.0)*aspectx,-(2.0*p.y()/height() - 1.0)*aspecty);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint pos = event->pos();
        QPointF posF = transformPosition(pos);
        if (clickedKnot >= 0) {
            knots.setValueX(clickedKnot,posF.x());
			if (this->drawCurve) {
				drawBSplineCurve();	// Zeichnung der Kurve updaten
			}
        }
        else {
			if (!drawCurve) {	// Kontrollpunkte dürfen nur bewegt werden, wenn die Kurve nicht gezeichnet wird
				points.setPointX(clickedPoint,posF.x());
				points.setPointY(clickedPoint,posF.y());
			}
        }
        update();
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    QPointF posF = transformPosition(pos);
    if (event->buttons() & Qt::LeftButton) {
        clickedPoint = points.getClosestPoint(posF.x(),posF.y());
        clickedKnot = knots.getClosestPoint(posF.x(),posF.y());
        if (points.getDistance(clickedPoint,posF.x(),posF.y()) <=
             knots.getDistance(clickedKnot, posF.x(),posF.y()))
        { // point was clicked
			if (!drawCurve) {	// Kontrollpunkte dürfen nur bewegt werden, wenn die Kurve nicht gezeichnet wird
				points.setPointX(clickedPoint,posF.x());
				points.setPointY(clickedPoint,posF.y());
				clickedKnot = -1;
			}
        }
        else
        { // knot was clicked
			knots.setValueX(clickedKnot,posF.x());
            clickedPoint = -1;
        }
    }
	if (event->buttons() & Qt::RightButton) {
		Knots knotsCopy = this->knots;
		int newKnotIndex = knotsCopy.insertKnotX(posF.x(), degree);
		if (newKnotIndex < 0) {
			// Knoten wurde nicht eingefügt, nichts zu tun.
			return;
		}
		// AUFGABE: Hier Knoten in eine B-Spline-Kurve einfügen.

		// 1. de Boor
		deBoorStarter(this->points, this->knots, 1, newKnotIndex, knotsCopy.getValue(newKnotIndex));

		// 2. neue Knotenliste einsetzen
		this->knots = knotsCopy;

		// 3. Kurve neu berechnen, falls gewünscht!
		if (this->drawCurve) {
			drawBSplineCurve();
		}
    }
    update();
}

/*
 * Keyboard-Event Handler
 */
void GLWidget::keyPressEvent(QKeyEvent *e)
{
	//qDebug() << "KeyPressEvent\n";
	if (e->key() == Qt::Key_D) {
		drawCurve = !drawCurve;
		if (drawCurve) {
			drawBSplineCurve();
			qDebug() << "Curve drawing on.\n";
		} else {
			qDebug() << "Curve drawing off.\n";
		}
	}
	else if (e->key() == Qt::Key_R) {
		resetPoints();
		qDebug() << "Reset\n";
	}
	else if (e->key() == Qt::Key_3) {
		setupThree();
		qDebug() << "Setup with degree 3\n";
	}
	else if (e->key() == Qt::Key_4) {
		setupFour();
		qDebug() << "Secondary setup with degree 4\n";
	}

	update();
}

void GLWidget::setEpsilonDraw(double value)
{
    epsilon_draw = value;
	update();
}


/*
 * #############################################################
 * Implementierung rund um deBoor
 * #############################################################
 */

/*
 * Ist dafür verantwortlich, dass die Knoten und Punkte für die Bézier-Darstellung
 * richtig berechnet werden
 */
void GLWidget::drawBSplineCurve()
{
	// Kopien erstellen
	this->bezierPoints = this->points;
	this->bezierKnots = this->knots;

	// jeden Knoten auf die Vielfachheit 'degree' erhöhen
	for (int i=degree; i<(bezierKnots.getCount()-degree); i++) {
		float value = bezierKnots.getValue(i);

		// Index herausfinden, bei dem der Knoten eingefügt werden wird
		Knots fakeKnots = bezierKnots;
		int index = fakeKnots.insertKnot(value, degree);

		// => deBoor anwerfen
		deBoorStarter(bezierPoints, bezierKnots, degree-1, index, value);

		// Knoten tatsächlich einfügen
		for (int j=0; j<degree-1; j++) {
			bezierKnots.insertKnot(value, degree);
		}

		i += degree-1; // Überspringe die gerade eingefügten Knoten (und berücksichtige, dass die Schleife auch noch 1 addiert)!
	}

	// die ersten und letzten m-1 Knoten entfernen
	for (int i=0; i<degree-1; i++) {
		this->bezierPoints.removeFirst();
		this->bezierPoints.removeLast();
	}
}


/*
 * Startet den deBoor-Algorithmus und gibt das Ergebnis zurück.

 * Parameter:
 * column - gibt an, welche Spalte aus dem Schema komplett verwendet wird
 * newKnot - INDEX des neu eingefügten Knotens

 */
void GLWidget::deBoorStarter(Points &localPoints, Knots &localKnots, int multiplicity, int newKnotIndex, float newKnotValue)
{
	Points startingPoints, result;

	// 1. Punkte raussuchen, die man braucht:
	for (int i=newKnotIndex-degree-1; i<newKnotIndex; i++) {
		startingPoints.addPoint(localPoints.getPointX(i), localPoints.getPointY(i));
	}

	// 2. deBoor starten
	deBoor(localKnots, startingPoints, multiplicity, newKnotIndex-1, newKnotValue, &result);

	// 3. neue Punkte einfügen
	Points newPointList;
	int i;

	for (i=0; i<newKnotIndex-1-degree; i++) {
		newPointList.addPoint(localPoints.getPointX(i), localPoints.getPointY(i));
	}
	for (int j=0; j<result.getCount(); j++) {
		newPointList.addPoint(result.getPointX(j), result.getPointY(j));
	}
	for (i+=(degree+1); i<localPoints.getCount(); i++) {
		newPointList.addPoint(localPoints.getPointX(i), localPoints.getPointY(i));
	}
	localPoints = newPointList;

}


/*
 * Rekursionsfunktion für den deBoor-Algorithmus

 * Parameter:
 * ps - durch deBoor errechnete Punkte
 * k - Rekursionsvariable
 * r - Index r
 * y - Wert des neu eingesetzten Knotens
 * *result - Ergebnispunkte

 */
void GLWidget::deBoor(Knots &localKnots, Points ps, int k, int r, float y, Points *result)
{
	Points tmp;
	QPointF pf;

	// Abbruchbedingung; Ende der Rekursion erreicht. Punkte der zuletzt errechneten Spalte kopieren.
	if (k == 0) {
		for (int i=0; i<ps.getCount(); i++) {
			result->addPoint(ps.getPointX(i), ps.getPointY(i));
		}
		return;
	}

	result->addPoint(ps.getPointX(0), ps.getPointY(0));

	// deBoor-Kern
	for (int loop=0; loop<ps.getCount()-1; loop++) {

		int i = (r-ps.getCount()+loop+2);
		int j = degree-ps.getCount()+2;
		float alpha = (y-localKnots.getValue(i))/(localKnots.getValue(degree+i+1-j)-localKnots.getValue(i));

		pf.setX((1-alpha)*ps.getPointX(loop) + alpha*ps.getPointX(loop+1));
		pf.setY((1-alpha)*ps.getPointY(loop) + alpha*ps.getPointY(loop+1));
		tmp.addPoint(pf.rx(), pf.ry());
	}

	// rekursiver Aufruf
	deBoor(localKnots, tmp, k - 1, r, y, result);

	result->addPoint(ps.getPointX(ps.getCount() - 1), ps.getPointY(ps.getCount() - 1));
}

