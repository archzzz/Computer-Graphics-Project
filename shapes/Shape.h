#ifndef SHAPE_H
#define SHAPE_H

#include "../UIInterface.h"
#include "../cse452.h"
#include <FL/Fl_Window.H>
#include "Point3.h"
#include "Vector3.h"
#include "Matrix3.h"
#include "../intersection/HitRecord.h"
#include "math.h"

#include <vector>

using namespace std;

class Shape {
public:
	//Store 3 points of a triangle in counter clock 
	//sequence, normal of triangle and normals of each points.
	struct triangle
	{
		Point3 p1;
		Point3 p2;
		Point3 p3;
		Vector3 nFace;
		vector<Vector3> nVertex;
	};

	//Store a point and normal of that point.
	struct point{
		Point3  p;
		Vector3 n;
	};

	Shape() {};
	~Shape() {};

	//Draw wireframe shape on the screen
	void drawShapeWireframe();
	//Draw flat shading shap on the screen
	void drawShapeFlat();
	//Draw smooth shading shape on the screen
	virtual void drawShapeSmooth();

protected:
	//Store all triangles and their normals.
	vector<struct triangle> tesselation;
	//Store points on fans.
	vector<Point3> fan;	
	HitRecord hr;

	//Find vertices on fans accroding to slices, and store these points.
	void fans(int slices);

	//Take in four points in counter clock sequence and normal of the face, 
	//and splits points into two triangles, and store them.
	void fourPoints(point p1, point p2, point p3, point p4, Vector3 nFace);

	//Tesselate fans.
	void threePointsFan(Point3 p1, Point3 p2, Point3 p3, Vector3 normal);

private:
	//Tesselate shape.
	virtual void NewTesselation(int slices, int stacks) { return; }
	virtual void intersect( const Point3& p, Vector3& dir){ return;}

	friend class IntersectionUI;
	friend class MyScene;
};

class Cube : public Shape{
public:
	Cube() : 
		P1(Point3(-0.5,-0.5,0.5)),  P2(Point3(0.5,-0.5,0.5)),
		P3(Point3(0.5,-0.5,-0.5)), P4(Point3(-0.5,-0.5,-0.5)), 
		P5(Point3(-0.5,0.5,0.5)), P6(Point3(0.5,0.5,0.5)), 
		P7(Point3(0.5,0.5,-0.5)), P8(Point3(-0.5,0.5,-0.5)) {}
	~Cube(){};

	void NewTesselation(int slices, int stacks) override;
	void intersect( const Point3& p, Vector3& dir) override;
	void tesselFace(Point3 ld, Point3 rd, Point3 lu, int tessel);
	void drawShapeSmooth() override { drawShapeFlat(); }

	boolean check(double a);

private:
	//8 vertice of cube.
	const Point3 P1;
	const Point3 P2;
	const Point3 P3;
	const Point3 P4;
	const Point3 P5;
	const Point3 P6;
	const Point3 P7;
	const Point3 P8;
};

class Cylinder : public Shape{
public:

	Cylinder() : 
		O1(Point3(0.0, 0.5, 0.0)), O2(Point3(0.0, -0.5, 0.0)) {}

	void NewTesselation(int slices, int stacks) override;
	void intersect( const Point3& p, Vector3& dir) override;

private:
	//Centre point of upper and lower surface.
	const Point3 O1;
	const Point3 O2;
};

class Cone : public Shape{
public:
	Cone() :
		O1(Point3(0.0, 0.5, 0.0)), O2(Point3(0.0, -0.5, 0.0)) {}

	void NewTesselation(int slices, int stacks) override;
	void intersect( const Point3& p, Vector3& dir) override;

private:
	//2 vertices on y axis.
	const Point3 O1;
	const Point3 O2;
};

class Sphere : public Shape{
public:
	Sphere() : 
		O1(Point3(0.0, 0.5, 0.0)), O2(Point3(0.0, -0.5, 0.0)){}

	void NewTesselation(int slices, int stacks) override;
	void intersect( const Point3& p, Vector3& dir) override;
	//Recusive method to tesselate.
	void recursive(const Point3 &p1, const Point3 &p2, const Point3 &p3, int n);
	//Find midpoint of p1 and p2
	Point3 midpoint(const Point3 &p1, const Point3 &p2);
	//Take in 3 points, calculate normals and store them as triangle.
	void threePoints(Point3 p1, Point3 p2, Point3 p3);
	//Find vertics on sphere fans and store these points.
	void sphereFans();

private:
	//2 vertices on y axis.
	const Point3 O1;
	const Point3 O2;
};

class Bunny : public Shape{
public:
	Bunny() {}

	void NewTesselation(int slices, int stacks) override;
	void intersect( const Point3& p, Vector3& dir) override;

private:
	bool insideTriangle(Point3 a, struct triangle tessel);
};
#endif
