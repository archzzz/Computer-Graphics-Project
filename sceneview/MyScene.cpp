#include "../cse452.h"
#include "MyScene.h"
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <cmath>

using namespace std;
// string constants used for parsing
// feel free to add any new keywords that you think may be helpfull
const std::string TOKEN_BACKGROUND     = "background";
const std::string TOKEN_COLOR          = "color";
const std::string TOKEN_OB             = "[";
const std::string TOKEN_CB             = "]";
const std::string TOKEN_CAMERA         = "camera";
const std::string TOKEN_EYE            = "eye";
const std::string TOKEN_LOOK           = "look";
const std::string TOKEN_FOCUS          = "focus";
const std::string TOKEN_UP             = "up";
const std::string TOKEN_ANGLE          = "angle";
const std::string TOKEN_NEAR_FAR       = "near-far";
const std::string TOKEN_LIGHT          = "light";
const std::string TOKEN_LIGHTTYPE      = "type";
const std::string TOKEN_POINT          = "point";
const std::string TOKEN_DIRECTIONAL    = "directional";
const std::string TOKEN_SPOTLIGHT      = "spotlight";
const std::string TOKEN_POSITION       = "position";
const std::string TOKEN_DIRECTION      = "direction";
const std::string TOKEN_FUNCTION       = "function";
const std::string TOKEN_APERTURE       = "aperture";
const std::string TOKEN_EXPONENT       = "exponent";
const std::string TOKEN_MASTERSUBGRAPH = "mastersubgraph";
const std::string TOKEN_TRANS          = "trans";
const std::string TOKEN_ROTATE         = "rotate";
const std::string TOKEN_TRANSLATE      = "translate";
const std::string TOKEN_SCALE          = "scale";
const std::string TOKEN_MATRIXRC       = "matrixRC";
const std::string TOKEN_MATRIXCR       = "matrixCR";
const std::string TOKEN_OBJECT         = "object";
const std::string TOKEN_CUBE           = "cube";
const std::string TOKEN_CYLINDER       = "cylinder";
const std::string TOKEN_CONE           = "cone";
const std::string TOKEN_SPHERE         = "sphere";
const std::string TOKEN_COW            = "cow";
const std::string TOKEN_AMBIENT        = "ambient";
const std::string TOKEN_DIFFUSE        = "diffuse";
const std::string TOKEN_SPECULAR       = "specular";
const std::string TOKEN_REFLECT        = "reflect";
const std::string TOKEN_TRANSPARENT    = "transparent";
const std::string TOKEN_EMIT           = "emit";
const std::string TOKEN_SHINE          = "shine";
const std::string TOKEN_IOR            = "ior";
const std::string TOKEN_TEXTURE        = "texture";
const std::string TOKEN_SUBGRAPH       = "subgraph";
const std::string TOKEN_PRISM          = "prism";

MyScene::MyScene() :
	ambientLight(0,0,0),root(Subgraph("root"))
{
	// initialize your variables here
	isLoaded = false;
	isFirst = true;
	stop = false;
	resetScene();
	cube.NewTesselation(1,1);
	cylinder.NewTesselation(40,1);
	prism.NewTesselation(3,1);
	cone.NewTesselation(40,1);
	sphere.NewTesselation(5,5);
}

MyScene::~MyScene() {
	// destroy your variables here
}

void MyScene::resetScene() {
	// reset scene to defaults (empty the scene tree)
	background = Color();
	camera.setFrom(Point3(0, 0, 1));
	camera.setLook(Vector3(0, 0, -1));
	camera.setUp(Vector3(0, 1, 0));
	camera.setZoom(60.0);
	camera.setNearFar(0.01, 100.0);
	camera.setProjectionCenter( Point3(0,0,0) );
	camera.setSkew(0.0);
	camera.setAspectRatioScale(1.0);

	lights.clear();
	//flattenTree.clear();

	ambientLight = Color(0,0,0);

	// reset your data here
	root.~Subgraph();
	subgraph.clear();
	isLoaded = false;
	isFirst = true;
	glDeleteLists(list,1);
}

bool MyScene::loadSceneFile(std::string filename) {
	// load the scenefile with the given filename
	// return true if the file was loaded succesfully
	resetScene();
	// parse file:
	Parser p(filename);
	bool done = false;
	errorMessage = "";
	do {
		done = p.nextToken();
		if (p.getToken() == TOKEN_BACKGROUND)
			done = !parseBackground(p);
		else if (p.getToken() == TOKEN_CAMERA)
			done = !parseCamera(p);
		else if (p.getToken() == TOKEN_LIGHT)
			done = !parseLight(p);
		else if (p.getToken() == TOKEN_MASTERSUBGRAPH)
			done = (parseMasterSubgraph(p) == 0);
		else if (!done) {
			errorMessage = "Unrecognized token at root level: \"" + p.getToken() + "\"";
			done = true;
		}
	} while (!done);
	// ToDo: check that there is a root mastersubgraph
	// and that no errors occured while loading
	isLoaded = errorMessage.length() == 0;
	if (isLoaded) {
		// perform any post-loading computations here
		// (such as flattening the tree or building
		// ray-acceleration structures)
	} else if (errorMessage.length() == 0)
		errorMessage = "Unable to locate root mastersubgraph";
	return isLoaded;
}

bool MyScene::parseBackground(Parser& p) {
	// parse the background block
	do {
		p.nextToken();
		if (p.getToken() == TOKEN_OB) {
		} else if (p.getToken() == TOKEN_CB) {
			break;
		} else if (p.getToken() == TOKEN_COLOR) {
			p.nextToken(); background[0] = static_cast<float>(p.getValue());
			p.nextToken(); background[1] = static_cast<float>(p.getValue());
			p.nextToken(); background[2] = static_cast<float>(p.getValue());
		} else {
			errorMessage = "Unrecognized token in background block: \"" + p.getToken() + "\"";
			return false;
		}
	} while (true);
	return true;
}

bool MyScene::parseCamera(Parser& p) {
	// parse the camera
	do {
		p.nextToken();
		if (p.getToken() == TOKEN_OB) {
		} else if (p.getToken() == TOKEN_CB) {
			break;
		} else if (p.getToken() == TOKEN_EYE) {
			Point3 eye;
			p.nextToken(); eye[0] = p.getValue();
			p.nextToken(); eye[1] = p.getValue();
			p.nextToken(); eye[2] = p.getValue();
			camera.setFrom(eye);
		} else if (p.getToken() == TOKEN_LOOK) {
			Vector3 look;
			p.nextToken(); look[0] = p.getValue();
			p.nextToken(); look[1] = p.getValue();
			p.nextToken(); look[2] = p.getValue();
			camera.setLook(look);
		} else if (p.getToken() == TOKEN_FOCUS) {
			Point3 focus;
			p.nextToken(); focus[0] = p.getValue();
			p.nextToken(); focus[1] = p.getValue();
			p.nextToken(); focus[2] = p.getValue();
			camera.setAt(focus);
		} else if (p.getToken() == TOKEN_UP) {
			Vector3 up;
			p.nextToken(); up[0] = p.getValue();
			p.nextToken(); up[1] = p.getValue();
			p.nextToken(); up[2] = p.getValue();
			camera.setUp(up);
		} else if (p.getToken() == TOKEN_ANGLE) {
			p.nextToken(); camera.setZoom(p.getValue());
		} else if (p.getToken() == TOKEN_NEAR_FAR) {
			double n, f;
			p.nextToken(); n = p.getValue();
			p.nextToken(); f = p.getValue();
			camera.setNearFar(n, f);
		} else {
			errorMessage = "Unrecognized token in camera block: \"" + p.getToken() + "\"";
			return false;
		}
	} while (true);
	return true;
}

bool MyScene::parseLight(Parser& p) {
	// parse a light source block
	Light l;
	do {
		p.nextToken();
		if (p.getToken() == TOKEN_OB) {
		} else if (p.getToken() == TOKEN_CB) {
			break;
		} else if (p.getToken() == TOKEN_LIGHTTYPE) {
			p.nextToken();
			if (p.getToken() == TOKEN_POINT)
				l.setType( Light::POINTLIGHT );
			else if (p.getToken() == TOKEN_DIRECTIONAL)
				l.setType( Light::DIRECTIONAL );
			else if (p.getToken() == TOKEN_SPOTLIGHT)
				l.setType( Light::SPOTLIGHT );
			else {
				errorMessage = "Unknown light type: \"" + p.getToken() + "\"";
				return false;
			}
		} else if (p.getToken() == TOKEN_POSITION) {
			Point3 pos;
			p.nextToken(); pos[0] = p.getValue();
			p.nextToken(); pos[1] = p.getValue();
			p.nextToken(); pos[2] = p.getValue();
			l.setPosition( pos );
		} else if (p.getToken() == TOKEN_COLOR) {
			Color color;
			p.nextToken(); color[0] = static_cast<float>(p.getValue());
			p.nextToken(); color[1] = static_cast<float>(p.getValue());
			p.nextToken(); color[2] = static_cast<float>(p.getValue());
			l.setColor( color );
		} else if (p.getToken() == TOKEN_FUNCTION) {
			float f0,f1,f2;
			p.nextToken(); f0 = static_cast<float>(p.getValue());
			p.nextToken(); f1 = static_cast<float>(p.getValue());
			p.nextToken(); f2 = static_cast<float>(p.getValue());
			l.setFalloff( f0, f1, f2 );
		} else if (p.getToken() == TOKEN_DIRECTION) {
			Vector3 dir;
			p.nextToken(); dir[0] = p.getValue();
			p.nextToken(); dir[1] = p.getValue();
			p.nextToken(); dir[2] = p.getValue();
			dir.normalize();
			l.setDirection( dir );
		} else if (p.getToken() == TOKEN_APERTURE) {
			p.nextToken(); l.setAperture( p.getValue() );
		} else if (p.getToken() == TOKEN_EXPONENT) {
			p.nextToken(); l.setExponent( p.getValue() );
		} else if (p.getToken() == TOKEN_AMBIENT) {
			p.nextToken(); ambientLight[0] = static_cast<float>(p.getValue());
			p.nextToken(); ambientLight[1] = static_cast<float>(p.getValue());
			p.nextToken(); ambientLight[2] = static_cast<float>(p.getValue());
		} else {
			errorMessage = "Unrecognized token in light block: \"" + p.getToken() + "\"";
			return false;
		}

	} while (true);
	assert( l.getColor()[0] >= 0.0 && l.getColor()[0] <= 1.0 );
	assert( l.getColor()[1] >= 0.0 && l.getColor()[1] <= 1.0 );
	assert( l.getColor()[2] >= 0.0 && l.getColor()[2] <= 1.0 );
	assert( l.getAperture() >  0.0 && l.getAperture() <= 180.0 );
	assert( l.getExponent() >= 0.0 );
	lights.push_back(l);
	return true;
}

Subgraph* MyScene::parseMasterSubgraph(Parser& p) {
	p.nextToken();
	std::string name = p.getToken();
	Subgraph *sub = new Subgraph(name);

	do {
		p.nextToken();
		if (p.getToken() == TOKEN_OB) {
		} else if (p.getToken() == TOKEN_CB) {
			break;
		} else if (p.getToken() == TOKEN_TRANS) {
			sub->children.push_back(parseTrans(p));
		} else {
			errorMessage = "Unrecognized token in mastersubgraph \"" + name + "\": \"" + p.getToken() + "\"";
			sub->~Subgraph();		
			return 0;
		}
	} while (true);

	subgraph.insert(make_pair(sub->name, sub));

	return sub;
}

Trans* MyScene::parseTrans(Parser& p) {
	Trans *trans = new Trans();
	do {
		p.nextToken();
		if (p.getToken() == TOKEN_OB) {
		} else if (p.getToken() == TOKEN_CB) {
			break;
		} else if (p.getToken() == TOKEN_ROTATE) {
			Vector3 axis;
			p.nextToken(); axis[0] = p.getValue();
			p.nextToken(); axis[1] = p.getValue();
			p.nextToken(); axis[2] = p.getValue();
			p.nextToken(); double angle = p.getValue() * M_PI / 180.0;
			trans->addTrans(Matrix4().rotation(axis, angle));

		} else if (p.getToken() == TOKEN_TRANSLATE) {
			Vector3 v;
			p.nextToken(); v[0] = p.getValue();
			p.nextToken(); v[1] = p.getValue();
			p.nextToken(); v[2] = p.getValue();
			trans->addTrans(Matrix4().translation(v));

		} else if (p.getToken() == TOKEN_SCALE) {
			Vector3 v;
			p.nextToken(); v[0] = p.getValue();
			p.nextToken(); v[1] = p.getValue();
			p.nextToken(); v[2] = p.getValue();
			trans->addTrans(Matrix4().scaling(v));

		} else if (p.getToken() == TOKEN_MATRIXRC) {
			Vector4 r0, r1, r2, r3;
			p.nextToken(); r0[0] = p.getValue();
			p.nextToken(); r0[1] = p.getValue();
			p.nextToken(); r0[2] = p.getValue();
			p.nextToken(); r0[3] = p.getValue();

			p.nextToken(); r1[0] = p.getValue();
			p.nextToken(); r1[1] = p.getValue();
			p.nextToken(); r1[2] = p.getValue();
			p.nextToken(); r1[3] = p.getValue();

			p.nextToken(); r2[0] = p.getValue();
			p.nextToken(); r2[1] = p.getValue();
			p.nextToken(); r2[2] = p.getValue();
			p.nextToken(); r2[3] = p.getValue();

			p.nextToken(); r3[0] = p.getValue();
			p.nextToken(); r3[1] = p.getValue();
			p.nextToken(); r3[2] = p.getValue();
			p.nextToken(); r3[3] = p.getValue();
			trans->addTrans(Matrix4(r0, r1, r2, r3));

		} else if (p.getToken() == TOKEN_MATRIXCR) {
			Vector4 r0, r1, r2, r3;
			p.nextToken(); r0[0] = p.getValue();
			p.nextToken(); r1[0] = p.getValue();
			p.nextToken(); r2[0] = p.getValue();
			p.nextToken(); r3[0] = p.getValue();

			p.nextToken(); r0[1] = p.getValue();
			p.nextToken(); r1[1] = p.getValue();
			p.nextToken(); r2[1] = p.getValue();
			p.nextToken(); r3[1] = p.getValue();

			p.nextToken(); r0[2] = p.getValue();
			p.nextToken(); r1[2] = p.getValue();
			p.nextToken(); r2[2] = p.getValue();
			p.nextToken(); r3[2] = p.getValue();

			p.nextToken(); r0[3] = p.getValue();
			p.nextToken(); r1[3] = p.getValue();
			p.nextToken(); r2[3] = p.getValue();
			p.nextToken(); r3[3] = p.getValue();
			trans->addTrans(Matrix4( r0, r1, r2, r3 ));

		} else if (p.getToken() == TOKEN_OBJECT) {
			trans->children.push_back(parseObject(p));

		} else if (p.getToken() == TOKEN_SUBGRAPH) {
			p.nextToken();
			std::string subgraphName = p.getToken();
			trans->children.push_back(subgraph.find(subgraphName)->second);
		} else {
			errorMessage = "Unrecognized token in trans block: \"" + p.getToken() + "\"";
			trans-> ~Trans();			
			return 0;
		}
	} while (true);

	return trans;
}

Object* MyScene::parseObject(Parser& p) {
	Object *obj = new Object();
	p.nextToken();
	if (p.getToken() == TOKEN_CUBE) {		
		obj->shape = &cube;
	} else if (p.getToken() == TOKEN_CYLINDER) {
		obj->shape = &cylinder;
	} else if (p.getToken() == TOKEN_CONE) {
		obj->shape = &cone;
	} else if (p.getToken() == TOKEN_SPHERE) {
		obj->shape = &sphere;
	} else if (p.getToken() == TOKEN_PRISM) {
		obj->shape = &prism;
	} else if (p.getToken() == TOKEN_COW) {
		// object is a cow (optional)
		// you may ignore this, or replace it with a different placeholder
		// shape if you wish

	} else {
		errorMessage = "Unrecognized object type: \"" + p.getToken() + "\"";
		return 0;
	}
	do {
		p.nextToken();
		if (p.getToken() == TOKEN_OB) {
		} else if (p.getToken() == TOKEN_CB) {
			break;
		} else if (p.getToken() == TOKEN_AMBIENT) {
			double r, g, b;
			p.nextToken(); r = p.getValue();
			p.nextToken(); g = p.getValue();
			p.nextToken(); b = p.getValue();
			obj->ambident = Color(r,g,b);

		} else if (p.getToken() == TOKEN_DIFFUSE) {
			double r, g, b;
			p.nextToken(); r = p.getValue();
			p.nextToken(); g = p.getValue();
			p.nextToken(); b = p.getValue();
			obj->diffuse = Color(r,g,b);

		} else if (p.getToken() == TOKEN_SPECULAR) {
			double r, g, b;
			p.nextToken(); r = p.getValue();
			p.nextToken(); g = p.getValue();
			p.nextToken(); b = p.getValue();
			obj->specular = Color(r,g,b);

		} else if (p.getToken() == TOKEN_REFLECT) {
			double r, g, b;
			p.nextToken(); r = p.getValue();
			p.nextToken(); g = p.getValue();
			p.nextToken(); b = p.getValue();
			obj->reflect = Color(r,g,b);

		} else if (p.getToken() == TOKEN_TRANSPARENT) {
			double r, g, b;
			p.nextToken(); r = p.getValue();
			p.nextToken(); g = p.getValue();
			p.nextToken(); b = p.getValue();
			obj->transparent = Color(r,g,b);

		} else if (p.getToken() == TOKEN_EMIT) {
			double r, g, b;
			p.nextToken(); r = p.getValue();
			p.nextToken(); g = p.getValue();
			p.nextToken(); b = p.getValue();
			obj->emit = Color(r,g,b);

		} else if (p.getToken() == TOKEN_SHINE) {
			double s;
			p.nextToken(); s = p.getValue();
			obj->shine = s;

		} else if (p.getToken() == TOKEN_IOR) {
			double ior;
			p.nextToken(); ior = p.getValue();
			obj->ior = ior;

		} else if (p.getToken() == TOKEN_TEXTURE) {
			std::string filename;
			double textureU, textureV;
			p.nextToken(); filename = p.getToken();
			p.nextToken(); textureU = p.getValue();
			p.nextToken(); textureV = p.getValue();
			obj->filename = filename;
			obj->textureU = textureU;
			obj->textureV = textureV;

		} else {
			errorMessage = "Unrecognized token in object block: \"" + p.getToken() + "\"";
			obj-> ~Object();	
			return 0;
		}
	} while (true);

	return obj;
}

std::string MyScene::getErrorMessage() const {
	// return the current error message
	return errorMessage;
}

const Camera & MyScene::getCamera() const {
	// return the current camera
	return camera;
}

Camera & MyScene::changeCamera() {
	// return the current camera
	return camera;
}

const Color &MyScene::getBackground() const {
	return background;
}

void Object::draw(Matrix4 &t){
	GLfloat dif[] = {diffuse[0], diffuse[1], diffuse[2]};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	GLfloat amb[] = {ambident[0], ambident[1], ambident[2]};
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	GLfloat spe[] = {specular[0], specular[1], specular[2]};
	glMaterialfv(GL_FRONT, GL_SPECULAR, spe);
	GLfloat emi[] = {emit[0], emit[1], emit[2]};
	glMaterialfv(GL_FRONT, GL_EMISSION, emi);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);

	glPushMatrix();
	glMultMatrixd(&t(0,0));
	shape->drawShapeSmooth();
	glPopMatrix();
}

void Trans::draw(Matrix4 &t){
	children[0]->draw(t* transMat);
}

void Subgraph::draw(Matrix4 &t){
	for (vector<Node *>::iterator it = children.begin(); it != children.end(); ++it) {
		(*it)->draw(t);
	}
}

void Subgraph::getMatrix(Matrix4 &t,vector<struct node> &a){
	for(vector<Node *>::iterator it = children.begin(); it != children.end(); ++it){
		(*it)->getMatrix(t, a);
	}
}

void Trans::getMatrix(Matrix4 &t, vector<struct node> &a){
	children[0]->getMatrix( t* transMat, a);
}

void Object::getMatrix(Matrix4 &t, vector<struct node> &a){
	struct node n = { n.obj = this, n.trans = t , n.inverse = t.inverse()}; 
	a.push_back(n);
}

/*void MyScene::resetFlattenTree(){
	flattenTree.clear();
	struct node head = {head.t = }; //positive_infinity

}*/