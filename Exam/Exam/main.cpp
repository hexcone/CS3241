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

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	//controls transformation
	glScalef(k, k, k);
	glTranslatef(tx, ty, 0);
	glRotatef(alpha, 0, 0, 1);

	//draw your stuff here
	reflectAlongAxis(); //Semester 1, 2016/2017, Question 2

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
	glutReshapeFunc(reshape);
	//glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
