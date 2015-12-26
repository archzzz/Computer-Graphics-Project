#include "../cse452.h"
#include "../Color.h"
#include <FL/Fl.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include "Shape.h"


using namespace std;

#define PI 3.141592

void Shape::drawShapeWireframe(){
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < tesselation.size(); i++)
	{
		glVertex3d(tesselation[i].p1[0], tesselation[i].p1[1], tesselation[i].p1[2]);
		glVertex3d(tesselation[i].p2[0], tesselation[i].p2[1], tesselation[i].p2[2]);
		glVertex3d(tesselation[i].p3[0], tesselation[i].p3[1], tesselation[i].p3[2]);
	}
	glEnd();
}

void Shape::drawShapeFlat(){
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < tesselation.size(); i++)
	{
		glNormal3d(tesselation[i].nFace[0], tesselation[i].nFace[1], tesselation[i].nFace[2]);
		glVertex3d(tesselation[i].p1[0], tesselation[i].p1[1], tesselation[i].p1[2]);
		glVertex3d(tesselation[i].p2[0], tesselation[i].p2[1], tesselation[i].p2[2]);
		glVertex3d(tesselation[i].p3[0], tesselation[i].p3[1], tesselation[i].p3[2]);
	}
	glEnd();
}

void Shape::drawShapeSmooth(){
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < tesselation.size(); i++)
	{
		glNormal3d(tesselation[i].nVertex[0][0], tesselation[i].nVertex[0][1], tesselation[i].nVertex[0][2]);
		glVertex3d(tesselation[i].p1[0], tesselation[i].p1[1], tesselation[i].p1[2]);
		glNormal3d(tesselation[i].nVertex[1][0], tesselation[i].nVertex[1][1], tesselation[i].nVertex[1][2]);
		glVertex3d(tesselation[i].p2[0], tesselation[i].p2[1], tesselation[i].p2[2]);
		glNormal3d(tesselation[i].nVertex[2][0], tesselation[i].nVertex[2][1], tesselation[i].nVertex[2][2]);
		glVertex3d(tesselation[i].p3[0], tesselation[i].p3[1], tesselation[i].p3[2]);
	}
	glEnd();
}


void Shape::fourPoints(point p1, point p2, point p3, point p4, Vector3 nFace){
	vector<Vector3> nVertex1;
	vector<Vector3> nVertex2;

	nVertex1.push_back(p1.n);
	nVertex1.push_back(p2.n);
	nVertex1.push_back(p3.n);
	struct triangle temp1 = {p1.p, p2.p, p3.p, nFace, nVertex1 };

	nVertex2.push_back(p1.n);
	nVertex2.push_back(p3.n);
	nVertex2.push_back(p4.n);
	struct triangle temp2 = {p1.p, p3.p, p4.p, nFace, nVertex2 };

	tesselation.push_back(temp1);
	tesselation.push_back(temp2);
}

void Shape::threePointsFan(Point3 p1, Point3 p2, Point3 p3, Vector3 normal){
	vector<Vector3> nVertex;

	nVertex.push_back(normal);
	nVertex.push_back(normal);
	nVertex.push_back(normal);
	struct triangle temp = {p1, p2, p3, normal, nVertex};
	tesselation.push_back(temp);
}

void Shape::fans(int slices){
	fan.clear();
	double angle = 2*PI / slices;
	Point3 r1 = Point3(0.0, -0.5, 0.5);
	Matrix3 R = Matrix3( Vector3(cos(angle), 0.0, -sin(angle)),  Vector3(0.0, 1.0, 0.0),Vector3(sin(angle), 0.0, cos(angle)));

	fan.push_back(r1);
	for(int i = 1; i<slices; i++){
		r1 = R * r1;
		fan.push_back(r1);
	}
}

