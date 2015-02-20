#include "knots.h"
#include <cstdio>

Knots::Knots():Points()
{
    insertKnot(0.0);
    insertKnot(1.0);
}

// copy constuctor
Knots::Knots(const Knots &ks)
{
	if (this==&ks) {
		return; // nothing to do, it's me
	}

	*this = ks;
}

// assignment operator
Knots &Knots::operator = (const Knots &ks)
{
	if (this != &ks) {
		this->pointlist.clear();
		insertKnot(0.0);
		insertKnot(1.0);
		for (int i = 1; i < ks.pointlist.size()-1; i++) {
			this->pointlist.insert(i,( QPointF(ks.pointlist.at(i).x(), ks.pointlist.at(i).y()) ));
		}
	}
}

/*
 * Funktion zum Einfügen der Start-Knoten
 */
void Knots::insertKnot(float value)
{
	if (value < 0.0 || value > 1.0) return;

	float xcoord = value * 1.8 - 0.9;
	if (pointlist.size()<2) {
		pointlist.append(QPointF(xcoord,-0.9));
		return;
	}

	int i = 0;
	while (i<pointlist.size() && getValue(i)<value) {
		i++;
	}

	if (i<pointlist.size()) {
		pointlist.insert(i,QPointF(xcoord,-0.9));
	}
	else {
		printf("Knoten nicht einfügbar bei %f\n",value);
	}
}

/*
 * Funktion zum online-Einfügen von Knoten
 *
 * Parameter:
 * value:	Wert des Knotens (0<= value <= 1)
 * degree:	Grad der Kurve
 *
 * Rückgabewert: Index, an dem der Knoten eingefügt wurde. -1, wenn er nicht eingefügt wurde.
 */
int Knots::insertKnot(float value, int degree)
{
	if (value < 0.0 || value > 1.0) return -1;

    float xcoord = value * 1.8 - 0.9;
    if (pointlist.size()<2) {
        pointlist.append(QPointF(xcoord,-0.9));
		return -1;
    }

	int i = 0;	// Index, an dem eingefügt wird
	int j = 0;	// Index, der zurückgegeben wird (r)
	float valueI = 0.0;

	if (value == getValue(pointlist.size()-degree-1)) {
		/*
		 * Sonderfall: Der Knoten x_m+1 soll erneut eingefügt werden.
		 * Da von hinten eingefügt wird, wäre der Index des neuen Knotens außerhalb des erlaubten Intervalls.
		 * Deshalb muss der Index so zurückgegeben werden, als wäre der neue Knoten kleiner als x_m+1.
		 */
		i = pointlist.size()-degree;
		j = degree+1;// für Extremfälle
		while (j<pointlist.size() && getValue(j)<value) {
			j++;
		}
	} else {
		while (i<pointlist.size() && getValue(i)<=value) {
			valueI = getValue(i);
			i++;
		}
		j = i;
	}

	if (j<=degree || j>(pointlist.size()-degree-1)) {
		qDebug("Knoten nicht einfügbar bei %f\n",value);
		return -1;
	}

    if (i<pointlist.size()) {
		pointlist.insert(i,QPointF(xcoord,-0.9));
		return j;
    }
    else {
		qDebug("Knoten nicht einfügbar bei %f\n",value);
		return -1;
    }
}

int Knots::insertKnotX(float x, int degree)
{
	return insertKnot((x+0.9) / 1.8, degree);
}

void Knots::setValueX(int i, float x)
{
    // do not move first and last knot
    if (i<=0 || i>=pointlist.size()-1) return;

    float xsafe = x;
    if (xsafe < pointlist[i-1].x()) { xsafe = pointlist[i-1].x(); }
    if (xsafe > pointlist[i+1].x()) { xsafe = pointlist[i+1].x(); }
    pointlist[i].setX(xsafe);
}

float Knots::getValue(int i)
{
    return (pointlist[i].x()+0.9) / 1.8;
}
