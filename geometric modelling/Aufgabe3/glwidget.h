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
	void resetPoints		();
	void setupThree			();

    float aspectx, aspecty;
    Points points;
    Knots knots;
    int clickedPoint;
    int clickedKnot;
    float epsilon_draw;
	bool drawCurve;			// gibt an, ob die Kurve gezeichnet werden soll oder nicht
	int degree;

	// deBoor-Sachen
	Points bezierPoints;
	Knots bezierKnots;

	void drawBSplineCurve();
	void deBoorStarter(Points &localPoints, Knots &localKnots, int multiplicity, int newKnotIndex, float newKnotValue);
	void deBoor(Knots &localKnots, Points ps, int k, int r, float t, Points *result);
};



#endif // GLWIDGET_H
