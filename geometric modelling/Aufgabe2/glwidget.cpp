
#include "glwidget.h"
#include <QtGui>
#include "mainwindow.h"
#include "bezier.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
	doIntersection       = false;
	doSelfIntersection   = false;
	epsilon_draw         = (float)0.05;
	epsilon_intersection = (float)0.000005;
	whichCurve           = 1;
	secondCurveIndex     = 1;

	curveSizes.append(5);
	curveSizes.append(4);

	// Hier Punkte hinzufügen (schönere Startpositionen!)
	points.addPoint(-1.00, -0.5);
	points.addPoint(-0.75, -0.5);
	points.addPoint(-0.50, -0.5);
	points.addPoint(-0.25, -0.5);
	points.addPoint( 0.00, -0.5);

	points.addPoint( 0.25,  0.5);
	points.addPoint( 0.50,  0.5);
	points.addPoint( 0.75,  0.5);
	points.addPoint( 1.00,  0.5);
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
	glColor3f(1.0,0.0,0.0);

	// Punkte
	glPointSize(7.0);
	glBegin(GL_POINTS);
	for (int i=0; i<points.getCount(); i++) {
		glVertex2f(points.getPointX(i),points.getPointY(i));
	}

	glEnd();

	// Hüllpolygone zeichnen
	glColor3f(0.0,0.0,1.0);

	for (int i=0; i<curveSizes.count(); i++) {
		glBegin(GL_LINE_STRIP);
		for (int j=0; j<curveSizes[i]; j++) {
			glVertex2f(points.getPointX(j+sumCurveSizes(i)),points.getPointY(j+sumCurveSizes(i)));
		}
		glEnd();
	}

	// Kurve
	glColor3f(1.0,1.0,1.0);

	// Kurvenpunkte auf Kurven aufteilen
	Points* pointsArray = new Points[curveSizes.count()];
	for (int j=0; j<curveSizes.count(); j++) {
		for (int i=sumCurveSizes(j); i<sumCurveSizes(j)+curveSizes[j]; i++) {
			pointsArray[j].addPoint(points.getPointX(i), points.getPointY(i));
		}
	}

	// Kurven zeichnen
	for (int i=0; i<curveSizes.count(); i++) {
		drawBezierCurve(pointsArray[i].getCount(), pointsArray[i], this->epsilon_draw);
	}

	// Schnittpunkte zeichnen

	if (doIntersection) {
		glColor3f(0.0,1.0,0.0);
		for (int i=0; i<curveSizes.count(); i++) {
			for (int j=i+1; j<curveSizes.count(); j++) {
				drawIntersect(pointsArray[i], pointsArray[j], this->epsilon_intersection);
			}
		}
	}
	if (doSelfIntersection) {
		glColor3f(1.0,0.0,1.0);
		for (int i=0; i<curveSizes.count(); i++) {
			drawSelfIntersect(pointsArray[i], this->epsilon_intersection);
		}
	}

	delete pointsArray;
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

int GLWidget::sumCurveSizes(int index)
{
	int retVal = 0;

	for (int i=0; i<index; i++) {
		retVal += curveSizes[i];
	}

	return retVal;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton) {
		QPoint pos = event->pos();
		QPointF posF = transformPosition(pos);
		points.setPointX(clickedPoint,posF.x());
		points.setPointY(clickedPoint,posF.y());
		update();
	}
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton) {
		QPoint pos = event->pos();
		QPointF posF = transformPosition(pos);
		clickedPoint = points.getClosestPoint(posF.x(),posF.y());
		points.setPointX(clickedPoint,posF.x());
		points.setPointY(clickedPoint,posF.y());
		update();

		return;
	}

	if (event->buttons() & Qt::RightButton) {
		QPoint pos = event->pos();
		QPointF newP = transformPosition(pos);
		newPoint(newP);
	}
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent *)
{
}

void GLWidget::newPoint(QPointF newP)
{
	if (curveSizes.count() > 4) {
		return;
	}

	Points oldCurvePoints;
	if (whichCurve == 1) {
		for (int i=0; i<curveSizes[0]; i++) {
			oldCurvePoints.addPoint(points.getPointX(i), points.getPointY(i));
		}
		Points newSegmentPoints = createSegment(oldCurvePoints, newP);
		for (int i=0; i<newSegmentPoints.getCount(); i++) {
			points.addPoint(newSegmentPoints.getPointX(i), newSegmentPoints.getPointY(i), curveSizes[0]+i);
		}
		curveSizes.insert(1, newSegmentPoints.getCount());
		this->secondCurveIndex++;
	} else {	// whichCurve == 2
		for (int i=0; i<curveSizes[secondCurveIndex]; i++) {
			oldCurvePoints.addPoint(points.getPointX(sumCurveSizes(secondCurveIndex)+i), points.getPointY(sumCurveSizes(secondCurveIndex)+i));
		}
		Points newSegmentPoints = createSegment(oldCurvePoints, newP);

		for (int i=0; i<newSegmentPoints.getCount(); i++) {
			points.addPoint(newSegmentPoints.getPointX(i), newSegmentPoints.getPointY(i));
		}
		curveSizes.append(newSegmentPoints.getCount());
	}

	update();
}

void GLWidget::setIntersection(int state)
{
	doIntersection = (state == Qt::Checked);
	update();
}

void GLWidget::setSelfIntersection(int state)
{
	doSelfIntersection = (state == Qt::Checked);
	update();
}

void GLWidget::setEpsilonDraw(double value)
{
	epsilon_draw = value;
	update();
}

void GLWidget::setEpsilonIntersection(double value)
{
	epsilon_intersection = value;
	update();
}
