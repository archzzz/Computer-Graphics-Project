#include "../cse452.h"
#include "../sceneview/MyScene.h"
#include "RenderingInterface.h"
#include <FL/gl.h>
#include <cfloat>
#include <queue>
#include <list>
#include <limits>
#include "math.h"

#define RECURSIVE_LIMIT 2
using namespace std;
Color clamp(const Color &col);

void MyScene::render(int type, int width, int height, unsigned char* pixels) {
	if (!isLoaded) {
		return;
	}

	// Add your rendering code here.
	// Keep track of your progress as a value between 0 and 1
	// so the progress bar can update as the rendering progresses
	switch (type) {
	case RenderingUI::RENDER_SCANLINE:  break;
	case RenderingUI::RENDER_RAY_TRACING: {
		rendering(width, height, pixels);} break;
	case RenderingUI::RENDER_PATH_TRACING:  break;
	default: break;
	}
}

void MyScene::stopRender()
{
	// Because this is threaded code, this function
	// can be called in the middle of your rendering code.
	// You should then stop at the next scanline
	stop = true;
	progress = 1;
}

double MyScene::getRenderProgress() {
	// return the current progress as a value between 0 and 1
	return progress ;
}

// add extra methods here

void MyScene::rendering(int width, int height, unsigned char* pixels){

	//flattening tree
	flattenTree.clear();
	subgraph.find("root")->second->getMatrix(Matrix4::identity(), flattenTree);

	progress = 0;
	stop = false;
	for(int h = 0; h < height; h++){
		if(!stop){
			for(int w = 0; w < width; w++){			
				Point3 eye = camera.getEye();
				Point3 qs = Point3((double)(2*w +1)/width -1, 1 - (double)(2*h + 1)/height, -1);
				Point3 qw = camera.getCameraToWorld() * qs;
				Vector3 ray = unit(qw - eye);

				Color col = clamp(RayTrace(RECURSIVE_LIMIT, eye, ray));
				int i = ((height - 1 - h)* width + w) * 3;
				pixels[i  ] = (unsigned char) (col[0] * 255.0f);
				pixels[i+1] = (unsigned char) (col[1] * 255.0f);
				pixels[i+2] = (unsigned char) (col[2] * 255.0f);
			}
			progress = (double) (h+1) / height;
			Fl::check();
		}
	}
}

double __clamp(double elt, double begin, double end) {
	return min(max(elt, begin), end);
}

Color clamp(const Color &col){
	return Color (__clamp(col[0], 0.0f, 1.0f), __clamp(col[1], 0.0f, 1.0f), __clamp(col[2], 0.0f, 1.0f));
}

bool MyScene::intersection(Point3 point, Vector3 dir, int &index){
	index = -1;
	double min_dis = (std::numeric_limits<double>::max)();
	for(unsigned int i = 0; i < flattenTree.size(); i++){
		Point3 pObject = flattenTree[i].inverse * point;
		Vector3 dObject = unit(flattenTree[i].inverse * dir);

		flattenTree[i].obj->shape->intersect(pObject, dObject);
		flattenTree[i].obj->shape->hr.sortHits();

		if(flattenTree[i].obj->shape->hr.getFirstHit(flattenTree[i].t, flattenTree[i].u, flattenTree[i].v,
			flattenTree[i].p, flattenTree[i].n)){

				flattenTree[i].n = unit(flattenTree[i].inverse.transpose() * flattenTree[i].n);
				flattenTree[i].p = flattenTree[i].trans * flattenTree[i].p;
				flattenTree[i].t = (flattenTree[i].p - point).length();		

				if(flattenTree[i].t < min_dis ){
					min_dis = flattenTree[i].t;
					index = i;
				}
		}	
	}
	if(index != -1){
		dir.normalize();
		flattenTree[index].theta = acos((-dir)*flattenTree[index].n);
		return true;
	}
	else 
		return false;
}

bool MyScene::hitObj(int &ind, int &lInd){
	for(unsigned int i = 0; i < flattenTree.size(); i++){
		Vector3 dObject = unit(flattenTree[i].inverse * (flattenTree[ind].p - lights[lInd].getPos() ));
		Point3 pObject = flattenTree[i].inverse * lights[lInd].getPos();

		if (i==ind) { continue; }

		flattenTree[i].obj->shape->intersect(pObject, dObject );
		flattenTree[i].obj->shape->hr.sortHits();

		Point3 p; 
		Vector3 n;
		double t,u,v;
		if(!flattenTree[i].obj->shape->hr.getFirstHit(t,u,v,p,n)) { continue; } 

		p = flattenTree[i].trans * p;
		t = (p - lights[lInd].getPos()).length();	

		double dist = (flattenTree[ind].p - lights[lInd].getPos()).length();

		if(t < dist ){
			return true;
		}
	}
	return false;
}

Color MyScene::RayTrace(int level, Point3 p, Vector3 dir){
	int ind; 
	if(intersection(p, dir, ind)){
		if(level == 1){
			return localIllumination(p, dir, ind);
		} else{
			Vector3 dirR = unit(Matrix4().rotation(flattenTree[ind].n, flattenTree[ind].theta) * dir);
			//Vector3 dirT = Vector3(0,0,0);						//transparency.
			if (flattenTree[ind].obj->reflect[0] == 0 && 
				flattenTree[ind].obj->reflect[1] == 0 &&
				flattenTree[ind].obj->reflect[2] == 0) {
				return localIllumination(p, dir, ind);
			} else {
				return localIllumination(p, dir, ind) + flattenTree[ind].obj->reflect * RayTrace(--level, flattenTree[ind].p, dirR);
			}
		}
	}
	else
		return background;
}

Color MyScene::localIllumination(Point3 p, Vector3 dir, int &ind){
	Color illumination = flattenTree[ind].obj->ambident * ambientLight;
	for (int i = 0; i < lights.size(); i++) {

		if(hitObj(ind,i)) { continue; }		

		double dl = (lights[i].getPos() - flattenTree[ind].p).length();
		double fatt = 1/(lights[i].getFalloff()[0] + lights[i].getFalloff()[1]*dl + lights[i].getFalloff()[2] *dl*dl);
		Vector3 light = unit(lights[i].getPos()-flattenTree[ind].p); 

		if (light * flattenTree[ind].n < 0) { continue; }

		Vector3 v = unit(camera.getEye() - flattenTree[ind].p);
		Vector3 r = Matrix4().rotation(flattenTree[ind].n, M_PI) * light;

		Color diffuse = lights[i].getColor() *fatt *(flattenTree[ind].obj->diffuse *(flattenTree[ind].n *light));
		Color specular = Color(0,0,0);
		if(r*v > 0){
			specular = lights[i].getColor() *fatt * flattenTree[ind].obj->specular * pow( r * v , flattenTree[i].obj->shine);
		}

		illumination += diffuse + specular;
	}
	return illumination;
}