////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "viewSystem.h"
#include <iostream>

viewSystem::viewSystem()
{
	RotMode = VIEW_MATRIX_MODE;

	p1 = p2 = temp = NULL;
}

viewSystem::viewSystem(const float eye[4], const float dir[4], const float up[4], float f)
{
	F = f;

	EyePoint.setData(eye);
	ViewDir .setData(dir);
	ViewUp  .setData(up);

	RotMode = VIEW_MATRIX_MODE;

	p1 = p2 = temp = NULL;
}

viewSystem::viewSystem(const CVec4f eye, const CVec4f dir, const CVec4f up, float f)
{
	F        = f;
	EyePoint = eye;
	ViewDir  = dir;
	ViewUp   = up;
	RotMode  = VIEW_MATRIX_MODE;

	p1 = p2 = temp = NULL;
}

viewSystem::~viewSystem()
{
}

void viewSystem::setData(const float eye[4], const float dir[4], const float up[4], float f)
{
	viewSystem buf(eye,dir,up,f);
	*this = buf;
}

void viewSystem::setData(const CVec4f eye, const CVec4f dir, const CVec4f up, float f)
{
	viewSystem buf(eye,dir,up,f);
	*this = buf;
}

void viewSystem::setMode(MatrixMode mode)
{
	RotMode = mode;
}
void viewSystem::Zoom(float f)
{
	F+=f;
}

void viewSystem::RotateX(float angle)
{
	switch (RotMode) {
	case VIEW_FORMULA_MODE:
	case VIEW_MATRIX_MODE:
		{
			CMat4f Rot;
			float c = cos(angle);
			float s = sin(angle);

			Rot(1,1) = c; Rot(1,2) = -s; 
			Rot(2,1) = s; Rot(2,2) =  c;

			Rotate(Rot);break;
		}
	case VIEW_QUATERNION_MODE: 
		{
			// AUFGABE02

			CVec4f xVec(1,0,0,0);
			Rotate(xVec, angle);

			break;
		}
	}	
}

void viewSystem::RotateY(float angle)
{
	switch (RotMode) {
	case VIEW_FORMULA_MODE: 	
	case VIEW_MATRIX_MODE:  
		{
			CMat4f Rot;
			float c = cos(angle);
			float s = sin(angle);

			Rot(0,0) =  c; Rot(0,2) = s; 
			Rot(2,0) = -s; Rot(2,2) = c; 
			
			Rotate(Rot);break;
		}
	case VIEW_QUATERNION_MODE: 
		{
			// AUFGABE02

			CVec4f yVec(0,1,0,0);
			Rotate(yVec, angle);

			break;
		}
	}	
}

void viewSystem::RotateZ(float angle)
{
	switch (RotMode) {
	case VIEW_FORMULA_MODE: 	
	case VIEW_MATRIX_MODE:  
		{
			CMat4f Rot;
			float c = cos(angle);
			float s = sin(angle);

			Rot(0,0) = c; Rot(0,1) = -s; 
			Rot(1,0) = s; Rot(1,1) =  c; 
			
			Rotate(Rot);
			break;
		}
	case VIEW_QUATERNION_MODE: 
		{
			// AUFGABE02

			CVec4f zVec(0,0,1,0);
			Rotate(zVec, angle);

			break;
		}
	}	
}

void viewSystem::RotateDir(float angle)
{
	switch (RotMode) {
	case VIEW_FORMULA_MODE: 	
	case VIEW_MATRIX_MODE:  
		ViewUp = RotationMatrix(ViewDir,angle)*ViewUp;
		break;
	case VIEW_QUATERNION_MODE: 
		{
			// AUFGABE02

			RotateWQuaternions(angle, ViewUp, ViewDir);

			break;
		}
	}	
	ViewUp.normalize();
}

void viewSystem::RotateUp(float angle)
{
	switch (RotMode) {
	case VIEW_FORMULA_MODE: 	
	case VIEW_MATRIX_MODE:  
		ViewDir = RotationMatrix(ViewUp,angle)*ViewDir;
		break;
	case VIEW_QUATERNION_MODE: 
		{
			// AUFGABE02

			RotateWQuaternions(angle, ViewDir, ViewUp);

			break;
		}
	}	
	ViewDir.normalize();
}

void viewSystem::RotateHor(float angle)
{
	ViewHor = ViewDir.cross(ViewUp);
	ViewHor.normalize();
	
	switch (RotMode) {
	case VIEW_FORMULA_MODE: 	
	case VIEW_MATRIX_MODE:  
		{
			CMat4f Rot = RotationMatrix(ViewHor,angle);
			ViewUp     = Rot*ViewUp;
			ViewDir    = Rot*ViewDir; 
		}
	case VIEW_QUATERNION_MODE: 
		{
			// AUFGABE02

			RotateWQuaternions(angle, ViewDir, ViewHor);
			RotateWQuaternions(angle, ViewUp, ViewHor);

			break;
		}
	}
	ViewUp .normalize();
	ViewDir.normalize();
}

void viewSystem::Rotate(CMat4f mat)
{
	EyePoint = mat * EyePoint;
	ViewDir  = mat * ViewDir;
	ViewUp   = mat * ViewUp;

	ViewDir.normalize();
	ViewUp .normalize();
}

void viewSystem::Rotate(CVec4f axis, float angle)
{
	switch (RotMode) {
	case VIEW_FORMULA_MODE: 	
	case VIEW_MATRIX_MODE:  
		Rotate(RotationMatrix(axis,angle));
		break;
	case VIEW_QUATERNION_MODE: 
		// AUFGABE02

		RotateWQuaternions(angle, EyePoint, axis);
		RotateWQuaternions(angle, ViewDir, axis);
		RotateWQuaternions(angle, ViewUp, axis);

		ViewDir.normalize();
		ViewUp.normalize();

		break;
	}
}


Quaternion viewSystem::RotateAQuaternion(float angle, CVec4f vec, CVec4f axis)
{
	float s = sin(angle/2);
	float c = cos(angle/2);

	Quaternion q(c, axis*s);
	Quaternion p(0, vec);
	Quaternion q_(c, axis*(-s));

	Quaternion result = q*p*q_;

	return result;
}


void viewSystem::RotateWQuaternions(float angle, CVec4f& vec, CVec4f axis)
{
	Quaternion result = RotateAQuaternion(angle, vec, axis);

	vec = result.getVector(vec(3)==0 ? 0 : 1);
}

void viewSystem::Translate(CVec4f vec)
{
	EyePoint += vec;
}

CVec4f viewSystem::Project(CVec4f Point)
{	// central projection of Point
	CMat4f M;
	M(2,2) = 0.0;
	M(3,2) = -(1/F);
	Point  = M*(Point*(1/(1-Point(2)/F)));	// re-normalization
	return Point;
}

CMat4f viewSystem::getTransform1()
{
	CMat4f M; 
	// AUFGABE01

	M = getTransform2();
	M = Inverse(M);
	 
	return M;
}

// world to view
CMat4f viewSystem::getTransform2()
{
	//CMat4f M; 
	// AUFGABE01

	CVec4f n = (/*EyePoint - */ViewDir);
	n.normalize();
	CVec4f u = n.cross(ViewUp);
	u.normalize();
	CVec4f v = n.cross(u);

	float matrixData[4][4] = {
		{u(0),	u(1),	u(2),	-(EyePoint*u)},
		{v(0),	v(1),	v(2),	-(EyePoint*v)},
		{n(0),	n(1),	n(2),	-(EyePoint*n)},
		{0,		0,		0,		1}};

	CMat4f M(matrixData);
	 
	return M;
}

CMat4f viewSystem::RotationMatrix(CVec4f axis, float angle)
{
	CMat4f Rot;
	
	switch (RotMode) {
	case VIEW_FORMULA_MODE: 
		{
			float c = cos(angle);
			float s = sin(angle);

			Rot(0,0) = axis(0)*axis(0)*(1-c) +         c; 
			Rot(0,1) = axis(0)*axis(1)*(1-c) - axis(2)*s; 
			Rot(0,2) = axis(0)*axis(2)*(1-c) + axis(1)*s; 
			Rot(1,0) = axis(1)*axis(0)*(1-c) + axis(2)*s;
			Rot(1,1) = axis(1)*axis(1)*(1-c) +         c;
			Rot(1,2) = axis(1)*axis(2)*(1-c) - axis(0)*s;
			Rot(2,0) = axis(2)*axis(0)*(1-c) - axis(1)*s; 
			Rot(2,1) = axis(2)*axis(1)*(1-c) + axis(0)*s; 
			Rot(2,2) = axis(2)*axis(2)*(1-c) +         c;
		
			break;
		}
	case VIEW_MATRIX_MODE: 
		{
			float d = sqrt(axis(0)*axis(0)+axis(1)*axis(1));
			float a = axis(0)/d;
			float b = axis(1)/d;

			CMat4f Rot1;
			if (d) {
				Rot1(0,0) = a; Rot1(1,0) = -b;
				Rot1(0,1) = b; Rot1(1,1) =  a;
			}

			CMat4f Rot2;
			Rot2(0,0) =  axis(2); Rot2(2,0) =  d;
			Rot2(0,2) = -d;       Rot2(2,2) =  axis(2);

			CMat4f Rot3;
			Rot3(0,0) =  cos(angle); Rot3(1,0) =  sin(angle);
			Rot3(0,1) = -sin(angle); Rot3(1,1) =  cos(angle);
	
			CMat4f Rot4(Rot2);
			Rot4(2,0) = Rot2(0,2); Rot4(0,2) = Rot2(2,0);

			CMat4f Rot5(Rot1);
			Rot5(1,0) = Rot1(0,1); Rot5(0,1) = Rot1(1,0); 
	
			Rot = Rot5*Rot4*Rot3*Rot2*Rot1;
			break;
		}
	case VIEW_QUATERNION_MODE:
		std::cerr << "Error in rotation mode: For quaternions no rotation matrix required.\n";
		break;
	}
	
	return Rot;
}

CMat4f viewSystem::Inverse(CMat4f M)
// Berechnet die Inverse einer 4x4 Matrix M der Art 
//             | A a |
//             | 0 1 |
// mit einer orthonormalen 3x3 Matrix A. 
// Die Inverse ist dann gegeben durch
//             | A^T -A^t*a |
//             |  0     1   |
{
	float Mat[4][4];
	CVec4f a;
	for (int i=0; i<4; i++) for (int j=0; j<4; j++) Mat[i][j]=M(i,j);
	for (int i=0; i<3; i++) { Mat[i][3]=0; a(i)=M(i,3); }
	for (int i=0; i<3; i++) for (int j=0; j<i; j++) { float x=Mat[i][j]; Mat[i][j]=Mat[j][i]; Mat[j][i]=x; }
	CMat4f tmp(Mat);
	a = tmp*a;
	for (int i=0; i<3; i++) tmp(i,3)=-a(i);

	return tmp;
}


// interpolation
void viewSystem::setupLerp(int angleDeg, CVec4f axis)
{
	float angleT = angleDeg*(PI/180);

	p1 = new CVec4f[2];
	p2 = new CVec4f[2];
	temp = new CVec4f[2];

	p1[0] = ViewDir;
	p1[1] = ViewUp;

	for (int i = 0; i < 2; i++) {
		p2[i]	= (RotateAQuaternion(angleT, p1[i], axis)).getVector(0);
		temp[i]	= CVec4f();
	}

	std::cout << "LERP set up.\n";
}


void viewSystem::lerp(float step)
{
	for (int i = 0; i < 2; i++) {
		temp[i] = p1[i]*(1-step) + p2[i]*step;
	}
	
	ViewDir	= temp[0];
	ViewUp = temp[1];

	ViewDir.normalize();
	ViewUp.normalize();
}


void viewSystem::setupSlerp(int angleDeg, CVec4f axis)
{
	angle = angleDeg*(PI/180);

	p1 = new CVec4f[2];
	p2 = new CVec4f[2];
	temp = new CVec4f[2];

	p1[0] = ViewDir;
	p1[1] = ViewUp;

	for (int i = 0; i < 2; i++) {
		//p1[i].normalize();
		p2[i]	= (RotateAQuaternion(angle, p1[i], axis)).getVector(0);
		temp[i]	= CVec4f();
	}

	std::cout << "SLERP set up.\n";
}


void viewSystem::slerp(float step)
{

	for (int i = 0; i < 2; i++) {
		temp[i] = p1[i]*((sin((1-step)*angle))/(sin(angle))) + p2[i]*((sin(step*angle))/(sin(angle)));
	}
	
	ViewDir	= temp[0];
	ViewUp = temp[1];

	//ViewDir.normalize();
	//ViewUp.normalize();

}


void viewSystem::setupNlerp(int angleDeg, CVec4f axis)
{
	
	angle = angleDeg*(PI/180);

	p1 = new CVec4f[2];
	p2 = new CVec4f[2];
	temp = new CVec4f[2];

	p1[0] = ViewDir;
	p1[1] = ViewUp;

	for (int i = 0; i < 2; i++) {
		p1[i].normalize();
		p2[i]	= (RotateAQuaternion(angle, p1[i], axis)).getVector(0);
		temp[i]	= CVec4f();
	}	

	std::cout << "NLERP set up.\n";
}


void viewSystem::nlerp(float step)
{

	for (int i = 0; i < 2; i++) {
		temp[i] = p1[i]*((sin((1-step)*angle))/(sin(angle))) + p2[i]*((sin(step*angle))/(sin(angle)));
	}
	
	ViewDir	= temp[0];
	ViewUp = temp[1];

	ViewDir.normalize();
	ViewUp.normalize();

}


void viewSystem::cleanUp()
{
	if (p1 != NULL) {
		delete p1;
		p1 = NULL;
	}

	if (p2 != NULL) {
		delete p2;
		p2 = NULL;
	}

	if (temp != NULL) {
		delete temp;
		temp = NULL;
	}
}
