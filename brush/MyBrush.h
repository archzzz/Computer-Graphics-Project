#ifndef _MY_BRUSH_H_
#define _MY_BRUSH_H_

#include "cse452.h"
#include "ScreenPoint.h"
#include "UIInterface.h"
#include <vector>
#include "Color.h"

using namespace std;



class BrushInterface;
class MyBrush : public UIInterface {
public:

	//A struct that contains a ScreenPoint p, a float pixelFlow, 
	//records the the flow rate of a point on canvas.
	struct pixel{
		ScreenPoint p;
		float pixelFlow;

	};

	enum ToolType {
		TOOL_BRUSH = 0,
		TOOL_LINE,
		TOOL_CIRCLE,
		TOOL_POLYGON,
		TOOL_FILTER
	};

	enum FilterType {
		FILTER_BLUR = 0,
		FILTER_SHARPEN,
		FILTER_EDGE
	};

	enum BrushType {
		BRUSH_CONSTANT = 0,
		BRUSH_LINEAR=1,
		BRUSH_QUADRATIC=2,
		BRUSH_GAUSSIAN=3,
		BRUSH_SPECIAL=4
	};

	MyBrush();
	~MyBrush();

	// Link to the brush widget
	void setUI( const BrushInterface *in_ui ) { brushUI = in_ui; }

	// Inherited from UIInterface
	// Implemented in MyBrush_UI.cpp
	virtual void resize( int width, int height );
	virtual void draw();
	virtual int  handle( int event );

	void loadImage(Fl_Image* image);

	// Called by the UI when the brush type or radius changes
	void changedBrush();


private:
	// read a pixel from the canvas, returns (r, g, b) values in the [0,1] range
	Color getPixel(int x, int y );
	// put a pixel on the canvas, (r, g, b) are to be given in the [0,1] range
	void putPixel(int x, int y, const Color &in_col);

	void generateColor(int x, int y, float pixelFlow, const Color & col);

	// Pixel and screen data
	std::vector<unsigned char> pixelData;
	int screenWidth, screenHeight;
	int imageWidth, imageHeight;

	// For interactive drawing, set by handle
	// You do not need to set these; the interface for that is
	// already set up
	bool isMouseDown;
	ScreenPoint mouseDown, mouseDrag;
	std::vector< ScreenPoint > polygon;
	static void draw_callback( void *in_data );

	// All of the widget data can be accessed through
	// methods on this class
	const BrushInterface *brushUI;

	// You need to write these
	// Stubs are in MyBrush.cpp
	void drawBrush( );
	void drawLine( );
	void drawCircle( );
	void drawPolygon( );
	void filterRegion( );


	// declare your extra variables/methods here:

	//Generate a line form p1 to p2 when the slope k is from 0 to 1.
	vector<ScreenPoint> edgeL(ScreenPoint p1, ScreenPoint p2);

	//Generate a line form p1 to p2 when the slope k is larger than 1.
	vector<ScreenPoint> edgeH(ScreenPoint p1,ScreenPoint p2);

	//Generate a line between any two points p1, p2.
	void edge(ScreenPoint p1, ScreenPoint p2);

	//Generate a circle.
	//Parameter: 
	// int x0,y0 : central point of circle ; 
	// int radius: radius of circle.
	vector<ScreenPoint>::size_type circle(int x0,int y0, int radius);

	//Generate the outline(a rectangle) of lines.
	//Parameter: 
	// int radius: width of line.
	void line(int radius);

	//Find intersections of scan-line and outline of airbrush.
	//Parameter: 
	// int raidus: radius of airbrush.
	void brushIntersec(int radius);

	//Find intersections of scan-line and outline of circles.
	//Parameter: 
	// int raidus : width of circle; 
	// int r1 : radius of outer edge of circle; 
	// int r2 : radius of inner edge of circle.
	void circleIntersec(int radius,int r1, int r2);

	//Find intersections of scan-line and outline of lines.
	//Parameter:
	// int radius: width of line.
	void lineIntersec(int radius);

	//Sort the vector by increasing order of _x.
	vector<ScreenPoint> sort(vector<ScreenPoint> a);

	vector<struct pixel> mask;
	vector<ScreenPoint> buffer;
	vector<vector<ScreenPoint>> intersec;

};

inline
	Color MyBrush::getPixel( int x, int y ) {
		Color col;
		const int i = (y * imageWidth + x) * 3;
		col[0] = pixelData[i    ] / 255.0f;
		col[1] = pixelData[i + 1] / 255.0f;
		col[2] = pixelData[i + 2] / 255.0f;

		return col;
}

inline
	void MyBrush::putPixel(int x, int y, const Color & col) {
		const int i = (y * imageWidth + x) * 3;
		pixelData[i    ] = (unsigned char) (col[0] * 255.0f);
		pixelData[i + 1] = (unsigned char) (col[1] * 255.0f);
		pixelData[i + 2] = (unsigned char) (col[2] * 255.0f);
}

// Generate RGB color which is a combination of exist color on canvas, 
// airbrush color and flow rate. And put color on canvas.
// Parameter:
// int x,y : coordinate of the point on canvas.
// float pixelFlow : flow rate with mask.
// Color col : color of the airbrush.
inline 
	void MyBrush::generateColor(int x, int y, float pixelFlow, const Color & col){
		const int i = (y * imageWidth + x) * 3;
		pixelData[i    ] = (unsigned char) (col[0]*pixelFlow *255.0f + pixelData[i    ] * (1-pixelFlow));
		pixelData[i + 1] = (unsigned char) (col[1]*pixelFlow *255.0f + pixelData[i + 1] * (1-pixelFlow));
		pixelData[i + 2] = (unsigned char) (col[2]*pixelFlow *255.0f + pixelData[i + 2] * (1-pixelFlow));

}



#endif /* _MY_BRUSH_H_ */
