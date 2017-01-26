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

void drawAUnitSquare()
{
	glBegin(GL_QUADS);
		glColor3f(1, 0, 0);
		glVertex2f(-1, -1);
		glVertex2f(-1, 1);
		glVertex2f(1, 1);
		glVertex2f(1, -1);
	glEnd();
}

void drawBranches()
{
	// top
	glPushMatrix();
		for (int i = 0; i < 5; i++) {
			glTranslatef(0, 2, 0);
			glRotatef(20, 0, 0, 1);
			glScalef(0.5, 0.5, 1);
			drawAUnitSquare();
		}
	glPopMatrix();
	
	//bottom
	glPushMatrix();
		for (int i = 0; i < 5; i++) {
			glTranslatef(0, -2, 0);
			glRotatef(20, 0, 0, 1);
			glScalef(0.5, 0.5, 1);
			drawAUnitSquare();
		}
	glPopMatrix();

	//left
	glPushMatrix();
		for (int i = 0; i < 5; i++) {
			glTranslatef(-2, 0, 0);
			glRotatef(20, 0, 0, 1);
			glScalef(0.5, 0.5, 1);
			drawAUnitSquare();
		}
	glPopMatrix();

	//right
	glPushMatrix();
		for (int i = 0; i < 5; i++) {
			glTranslatef(2, 0, 0);
			glRotatef(20, 0, 0, 1);
			glScalef(0.5, 0.5, 1);
			drawAUnitSquare();
		}
	glPopMatrix();
}

void drawSpiral() {
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 0);
	for (int i = 0; i < 181; i++) {
		float degInRad = i * M_PI / 180;
		glVertex3f(cos(degInRad) * (10 * i / (float)360), sin(degInRad) * (10 * i / (float)360), 0);
	}
	glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	//controls transformation
	glScalef(k, k, k);
	glTranslatef(tx, ty, 0);
	glRotatef(alpha, 0, 0, 1);

	//draw question 1
	/*
	drawAUnitSquare();
	drawBranches();
	glRotatef(45, 0, 0, 1);
	drawAUnitSquare();
	drawBranches();
	*/

	//draw question 2
	glRotatef(90, 0, 0, 1);
	drawSpiral();
	glRotatef(180, 1, 0, 0);
	drawSpiral();

	glPopMatrix();
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

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glShadeModel(GL_SMOOTH);
}



void keyboard(unsigned char key, int x, int y)
{
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

int main(int argc, char **argv)
{
	cout << "CS3241 Lab 1\n\n";
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
