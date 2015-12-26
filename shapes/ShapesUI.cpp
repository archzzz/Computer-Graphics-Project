#include "../cse452.h"
#include "ShapesUI.h"
#include "ShapesInterface.h"
#include "../Color.h"
#include <FL/Fl.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include "Shape.h"

using namespace std;

#define PI 3.141592

Shape shape;
Cube cube;
Cylinder cylinder;
Cone cone;
Sphere sphere;
Bunny bunny;

ShapesUI::ShapesUI() {
	width = height = 0;

	// ToDo: initialize your variables here
}

ShapesUI::~ShapesUI() {
	// ToDo: delete your variables here
}

void ShapesUI::resize(int w, int h) {
	width = w;
	height = h;
}

void ShapesUI::draw() {
	// Sets up the viewport and background color
	setup3DDrawing( Color( 0,0,0 ), width, height, true );

	// Changes the way triangles are drawn
	switch ( shapesUI->getDisplayType() ) {
	case DISPLAY_WIREFRAME: {
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT, GL_LINE);
		glColor3f(1.0f, 1.0f, 1.0f);
							} break;
	case DISPLAY_FLAT_SHADING: {
		glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT, GL_FILL);
		glColor3f(1.0f, 1.0f, 1.0f);
		glShadeModel(GL_FLAT);
							   } break;
	case DISPLAY_SMOOTH_SHADING: {
		glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT, GL_FILL);
		glColor3f(1.0f, 1.0f, 1.0f);
		glShadeModel(GL_SMOOTH);
								 } break;
	default: break;
	}

	// Setup the camera
	gluLookAt( 3.5 * cos( shapesUI->getYRot() ) * cos( shapesUI->getXRot() ), 
		3.5 * sin( shapesUI->getYRot() ), 
		3.5 * cos( shapesUI->getYRot() ) * sin( shapesUI->getXRot() ), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// ToDo: draw your shape here
	// DO NOT put the actual draw OpenGL code here - put it in the shape class and call the draw method
	int tessel1 =shapesUI->getTessel1();
	int tessel2 =shapesUI->getTessel2();

	switch (shapesUI->getShapeType()) {
	case SHAPE_SPHERE:{
		if(shapesUI->getDisplayType() == DISPLAY_WIREFRAME){
			sphere.drawShapeWireframe();
		}else if(shapesUI->getDisplayType() == DISPLAY_FLAT_SHADING){
			sphere.drawShapeFlat();
		}else{
			sphere.drawShapeSmooth();
		}

					  } break;
	case SHAPE_CONE:{
		if(shapesUI->getDisplayType() == DISPLAY_WIREFRAME){
			cone.drawShapeWireframe();
		}else if(shapesUI->getDisplayType() == DISPLAY_FLAT_SHADING){
			cone.drawShapeFlat();
		}else{
			cone.drawShapeSmooth();
		}
					} break;
	case SHAPE_CYLINDER:{
		if(shapesUI->getDisplayType() == DISPLAY_WIREFRAME){
			cylinder.drawShapeWireframe();
		}else if(shapesUI->getDisplayType() == DISPLAY_FLAT_SHADING){
			cylinder.drawShapeFlat();
		}else{
			cylinder.drawShapeSmooth();
		}
						} break;
	case SHAPE_CUBE:{

		if(shapesUI->getDisplayType() == DISPLAY_WIREFRAME){
			cube.drawShapeWireframe();
		}else if(shapesUI->getDisplayType() == DISPLAY_FLAT_SHADING){
			cube.drawShapeFlat();
		}else{
			cube.drawShapeFlat();
		}
					} break;
	case SHAPE_BUNNY:{
		bunny.drawShapeFlat();
					 } break;
	default:
		break;
	}
	endDrawing();
}

int ShapesUI::handle(int event) {
	return 0;
}

void ShapesUI::changedShape()
{
	// ToDo: Switch shapes
	int tessel1 =shapesUI->getTessel1();
	int tessel2 =shapesUI->getTessel2();
	switch (shapesUI->getShapeType()) {
	case SHAPE_CYLINDER:{
		cylinder.NewTesselation(tessel1,tessel2);
						} break;
	case SHAPE_CUBE:{
		cube.NewTesselation(tessel1, tessel1);
					} break;
	case SHAPE_CONE:{
		cone.NewTesselation(tessel1,tessel2);
					} break;
	case SHAPE_SPHERE:{
		sphere.NewTesselation(tessel1,tessel1);
					  } break;
	case SHAPE_BUNNY:{
		bunny.NewTesselation(tessel1,tessel1);
					 } break;
	default:
		break;
	}

	RedrawWindow();
}

void ShapesUI::changedTessel( ) {
	// ToDo: tessellate your shape here
	int tessel1 = shapesUI->getTessel1();
	int tessel2 = shapesUI->getTessel2();

	switch (shapesUI->getShapeType()){
	case SHAPE_CUBE:{
		cube.NewTesselation(tessel1,tessel1);
					}break;
	case SHAPE_CYLINDER:{
		cylinder.NewTesselation(tessel1,tessel2);
						} break;
	case SHAPE_CONE:{
		cone.NewTesselation(tessel1,tessel2);
					} break;
	case SHAPE_SPHERE:{
		sphere.NewTesselation(tessel1,tessel2);
					  } break;
	case SHAPE_BUNNY:{
		bunny.NewTesselation(tessel1,tessel1);
					 } break;
	default:
		break;
	}

	RedrawWindow();
}
