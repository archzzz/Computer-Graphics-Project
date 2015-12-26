#include "../cse452.h"
#include "../Color.h"
#include <FL/Fl.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include "Shape.h"
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

void Bunny::NewTesselation(int slices, int stacks){
	string line;
	vector<Point3> vertex;

	ifstream file;
	file.open("../data/mesh/bunny_mid.m");
	/*	if(file == NULL ) {
	perror("open fail");
	}*/

	while (getline(file, line)){
		string type;
		int n;
		istringstream read(line);
		read >> type;

		if (type == "Vertex"){
			Point3 p;
			read >> n >> p[0] >> p[1] >> p[2];
			p[0] = p[0] * 0.05;
			p[1] = p[1] * 0.05;
			p[2] = p[2] * 0.05;
			vertex.push_back(p);
		}

		if (type == "Face"){
			read >> n;
			int index;
			vector<Point3> face;
			while (read >> index){
				face.push_back(vertex[index - 1]);
			}
			for (int j = 1; j < face.size() - 1; j++){
				Vector3 normal = (face[j] - face[0]) ^ (face[j + 1] - face[0]);
				normal.normalize();
				struct triangle u = { face[j], face[j + 1], face[0], normal, vector<Vector3>() };
				tesselation.push_back(u);
			}
		}
	}
	file.close();
}

void Bunny::intersect(const Point3& p, Vector3& dir){
	double t;
	for (int i = 0; i < tesselation.size(); i++){
		if (dir* tesselation[i].nFace != 0){
			t = (tesselation[i].p1 - p) * tesselation[i].nFace / (dir* tesselation[i].nFace);
			if (t >= 0 && t <= 4){
				Point3 a = p + t*dir;
				if (insideTriangle(a, tesselation[i])){
					hr.addHit(t, 0, 0, a, tesselation[i].nFace);
				}
			}
		}
	}
}

bool Bunny::insideTriangle(Point3 a, struct triangle p){
	double tri = ((p.p1 - p.p3) ^ (p.p2 - p.p3)).length()+ 1e-15;
	double check = ((a - p.p1) ^ (p.p2 - p.p1)).length() + ((a - p.p2) ^ (p.p3 - p.p2)).length() + ((a - p.p3) ^ (p.p1 - p.p3)).length();
	return (tri >= check);
}