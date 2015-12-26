#include "cse452.h"
#include "ScreenPoint.h"
#include "BrushInterface.h"
#include <FL/Fl.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/fl_draw.H>
#include <FL/gl.h>
#include <cstring>
#include <cmath>
#include <algorithm>

using namespace std;

#define CLEAR(x, y)	memset(&x,y,sizeof(x))

MyBrush::MyBrush() 
{
	isMouseDown = false;

	imageWidth  = screenWidth = 0;
	imageHeight = screenHeight = 0;

	// initialize your data here
}

MyBrush::~MyBrush() {
	// destroy your data here
}

void MyBrush::resize(int width, int height) {
	screenWidth  = width;
	screenHeight = height;

	// First time initialization
	//if ( imageWidth == 0 ) {
	imageWidth = screenWidth;
	imageHeight = screenHeight;

	// Make image black
	pixelData.resize( width * height * 3, 0 );
	//}
}

void MyBrush::loadImage(Fl_Image* image) {
	imageWidth = image->w();
	imageHeight = image->h();
	// Reset viewport
	resize( screenWidth, screenHeight );
	pixelData.resize( imageWidth * imageHeight * 3, 0 );

	// OpenGL's windows are reversed in y
	const int delta = imageWidth * 3;
	unsigned char* src = (unsigned char*) *image->data();
	for (int i = 0; i < imageHeight; i++) {
		// Ok, this is ugly
		unsigned char* dest = &pixelData[ ((imageHeight - 1 - i) * imageWidth * 3) ];
		memcpy(dest, src, delta);
		src += delta;
	}
}

void MyBrush::draw() {
	// Set up camera for drawing
	setup2DDrawing( Color(0,0,0), screenWidth, screenHeight );

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Draw a border around the actual image
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2i( 0,            0 );
	glVertex2i( imageWidth+1, 0 );
	glVertex2i( imageWidth+1, imageHeight+1 );
	glVertex2i( 0,            imageHeight+1 );
	glEnd();


	glRasterPos2i(0, 0);
	// Copy data into window
	//for ( int iX = 0; iX < 100; iX++ )
	//putPixel( iX, iX, Color(1,0,0) );

	glDrawPixels(imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, &pixelData[0]);

	// These 5 lines draw a white line across your canvas
	// Remove this and replace it with intelligent OpenGL preview code


	// Add in your OpenGL pre-view code here
	const int radius = brushUI->getRadius();	
	const float pixelFlow = brushUI->getFlow();
	const Color colBrush = brushUI->getColor();

	glColor3f(colBrush[0],colBrush[1],colBrush[2]);

	buffer.clear();
	circle(mouseDrag[0],mouseDrag[1], radius);

	if(brushUI->getToolType() == TOOL_BRUSH){
		glBegin(GL_POINTS);
		for(int i=0;i<buffer.size();i++){
			if(buffer[i][0]>0 && buffer[i][0]<screenWidth && buffer[i][1]>0 && buffer[i][1]<screenHeight){
				glVertex2d(buffer[i][0],buffer[i][1]);
			}
		}
		glEnd();
	}
	// display draw in progress (mouse is down)
	if (isMouseDown) {

		if(brushUI->getToolType() == TOOL_CIRCLE){

			int r1=(int) sqrt(pow(mouseDrag[0]-mouseDown[0],2)+pow(mouseDrag[1]-mouseDown[1],2))+0.5*radius;
			int r2=max((int) sqrt(pow(mouseDrag[0]-mouseDown[0],2)+pow(mouseDrag[1]-mouseDown[1],2))-0.5*radius,0);

			circleIntersec(radius,r1,r2);

			glBegin(GL_POINTS);
			for(int i=0;i<buffer.size();i++){
				if(buffer[i][0]>0 && buffer[i][0]<screenWidth && buffer[i][1]>0 && buffer[i][1]<screenHeight){
					glVertex2d(buffer[i][0],buffer[i][1]);
				}
			}
			for(int i=0;i<intersec.size();i++){
				if(intersec[i].size()==2){
					for(int j=intersec[i][0][0]; j<intersec[i][1][0];j++ ){
						if(	j<screenWidth && j>0){
							glVertex2d(j,intersec[i][0][1]);
						}
					}
				}
				if(intersec[i].size()==4){
					for(int j=intersec[i][0][0]; j<intersec[i][1][0];j++ ){
						if(	j<screenWidth && j>0){
							glVertex2d(j,intersec[i][0][1]);
						}
					}
					for(int j=intersec[i][2][0]; j<intersec[i][3][0];j++ ){
						if(	j<screenWidth && j>0){
							glVertex2d(j,intersec[i][0][1]);
						}
					}
				}
			}

			glEnd();
		}



		if(brushUI->getToolType() == TOOL_LINE){
			if(radius==1){
				buffer.clear();
				edge(mouseDown,mouseDrag);
				glBegin(GL_POINTS);
				for(int i=0;i<buffer.size();i++){
					if(buffer[i][0]>0 && buffer[i][0]<screenWidth && buffer[i][1]>0 && buffer[i][1]<screenHeight){
						glVertex2d(buffer[i][0],buffer[i][1]);
					}

				}
				glEnd();
			}else{
				lineIntersec(radius);

				glBegin(GL_POINTS);
				for(int i=0;i<intersec.size();i++){
					for(int j=1;j<intersec[i].size();j=j+2){
						for(int k=intersec[i][j-1][0];k<intersec[i][j][0];k++){
							if(k<screenWidth && k>0){
								glVertex2d(k, intersec[i][0][1]);
							}
						}
					}
				}
				for(int i=0;i<buffer.size();i++){
					if(buffer[i][0]>0 && buffer[i][0]<screenWidth && buffer[i][1]>0 && buffer[i][1]<screenHeight){
						glVertex2d(buffer[i][0],buffer[i][1]);
					}
				}
				glEnd();
			}
		}
		endDrawing();
	}
}
// This does pixel flow
void MyBrush::draw_callback( void *in_data )
{
	MyBrush *opMe = static_cast<MyBrush *>( in_data );
	// Repeat the time out if we're not done yet
	if ( opMe->isMouseDown == true ) {
		opMe->drawBrush();

		Fl::repeat_timeout( 0.05, MyBrush::draw_callback, (void *) opMe );

		RedrawWindow();
	}
}


int MyBrush::handle(int event) {
	// OpenGL & FLTK's y axes are oriented differently
	const ScreenPoint pt = ScreenPoint( Fl::event_x(), screenHeight - 1 - Fl::event_y() );

	switch (event) {
	case FL_PUSH: {
		mouseDrag = pt;
		mouseDown = pt;

		if (brushUI->getToolType() == TOOL_POLYGON) {
			if (isMouseDown == true) {
				polygon.push_back( mouseDrag );
			} else {
				isMouseDown = true;
				polygon.resize(0);
				polygon.push_back( mouseDrag );
			}
		} else {
			isMouseDown = true;
			if (brushUI->getToolType() == TOOL_BRUSH)
				Fl::add_timeout(0, draw_callback, this);
		}
		return 1;
				  }
	case FL_DRAG: mouseDrag = pt; RedrawWindow(); return 1;
	case FL_MOVE: 
		mouseDrag = pt;
		if ( brushUI->getToolType() == TOOL_BRUSH || ( brushUI->getToolType() == TOOL_POLYGON && isMouseDown ) )
			RedrawWindow();
		return 1;
	case FL_RELEASE: {
		mouseDrag = pt;
		if (brushUI->getToolType() != TOOL_POLYGON) {
			isMouseDown = false;
			switch (brushUI->getToolType()) {
			case TOOL_BRUSH: 
				break;
			case TOOL_LINE: 
				drawLine( ); 
				break;
			case TOOL_CIRCLE: 
				drawCircle( );
				break;
			case TOOL_FILTER: 
				filterRegion( ); 
				break;
			default: break;
			}
		} else if ( Fl::event_button3() || Fl::event_state( FL_SHIFT ) ) {
			isMouseDown = false;
			drawPolygon();
		}
		RedrawWindow();
		return 1;
					 }
	default: return 0;
	}
}

void MyBrush::circleIntersec(int radius,int r1, int r2){
	buffer.clear();
	intersec.clear();
	auto sizeL = circle(mouseDown[0],mouseDown[1],r1);
	auto sizeS = circle(mouseDown[0],mouseDown[1],r2);

	for(int i=1;i<screenHeight;i++){
		vector<ScreenPoint> intersectionL;
		vector<ScreenPoint> intersectionS;
		for(auto j=0;j<sizeL;j++){
			if(buffer[j][1]==i){
				intersectionL.push_back(buffer[j]);
			}
		}
		for(auto j=sizeL;j<sizeS;j++){
			if(buffer[j][1]==i){
				intersectionS.push_back(buffer[j]);
			}
		}
		if(intersectionL.size()!=0){
			vector<ScreenPoint> a;
			intersectionL=MyBrush::sort(intersectionL);
			a.push_back(intersectionL.front());
			a.push_back(intersectionL.back());

			if(intersectionS.size()!=0){
				intersectionS=MyBrush::sort(intersectionS);
				a.push_back(intersectionS.front());
				a.push_back(intersectionS.back());
			}
			a=MyBrush::sort(a);
			intersec.push_back(a);
		}
	}
}

vector<ScreenPoint> MyBrush::sort(vector<ScreenPoint> a){
	for(decltype(a.size()) i=1;i<a.size();i++){
		decltype(a.size()) j=i;
		while(j>0 && a[j-1][0]>a[j][0]){
			swap(a[j-1],a[j]);
			j--;
		}
	}
	return a;
}



void MyBrush::lineIntersec(int radius){
	intersec.clear();

	line(radius);
	for(int i=1;i<600;i++){
		vector<ScreenPoint> intersection;
		for(int j=0;j<buffer.size();j++){
			if(buffer[j][1]==i){
				intersection.push_back(buffer[j]);
			}
		}
		if(intersection.size()!=0){
			intersection=MyBrush::sort(intersection);
			vector<ScreenPoint> a;
			a.push_back(intersection.front());
			a.push_back(intersection.back());
			intersec.push_back(a);
		}
	}
}