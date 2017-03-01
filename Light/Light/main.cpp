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

bool m_Smooth = false;
bool m_Highlight = false;
GLfloat angle = 0;   /* in degrees */
GLfloat angle2 = 0;   /* in degrees */
GLfloat zoom = 1.0;
GLfloat field_of_view = 40.0;
GLfloat x_translation = 0.0;

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


void drawSphere(double r) {

	float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float mat_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float mat_diffuse[] = { 0.1f, 0.5f, 0.8f, 1.0f };
	float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
	float no_shininess = 0.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	if (m_Highlight)
	{
		// your codes for highlight here
	}
	else {
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
				// where theta is our (i*M_PI / n)
				// and phi is our (j*M_PI / n)
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

void display(void) {
	//Add Projection tool and Camera Movement somewhere here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0, 0, -6);
	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	glScalef(zoom, zoom, zoom);

	switch (current_object) {
	case 0:
		drawSphere(1);
		break;
	case 1:
		// draw your second primitive object here
		break;
	case 2:
		// draw your first composite object here
		break;
	case 3:
		// draw your second composite object here
		break;
	default:
		break;
	};
	glPopMatrix();
	glutSwapBuffers();
}


void resetCamera() {
	/*
	//fill in values below.
	zoom =
	angle =
	angle2 =
	zoom =
	field_of_view =
	x_translation =
	// include resetting of gluPerspective & gluLookAt.
	*/
	return;
}

void setCameraBestAngle() {
	/*
	//fill in values below
	zoom =
	angle =
	angle2 =
	zoom =
	field_of_view =
	x_translation =
	//TIPS: Adjust gluLookAt function to change camera position
	*/
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
		// -Insert code to adjust Near-
		break;

	case 'N':
		// -Insert code to adjust Near-
		break;

	case 'f':
		// -Insert code to adjust Far-
		break;

	case 'F':
		// -Insert code to adjust Far-
		break;

	case 'o':
		// -Insert code to adjust Fovy-
		break;

	case 'O':
		// -Insert code to adjust Fovy-
		break;

	case 'r':
		resetCamera();
		break;

	case 'R':
		setCameraBestAngle();
		break;

	case '1':
	case '2':
	case '3':
	case '4':
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
		}
		else zoom += ((y - starty)*0.01);
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
	glMatrixMode(GL_PROJECTION);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	setupLighting();
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	//REMOVE FROM THIS SECTION

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( /* field of view in degree */ 40.0,
		/* aspect ratio */ 1.0,
		/* Z near */ 1.0, /* Z far */ 80.0);
	glMatrixMode(GL_MODELVIEW);
	int upVector = 1;
	gluLookAt(1, 1, 1, 1, 1, -1, 0, 1, 0);
	//TO THIS SECTION, after implementation of Projection and Camera movement tools
	//Hint: Transfer these functions over into void display(void), then add other variables

	glutMainLoop();

	return 0;
}