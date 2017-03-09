// CS3241Lab2.cpp : Defines the entry point for the console application.
#include <cmath>
#include <iostream>
#include <vector>

/* Include header files depending on platform */
#ifdef _WIN32
#include "GL\freeglut.h"
#define M_PI 3.14159
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

using namespace std;

#define DEG2RAD 3.14159/180.0
#define numStar 100

class nugget
{
public:
	float size;
	float angle;
	float speed;
	float elliptic_offset;

	nugget()
	{
		size = ((rand() % 40) + 10) / 100.0;
		angle = rand() % 361;
		speed = ((rand() % 100) + 10) / 1000.0;
		elliptic_offset = rand() % 300;
	}
};

class star
{
public:
	int x;
	int y;
	float shine;
	bool shine_incease;

	star()
	{
		x = (rand() % 1000) - 500;
		y = (rand() % 1000) - 500;
		shine = (rand() % 100) / 100.0;
		shine_incease = rand() & 1;
	}
};

float alpha = 0.0, k = 1;
float tx = 0.0, ty = 0.0;
std::vector<nugget> nuggetList;
std::vector<star> starList;

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-500, 500, -500, 500, -500, 500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void)
{
	glClearColor(68 / 255.0, 25 / 255.0, 23 / 255.0, 1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void drawEllipse(int x, int y)
{
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		float rad = i*DEG2RAD;
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
	glVertex2f(cos(0 * DEG2RAD) * 100, (sin(0 * DEG2RAD) * 80) - 10);
	for (int i = 0; i < 181; i++)
	{
		float rad = i*DEG2RAD;
		glVertex2f(cos(rad) * 100, sin(rad) * 80);
	}
	glVertex2f(cos(180 * DEG2RAD) * 100, (sin(180 * DEG2RAD) * 80) - 10);
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
		float rad = i*DEG2RAD;
		glVertex2f(cos(rad) * 20, sin(rad) * 20);
	}
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(100, 0, 0);
	glBegin(GL_POLYGON);
	for (int i = -90; i < 91; i++)
	{
		float rad = i*DEG2RAD;
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

double getAngle(float speed)
{
	double angle = fmod((glutGet(GLUT_ELAPSED_TIME) * speed), 360.0);
	return angle;
}

void drawBurger()
{
	int runtime = 0;
	if ((glutGet(GLUT_ELAPSED_TIME)) < 10000) {
		runtime = (glutGet(GLUT_ELAPSED_TIME));
	}
	else {
		runtime = 10000;
	}

	glPushMatrix();
	// Patty is our reference meat
	glPushMatrix();
	glRotatef(getAngle(0.01), 0, 0, 1);
	drawPatty();
	glPopMatrix();

	// Above the patty
	glPushMatrix();
	glRotatef(getAngle(0.02), 0, 0, 1);
	glTranslatef(0, 23 + runtime * 0.01, 0);
	drawCheese();
	glPopMatrix();
	glPushMatrix();
	glRotatef(getAngle(0.03), 0, 0, 1);
	glTranslatef(0, 36 + runtime * 0.02, 0);
	drawTomato();
	glPopMatrix();
	glPushMatrix();
	glRotatef(getAngle(0.04), 0, 0, 1);
	glTranslatef(0, 56 + runtime * 0.03, 0);
	drawTopBun();
	glPopMatrix();

	//Below the patty
	glPushMatrix();
	glRotatef(getAngle(0.03), 0, 0, 1);
	glTranslatef(0, -50 - runtime * 0.02, 0);
	drawBottomBun();
	glPopMatrix();
	glPushMatrix();
	glRotatef(getAngle(0.02), 0, 0, 1);
	glTranslatef(0, -30 - runtime * 0.01, 0);
	drawVeggie();
	glPopMatrix();
	glPopMatrix();
}

void drawNuggetBack(void) {
	for (std::vector<nugget>::iterator current = nuggetList.begin(); current != nuggetList.end(); ++current) {
		glPushMatrix();
		float rad = fmod((glutGet(GLUT_ELAPSED_TIME)* current->speed), 360.0) * DEG2RAD;
		// Angle of back view
		if ((rad > M_PI) && (rad <= 2 * M_PI)) {
			glRotatef(current->angle, 0, 0, 1);
			glScalef(current->size, current->size, 0);
			glTranslatef(cos(rad) * 1000 + current->elliptic_offset, sin(rad) * 400, 0);

			glColor3f(244 / 255.0, 176 / 255.0, 66 / 255.0);
			glBegin(GL_POLYGON);
			glVertex2f(-94, -44);
			glVertex2f(-30, -96);
			glVertex2f(3, -102);
			glVertex2f(65, -83);
			glVertex2f(104, 3);
			glVertex2f(68, 88);
			glVertex2f(-88, 68);
			glEnd();
		}
		glPopMatrix();
	}
}

void drawNuggetFront(void)
{
	for (std::vector<nugget>::iterator current = nuggetList.begin(); current != nuggetList.end(); ++current) {
		glPushMatrix();
		float rad = fmod((glutGet(GLUT_ELAPSED_TIME)* current->speed), 360.0) * DEG2RAD;
		// Angle of front view
		if ((rad >= 0) && (rad <= M_PI)) {
			glRotatef(current->angle, 0, 0, 1);
			glScalef(current->size, current->size, 0);
			glTranslatef(cos(rad) * 1000 + current->elliptic_offset, sin(rad) * 400, 0);

			glColor3f(244 / 255.0, 176 / 255.0, 66 / 255.0);
			glBegin(GL_POLYGON);
			glVertex2f(-94, -44);
			glVertex2f(-30, -96);
			glVertex2f(3, -102);
			glVertex2f(65, -83);
			glVertex2f(104, 3);
			glVertex2f(68, 88);
			glVertex2f(-88, 68);
			glEnd();
		}
		glPopMatrix();
	}
}

void drawCokeBackground(void)
{

	for (std::vector<star>::iterator current = starList.begin(); current != starList.end(); ++current) {
		if (current->shine_incease) {
			current->shine += 0.005;
			if (current->shine >= 0.95) {
				current->shine_incease = false;
			}
		}
		else {
			current->shine -= 0.005;
			if (current->shine <= 0.05) {
				current->shine_incease = true;
			}
		}
		glPushMatrix();
		glTranslatef(current->x, current->y, 0);
		glColor4f(255 / 255.0, 255 / 255.0, 255 / 255.0, current->shine);
		drawEllipse(3, 3);
		glPopMatrix();
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	//controls transformation
	glScalef(k, k, k);
	glTranslatef(tx, ty, 0);
	glRotatef(alpha, 0, 0, 1);

	//draw stuff here!
	drawCokeBackground();
	drawNuggetBack();
	drawBurger();
	drawNuggetFront();

	glPopMatrix();
	glFlush();
}

// Add a single nugget
void addNugget(void) {
	nugget* newNugget = new nugget();
	nuggetList.push_back(*newNugget);
}

// One-time initialization of position of stars
void initStar(void) {
	for (int i = 0; i < numStar; i++) {
		star* newStar = new star();
		starList.push_back(*newStar);
	}
}

void idle()
{
	// update animation here
	glPushMatrix();
	drawCokeBackground();
	drawNuggetBack();
	drawBurger();
	drawNuggetFront();
	glPopMatrix();

	// after updating, draw the screen again
	glutPostRedisplay();
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

		// Add a new nugget
	case 'n':
		addNugget();
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
	cout << "CS3241 Lab 2\n\n";
	cout << "+++++CONTROL BUTTONS+++++++\n\n";
	cout << "Scale Up/Down: Q/E\n";
	cout << "Rotate Clockwise/Counter-clockwise: A/D\n";
	cout << "Move Up/Down: W/S\n";
	cout << "Move Left/Right: Z/C\n";
	cout << "Add a new nugget: N\n";
	cout << "ESC: Quit\n";

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);
	init();
	initStar(); // initialize stars
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	//glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
