// main.cpp : Defines the entry point for the console application.
#include <cmath>
#include <iostream>

/* Include header files depending on platform */
#ifdef _WIN32
#include "GL\freeglut.h"
#define M_PI 3.14159
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

using namespace std;

float alpha = 0.0, k = 1;
float tx = 0.0, ty = 0.0;

void drawAxisForReflect(int m, int c) {
	glLineWidth(1);
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_LINES);
	glVertex3f(10, 0, 0);
	glVertex3f(-10, 0, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0, 10, 0);
	glVertex3f(0, -10, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0, c, 0);
	glVertex3f(-c / m, 0, 0);
	glEnd();
}

void drawSquareForReflect() {
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(1, 1);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2f(3, 1);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(3, -1);
	glVertex2f(1, -1);
	glEnd();
}

void reflectAlongAxis() {
	// line: y = mx + c
	int m = -1, c = -5;

	// set up axis
	drawAxisForReflect(m, c);

	// draw square
	drawSquareForReflect();

	// reflect along line
	glTranslatef(-c / m, c, 0);
	if (m < 0) {
		glRotatef(90, 0, 0, 1);
	} else {
		glRotatef(-90, 0, 0, 1);
	}
	glScalef(-1, 1, 1);

	// draw another square
	drawSquareForReflect();
}

void drawHalfCircleForPattern(int radius) {
	glLineWidth(1);
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 180; i++) {
		float degInRad = i * M_PI / 180;
		glVertex2f(radius + cos(degInRad) * radius, sin(degInRad) * radius);
	}
	glEnd();
}

void drawPattern(int radius) {
	// draw and transform
	for (int i = 0; i < 5; i++) {
		drawHalfCircleForPattern(radius);
		glTranslatef(radius, radius, 0);
		glScalef(0.5, 0.5, 1);
	}
}

void drawSquareForRecursiveSquare() {
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glVertex3f(1, 1, 0);
	glVertex3f(1, -1, 0);
	glVertex3f(-1, -1, 0);
	glVertex3f(-1, 1, 0);
	glEnd();
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(1, -1, 0);
	glVertex3f(-1, -1, 0);
	glVertex3f(-1, 1, 0);
	glEnd();
}

void drawRecursiveSquare(int n) {
	drawSquareForRecursiveSquare();
	if (n > 1) {
		for (int i = -1; i <= 1; i += 2) {
			for (int j = -1; j <= 1; j += 2) {
				glPushMatrix();
				glTranslatef(i, j, -1);
				glScalef(0.5, 0.5, 0);
				drawRecursiveSquare(n - 1);
				glPopMatrix();
			}
		}
	}
}

void drawCircleForRecursiveCircle(int radius) {
	glLineWidth(1);
	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 360; i++) {
		float degInRad = i * M_PI / 180;
		glVertex2f(cos(degInRad) * radius, sin(degInRad) * radius);
	}
	glEnd();
}

void drawRecursiveCircle(int n) {
	int radius = 10;

	drawCircleForRecursiveCircle(radius);
	
	if (n > 1) {
		for (int i = 0; i < 3; i++) {
			glPushMatrix();
			glRotatef(i * 120, 0, 0, 1);
			glScalef(0.5, 0.5, 0);
			glTranslatef(0, 3 * radius, 0);
			drawRecursiveCircle(n - 1);
			glPopMatrix();
		}
	}
}

void drawMoonForSolarSystem() {
	double radius = 0.2;
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 360; i++) {
		float degInRad = i * M_PI / 180;
		glVertex2f(cos(degInRad) * radius, sin(degInRad) * radius);
	}
	glEnd();
}

void drawEarthForSolarSystem() {
	double radius = 0.5;
	glColor3f(0, 1, 1);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 360; i++) {
		float degInRad = i * M_PI / 180;
		glVertex2f(cos(degInRad) * radius, sin(degInRad) * radius);
	}
	glEnd();
}

void drawSunForSolarSystem() {
	double radius = 1;
	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 360; i++) {
		float degInRad = i * M_PI / 180;
		glVertex2f(cos(degInRad) * radius, sin(degInRad) * radius);
	}
	glEnd();
}

void drawSolarSystem() {
	int doy = (glutGet(GLUT_ELAPSED_TIME) / 50) % 365 + 1; // 1-365
	
	drawSunForSolarSystem();
	glRotatef(45 + (doy - 1) / 365.0 * 360, 0, 0, -1);
	glTranslatef(0, 5, 0);

	drawEarthForSolarSystem();
	glRotatef((doy - 1) / 28.0 * 360, 0, 0, -1);
	glTranslatef(0, 1, 0);

	drawMoonForSolarSystem();
}

class Vector2d {
public:
	double x;
	double y;

	Vector2d(double _x, double _y) {
		x = _x;
		y = _y;
	}
};

Vector2d BezierCurve(double t) {
	double x = pow((1 - t), 2) * -200 +
		0 +
		pow(t, 2) * 200;

	double y = pow((1 - t), 2) * 100 +
		2 * (1 - t) * t * -100 +
		pow(t, 2) * 100;
	
	return Vector2d(x, y);
}

void drawValley() {
	glScalef(0.03, 0.03, 0.03);

	glColor3f(0.5, 0.5, 0.5);
	Vector2d curr = BezierCurve(0);
	Vector2d first = Vector2d(-200, -100);
	Vector2d last = Vector2d(200, -100);
	for (double i = 1; i < 11; i++) {
		glBegin(GL_TRIANGLES);
		Vector2d point = BezierCurve(i / 10.0);
		glVertex2f(curr.x, curr.y);
		glVertex2f(point.x, point.y);
		if (i <= 5) {
			glVertex2f(first.x, first.y);
		}
		else {
			glVertex2f(last.x, last.y);		}
		curr = point;
		glEnd();

		if (i == 5) {
			glBegin(GL_TRIANGLES);
			glVertex2f(first.x, first.y);
			glVertex2f(last.x, last.y);
			glVertex2f(curr.x, curr.y);
			glEnd();
		}
	}/*
	*/
	
}

Vector2d unitTangentVector(double t) {
	double x = 2 * (t - 1) * -200 +
		0 +
		2 * t * 200;

	double y = 2 * (t - 1) * 100 +
		(2 - (4 * t)) * -100 +
		2 * t * 100;
	
	double denom = sqrt(pow(x, 2) + pow(y, 2));

	return Vector2d(x / denom, y / denom);
}

void drawAFlower() {
	glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(0, 0);
	glVertex2f(20, -5);
	glVertex2f(20, 5);
	glEnd();
}
void drawFlowersAndValley() {
	drawValley();

	for (double i = 1; i < 10; i++) {
		Vector2d point = BezierCurve(i / 10.0);
		Vector2d tangent = unitTangentVector(i / 10.0);
		double angle = atan(tangent.y / tangent.x) * 180 / 3.14159265;
		if (tangent.x < 0) {
			angle += 180;
		}
		
		glPushMatrix();
		glTranslatef(point.x, point.y, 0);
		glRotatef(90, 0, 0, 1);
		glRotatef(angle, 0, 0, 1);
		drawAFlower();
		glPopMatrix();
		
	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	//controls transformation
	glScalef(k, k, k);
	glTranslatef(tx, ty, 0);
	glRotatef(alpha, 0, 0, 1);

	//draw your stuff here
	//reflectAlongAxis(); //Semester 1, 2016/2017, Question 2
	//drawPattern(50); //Semester 2, 2015/2016, Question 1
	//drawRecursiveSquare(3); //Semester 1, 2015/2016, Question 4 
	//drawRecursiveCircle(3);  //Semester 2, 2014/2015, Question 2
	//drawSolarSystem(); //Semester 2, 2013/2014, Question 2
	drawFlowersAndValley(); //Semester 2, 2013/2014, Question 3

	//end
	glPopMatrix();
	glFlush();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-10, 10, -10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glShadeModel(GL_SMOOTH);
}



void keyboard(unsigned char key, int x, int y) {
	//keys to control scaling - k
	//keys to control rotation - alpha
	//keys to control translation - tx, ty
	switch (key) {

	case 'a':
		alpha += 10;
		glutPostRedisplay();
		break;

	case 'd':
		alpha -= 10;
		glutPostRedisplay();
		break;

	case 'q':
		k += 0.1;
		glutPostRedisplay();
		break;

	case 'e':
		if (k>0.1)
			k -= 0.1;
		glutPostRedisplay();
		break;

	case 'z':
		tx -= 0.1;
		glutPostRedisplay();
		break;

	case 'c':
		tx += 0.1;
		glutPostRedisplay();
		break;

	case 's':
		ty -= 0.1;
		glutPostRedisplay();
		break;

	case 'w':
		ty += 0.1;
		glutPostRedisplay();
		break;

	case 27:
		exit(0);
		break;

	default:
		break;
	}
}

int main(int argc, char **argv) {
	cout << "Exam\n\n";
	cout << "+++++CONTROL BUTTONS+++++++\n\n";
	cout << "Scale Up/Down: Q/E\n";
	cout << "Rotate Clockwise/Counter-clockwise: A/D\n";
	cout << "Move Up/Down: W/S\n";
	cout << "Move Left/Right: Z/C\n";
	cout << "ESC: Quit\n";

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	//glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glutMainLoop();

	return 0;
}
