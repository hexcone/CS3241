// CS3241Lab4.cpp : Defines the entry point for the console application.
//#include <cmath>
#include "math.h"
#include <iostream>
#include <fstream>

/* Include header files depending on platform */
#ifdef _WIN32
#include "GL\freeglut.h"
#define M_PI 3.14159
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

#define MAXPTNO 1000
#define NLINESEGMENT 32
#define NOBJECTONCURVE 8

using namespace std;

// Global variables that you can use
struct Point {
	int x, y;
};

// Storage of control points
int nPt = 0;
Point ptList[MAXPTNO];
Point c1List[MAXPTNO];

// Display options
bool displayControlPoints = true;
bool displayControlLines = true;
bool displayTangentVectors = false;
bool displayObjects = false;
bool C1Continuity = false;

// Burger from assignment 2
void drawEllipse(int x, int y)
{
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		float rad = i * M_PI / 180;
		glVertex2f(cos(rad) * x, sin(rad) * y);
	}
	glEnd();
}
void drawSesemeSeed(void)
{
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glPushMatrix();
			glTranslatef(-50, 30, 0);
			glRotatef(35, 0, 0, 1);
			drawEllipse(10, 5);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-20, 40, 0);
			glRotatef(-15, 0, 0, 1);
			drawEllipse(10, 5);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(10, 30, 0);
			glRotatef(25, 0, 0, 1);
			drawEllipse(10, 5);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(40, 40, 0);
			glRotatef(-25, 0, 0, 1);
			drawEllipse(10, 5);
		glPopMatrix();
	glPopMatrix();
}

void drawTopBun(void)
{
	glPushMatrix();
		glColor3f(246 / 255.0, 200 / 255.0, 71 / 255.0);
		glBegin(GL_POLYGON);
			glVertex2f(cos(0 * M_PI / 180) * 100, (sin(0 * M_PI / 180) * 80) - 10);
			for (int i = 0; i < 181; i++)
			{
				float rad = i * M_PI / 180;
				glVertex2f(cos(rad) * 100, sin(rad) * 80);
			}
			glVertex2f(cos(180 * M_PI / 180) * 100, (sin(180 * M_PI / 180) * 80) - 10);
		glEnd();

		drawSesemeSeed();
	glPopMatrix();
}

void drawTomato(void)
{
	glPushMatrix();
		glColor3f(177 / 255.0, 36 / 255.0, 17 / 255.0);
		glBegin(GL_QUADS);
			glVertex2f(-95, -10);
			glVertex2f(-95, 10);
			glVertex2f(95, 10);
			glVertex2f(95, -10);
		glEnd();
	glPopMatrix();
}

void drawPatty(void)
{
	glPushMatrix();
		glColor3f(123 / 255.0, 77 / 255.0, 61 / 255.0);
		glBegin(GL_QUADS);
			glVertex2f(-100, -20);
			glVertex2f(-100, 20);
			glVertex2f(100, 20);
			glVertex2f(100, -20);
		glEnd();
		glPushMatrix();
			glTranslatef(-100, 0, 0);
			glBegin(GL_POLYGON);
			for (int i = 90; i < 271; i++)
			{
				float rad = i * M_PI / 180;
				glVertex2f(cos(rad) * 20, sin(rad) * 20);
			}
			glEnd();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(100, 0, 0);
			glBegin(GL_POLYGON);
			for (int i = -90; i < 91; i++)
			{
				float rad = i * M_PI / 180;
				glVertex2f(cos(rad) * 20, sin(rad) * 20);
			}
			glEnd();
		glPopMatrix();
	glPopMatrix();
}

void drawCheese(void)
{
	glPushMatrix();
		glColor3f(246 / 255.0, 233 / 255.0, 145 / 255.0);
		glBegin(GL_QUADS);
			glVertex2f(-100, -3);
			glVertex2f(-100, 3);
			glVertex2f(100, 3);
			glVertex2f(100, -3);
		glEnd();
		glBegin(GL_TRIANGLES);
			glVertex2f(0, -3);
			glVertex2f(80, -3);
			glVertex2f(40, -25);
		glEnd();
	glPopMatrix();
}

void drawBottomBun(void)
{
	glPushMatrix();
		glColor3f(246 / 255.0, 200 / 255.0, 71 / 255.0);
		glBegin(GL_QUADS);
			glVertex2f(-100, -10);
			glVertex2f(-100, 10);
			glVertex2f(100, 10);
			glVertex2f(100, -10);
		glEnd();
		glBegin(GL_QUADS);
			glVertex2f(-80, -30);
			glVertex2f(-80, 10);
			glVertex2f(80, 10);
			glVertex2f(80, -30);
		glEnd();
		glPushMatrix();
			glTranslatef(-80, -10, 0);
			drawEllipse(20, 20);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(80, -10, 0);
			drawEllipse(20, 20);
		glPopMatrix();
	glPopMatrix();
}

void drawVeggie(void)
{
	glPushMatrix();
		glColor3f(182 / 255.0, 248 / 255.0, 77 / 255.0);
		glBegin(GL_QUADS);
			glVertex2f(-100, -10);
			glVertex2f(-100, 10);
			glVertex2f(100, 10);
			glVertex2f(100, -10);
		glEnd();
		glTranslatef(-125, -5, 0);
		for (int i = 0; i < 9; i++) {
			glTranslatef(25, 0, 0);
			drawEllipse(15, 15);
		}
	glPopMatrix();
}

void drawBurger()
{
	glPushMatrix();
		// Patty is our reference meat
		glPushMatrix();
			drawPatty();
		glPopMatrix();

		// Above the patty
		glPushMatrix();
			drawCheese();
		glPopMatrix();
		glPushMatrix();
			drawTomato();
		glPopMatrix();
		glPushMatrix();
			drawTopBun();
		glPopMatrix();

		//Below the patty
		glPushMatrix();
			drawBottomBun();
		glPopMatrix();
		glPushMatrix();
			drawVeggie();
		glPopMatrix();
	glPopMatrix();
}

// Assignment requirements
void recomputeAllC1() {
	for (int i = 0; i < nPt; i++) {
		if ((i > 3) && ((i + 2) % 3 == 0)) {
			Point p1, p2;
			p1 = ptList[i - 1];
			p2 = ptList[i - 2];
			c1List[i].x = p1.x - (p2.x - p1.x);
			c1List[i].y = p1.y - (p2.y - p1.y);
		}
		else {
			c1List[i].x = ptList[i].x;
			c1List[i].y = ptList[i].y;
		}
	}
}

void recomputeC1() {
	if ((nPt > 4) && ((nPt + 1) % 3 == 0)) {
		Point p1, p2;
		p1 = ptList[nPt - 2];
		p2 = ptList[nPt - 3];
		c1List[nPt - 1].x = p1.x - (p2.x - p1.x);
		c1List[nPt - 1].y = p1.y - (p2.y - p1.y);
	} else {
		c1List[nPt - 1].x = ptList[nPt - 1].x;
		c1List[nPt - 1].y = ptList[nPt - 1].y;
	}
}

void drawRightArrow() {
	glColor3f(0, 1, 0);
	glBegin(GL_LINE_STRIP);
		glVertex2f(0, 0);
		glVertex2f(100, 0);
		glVertex2f(95, 5);
		glVertex2f(100, 0);
		glVertex2f(95, -5);
	glEnd();
}

void drawControlPoints() {
	glPointSize(5);
	glBegin(GL_POINTS);
	for (int i = 0; i < nPt; i++) {
		if (C1Continuity) {
			if ((i > 3) && ((i + 2) % 3 == 0)) {
				// red
				glColor3f(1, 0, 0);
				glVertex2d(c1List[i].x, c1List[i].y);
				//grey
				glColor3f(0.7, 0.7, 0.7);
				glVertex2d(ptList[i].x, ptList[i].y);
			} else {
				// black
				glColor3f(0, 0, 0);
				glVertex2d(ptList[i].x, ptList[i].y);
			}
		}
		else {
			// all black
			glColor3f(0, 0, 0);
			glVertex2d(ptList[i].x, ptList[i].y);
		}
	}
	glEnd();
	glPointSize(1);
}

void drawControlLines() {
	glColor3f(0, 1, 0);
	for (int i = 1; i < nPt; i++) {
		glBegin(GL_LINES);
		if (C1Continuity) {
			glVertex2f(c1List[i - 1].x, c1List[i - 1].y);
			glVertex2f(c1List[i].x, c1List[i].y);
		} else {
			glVertex2f(ptList[i - 1].x, ptList[i - 1].y);
			glVertex2f(ptList[i].x, ptList[i].y);
		}
		glEnd();
	}
}

void drawBezierCurve() {
	Point p1, p2, p3, p4, p;
	for (int i = 0; i < nPt; i += 3) {
		if (i + 3 < nPt) {
			if (C1Continuity) {
				p1 = c1List[i];
				p2 = c1List[i + 1];
				p3 = c1List[i + 2];
				p4 = c1List[i + 3];
			} else {
				p1 = ptList[i];
				p2 = ptList[i + 1];
				p3 = ptList[i + 2];
				p4 = ptList[i + 3];
			}

			glLineWidth(1);
			glColor3f(0, 0, 1);
			glBegin(GL_LINE_STRIP);
			for (double j = 0; j < NLINESEGMENT + 1; j++) {
				double t = j / NLINESEGMENT;
				p.x = pow((1 - t), 3) * p1.x +
					pow((1 - t), 2) * t * 3 * p2.x +
					(1 - t) * pow(t, 2) * 3 * p3.x +
					pow(t, 3) * p4.x;
				p.y = pow((1 - t), 3) * p1.y +
					pow((1 - t), 2) * t * 3 * p2.y +
					(1 - t) * pow(t, 2) * 3 * p3.y +
					pow(t, 3) * p4.y;
				glVertex2d(p.x, p.y);
			}
			glEnd();
		}

	}
}

void drawTangentVectors() {
	Point p1, p2, p3, p4, p, pd;
	for (int i = 0; i < nPt; i += 3) {
		if (i + 3 < nPt) {
			if (C1Continuity) {
				p1 = c1List[i];
				p2 = c1List[i + 1];
				p3 = c1List[i + 2];
				p4 = c1List[i + 3];
			}
			else {
				p1 = ptList[i];
				p2 = ptList[i + 1];
				p3 = ptList[i + 2];
				p4 = ptList[i + 3];
			}

			for (double j = 0; j < NOBJECTONCURVE + 1; j++) {
				double t = j / NOBJECTONCURVE;
				p.x = pow((1 - t), 3) * p1.x +
					pow((1 - t), 2) * t * 3 * p2.x +
					(1 - t) * pow(t, 2) * 3 * p3.x +
					pow(t, 3) * p4.x;
				p.y = pow((1 - t), 3) * p1.y +
					pow((1 - t), 2) * t * 3 * p2.y +
					(1 - t) * pow(t, 2) * 3 * p3.y +
					pow(t, 3) * p4.y;

				pd.x = pow((1 - t), 2) * -3 * p1.x +
					(3 * pow(t, 2) - 4 * t + 1) * 3 * p2.x +
					(-3 * pow(t, 2) + 2 * t) * 3 * p3.x +
					3 * pow(t, 2) * p4.x;

				pd.y = pow((1 - t), 2) * -3 * p1.y +
					(3 * pow(t, 2) - 4 * t + 1) * 3 * p2.y +
					(-3 * pow(t, 2) + 2 * t) * 3 * p3.y +
					3 * pow(t, 2) * p4.y;

				glPushMatrix();
					glTranslatef(p.x, p.y, 0);
					double angle = atan((double)pd.y / (double)pd.x) * 180 / M_PI;
					if (pd.x < 0) {
						angle += 180;
					}
					glRotatef(angle, 0, 0, 1);
					drawRightArrow();
				glPopMatrix();
			}
			glEnd();
		}

	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	// draw control points
	if (displayControlPoints) {
		drawControlPoints();
	}

	// draw control lines
	if (displayControlLines) {
		drawControlLines();
	}

	// draw bezier curve
	drawBezierCurve();

	if (displayTangentVectors) {
		drawTangentVectors();
	}

	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void readFile() {
	std::ifstream file;
	file.open("savefile.txt");
	file >> nPt;

	if (nPt > MAXPTNO) {
		cout << "Error: File contains more than the maximum number of points." << endl;
		nPt = MAXPTNO;
	}

	for (int i = 0; i < nPt; i++) {
		file >> ptList[i].x;
		file >> ptList[i].y;
	}

	recomputeAllC1();

	file.close(); // is not necessary because the destructor closes the open file by default
}

void writeFile() {
	std::ofstream file;
	file.open("savefile.txt");
	file << nPt << endl;

	for (int i = 0; i < nPt; i++) {
		// always write the normal version, without conversion to c1 continuity
		file << ptList[i].x << " ";
		file << ptList[i].y << endl;
	}
	file.close(); // is not necessary because the destructor closes the open file by default
}

void clearAllControlPoints() {
	nPt = 0;
	// not necessary but let's just do it
	memset(&ptList[0], 0, sizeof(ptList));
	memset(&c1List[0], 0, sizeof(c1List));
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'r':
		case 'R':
			readFile();
			break;

		case 'w':
		case 'W':
			writeFile();
			break;

		case 'T':
		case 't':
			displayTangentVectors = !displayTangentVectors;
			break;

		case 'o':
		case 'O':
			displayObjects = !displayObjects;
			break;

		case 'p':
		case 'P':
			displayControlPoints = !displayControlPoints;
			break;

		case 'L':
		case 'l':
			displayControlLines = !displayControlLines;
			break;

		case 'C':
		case 'c':
			C1Continuity = !C1Continuity;
			break;

		case 'e':
		case 'E':
			// Do something to erase all the control points added
			clearAllControlPoints();
			break;

		case 27:
			exit(0);
			break;

		default:
			break;
	}

	glutPostRedisplay();
}



void mouse(int button, int state, int x, int y)
{
	/*button: GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON */
	/*state: GLUT_UP or GLUT_DOWN */
	enum {
		MOUSE_LEFT_BUTTON = 0,
		MOUSE_MIDDLE_BUTTON = 1,
		MOUSE_RIGHT_BUTTON = 2,
		MOUSE_SCROLL_UP = 3,
		MOUSE_SCROLL_DOWN = 4
	};

	if ((button == MOUSE_LEFT_BUTTON) && (state == GLUT_UP)) {
		if (nPt == MAXPTNO) {
			cout << "Error: Exceeded the maximum number of points." << endl;
			return;
		}
		ptList[nPt].x = x;
		ptList[nPt].y = y;
		nPt++;

		recomputeC1();
	}
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	cout << "CS3241 Lab 4" << endl << endl;
	cout << "Left mouse click: Add a control point" << endl;
	cout << "ESC: Quit" << endl;
	cout << "P: Toggle displaying control points" << endl;
	cout << "L: Toggle displaying control lines" << endl;
	cout << "E: Erase all points (Clear)" << endl;
	cout << "C: Toggle C1 continuity" << endl;
	cout << "T: Toggle displaying tangent vectors" << endl;
	cout << "O: Toggle displaying objects" << endl;
	cout << "R: Read in control points from \"savefile.txt\"" << endl;
	cout << "W: Write control points to \"savefile.txt\"" << endl;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("CS3241 Assignment 4");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}