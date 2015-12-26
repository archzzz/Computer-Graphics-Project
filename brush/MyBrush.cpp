#include "cse452.h"
#include "MyBrush.h"
#include "BrushInterface.h"
#include <cmath>
#include <iostream>
#include <list>
#include "Math.h"
#include "ScreenPoint.h"
#include <algorithm>

using namespace std;


#define PI 3.1415926535

void MyBrush::changedBrush() {
	// this is called anytime the brush type or brush radius changes
	// it should recompute the brush mask appropriately

	const int radius = brushUI->getRadius();
	const int brushType = brushUI->getBrushType();	   

	mask.clear();
	brushIntersec(radius);

	if(brushType==0){		
		for(int i=0;i<intersec.size();i++){
			for(int j=intersec[i][0][0];j<=intersec[i][1][0];j++){
				struct pixel a;
				a.p[0]=j;
				a.p[1]=intersec[i][0][1];
				a.pixelFlow=1;
				mask.push_back(a);
			}
		}
	}
	if(brushType==1){
		for(int i=0;i<intersec.size();i++){
			for(int j=intersec[i][0][0];j<=intersec[i][1][0];j++){
				struct pixel a;
				a.p[0]=j;
				a.p[1]=intersec[i][0][1];
				a.pixelFlow= max((1-((float)sqrt(a.p[0]*a.p[0]+a.p[1]*a.p[1])/radius)),0);
				mask.push_back(a);
			}
		}
	}
	if(brushType==2){
		for(int i=0;i<intersec.size();i++){
			for(int j=intersec[i][0][0];j<=intersec[i][1][0];j++){
				struct pixel a;
				a.p[0]=j;
				a.p[1]=intersec[i][0][1];
				a.pixelFlow=max(pow(radius-(float)sqrt(a.p[0]*a.p[0]+a.p[1]*a.p[1]),2)/(radius*radius),0);
				mask.push_back(a);
			}
		}
	}
	if(brushType==3){
		for(int i=0;i<intersec.size();i++){
			for(int j=intersec[i][0][0];j<=intersec[i][1][0];j++){
				struct pixel a;
				a.p[0]=j;
				a.p[1]=intersec[i][0][1];
				float r=sqrtf( a.p[0]*a.p[0]+a.p[1]*a.p[1])/radius;
				a.pixelFlow=max((float) 1/(0.3*sqrtf(2*PI))*expf(-r*r/0.18) ,0)  ;
				mask.push_back(a);
			}
		}
	}
}

void MyBrush::drawBrush( ) {
	// apply the current brush mask to image location (x,y)
	// the mouse location is in mouseDrag

	const int radius = brushUI->getRadius();																			
	const float pixelFlow = brushUI->getFlow();
	const Color colBrush = brushUI->getColor();

	for(int i=0;i<mask.size();i++){
		if(mask[i].p[0]+mouseDrag[0]>0 && mask[i].p[0]+mouseDrag[0]<screenWidth 
			&& mask[i].p[1]+mouseDrag[1]>0 && mask[i].p[1]+mouseDrag[1]<screenHeight){

			generateColor(mask[i].p[0]+mouseDrag[0],mask[i].p[1]+mouseDrag[1],
				pixelFlow*mask[i].pixelFlow,colBrush);
		}
	}
}

void MyBrush::drawLine( ) {
	// draw a thick line from mouseDown to mouseDrag
	// the width of the line is given by the current brush radius
	const int radius = brushUI->getRadius();
	const Color colBrush = brushUI->getColor();



	for(int i=0;i<buffer.size();i++){
		if(buffer[i][0]>0 && buffer[i][0]<screenWidth && buffer[i][1]>0 && buffer[i][1]<screenHeight){
			putPixel(buffer[i][0],buffer[i][1],colBrush);
		}
	}
	if(radius>1){
		for(int i=0;i<intersec.size();i++){
			for(int j=1;j<intersec[i].size();j=j+2){
				for(int k=intersec[i][j-1][0];k<intersec[i][j][0];k++){
					if(k<screenWidth && k>0){
						putPixel(k, intersec[i][0][1], colBrush);
					}
				}
			}
		}
	}



}


void MyBrush::drawCircle() {
	// draw a thick circle at mouseDown with radius r
	// the width of the circle is given by the current brush radius

	const int radius = brushUI->getRadius();
	const Color colBrush = brushUI->getColor();

	for(int i=0;i<buffer.size();i++){
		if(buffer[i][0]>0 && buffer[i][0]<screenWidth && buffer[i][1]>0 && buffer[i][1]<screenHeight){
			putPixel(buffer[i][0],buffer[i][1],colBrush);
		}
	}
	for(int i=0;i<intersec.size();i++){
		for(int j=1;j<intersec[i].size();j=j+2){
			for(int k=intersec[i][j-1][0];k<intersec[i][j][0];k++){
				if(k<screenWidth && k>0){
					putPixel(k, intersec[i][0][1],colBrush);
				}
			}
		}
	}
}


void MyBrush::drawPolygon() {
	// draw a polygon with numVertices whos coordinates are stored in the
	// polygon array: {x0, y0, x1, y1, ...., xn-1, yn-1}
}

void MyBrush::filterRegion( ) {
	// apply the filter indicated by filterType to the square
	// defined by the two corner points mouseDown and mouseDrag
	// these corners are not guarenteed to be in any order
	// The filter width is given by the brush radius
}






// draw a line from (0,0) to p1
vector<ScreenPoint> MyBrush::edgeL(ScreenPoint p1,ScreenPoint p2){

	vector<ScreenPoint> a;

	int dx=p2[0]-p1[0];
	int dy=p2[1]-p1[1];
	int d=2*dy-dx;
	int incrE=2*dy;
	int incrNE=2*(dy-dx);
	int x=p1[0], y=p1[1];
	a.push_back(ScreenPoint(x,y));
	while(x<p2[0]){
		if(d<=0) 
			d=d+incrE;
		else {
			d=d+incrNE;++y;
		}
		++x;
		a.push_back(ScreenPoint(x,y));
	}
	return a;
}

vector<ScreenPoint> MyBrush::edgeH(ScreenPoint p1,ScreenPoint p2){

	vector<ScreenPoint> a;

	int dx=p2[0]-p1[0];
	int dy=p2[1]-p1[1];
	int d=2*dx-dy;
	int incrN=2*dx;
	int incrNE=2*(dx-dy);
	int x=p1[0], y=p1[1];
	a.push_back(ScreenPoint(x,y));
	while(y<p2[1]){
		if(d<=0) 
			d=d+incrN;
		else {
			d=d+incrNE;++x;
		}
		++y;
		a.push_back(ScreenPoint(x,y));
	}
	return a;
}

void MyBrush::edge(ScreenPoint p1, ScreenPoint p2){
	int coord=1;
	vector<ScreenPoint> a;
	if(p2[0]-p1[0]<0 && p2[1]-p1[1]>0){
		p2[0]= p2[0]+ 2*(p1[0]-p2[0]);
		coord=2;
	}
	else if(p2[0]-p1[0]<0 && p2[1]-p1[1]<0){
		p2[0]= p2[0] +2*(p1[0]-p2[0]);
		p2[1]= p2[1]+ 2*(p1[1]-p2[1]);
		coord=3;
	}
	else if(p2[0]-p1[0]>0 && p2[1]-p1[1]<0){
		p2[1]= p2[1]+ 2*(p1[1]-p2[1]);
		coord=4;
	}
	else if(p2[0]-p1[0]==0){
		if(p2[1]>p1[1]){
			for(int i=p1[1];i<p2[1];i++){
				buffer.push_back(ScreenPoint(p1[0],i));
			}
			return;
		}
		else {
			for(int i=p2[1];i<p1[1];i++){
				buffer.push_back(ScreenPoint(p1[0],i));
			}
			return;
		}
	}
	else if(p2[1]-p1[1]==0){
		if(p2[0]>p1[0]){
			for(int i=p1[0];i<p2[0];i++){
				buffer.push_back(ScreenPoint(i,p1[1]));
			}
			return;
		}
		else {
			for(int i=p2[0];i<p1[0];i++){
				buffer.push_back(ScreenPoint(i,p1[1]));
			}
			return;
		}
	}

	if(p2[1]-p1[1] <= p2[0]-p1[0]){
		a= edgeL(p1,p2);
	}
	else {
		a=edgeH(p1,p2);
	}
	if(coord==2){
		for(int i=0;i<a.size();i++){
			a[i][0] -= 2*(a[i][0]-p1[0]);
		}
	}
	else if(coord==3){
		for(int i=0;i<a.size();i++){
			a[i][0] -= 2*(a[i][0]-p1[0]);
			a[i][1] -= 2*(a[i][1]-p1[1]);
		}
	}
	else if(coord==4){
		for(int i=0;i<a.size();i++){
			a[i][1] -= 2*(a[i][1]-p1[1]);
		}
	}
	for(int i=0;i<a.size();i++){
		buffer.push_back(a[i]);
	}
	return;
}


vector<ScreenPoint>::size_type MyBrush::circle(int x0,int y0, int radius){

	int x=0;
	int y=radius;
	double d=(x+1)*(x+1)+(y-0.5)*(y-0.5)-radius*radius;

	buffer.push_back(ScreenPoint(x0+x,y0+y));
	buffer.push_back(ScreenPoint(x0+x,y0-y));
	buffer.push_back(ScreenPoint(x0-x,y0+y));
	buffer.push_back(ScreenPoint(x0-x,y0-y));
	buffer.push_back(ScreenPoint(x0+y,y0+x));
	buffer.push_back(ScreenPoint(x0+y,y0-x));
	buffer.push_back(ScreenPoint(x0-y,y0+x));
	buffer.push_back(ScreenPoint(x0-y,y0-x));

	while (y>x){
		if (d<0){
			d+=2*x+3;
			x++;
			buffer.push_back(ScreenPoint(x0+x,y0+y));
			buffer.push_back(ScreenPoint(x0+x,y0-y));
			buffer.push_back(ScreenPoint(x0-x,y0+y));
			buffer.push_back(ScreenPoint(x0-x,y0-y));
			buffer.push_back(ScreenPoint(x0+y,y0+x));
			buffer.push_back(ScreenPoint(x0+y,y0-x));
			buffer.push_back(ScreenPoint(x0-y,y0+x));
			buffer.push_back(ScreenPoint(x0-y,y0-x));
		}
		else
		{
			d+=2*x-2*y+5;
			x++;
			y--;
			buffer.push_back(ScreenPoint(x0+x,y0+y));
			buffer.push_back(ScreenPoint(x0+x,y0-y));
			buffer.push_back(ScreenPoint(x0-x,y0+y));
			buffer.push_back(ScreenPoint(x0-x,y0-y));
			buffer.push_back(ScreenPoint(x0+y,y0+x));
			buffer.push_back(ScreenPoint(x0+y,y0-x));
			buffer.push_back(ScreenPoint(x0-y,y0+x));
			buffer.push_back(ScreenPoint(x0-y,y0-x));

		}
	}
	return buffer.size();

}

void MyBrush::brushIntersec(int radius){
	buffer.clear();
	intersec.clear();
	auto size=circle(0,0,radius);

	for(int y=-radius; y<=radius; y++){
		vector<ScreenPoint> intersection;
		for(auto i=0; i<size; i++){
			if(buffer[i][1]==y){
				intersection.push_back(buffer[i]);
			}
		}
		if(intersection.size()!=0){
			vector<ScreenPoint> a;
			intersection=MyBrush::sort(intersection);
			a.push_back(intersection.front());
			a.push_back(intersection.back());
			intersec.push_back(a);
		}
	}
}



void MyBrush::line(int radius){
	buffer.clear();

	vector<ScreenPoint> vertex;
	int	x=(int) (0.5*radius*cosf(atanf(((mouseDrag[1]-mouseDown[1])/(double)(mouseDrag[0]-mouseDown[0])))+ PI/2.0)+0.5);
	int y=(int) (0.5*radius*sinf(atanf(((mouseDrag[1]-mouseDown[1])/(double)(mouseDrag[0]-mouseDown[0])))+ PI/2.0)+0.5);

	vertex.push_back(ScreenPoint(mouseDown[0]+x,mouseDown[1]+y));
	vertex.push_back(ScreenPoint(mouseDown[0]-x,mouseDown[1]-y));
	vertex.push_back(ScreenPoint(mouseDown[0]-x+(mouseDrag[0]-mouseDown[0]),mouseDown[1]-y+(mouseDrag[1]-mouseDown[1])));
	vertex.push_back(ScreenPoint(mouseDown[0]+x+(mouseDrag[0]-mouseDown[0]),mouseDown[1]+y+(mouseDrag[1]-mouseDown[1])));


	edge(vertex[0],vertex[1]);
	edge(vertex[1],vertex[2]);
	edge(vertex[2],vertex[3]);
	edge(vertex[3],vertex[0]);
}







