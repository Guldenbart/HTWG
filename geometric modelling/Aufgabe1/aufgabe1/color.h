////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////
#ifndef _COLOR_H_
#define _COLOR_H_

struct Color {
	Color (float r=1.0f, float g=1.0f, float b=1.0f) { this->r = r; this->g = g; this->b = b; }
	float r, g, b;
};


#endif