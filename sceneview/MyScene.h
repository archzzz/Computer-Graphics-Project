#ifndef _MY_SCENE_H_
#define _MY_SCENE_H_

#include "cse452.h"
#include "Color.h"
#include "camera/Camera.h"
#include "parser.h"
#include "Light.h"
#include <FL/Fl_Image.H>
#include <string>
#include <vector>
#include "shapes/Shape.h"
#include <map>

using namespace std;
/*
* This class holds all of the data in the scene file.
*  Camera
*  Lights
*  Subgraphs
*  Root subgraph
*/
class Object;
struct node{
	Object *obj;
	Matrix4 trans;
	Matrix4 inverse;
	Point3 p;
	Vector3 n;
	double t,u,v,theta;
};

class Node{
public:
	Node(){
	}
	virtual	~Node(){
		children.clear();
	}

	virtual void draw(Matrix4 &t){return;};
	virtual void getMatrix(Matrix4 &t, vector<struct node> &a){return;};
	vector<Node *> children;

private:
};

class Object : public Node{
public:
	Object(){
		ambident = Color(0,0,0);
		diffuse = Color(1,1,1);
		reflect = Color(0,0,0);
		specular = Color(0,0,0);
		shine = 1;
		transparent = Color(0,0,0);
		ior = 1;
		textureU = 2;
		textureV = 4;
		emit = Color(0,0,0);	
	}
	~Object(){
		delete shape;
		shape = nullptr;
	}

	void draw(Matrix4 &t) override;
	void getMatrix(Matrix4 &t, vector<struct node> &a) override;

	Shape * shape;
	Color diffuse, ambident, reflect, specular, transparent, emit;
	int shine, ior;
	string filename;
	double textureU, textureV;

private:
};

class Trans : public Node{
public:
	Trans(){
		transMat = Matrix4().identity();
	}
	~Trans(){}

	void draw(Matrix4 &t) override;
	void getMatrix(Matrix4 &t, vector<struct node> &a) override;
	void addTrans(Matrix4 m){ transMat*= m; };

	Matrix4 transMat;
};

class Subgraph : public Node{
public:
	Subgraph();
	Subgraph(string _name) : name( _name) {}
	~Subgraph(){}

	void draw(Matrix4 &t) override;
	void getMatrix(Matrix4 &t, vector<struct node> &a) override;
	string name;
};

class MyScene {
public:
	MyScene();
	~MyScene();

	// The start of the parsing routine
	// Called once when a new scene file is opened
	bool loadSceneFile(std::string filename);

	// Return the current error mesage (if any) generated by parsing
	std::string getErrorMessage() const;

	// Return the parsed camera
	Camera &changeCamera();

	// Return the parsed camera
	const Camera &getCamera() const;

	// Return the parsed camera
	const Color &getBackground() const;

	// Called when the window is re-sized
	void resize(int w, int h);
	// Called when the window needs to be re-drawn
	// Stub can be found in sceneview/MyScene_draw.cpp
	void draw();

	/* 
	* The following are for the user interaction assignment 
	* Stubs for them can be found in Interactive/MySceneSelect.cpp
	*/
	// User clicked on the screen; find the corresponding shape
	bool selectNode(int x, int y);
	//
	void moveSelectedNode( const Matrix4 &matChange );

	/*
	* The following are for the ray tracing assignment
	* Stubs for them can be found in rendering/MyScene_render.cpp
	*/
	// 
	void render(int type, int width, int height, unsigned char* pixels);
	void stopRender();
	double getRenderProgress();

	/*
	* The following are for the sceneview assignment
	* They can be found in sceneview/MyScene.cpp
	*/
	// Clear out any existing data
	void resetScene();

	// Gets the background color from the scenefile
	bool parseBackground(Parser& p);

	// Gets the camera info (if any) from the scene file
	bool parseCamera(Parser& p);

	// Gets the light info (if any) from the scene file
	bool parseLight(Parser& p);

	/*
	* The following are the parsing routines you need to finish filling in.
	* Feel free to change the return type or pass in additional parameters.
	*/
	// Called when subgraph _name_ [ is encountered in the scene file
	// Should return true when root subgraph encountered
	Subgraph* parseMasterSubgraph(Parser& p);

	// Called when trans [ is encountered
	Trans* parseTrans(Parser& p);

	// Called when Object _name_ [ is encountered
	Object* parseObject(Parser& p);

	bool intersection(Point3 point, Vector3 dir, int &index);
	void rendering(int width, int height, unsigned char* pixels);
	Color RayTrace(int level, Point3 p, Vector3 dir);
	Color localIllumination(Point3 p, Vector3 dir, int &ind);
	bool hitObj(int &ind, int &lInd);

	vector<struct node> flattenTree;


private:
	// Has loadSceneFile been called yet?
	bool isLoaded;
	// Keep track of the current error message
	std::string errorMessage;

	// Store camera info; remember that your default camera should be
	// set to reasonable values in case there is no camera information
	// in the scene file
	Camera camera;

	// Stores the background color found in the scenefile (if any).
	// Defaults to black
	Color background;

	// The stored lights
	Color ambientLight;
	std::vector<Light> lights;

	// your functions/variables go here

	Subgraph root;

	map<string, Subgraph* > subgraph;
	HitRecord allHits;

	Cube cube;
	Cylinder cylinder;
	Cylinder prism;
	Cone cone;
	Sphere sphere;

	GLuint list;
	bool isFirst;

	double progress;
	bool stop;
};

#endif /* _MY_SCENE_H_ */
