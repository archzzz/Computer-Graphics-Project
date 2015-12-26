#include "../cse452.h"
#include "../Color.h"
#include <FL/Fl.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include "Shape.h"

using namespace std;

void Cube::tesselFace(Point3 ld, Point3 rd, Point3 lu, int tessel)
{
	Vector3 v1 = (rd - ld) * (1.0 / tessel);
	Vector3 v2 = (lu - ld) * (1.0 / tessel);	

	Vector3 nor = v1 ^ (v2);
	nor.normalize();

	vector<vector<Point3>> a;

	for(int i = 0; i<=tessel; i++){
		vector<Point3> b;
		for(int j = 0; j<=tessel; j++){
			b.push_back(Point3(ld + (v1*i + v2*j ) ));
		}
		a.push_back(b);
	}

	for (int i = 0; i < tessel; i++){
		for (int j = 0; j < tessel; j++)
		{
			struct triangle temp1 = { a[i][j], a[i+1][j], a[i+1][j+1], nor };
			struct triangle temp2 = { a[i][j], a[i+1][j+1], a[i][j+1], nor };
			tesselation.push_back(temp1);
			tesselation.push_back(temp2);
		}
	}
}

void Cube::NewTesselation(int slices, int stacks){

	tesselation.clear ();
	slices = max(slices,1);

	tesselFace(P1,P2,P5,slices);
	tesselFace(P2,P3,P6,slices);
	tesselFace(P3,P4,P7,slices);
	tesselFace(P4,P1,P8,slices);
	tesselFace(P1,P4,P2,slices);
	tesselFace(P5,P6,P8,slices);
}

void Cube::intersect( const Point3& p, Vector3& dir){
	hr.hits.clear();
	Vector3 n[] = {Vector3(0,1,0), Vector3(0,-1,0), Vector3(1,0,0), Vector3(-1,0,0), Vector3(0,0,1), Vector3(0,0,-1)};
	Point3 g[] = {Point3(0,0.5,0), Point3(0,-0.5,0), Point3(0.5,0,0), Point3(-0.5,0,0), Point3(0,0,0.5), Point3(0,0,-0.5)};
	double t;
	

	for(int i = 0; i<6; i++){
		if(dir* n[i] !=0){
			t = (g[i]-p) * n[i] /(dir*n[i]);
//			if(t>=0 && t<=4){
				Point3 a = p+t*dir;
				if(check(a[0]) && check(a[1]) && check(a[2]) ){
					hr.addHit(t, 0, 0, a, n[i]);
				}
//			}
		}
	}
	hr.hits.sort();
}

boolean Cube::check(double a){
	return (abs(a) < 0.5 + 1e-10);
}