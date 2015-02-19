#include "glwidget.h"
#include <QtGui>
#include "mainwindow.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);

    epsilon_draw = 0.05;
	drawCurve = false;
	degree = 1;

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

    // Kurve zeichnen
    glColor3f(1.0,1.0,1.0);
    // AUFGABE: Hier Kurve zeichnen
    // dabei epsilon_draw benutzen

	if (drawCurve) {

	}
}


void GLWidget::initializeGL()
{
    resizeGL(width(),height());
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
        }
        else {
            points.setPointX(clickedPoint,posF.x());
            points.setPointY(clickedPoint,posF.y());
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
            points.setPointX(clickedPoint,posF.x());
            points.setPointY(clickedPoint,posF.y());
            clickedKnot = -1;
        }
        else
        { // knot was clicked
			knots.setValueX(clickedKnot,posF.x());
            clickedPoint = -1;
        }
    }
    if (event->buttons() & Qt::RightButton) {
		int newKnotIndex = knots.insertKnotX(posF.x());
		// AUFGABE: Hier Knoten in eine B-Spline-Kurve einfügen.

		// Knoten rauskopieren, die betroffen sind:

		// de Boor

		// neuen Punktearray erstellen
    }
    update();
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
	qDebug() << "KeyPressEvent\n";
	if (e->key() == Qt::Key_D) {
		drawCurve = !drawCurve;
		if (drawCurve) {
			qDebug() << "Curve drawing on.\n";
		} else {
			qDebug() << "Curve drawing off.\n";
		}
	}
}

void GLWidget::setEpsilonDraw(double value)
{
    epsilon_draw = value;
}


/*
 * #############################################################
 * Implementierung rund um deBoor
 * #############################################################
 */

/*
 * Startet den deBoor-Algorithmus und gibt das Ergebnis zurück.

 * Parameter:
 * column - gibt an, welche Spalte aus dem Schema komplett verwendet wird
 * newKnot - INDEX des neu eingefügten Knotens

 * Rückgabewert: durch den deBoor-Algorithmus errechneten Punkte
 */
Points GLWidget::deBoorStarter(int multiplicity, int newKnot)
{
	Points startingPoints, result;

	// Punkte raussuchen, die man braucht:
	for (int i=newKnot-degree-1; i<newKnot; i++) {
		if (i<0) {
			// Punkt nicht vorhanden
			startingPoints.addPoint(0.0, 0.0);
		} else {
			startingPoints.addPoint(points.getPointX(i), points.getPointY(i));
		}
	}

	// deBoor starten
	void deBoor(startingPoints, degree-multiplicity, column, newKnot-1, &result);

	return result;
}


/*
 * Rekursionsfunktion für den deBoor-Algorithmus

 * Parameter:
 * ps - durch deBoor errechnete Punkte
 * k - Rekursionsvariable
 * r - Index r
 */
void GLWidget::deBoor(Points ps, int k, int r, Points *result)
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

	// rekursiver Aufruf
	deCasteljau(tmp, k - 1, ratio, result);

	result->addPoint(ps.getPointX(ps.getCount() - 1), ps.getPointY(ps.getCount() - 1));
}
