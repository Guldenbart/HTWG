#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QDebug>
#include "knots.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent=0);
    ~GLWidget();
public slots:
    void setEpsilonDraw        (double value);
protected:
    void paintGL              ();
    void initializeGL         ();
    void resizeGL             (int width, int height);
    void mouseMoveEvent       (QMouseEvent *event);
    void mousePressEvent      (QMouseEvent *event);
	void keyPressEvent		  (QKeyEvent *);
private:
    QPointF transformPosition(QPoint p);
    float aspectx, aspecty;
    Points points;
    Knots knots;
    int clickedPoint;
    int clickedKnot;
    float epsilon_draw;

	bool drawCurve;
	int degree;

	// deBoor-Sachen
	Points deBoorStarter(int multiplicity, int newKnot);
	void deBoor(Points ps, int k, int r, Points *result);
};



#endif // GLWIDGET_H
