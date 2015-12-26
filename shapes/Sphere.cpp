#include "../cse452.h"
#include "../Color.h"
#include <FL/Fl.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include "Shape.h"

using namespace std;

#define PI 3.141592

void Sphere::recursive(const Point3 &p1, const Point3 &p2, const Point3 &p3, int n){
	if(n == 1){
		threePoints(p1,p2,p3);
	}

	else{
		Point3 p4 = midpoint(p2, p1);
		Point3 p5 = midpoint(p3, p2);
		Point3 p6 = midpoint(p1, p3);

		recursive(p1, p4, p6, n-1);
		recursive(p4, p2, p5, n-1);
		recursive(p6, p5, p3, n-1);
		recursive(p4, p5, p6, n-1);
	}
}

Point3 Sphere::midpoint(const Point3 &p1, const Point3 &p2){
	Vector3 m = Vector3( (p1[0]+p2[0])*0.5, (p1[1]+p2[1])*0.5, (p1[2]+p2[2])*0.5 );
	m.normalize();
	Point3 a= Point3(m[0]*0.5 ,m[1]*0.5 ,m[2]*0.5 );
	return a;
}

void Sphere::threePoints(Point3 p1, Point3 p2, Point3 p3){
	Vector3 nFace = (p2-p1)^(p3-p1);
	vector<Vector3> nVertex;

	Vector3 n1 = Vector3(p1[0], p1[1], p1[2]);
	n1.normalize();
	Vector3 n2 = Vector3(p2[0], p2[1], p2[2]);
	n2.normalize();
	Vector3 n3 = Vector3(p3[0], p3[1], p3[2]);
	n3.normalize();

	nVertex.push_back(n1);
	nVertex.push_back(n2);
	nVertex.push_back(n3);

	struct  triangle temp = {p1, p2, p3, nFace, nVertex};
	tesselation.push_back(temp);
}

void Sphere::NewTesselation(int slices, int stacks){
	tesselation.clear();
	slices = max(slices,1);
	slices = min(slices,5);

	sphereFans();

	for(int i=0; i<4 ; i++){
		recursive(O2, fan[i], fan[i+1], slices);
		recursive(O1, fan[i+6], fan[i+5], slices);
	}
	recursive(O2, fan[4], fan[0], slices);
	recursive(O1, fan[5], fan[9], slices);

	for(int i = 0; i<4 ;i++){
		recursive(fan[i], fan[i+5], fan[i+6], slices);
		recursive(fan[i+1], fan[i], fan[i+6], slices);
	}
	recursive(fan[4], fan[9], fan[5], slices);
	recursive(fan[0], fan[4], fan[5], slices);
}

void Sphere::sphereFans(){
	fan.clear();

	double angle1 = PI / 2.5 ; 
	double angle2 = -PI / 5 ;
	double angle3 = 26.565 * PI / 180.0 ;

	Matrix3 R1 = Matrix3( Vector3(cos(angle1), 0.0, -sin(angle1)),  Vector3(0.0, 1.0, 0.0),Vector3(sin(angle1), 0.0, cos(angle1)));
	Matrix3 R2 = Matrix3( Vector3(cos(angle2), 0.0, -sin(angle2)),  Vector3(0.0, 1.0, 0.0),Vector3(sin(angle2), 0.0, cos(angle2)));

	Point3 p1 = Point3( 0, -sin(angle3)*0.5, cos(angle3)*0.5);
	Point3 p2 = R2 * Point3(0, sin(angle3)* 0.5, cos(angle3)*0.5);

	fan.push_back(p1);
	for(int i=0; i<4 ; i++){
		p1 = R1 * p1;
		fan.push_back(p1);
	}
	fan.push_back(p2);
	for(int i=0; i<4 ; i++){
		p2 = R1 * p2;
		fan.push_back(p2);
	}
}

void Sphere::intersect( const Point3& p, Vector3& dir){
	hr.hits.clear() ;
	double A = dir[0]* dir[0] + dir[1]* dir[1] + dir[2]*dir[2];
	double B = 2*( p[0]* dir[0] + p[1]* dir[1] + p[2]* dir[2]);
	double C = (p[0]* p[0] + p[1]* p[1] + p[2]* p[2] - 0.25);
	double t;
	Vector3 n;
	Point3 a;

	if(B* B > 4*A*C){
		t = (-B - sqrt(B*B - 4*A*C))/(2*A); 
//		if(t>0 && t<4){
			a= p+ t*dir;
			n = a-Point3(0,0,0);
			n.normalize();
			hr.addHit(t, 0, 0, a, n);
//		}
		t = (-B + sqrt(B*B - 4*A*C))/(2*A);
//		if(t>0 && t<4){
			a= p+ t* dir;
			n = a-Point3(0,0,0);
			n.normalize();
			hr.addHit(t, 0, 0, a, n);
//		}
	}
	hr.hits.sort();
}