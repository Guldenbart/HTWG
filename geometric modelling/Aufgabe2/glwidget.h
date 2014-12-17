#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "points.h"

class GLWidget : public QGLWidget
{
	Q_OBJECT
public:
	GLWidget(QWidget *parent=0);
	~GLWidget();

public slots:
	void setIntersection       (int    state);
	void setSelfIntersection   (int    state);
	void setEpsilonDraw        (double value);
	void setEpsilonIntersection(double value);

protected:
	void paintGL              ();
	void initializeGL         ();
	void resizeGL             (int width, int height);
	void mouseMoveEvent       (QMouseEvent *event);
	void mousePressEvent      (QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);

private:
	QPointF transformPosition(QPoint p);
	int sumCurveSizes(int index);
	void newPoint(QPointF newPoint);

	Points  points;
	float   aspectx, aspecty;
	float   epsilon_draw, epsilon_intersection;
	int     clickedPoint;
	bool    doIntersection,doSelfIntersection;
	int		whichCurve;			// welche Kurve bei Rechtsklick erweitert werden soll
	int		secondCurveIndex;	// Index, an dem die Anzahl der Punkte der "urspr�nglichen" zweiten Kurve liegt
	QList<int> curveSizes;
};



#endif // GLWIDGET_H
