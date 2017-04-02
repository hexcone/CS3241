// CS3241Lab5.cpp
#include <cmath>
#include <iostream>
#include "vector3D.h"
#include <chrono>


/* Include header files depending on platform */
#ifdef _WIN32
#include "GL\freeglut.h"
#define M_PI 3.14159
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

using namespace std;

#define WINWIDTH 600
#define WINHEIGHT 400
#define NUM_OBJECTS 4
#define MAX_RT_LEVEL 50
#define NUM_SCENE 2

float* pixelBuffer = new float[WINWIDTH * WINHEIGHT * 3];

class Ray { // a ray that start with "start" and going in the direction "dir"
public:
	Vector3 start, dir;
};

class RtObject {

public:
	virtual double intersectWithRay(Ray, Vector3& pos, Vector3& normal) = 0; // return a -ve if there is no intersection. Otherwise, return the smallest postive value of t
																			 // Materials Properties
	double ambiantReflection[3];
	double diffusetReflection[3];
	double specularReflection[3];
	double speN = 300;


};
class Sphere : public RtObject {
	Vector3 center_;
	double r_;
public:
	Sphere(Vector3 c, double r) { center_ = c; r_ = r; };
	Sphere() {};
	void set(Vector3 c, double r) { center_ = c; r_ = r; };
	double intersectWithRay(Ray, Vector3& pos, Vector3& normal);
};

RtObject **objList; // The list of all objects in the scene


					// Global Variables
					// Camera Settings
Vector3 cameraPos(0, 0, -500);

// assume the the following two vectors are normalised
Vector3 lookAtDir(0, 0, 1);
Vector3 upVector(0, 1, 0);
Vector3 leftVector(1, 0, 0);
float focalLen = 500;

// Light Settings

Vector3 lightPos(900, 1000, -1500);
double ambiantLight[3] = { 0.4,0.4,0.4 };
double diffusetLight[3] = { 0.7,0.7, 0.7 };
double specularLight[3] = { 0.5,0.5, 0.5 };


double bgColor[3] = { 0.1,0.1,0.4 };

int sceneNo = 0;
/*==============================
Modify Section Here
Hint: Add additional methods to help you in solving equations
==============================*/

double distanceToLight(Ray r, double t, Vector3& intersection, Vector3& lightV)
// distance from point on sphere to light  
{
	double distance = sqrt(pow((lightPos.x[0] - intersection.x[0]), 2) +
		pow((lightPos.x[1] - intersection.x[1]), 2) +
		pow((lightPos.x[2] - intersection.x[2]), 2));

	// calculate directional vector to light
	lightV = (lightPos - intersection);
	lightV.normalize();

	return distance;
}

Vector3 directionToViewPoint(Ray ray, Vector3& intersection)
// direction from point on sphere to camera
{
	Vector3 viewdir;

	viewdir = (ray.start - intersection);
	viewdir.normalize();

	return viewdir;
}

bool applyShadow(int obj, Vector3 lightV)
// check if direction to light is blocked by other objects
{
	int mini = obj, i;
	double mint = DBL_MAX, t;
	Ray ray;
	Vector3 intersection, normal;
	
	ray.start = lightPos;
	ray.dir = -lightV;

	for (i = 0; i < NUM_OBJECTS; i++)
	{
		if (((t = objList[i]->intersectWithRay(ray, intersection, normal)) > 0) && (t < mint))
		{
			mint = t;
			mini = i;
		}
	}

	if (mini != obj) {
		return true;
	}
	else {
		return false;
	}
}


double Sphere::intersectWithRay(Ray r, Vector3& intersection, Vector3& normal)
// return a -ve if there is no intersection. Otherwise, return the smallest postive value of t
{// Step 1
	double a, b, c, d, t1, t2, t;

	a = dot_prod(r.dir, r.dir);
	b = dot_prod((r.dir * 2), (r.start - Sphere::center_));
	c = dot_prod((r.start - Sphere::center_), (r.start - Sphere::center_)) - pow(Sphere::r_, 2);
	d = pow(b, 2) - (4 * a * c);
	
	if (d > 0) {
		// 2 roots
		t1 = (-b + sqrt(d)) / (2 * a);
		t2 = (-b - sqrt(d)) / (2 * a);
		if (t1 < t2) {
			t = t1;
		} else {
			t = t2;
		}
	} else if (d == 0) {
		// 1 root
		t = -b / (2 * a);
	} else {
		// no root
		t = -1;
		return t;
	}

	// calculate intersection
	intersection = r.start + (r.dir * t);

	// calculate normal
	normal = (intersection - Sphere::center_);
	normal.normalize();

	return t;
}

void addAnotherScene() {
	//Step 5: Modify objList here
	((Sphere*)objList[0])->set(Vector3(100, 0, -300), 30);//blue
	((Sphere*)objList[1])->set(Vector3(50, 50, -250), 50);//yellow
	((Sphere*)objList[2])->set(Vector3(-100, 100, -150), 100);//green
	((Sphere*)objList[3])->set(Vector3(0, -250, -100), 250);//purple

	objList[0]->ambiantReflection[0] = 0.1;
	objList[0]->ambiantReflection[1] = 0.4;
	objList[0]->ambiantReflection[2] = 0.4;
	objList[0]->diffusetReflection[0] = 0;
	objList[0]->diffusetReflection[1] = 1;
	objList[0]->diffusetReflection[2] = 1;
	objList[0]->specularReflection[0] = 0.2;
	objList[0]->specularReflection[1] = 0.4;
	objList[0]->specularReflection[2] = 0.4;
	objList[0]->speN = 300;

	objList[1]->ambiantReflection[0] = 0.6;
	objList[1]->ambiantReflection[1] = 0.6;
	objList[1]->ambiantReflection[2] = 0.2;
	objList[1]->diffusetReflection[0] = 1;
	objList[1]->diffusetReflection[1] = 1;
	objList[1]->diffusetReflection[2] = 0;
	objList[1]->specularReflection[0] = 0.0;
	objList[1]->specularReflection[1] = 0.0;
	objList[1]->specularReflection[2] = 0.0;
	objList[1]->speN = 50;

	objList[2]->ambiantReflection[0] = 0.1;
	objList[2]->ambiantReflection[1] = 0.6;
	objList[2]->ambiantReflection[2] = 0.1;
	objList[2]->diffusetReflection[0] = 0.1;
	objList[2]->diffusetReflection[1] = 1;
	objList[2]->diffusetReflection[2] = 0.1;
	objList[2]->specularReflection[0] = 0.3;
	objList[2]->specularReflection[1] = 0.7;
	objList[2]->specularReflection[2] = 0.3;
	objList[2]->speN = 650;

	objList[3]->ambiantReflection[0] = 0.3;
	objList[3]->ambiantReflection[1] = 0.3;
	objList[3]->ambiantReflection[2] = 0.3;
	objList[3]->diffusetReflection[0] = 0.7;
	objList[3]->diffusetReflection[1] = 0.7;
	objList[3]->diffusetReflection[2] = 0.7;
	objList[3]->specularReflection[0] = 0.6;
	objList[3]->specularReflection[1] = 0.6;
	objList[3]->specularReflection[2] = 0.6;
	objList[3]->speN = 650;
}

void rayTrace2(Ray ray, double& r, double& g, double& b, int fromObj, int level)
{
	// Step 4
	// Checks if it reaches MAX_RT_LEVEL
	if (level == MAX_RT_LEVEL) {
		return;
	}

	int goBackGround = 1, i = 0;
	double inColor[3];

	Vector3 intersection, normal;
	Vector3 lightV;
	Vector3 viewV;
	Vector3 lightReflectionV;
	Vector3 rayReflectionV;

	Ray newRay;
	double mint = DBL_MAX, t;

	for (i = 0; i < NUM_OBJECTS; i++)
	{
		if (((t = objList[i]->intersectWithRay(ray, intersection, normal)) > 0) 
			&& (t < mint) && (fromObj != i))
		{
			mint = t;
			// Step 2
			double ambient_r, ambient_g, ambient_b;
			ambient_r = objList[i]->ambiantReflection[0] * ambiantLight[0];
			ambient_g = objList[i]->ambiantReflection[1] * ambiantLight[1];
			ambient_b = objList[i]->ambiantReflection[2] * ambiantLight[2];

			// Step 3
			double diffuse_r, diffuse_g, diffuse_b;
			double d = distanceToLight(ray, t, intersection, lightV);
			double userdefined = 0.00000025;
			double f = (1 / (userdefined + userdefined * d + userdefined * pow(d, 2)));
			double NL = dot_prod(normal, lightV);
			diffuse_r = f * diffusetLight[0]
				* objList[i]->diffusetReflection[0] * NL;
			diffuse_g = f * diffusetLight[1]
				* objList[i]->diffusetReflection[1] * NL;
			diffuse_b = f * diffusetLight[2]
				* objList[i]->diffusetReflection[2] * NL;
			if (diffuse_r < 0) {
				diffuse_r = 0;
			}
			if (diffuse_g < 0) {
				diffuse_g = 0;
			}
			if (diffuse_b < 0) {
				diffuse_b = 0;
			}

			double specular_r, specular_g, specular_b;
			lightReflectionV = (normal * (2 * NL)) - lightV;
			viewV = directionToViewPoint(ray, intersection);
			double RVn = pow(dot_prod(lightReflectionV, viewV), objList[i]->speN);
			specular_r = f * specularLight[0]
				* objList[i]->specularReflection[0]
				* RVn;
			specular_g = f * specularLight[1]
				* objList[i]->specularReflection[1]
				* RVn;
			specular_b = f * specularLight[2]
				* objList[i]->specularReflection[2]
				* RVn;
			
			double NV = dot_prod(normal, viewV);
			rayReflectionV = (normal * (2 * NV)) - viewV;
			newRay.start = intersection;
			newRay.dir = rayReflectionV;
			double new_r = 0.0, new_g = 0.0, new_b = 0.0;
			rayTrace2(newRay, new_r, new_g, new_b, i, level + 1);

			double reflectionweight = (objList[i]->speN - 50) / 1000.0;
			r = ambient_r + diffuse_r + specular_r + (reflectionweight * new_r);
			g = ambient_g + diffuse_g + specular_g + (reflectionweight * new_g);
			b = ambient_b + diffuse_b + specular_b + (reflectionweight * new_b);

			goBackGround = 0;
		}
	}

	if (goBackGround)
	{
		r = bgColor[0];
		g = bgColor[1];
		b = bgColor[2];
	}

}

void rayTrace(Ray ray, double& r, double& g, double& b, int fromObj = -1, int level = 0)
{
	// Step 4
	// Checks if it reaches MAX_RT_LEVEL

	int goBackGround = 1, i = 0;
	double inColor[3];

	Vector3 intersection, normal;
	Vector3 lightV;
	Vector3 viewV;
	Vector3 lightReflectionV;
	Vector3 rayReflectionV;

	Ray newRay;
	double mint = DBL_MAX, t;

	for (i = 0; i < NUM_OBJECTS; i++)
	{
		if (((t = objList[i]->intersectWithRay(ray, intersection, normal)) > 0) && (t < mint))
		{
			mint = t;
			// Step 2
			double ambient_r, ambient_g, ambient_b;
			ambient_r = objList[i]->ambiantReflection[0] * ambiantLight[0];
			ambient_g = objList[i]->ambiantReflection[1] * ambiantLight[1];
			ambient_b = objList[i]->ambiantReflection[2] * ambiantLight[2];

			// Step 3
			double diffuse_r, diffuse_g, diffuse_b;
			double d = distanceToLight(ray, t, intersection, lightV);
			double userdefined = 0.00000025;
			double f = (1 / (userdefined + userdefined * d + userdefined * pow(d, 2)));
			double NL = dot_prod(normal, lightV);
			diffuse_r = f * diffusetLight[0]
				* objList[i]->diffusetReflection[0] * NL;
			diffuse_g = f * diffusetLight[1]
				* objList[i]->diffusetReflection[1] * NL;
			diffuse_b = f * diffusetLight[2]
				* objList[i]->diffusetReflection[2] * NL;
			if (diffuse_r < 0) {
				diffuse_r = 0;
			}
			if (diffuse_g < 0) {
				diffuse_g = 0;
			}
			if (diffuse_b < 0) {
				diffuse_b = 0;
			}

			double specular_r, specular_g, specular_b;
			lightReflectionV = (normal * (2 * NL)) - lightV;
			viewV = directionToViewPoint(ray, intersection);
			double RVn = pow(dot_prod(lightReflectionV, viewV), objList[i]->speN);
			specular_r = f * specularLight[0]
				* objList[i]->specularReflection[0]
				* RVn;
			specular_g = f * specularLight[1]
				* objList[i]->specularReflection[1]
				* RVn;
			specular_b = f * specularLight[2]
				* objList[i]->specularReflection[2]
				* RVn;

			double NV = dot_prod(normal, viewV);
			rayReflectionV = (normal * (2 * NV)) - viewV;
			newRay.start = intersection;
			newRay.dir = rayReflectionV;
			double new_r = 0.0, new_g = 0.0, new_b = 0.0;
			rayTrace2(newRay, new_r, new_g, new_b, i, level + 1);

			double reflectionweight = (objList[i]->speN - 50) / 1000.0;
			r = ambient_r + diffuse_r + specular_r + (reflectionweight * new_r);
			g = ambient_g + diffuse_g + specular_g + (reflectionweight * new_g);
			b = ambient_b + diffuse_b + specular_b + (reflectionweight * new_b);

			if (applyShadow(i, lightV)) {
				r -= 0.5;
				g -= 0.5;
				b -= 0.5;
			}

			goBackGround = 0;
		}
	}

	if (goBackGround)
	{
		r = bgColor[0];
		g = bgColor[1];
		b = bgColor[2];
	}

}

/*==============================
End of Section,
IT IS NOT RECOMMENDED TO CHANGE ANYTHING BELOW
==============================*/



void drawInPixelBuffer(int x, int y, double r, double g, double b)
{
	pixelBuffer[(y*WINWIDTH + x) * 3] = (float)r;
	pixelBuffer[(y*WINWIDTH + x) * 3 + 1] = (float)g;
	pixelBuffer[(y*WINWIDTH + x) * 3 + 2] = (float)b;
}

void renderScene()
{
	int x, y;
	Ray ray;
	double r, g, b;

	cout << "Rendering Scene " << sceneNo << " with resolution " << WINWIDTH << "x" << WINHEIGHT << "........... ";
	__int64 time1 = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count(); // marking the starting time

	ray.start = cameraPos;

	Vector3 vpCenter = cameraPos + lookAtDir * focalLen;  // viewplane center
	Vector3 startingPt = vpCenter + leftVector * (-WINWIDTH / 2.0) + upVector * (-WINHEIGHT / 2.0);
	Vector3 currPt;

	for (x = 0; x<WINWIDTH; x++)
		for (y = 0; y < WINHEIGHT; y++)
		{
			currPt = startingPt + leftVector*x + upVector*y;
			ray.dir = currPt - cameraPos;
			ray.dir.normalize();
			rayTrace(ray, r, g, b);
			drawInPixelBuffer(x, y, r, g, b);
		}

	__int64 time2 = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count(); // marking the ending time

	cout << "Done! \nRendering time = " << time2 - time1 << "ms" << endl << endl;
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_DOUBLEBUFFER);
	glDrawPixels(WINWIDTH, WINHEIGHT, GL_RGB, GL_FLOAT, pixelBuffer);
	glutSwapBuffers();
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-10, 10, -10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void setScene(int i = 0)
{
	if (i > NUM_SCENE)
	{
		cout << "Warning: Invalid Scene Number" << endl;
		return;
	}

	if (i == 0)
	{

		((Sphere*)objList[0])->set(Vector3(-130, 80, 120), 100);
		((Sphere*)objList[1])->set(Vector3(130, -80, -80), 100);
		((Sphere*)objList[2])->set(Vector3(-130, -80, -80), 100);
		((Sphere*)objList[3])->set(Vector3(130, 80, 120), 100);

		objList[0]->ambiantReflection[0] = 0.1;
		objList[0]->ambiantReflection[1] = 0.4;
		objList[0]->ambiantReflection[2] = 0.4;
		objList[0]->diffusetReflection[0] = 0;
		objList[0]->diffusetReflection[1] = 1;
		objList[0]->diffusetReflection[2] = 1;
		objList[0]->specularReflection[0] = 0.2;
		objList[0]->specularReflection[1] = 0.4;
		objList[0]->specularReflection[2] = 0.4;
		objList[0]->speN = 300;

		objList[1]->ambiantReflection[0] = 0.6;
		objList[1]->ambiantReflection[1] = 0.6;
		objList[1]->ambiantReflection[2] = 0.2;
		objList[1]->diffusetReflection[0] = 1;
		objList[1]->diffusetReflection[1] = 1;
		objList[1]->diffusetReflection[2] = 0;
		objList[1]->specularReflection[0] = 0.0;
		objList[1]->specularReflection[1] = 0.0;
		objList[1]->specularReflection[2] = 0.0;
		objList[1]->speN = 50;

		objList[2]->ambiantReflection[0] = 0.1;
		objList[2]->ambiantReflection[1] = 0.6;
		objList[2]->ambiantReflection[2] = 0.1;
		objList[2]->diffusetReflection[0] = 0.1;
		objList[2]->diffusetReflection[1] = 1;
		objList[2]->diffusetReflection[2] = 0.1;
		objList[2]->specularReflection[0] = 0.3;
		objList[2]->specularReflection[1] = 0.7;
		objList[2]->specularReflection[2] = 0.3;
		objList[2]->speN = 650;

		objList[3]->ambiantReflection[0] = 0.3;
		objList[3]->ambiantReflection[1] = 0.3;
		objList[3]->ambiantReflection[2] = 0.3;
		objList[3]->diffusetReflection[0] = 0.7;
		objList[3]->diffusetReflection[1] = 0.7;
		objList[3]->diffusetReflection[2] = 0.7;
		objList[3]->specularReflection[0] = 0.6;
		objList[3]->specularReflection[1] = 0.6;
		objList[3]->specularReflection[2] = 0.6;
		objList[3]->speN = 650;

	}

	if (i == 1)
	{

		// Step 5
		addAnotherScene();
	}
}

void keyboard(unsigned char key, int x, int y)
{
	//keys to control scaling - k
	//keys to control rotation - alpha
	//keys to control translation - tx, ty
	switch (key) {
	case 's':
	case 'S':
		sceneNo = (sceneNo + 1) % NUM_SCENE;
		setScene(sceneNo);
		renderScene();
		glutPostRedisplay();
		break;
	case 'q':
	case 'Q':
		exit(0);

	default:
		break;
	}
}

int main(int argc, char **argv)
{


	cout << "<<CS3241 Lab 5>>\n\n" << endl;
	cout << "S to go to next scene" << endl;
	cout << "Q to quit" << endl;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINWIDTH, WINHEIGHT);

	glutCreateWindow("CS3241 Lab 5: Ray Tracing");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);

	objList = new RtObject*[NUM_OBJECTS];

	// create four spheres
	objList[0] = new Sphere(Vector3(-130, 80, 120), 100);
	objList[1] = new Sphere(Vector3(130, -80, -80), 100);
	objList[2] = new Sphere(Vector3(-130, -80, -80), 100);
	objList[3] = new Sphere(Vector3(130, 80, 120), 100);

	setScene(0);

	setScene(sceneNo);
	renderScene();

	glutMainLoop();

	for (int i = 0; i < NUM_OBJECTS; i++)
		delete objList[i];
	delete[] objList;

	delete[] pixelBuffer;

	return 0;
}
