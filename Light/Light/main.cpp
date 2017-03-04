// CS3241Lab3.cpp : Defines the entry point for the console application.
//#include <cmath>
#include "math.h"
#include <iostream>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#include "GL\freeglut.h"
#define M_PI 3.141592654
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

// global variable

#define DEFAULT_ANGLE 0;
#define DEFAULT_ANGLE2 0;
#define DEFAULT_ZOOM 1.0;
#define DEFAULT_FIELD_OF_VIEW 40.0;
#define DEFAULT_ASPECT_RATIO 1.0;
#define DEFAULT_Z_NEAR 1.0;
#define DEFAULT_Z_FAR 80.0;
#define DEFAULT_X_TRANSLATION 0.0;
#define DEFAULT_EYEX 0.0;
#define DEFAULT_EYEY 0.0;
#define DEFAULT_EYEZ 0.0;
#define DEFAULT_CENTERX 0.0;
#define DEFAULT_CENTERY 0.0;
#define DEFAULT_CENTERZ -1.0;
#define DEFAULT_UPX 0.0;
#define DEFAULT_UPY 1.0;
#define DEFAULT_UPZ 0.0;

bool m_Smooth = false;
bool m_Highlight = false;
bool animate_camera = false;
bool disco_light = false;
GLfloat angle = DEFAULT_ANGLE;   /* in degrees */
GLfloat angle2 = DEFAULT_ANGLE2;   /* in degrees */
GLfloat zoom = DEFAULT_ZOOM;
GLfloat field_of_view = DEFAULT_FIELD_OF_VIEW;
GLfloat aspect_ratio = DEFAULT_ASPECT_RATIO;
GLfloat z_near = DEFAULT_Z_NEAR;
GLfloat z_far = DEFAULT_Z_FAR;
GLfloat x_translation = DEFAULT_X_TRANSLATION;
GLdouble eyex = DEFAULT_EYEX;
GLdouble eyey = DEFAULT_EYEY;
GLdouble eyez = DEFAULT_EYEZ;
GLdouble centerx = DEFAULT_CENTERX;
GLdouble centery = DEFAULT_CENTERY;
GLdouble centerz = DEFAULT_CENTERZ;
GLdouble upx = DEFAULT_UPX;
GLdouble upy = DEFAULT_UPY;
GLdouble upz = DEFAULT_UPZ;

int mouseButton = 0;
int moving, startx, starty;

#define NO_OBJECT 4;
int current_object = 0;

using namespace std;

void setupLighting() {
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

	// Lights, material properties
	GLfloat	ambientProperties[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat	diffuseProperties[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat	specularProperties[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightPosition[] = { -100.0f, 100.0f, 100.0f, 1.0f };

	if (disco_light) {
		int value = fmod((glutGet(GLUT_ELAPSED_TIME) * 0.3), 768);
		GLfloat r = 0, g = 0 , b = 0;
		if (value < 256) {
			r = value;
			g = 255 - value;
			b = 255;
		} else if (value < 512) {
			value = value - 256;
			r = 255;
			g = value;
			b = 255 - value;
		} else if (value < 768) {
			value = value - 512;
			r = 255 - value;
			g = 255;
			b = value;
		}
		cout << "value: " << value << "\n";
		GLfloat	newSpecularProperties[] = { r / 255.0, g / 255.0, b / 255.0, 1.0f };
		for (int i = 0; i < 4; i++) {
			specularProperties[i] = newSpecularProperties[i];
		}
	}

	glClearDepth(1.0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientProperties);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularProperties);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	// Default : lighting
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

void drawSphere(double r, float mat_diffuse[]) {
	float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float mat_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
	float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float no_shininess = 0.0f;
	float shininess = 100.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	if (m_Highlight) {
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	} else {
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
	}

	int i, j;
	int n = 20;
	for (i = 0; i < n; i++) {
		for (j = 0; j < 2 * n; j++) {
			if (m_Smooth) {
				glBegin(GL_POLYGON);

				// the normal of each vertex is actaully its own coordinates normalized for a sphere
				// the vector from the origin to the vertex is the normal
				// from http://mathworld.wolfram.com/SphericalCoordinates.html
				// x = r*cos(theta)*sin(phi)
				// y = r*sin(theta)*sin(phi)
				// z = r*cos(phi)
				glNormal3d(sin(i*M_PI / n)*cos(j*M_PI / n), cos(i*M_PI / n)*cos(j*M_PI / n), sin(j*M_PI / n));
				glVertex3d(r*sin(i*M_PI / n)*cos(j*M_PI / n), r*cos(i*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));

				glNormal3d(sin((i + 1)*M_PI / n)*cos(j*M_PI / n), cos((i + 1)*M_PI / n)*cos(j*M_PI / n), sin(j*M_PI / n));
				glVertex3d(r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));

				glNormal3d(sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), sin((j + 1)*M_PI / n));
				glVertex3d(r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));

				glNormal3d(sin(i*M_PI / n)*cos((j + 1)*M_PI / n), cos(i*M_PI / n)*cos((j + 1)*M_PI / n), sin((j + 1)*M_PI / n));
				glVertex3d(r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n), r*cos(i*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				glEnd();
			} else {
				glBegin(GL_POLYGON);
				// Explanation: the normal of the whole polygon is the coordinate of the center of the polygon for a sphere
				glNormal3d(sin((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n), cos((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n), sin((j + 0.5)*M_PI / n));
				glVertex3d(r*sin(i*M_PI / n)*cos(j*M_PI / n), r*cos(i*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
				glVertex3d(r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
				glVertex3d(r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				glVertex3d(r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n), r*cos(i*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				glEnd();
			}
		}
	}
}

void drawSemiSphere(double r, float mat_diffuse[]) {
	float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float mat_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
	float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float no_shininess = 0.0f;
	float shininess = 100.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	if (m_Highlight) {
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	} else {
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
	}

	int i, j;
	int n = 20;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (m_Smooth) {
				glBegin(GL_POLYGON);

				// the normal of each vertex is actaully its own coordinates normalized for a sphere
				// the vector from the origin to the vertex is the normal
				// from http://mathworld.wolfram.com/SphericalCoordinates.html
				// x = r*cos(theta)*sin(phi)
				// y = r*sin(theta)*sin(phi)
				// z = r*cos(phi)
				glNormal3d(sin(i*M_PI / n)*cos(j*M_PI / n), cos(i*M_PI / n)*cos(j*M_PI / n), sin(j*M_PI / n));
				glVertex3d(r*sin(i*M_PI / n)*cos(j*M_PI / n), r*cos(i*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));

				glNormal3d(sin((i + 1)*M_PI / n)*cos(j*M_PI / n), cos((i + 1)*M_PI / n)*cos(j*M_PI / n), sin(j*M_PI / n));
				glVertex3d(r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));

				glNormal3d(sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), sin((j + 1)*M_PI / n));
				glVertex3d(r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));

				glNormal3d(sin(i*M_PI / n)*cos((j + 1)*M_PI / n), cos(i*M_PI / n)*cos((j + 1)*M_PI / n), sin((j + 1)*M_PI / n));
				glVertex3d(r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n), r*cos(i*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				glEnd();
			} else {
				glBegin(GL_POLYGON);
				// Explanation: the normal of the whole polygon is the coordinate of the center of the polygon for a sphere
				glNormal3d(sin((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n), cos((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n), sin((j + 0.5)*M_PI / n));
				glVertex3d(r*sin(i*M_PI / n)*cos(j*M_PI / n), r*cos(i*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
				glVertex3d(r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
				glVertex3d(r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				glVertex3d(r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n), r*cos(i*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				glEnd();
			}
		}
	}
}

void drawEllipsoid(double a, double b, double c, float mat_diffuse[]) {
	float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float mat_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
	float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float no_shininess = 0.0f;
	float shininess = 100.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	if (m_Highlight) {
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	} else {
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
	}

	int i, j;
	int n = 20;
	for (i = 0; i < n; i++) {
		for (j = 0; j < 2 * n; j++) {
			if (m_Smooth) {
				glBegin(GL_POLYGON);

				// from http://mathworld.wolfram.com/Ellipsoid.html
				// x = a*cos(u)*sin(v)
				// y = b*sin(u)*sin(v)
				// z = c*cos(v)
				// where 0 <= v <= PI, is our (i*M_PI / n)
				// and 0 <= u <= 2*PI, is our (j*M_PI / n)
				glNormal3d(cos(j*M_PI / n)*sin(i*M_PI / n), sin(j*M_PI / n)*sin(i*M_PI / n), cos(i*M_PI / n));
				glVertex3d(a*cos(j*M_PI / n)*sin(i*M_PI / n), b*sin(j*M_PI / n)*sin(i*M_PI / n), c*cos(i*M_PI / n));

				glNormal3d(cos(j*M_PI / n)*sin((i + 1)*M_PI / n), sin(j*M_PI / n)*sin((i + 1)*M_PI / n), cos((i + 1)*M_PI / n));
				glVertex3d(a*cos(j*M_PI / n)*sin((i + 1)*M_PI / n), b*sin(j*M_PI / n)*sin((i + 1)*M_PI / n), c*cos((i + 1)*M_PI / n));

				glNormal3d(cos((j + 1)*M_PI / n)*sin((i + 1)*M_PI / n), sin((j + 1)*M_PI / n)*sin((i + 1)*M_PI / n), cos((i + 1)*M_PI / n));
				glVertex3d(a*cos((j + 1)*M_PI / n)*sin((i + 1)*M_PI / n), b*sin((j + 1)*M_PI / n)*sin((i + 1)*M_PI / n), c*cos((i + 1)*M_PI / n));

				glNormal3d(cos((j + 1)*M_PI / n)*sin(i*M_PI / n), sin((j + 1)*M_PI / n)*sin(i*M_PI / n), cos(i*M_PI / n));
				glVertex3d(a*cos((j + 1)*M_PI / n)*sin(i*M_PI / n), b*sin((j + 1)*M_PI / n)*sin(i*M_PI / n), c*cos(i*M_PI / n));
				glEnd();
			} else {
				glBegin(GL_POLYGON);
				// Explanation: the normal of the whole polygon is the coordinate of the center of the polygon for a sphere
				glNormal3d(cos((j + 0.5)*M_PI / n)*sin((i + 0.5)*M_PI / n), sin((j + 0.5)*M_PI / n)*sin((i + 0.5)*M_PI / n), cos((i + 0.5)*M_PI / n));
				glVertex3d(a*cos(j*M_PI / n)*sin(i*M_PI / n), b*sin(j*M_PI / n)*sin(i*M_PI / n), c*cos(i*M_PI / n));
				glVertex3d(a*cos(j*M_PI / n)*sin((i + 1)*M_PI / n), b*sin(j*M_PI / n)*sin((i + 1)*M_PI / n), c*cos((i + 1)*M_PI / n));
				glVertex3d(a*cos((j + 1)*M_PI / n)*sin((i + 1)*M_PI / n), b*sin((j + 1)*M_PI / n)*sin((i + 1)*M_PI / n), c*cos((i + 1)*M_PI / n));
				glVertex3d(a*cos((j + 1)*M_PI / n)*sin(i*M_PI / n), b*sin((j + 1)*M_PI / n)*sin(i*M_PI / n), c*cos(i*M_PI / n));
				glEnd();
			}
		}
	}
}

void drawCylinder(double h, double r, float mat_diffuse[]) {
	float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float mat_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
	float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float no_shininess = 0.0f;
	float shininess = 100.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	if (m_Highlight) {
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	} else {
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
	}

	int i, j;
	int n = 20;
	for (j = 0; j < 2 * n; j++) {
		if (m_Smooth) {
			glBegin(GL_POLYGON);

			// from http://mathworld.wolfram.com/Cylinder.html
			// x = r*cos(theta)
			// y = r*sin(theta)
			// z = z
			// where 0 <= u <= h, is our (i / n * h)
			// and 0 <= theta < 2*PI, is our (j*M_PI / n)	
			glNormal3d(cos(j*M_PI / n), sin(j*M_PI / n), 0);
			glVertex3d(r*cos(j*M_PI / n), r*sin(j*M_PI / n), 0);

			glNormal3d(cos(j*M_PI / n), sin(j*M_PI / n), h);
			glVertex3d(r*cos(j*M_PI / n), r*sin(j*M_PI / n), h);

			glNormal3d(cos((j + 1)*M_PI / n), sin((j + 1)*M_PI / n), h);
			glVertex3d(r*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n), h);

			glNormal3d(cos((j + 1)*M_PI / n), sin((j + 1)*M_PI / n), 0);
			glVertex3d(r*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n), 0);
			glEnd();
		} else {
			glBegin(GL_POLYGON);
			// Explanation: the normal of the whole polygon is the coordinate of the center of the polygon for a sphere
			glNormal3d(cos((j + 0.5)*M_PI / n), sin((j + 0.5)*M_PI / n), h / 2);
			glVertex3d(r*cos(j*M_PI / n), r*sin(j*M_PI / n), 0);
			glVertex3d(r*cos(j*M_PI / n), r*sin(j*M_PI / n), h);
			glVertex3d(r*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n), h);
			glVertex3d(r*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n), 0);
			glEnd();
		}
	}

	// draw lid

	glBegin(GL_POLYGON);
	glNormal3d(0, 0, 1);
	for (int i = 0; i < 360; i++)
	{
		float rad = i*M_PI / 180;
		glVertex3f(r * cos(rad), r* sin(rad), h);
	}
	glEnd();

	glPushMatrix();
		glRotatef(180, 1, 0, 0);
		glTranslatef(0, 0, -h);
		// draw bottom
		glBegin(GL_POLYGON);
		glNormal3d(0, 0, 1);
		for (int i = 0; i < 360; i++)
		{
			float rad = i*M_PI / 180;
			glVertex3f(r * cos(rad), r* sin(rad), h);
		}
		glEnd();
	glPopMatrix();
}

void drawTorus(double a, double c, float mat_diffuse[]) {
	float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float mat_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
	float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float no_shininess = 0.0f;
	float shininess = 100.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	if (m_Highlight) {
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	} else {
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
	}

	int i, j;
	int n = 20;
	for (i = 0; i < 2 * n; i++) {
		for (j = 0; j < 2 * n; j++) {
			if (m_Smooth) {
				glBegin(GL_POLYGON);

				// from http://mathworld.wolfram.com/Torus.html
				// x = [c + a * cos(v)] * cos(u)
				// y = [c + a * cos(v)] * sin(u)
				// z = a * sin(v)
				// where a is the radius of the tube and c is the radius of the hole
				// and 0 <= u, v < 2*PI, is our (i*M_PI / n) and (j*M_PI / n)
				glNormal3d((cos(j*M_PI / n) * cos(i*M_PI / n)), (cos(j*M_PI / n) * sin(i*M_PI / n)), (sin(j*M_PI / n)));
				glVertex3d(((c + a * cos(j*M_PI / n)) * cos(i*M_PI / n)), ((c + a * cos(j*M_PI / n)) * sin(i*M_PI / n)), (a * sin(j*M_PI / n)));

				glNormal3d((cos(j*M_PI / n) * cos((i + 1)*M_PI / n)), (cos(j*M_PI / n) * sin((i + 1)*M_PI / n)), (sin(j*M_PI / n)));
				glVertex3d(((c + a * cos(j*M_PI / n)) * cos((i + 1)*M_PI / n)), ((c + a * cos(j*M_PI / n)) * sin((i + 1)*M_PI / n)), (a * sin(j*M_PI / n)));

				glNormal3d((cos((j + 1)*M_PI / n) * cos((i + 1)*M_PI / n)), (cos((j + 1)*M_PI / n) * sin((i + 1)*M_PI / n)), (sin((j + 1)*M_PI / n)));
				glVertex3d(((c + a * cos((j + 1)*M_PI / n)) * cos((i + 1)*M_PI / n)), ((c + a * cos((j + 1)*M_PI / n)) * sin((i + 1)*M_PI / n)), (a * sin((j + 1)*M_PI / n)));

				glNormal3d((cos((j + 1)*M_PI / n) * cos(i*M_PI / n)), (cos((j + 1)*M_PI / n) * sin(i*M_PI / n)), (sin((j + 1)*M_PI / n)));
				glVertex3d(((c + a * cos((j + 1)*M_PI / n)) * cos(i*M_PI / n)), ((c + a * cos((j + 1)*M_PI / n)) * sin(i*M_PI / n)), (a * sin((j + 1)*M_PI / n)));
				glEnd();
			} else {
				glBegin(GL_POLYGON);
				glNormal3d((cos((j + 0.5)*M_PI / n) * cos((i + 0.5)*M_PI / n)), (cos((j + 0.5)*M_PI / n) * sin((i + 0.5)*M_PI / n)), (sin((j + 0.5)*M_PI / n)));
				glVertex3d(((c + a * cos(j*M_PI / n)) * cos(i*M_PI / n)), ((c + a * cos(j*M_PI / n)) * sin(i*M_PI / n)), (a * sin(j*M_PI / n)));
				glVertex3d(((c + a * cos(j*M_PI / n)) * cos((i + 1)*M_PI / n)), ((c + a * cos(j*M_PI / n)) * sin((i + 1)*M_PI / n)), (a * sin(j*M_PI / n)));
				glVertex3d(((c + a * cos((j + 1)*M_PI / n)) * cos((i + 1)*M_PI / n)), ((c + a * cos((j + 1)*M_PI / n)) * sin((i + 1)*M_PI / n)), (a * sin((j + 1)*M_PI / n)));
				glVertex3d(((c + a * cos((j + 1)*M_PI / n)) * cos(i*M_PI / n)), ((c + a * cos((j + 1)*M_PI / n)) * sin(i*M_PI / n)), (a * sin((j + 1)*M_PI / n)));
				glEnd();
			}
		}
	}
}

void drawHalfTorus(double a, double c, float mat_diffuse[]) {
	float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float mat_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
	float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float no_shininess = 0.0f;
	float shininess = 100.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	if (m_Highlight) {
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	} else {
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
	}

	int i, j;
	int n = 20;
	for (i = 0; i < n; i++) {
		for (j = 0; j < 2 * n; j++) {
			if (m_Smooth) {
				glBegin(GL_POLYGON);

				// from http://mathworld.wolfram.com/Torus.html
				// x = [c + a * cos(v)] * cos(u)
				// y = [c + a * cos(v)] * sin(u)
				// z = a * sin(v)
				// where a is the radius of the tube and c is the radius of the hole
				// and 0 <= u, v < 2*PI, is our (i*M_PI / n) and (j*M_PI / n)
				glNormal3d((cos(j*M_PI / n) * cos(i*M_PI / n)), (cos(j*M_PI / n) * sin(i*M_PI / n)), (sin(j*M_PI / n)));
				glVertex3d(((c + a * cos(j*M_PI / n)) * cos(i*M_PI / n)), ((c + a * cos(j*M_PI / n)) * sin(i*M_PI / n)), (a * sin(j*M_PI / n)));

				glNormal3d((cos(j*M_PI / n) * cos((i + 1)*M_PI / n)), (cos(j*M_PI / n) * sin((i + 1)*M_PI / n)), (sin(j*M_PI / n)));
				glVertex3d(((c + a * cos(j*M_PI / n)) * cos((i + 1)*M_PI / n)), ((c + a * cos(j*M_PI / n)) * sin((i + 1)*M_PI / n)), (a * sin(j*M_PI / n)));

				glNormal3d((cos((j + 1)*M_PI / n) * cos((i + 1)*M_PI / n)), (cos((j + 1)*M_PI / n) * sin((i + 1)*M_PI / n)), (sin((j + 1)*M_PI / n)));
				glVertex3d(((c + a * cos((j + 1)*M_PI / n)) * cos((i + 1)*M_PI / n)), ((c + a * cos((j + 1)*M_PI / n)) * sin((i + 1)*M_PI / n)), (a * sin((j + 1)*M_PI / n)));

				glNormal3d((cos((j + 1)*M_PI / n) * cos(i*M_PI / n)), (cos((j + 1)*M_PI / n) * sin(i*M_PI / n)), (sin((j + 1)*M_PI / n)));
				glVertex3d(((c + a * cos((j + 1)*M_PI / n)) * cos(i*M_PI / n)), ((c + a * cos((j + 1)*M_PI / n)) * sin(i*M_PI / n)), (a * sin((j + 1)*M_PI / n)));
				glEnd();
			} else {
				glBegin(GL_POLYGON);
				glNormal3d((cos((j + 0.5)*M_PI / n) * cos((i + 0.5)*M_PI / n)), (cos((j + 0.5)*M_PI / n) * sin((i + 0.5)*M_PI / n)), (sin((j + 0.5)*M_PI / n)));
				glVertex3d(((c + a * cos(j*M_PI / n)) * cos(i*M_PI / n)), ((c + a * cos(j*M_PI / n)) * sin(i*M_PI / n)), (a * sin(j*M_PI / n)));
				glVertex3d(((c + a * cos(j*M_PI / n)) * cos((i + 1)*M_PI / n)), ((c + a * cos(j*M_PI / n)) * sin((i + 1)*M_PI / n)), (a * sin(j*M_PI / n)));
				glVertex3d(((c + a * cos((j + 1)*M_PI / n)) * cos((i + 1)*M_PI / n)), ((c + a * cos((j + 1)*M_PI / n)) * sin((i + 1)*M_PI / n)), (a * sin((j + 1)*M_PI / n)));
				glVertex3d(((c + a * cos((j + 1)*M_PI / n)) * cos(i*M_PI / n)), ((c + a * cos((j + 1)*M_PI / n)) * sin(i*M_PI / n)), (a * sin((j + 1)*M_PI / n)));
				glEnd();
			}
		}
	}
}

void drawPokeball() {
	float mat_diffuse_red[] = { 0.8f, 0.1f, 0.1f, 1.0f };
	float mat_diffuse_black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float mat_diffuse_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// main ball
	drawSphere(1.05, mat_diffuse_black);
	glPushMatrix();
		glTranslatef(0, 0, 0.1);
		drawSemiSphere(1.1, mat_diffuse_red);
	glPopMatrix();
	glPushMatrix();
		glRotatef(180, 1, 0, 0);
		glTranslatef(0, 0, 0.1);
		drawSemiSphere(1.1, mat_diffuse_white);
	glPopMatrix();
	// button
	glPushMatrix();
		glRotatef(90, 1, 0, 0);
		glTranslatef(0, 0, 1);
		drawCylinder(0.1, 0.4, mat_diffuse_black);
		drawCylinder(0.15, 0.25, mat_diffuse_white);
		drawCylinder(0.17, 0.2, mat_diffuse_white);
	glPopMatrix();
}

void drawPuff(float mat_diffuse_jumpluff_stalk_color[], float mat_diffuse_jumpluff_cream[], float puff_size) {
	glPushMatrix();
		glTranslatef(0, 0.65, 0);
		glRotatef(270, 1, 0, 0);
		drawCylinder(0.5, 0.04, mat_diffuse_jumpluff_stalk_color);
		glTranslatef(0, 0, 0.8);
		drawSphere(puff_size, mat_diffuse_jumpluff_cream);
		for (int i = 0; i < 700; i++) {
			glPushMatrix();
				glRotatef(rand() % 360, 1, 0, 0);
				glRotatef(rand() % 360, 0, 1, 0);
				glRotatef(rand() % 360, 0, 0, 1);

				glTranslatef(0, puff_size - 0.05, 0);
				glRotatef(270, 1, 0, 0);
				drawCylinder(0.1, 0.005, mat_diffuse_jumpluff_cream);
			glPopMatrix();
		}
	glPopMatrix();
}

void drawJumpluff() {
	float mat_diffuse_black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float mat_diffuse_red[] = { 0.8f, 0.1f, 0.1f, 1.0f };
	float mat_diffuse_jumpluff_blue[] = { 0.45f, 0.55f, 0.85f, 1.0f };
	float mat_diffuse_jumpluff_cream[] = { 0.95f, 0.95f, 0.75f, 1.0f };
	float mat_diffuse_jumpluff_green[] = { 0.6f, 0.8f, 0.25f, 1.0f };

	// main body
	drawSphere(0.7, mat_diffuse_jumpluff_blue);
	// draw left leg
	glPushMatrix();
		glRotatef(140, 1, 0, 0); // up down
		glRotatef(30, 0, 0, 1); // left right
		glRotatef(25, 0, 1, 0);
		glTranslatef(0, 0.71, 0);
		drawEllipsoid(0.1, 0.1, 0.15, mat_diffuse_jumpluff_blue);
	glPopMatrix();
	// draw right leg
	glPushMatrix();
		glRotatef(140, 1, 0, 0);
		glRotatef(-30, 0, 0, 1);
		glRotatef(-25, 0, 1, 0);
		glTranslatef(0, 0.71, 0);
		drawEllipsoid(0.1, 0.1, 0.15, mat_diffuse_jumpluff_blue);
	glPopMatrix();
	// draw tail
	glPushMatrix();
		glRotatef(70, 1, 0, 0);
		glRotatef(180, 0, 0, 1);
		glTranslatef(0, 0.71, 0);
		drawSphere(0.2, mat_diffuse_jumpluff_blue);
	glPopMatrix();
	// draw left eye
	glPushMatrix();
		glRotatef(60, 1, 0, 0);
		glRotatef(15, 0, 0, 1);
		glTranslatef(0, 0.7, 0);
		drawSphere(0.05, mat_diffuse_red);
	glPopMatrix();
	// draw right eye
	glPushMatrix();
		glRotatef(60, 1, 0, 0);
		glRotatef(-15, 0, 0, 1);
		glTranslatef(0, 0.7, 0);
		drawSphere(0.05, mat_diffuse_red);
	glPopMatrix();
	// draw mouth
	glPushMatrix();
		glRotatef(65, 1, 0, 0);
		glTranslatef(0, 0.7, 0);
		glRotatef(90, 1, 0, 0);
		drawHalfTorus(0.01, 0.05, mat_diffuse_black);
	glPopMatrix();
	// draw leaf
	glPushMatrix();
		glPushMatrix();
			glRotatef(-6, 1, 0, 0);
			glTranslatef(0, 0.77, 0);
			glRotatef(60, 1, 0, 0);
			drawEllipsoid(0.05, 0.03, 0.1, mat_diffuse_jumpluff_green);
		glPopMatrix();
		glPushMatrix();
			glRotatef(130, 0, 1, 0);
			glRotatef(-6, 1, 0, 0);
			glTranslatef(0, 0.77, 0);
			glRotatef(60, 1, 0, 0);
			drawEllipsoid(0.05, 0.03, 0.1, mat_diffuse_jumpluff_green);
		glPopMatrix();
		glPushMatrix();
			glRotatef(230, 0, 1, 0);
			glRotatef(-6, 1, 0, 0);
			glTranslatef(0, 0.77, 0);
			glRotatef(60, 1, 0, 0);
			drawEllipsoid(0.05, 0.03, 0.1, mat_diffuse_jumpluff_green);
		glPopMatrix();
	glPopMatrix();
	// draw puff
	glPushMatrix();
		drawPuff(mat_diffuse_jumpluff_cream, mat_diffuse_jumpluff_cream, 0.5);
		// draw left puff
		glPushMatrix();
			glRotatef(70, 0, 0, 1);
			drawPuff(mat_diffuse_jumpluff_blue, mat_diffuse_jumpluff_cream, 0.45);
		glPopMatrix();
		// draw right puff
		glPushMatrix();
			glRotatef(-70, 0, 0, 1);
			drawPuff(mat_diffuse_jumpluff_blue, mat_diffuse_jumpluff_cream, 0.45);
		glPopMatrix();
	glPopMatrix();
}

void display(void) {
	// Projection tool
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(field_of_view,
		aspect_ratio,
		z_near, z_far);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0, 0, -6);
	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	// Camera movement
	if (animate_camera) {
		double angle = fmod((glutGet(GLUT_ELAPSED_TIME) * 0.001), 360);
		double x = cos(angle);
		double y = sin(angle);
		gluLookAt(x, y, eyez, centerx, centery, centerz, upx, upy, upz);
	} else {
		gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
	}

	glScalef(zoom, zoom, zoom);

	float mat_diffuse_black[] = { 0.1f, 0.1f, 0.1f, 1.0f };

	switch (current_object) {
	case 0:
		drawSphere(1, mat_diffuse_black);
		break;
	case 1:
		drawTorus(0.5, 1, mat_diffuse_black);
		break;
	case 2:
		drawPokeball();
		break;
	case 3:
		drawJumpluff();
		break;
	default:
		break;
	};
	glPopMatrix();
	glutSwapBuffers();

	setupLighting();
}

void resetCamera() {
	angle = DEFAULT_ANGLE;
	angle2 = DEFAULT_ANGLE2;
	zoom = DEFAULT_ZOOM;
	field_of_view = DEFAULT_FIELD_OF_VIEW;
	aspect_ratio = DEFAULT_ASPECT_RATIO;
	z_near = DEFAULT_Z_NEAR;
	z_far = DEFAULT_Z_FAR;
	x_translation = DEFAULT_X_TRANSLATION;
	eyex = DEFAULT_EYEX;
	eyey = DEFAULT_EYEY;
	eyez = DEFAULT_EYEZ;
	centerx = DEFAULT_CENTERX;
	centery = DEFAULT_CENTERY;
	centerz = DEFAULT_CENTERZ;
	upx = DEFAULT_UPX;
	upy = DEFAULT_UPY;
	upz = DEFAULT_UPZ;
	animate_camera = false;
	disco_light = false;
	return;
}

void setCameraBestAngle() {
	switch (current_object) {
	case 0:
		break;
	case 1:
		angle = -30;
		angle2 = -40;
		break;
	case 2:
		angle = 0;
		angle2 = -90;
		upx = -0.2;
		break;
	case 3:
		eyex = 0.2;
		eyey = 0.6;
		centerx = -0.2;
		centery = 0.5;
		break;
	}
	return;
}

void keyboard(unsigned char key, int x, int y) {
	//add additional commands here to change Field of View and movement
	switch (key) {
	case 'p':
	case 'P':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'w':
	case 'W':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'v':
	case 'V':
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case 's':
	case 'S':
		m_Smooth = !m_Smooth;
		break;
	case 'h':
	case 'H':
		m_Highlight = !m_Highlight;
		break;

	case 'n':
		z_near -= 0.1;
		if (z_near <= 0) {
			z_near = 0.1;
		}
		break;

	case 'N':
		z_near += 0.1;
		break;

	case 'f':
		z_far -= 0.1;
		if (z_far <= 0) {
			z_far = 0.1;
		}
		break;

	case 'F':
		z_far += 0.1;
		break;

	case 'o':
		field_of_view -= 10;
		if (field_of_view == -10) {
			field_of_view = 350;
		}
		break;

	case 'O':
		field_of_view += 10;
		if (field_of_view == 360) {
			field_of_view = 0;
		}
		break;

	case 'r':
		resetCamera();
		break;

	case 'R':
		resetCamera();
		setCameraBestAngle();
		break;

	case 'c':
	case 'C':
		animate_camera = !animate_camera;
		break;

	case 'd':
	case 'D':
		disco_light = !disco_light;
		break;

	case '1':
	case '2':
	case '3':
	case '4':
		resetCamera();
		current_object = key - '1';
		break;

	case 27:
		exit(0);
		break;

	default:
		break;
	}

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		mouseButton = button;
		moving = 1;
		startx = x;
		starty = y;
	}
	if (state == GLUT_UP) {
		mouseButton = button;
		moving = 0;
	}
}

void motion(int x, int y) {
	if (moving) {
		if (mouseButton == GLUT_LEFT_BUTTON) {
			angle = angle + (x - startx);
			angle2 = angle2 + (y - starty);
		} else {
			zoom += ((y - starty)*0.01);
		}
		startx = x;
		starty = y;
		glutPostRedisplay();
	}
}

int main(int argc, char **argv) {
	cout << "CS3241 Lab 3" << endl << endl;

	cout << "1-4: Draw different objects" << endl;
	cout << "S: Toggle Smooth Shading" << endl;
	cout << "H: Toggle Highlight" << endl;
	cout << "W: Draw Wireframe" << endl;
	cout << "P: Draw Polygon" << endl;
	cout << "V: Draw Vertices" << endl;
	cout << "n, N: Reduce or increase the distance of the near plane from the camera" << endl;
	cout << "f, F: Reduce or increase the distance of the far plane from the camera" << endl;
	cout << "o, O: Reduce or increase the distance of the povy plane from the camera" << endl;
	cout << "r: Reset camera to the initial parameters when the program starts" << endl;
	cout << "R: Change camera to another setting that is has the best viewing angle for your object" << endl;
	cout << "C: Animate camera" << endl;
	cout << "D: Disco lights" << endl;
	cout << "ESC: Quit" << endl << endl;

	cout << "Left mouse click and drag: rotate the object" << endl;
	cout << "Right mouse click and drag: zooming" << endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("CS3241 Assignment 3");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glMatrixMode(GL_PROJECTION);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
//	setupLighting();
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glutMainLoop();

	return 0;
}