#include "../cse452.h"
#include "../Color.h"
#include <FL/Fl.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include "Shape.h"

using namespace std;

#define PI 3.141592

void Cylinder::NewTesselation(int slices, int stacks){
	tesselation.clear();
	slices = max(slices,3);
	fans(slices);

	for (int i = 0; i < slices-1 ; i++)
	{
		threePointsFan(fan[i], fan[i+1], O2, Vector3(0.0,-1.0,0.0));
		threePointsFan(fan[i]+Vector3(0.0,1.0,0.0), O1, fan[i+1]+Vector3(0.0,1.0,0.0), Vector3(0.0,1.0,0.0));
	}
	threePointsFan(fan[slices-1], fan[0], O2, Vector3(0.0,-1.0,0.0));
	threePointsFan(fan[slices-1]+Vector3(0.0,1.0,0.0),O1, fan[0]+Vector3(0.0,1.0,0.0),Vector3(0.0,1.0,0.0));


	Vector3 v = Vector3(0.0, 1.0, 0.0) * (1.0 / stacks);
	vector<vector<point>> a;

	for (int i = 0; i < fan.size() ; i++){
		vector<point> b;
		Vector3 normal = Vector3(fan[i][0], 0.0, fan[i][2]);
		normal.normalize();
		for(int j=0; j<= stacks; j++){
			Point3 a = fan[i] + v*j;
			point p =  { a , normal}; 
			b.push_back(p);
		}
		a.push_back(b);
	}

	for(int i = 0; i< slices-1 ; i++){
		Vector3 normal =(a[i][1].p - a[i][0].p) ^ (a[i+1][0].p - a[i][0].p);
		normal.normalize();
		for(int j = 0; j< stacks; j++){
			fourPoints(a[i][j], a[i][j+1], a[i+1][j+1], a[i+1][j], normal);
		}
	}

	Vector3 normal =(a[slices-1][1].p- a[slices-1][0].p) ^ (a[0][0].p- a[slices-1][0].p);
	normal.normalize();
	for(int j=0; j< stacks; j++){
		fourPoints(a[slices-1][j], a[slices-1][j+1], a[0][j+1], a[0][j], normal);
	}
}

void Cylinder::intersect( const Point3& p, Vector3& dir){
	hr.hits.clear();
	double A = dir[0]* dir[0] + dir[2]*dir[2];
	double B = 2*( p[0]* dir[0] + p[2]* dir[2]);
	double C = (p[0]* p[0] + p[2]* p[2] - 0.25);
	double t,f;
	Vector3 n = Vector3(0,1,0);
	Point3 a;

	if(dir* n !=0){
		t = (Point3(0,0.5,0) - p) * n /(dir * n);
		a = p + t*dir;
		f = a[0]* a[0] + a[2]* a[2] - 0.25;
//		if(t>=0 && t<=4 && f<=0){
		if(f <= 0){
			hr.addHit(t, 0, 0, a, n);
		}

		t = (Point3(0,-0.5,0) - p) * n /(dir * n );
		a = p + t* dir;
		f = a[0]* a[0] + a[2]* a[2] - 0.25;
//		if(t>0 && t<4 && f<0){
		if(f < 0){
			hr.addHit(t, 0, 0, a, Vector3(0,-1,0));
		}
	}

	if(B* B > 4*A*C){
		t = (-B - sqrt(B*B - 4*A*C))/(2*A); 
//		if(t>=0 && t<=4){
			a= p+ t*dir;
			if(a[1] >= -0.5 && a[1] < 0.5){
				n = a-Point3(0,a[1],0);
				n.normalize();
				hr.addHit(t, 0, 0, a, n);
//			}
		}
		t = (-B + sqrt(B*B - 4*A*C))/(2*A);
//		if(t>=0 && t<= 4){
			a= p+ t*dir;
			if(a[1] >= -0.5 && a[1] < 0.5){
				n = a-Point3(0,a[1],0);
				n.normalize();
				hr.addHit(t, 0, 0, a, n);
//			}
		}
	} 
	hr.hits.sort();
}