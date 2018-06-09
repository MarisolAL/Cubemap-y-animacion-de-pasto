/*
 * pasto.c
 *
 *  Created on: 9 jun. 2018
 *      Author: marisol
 */

#include <GL/glut.h>

static float x_pos = 0;
static float y_pos = 0;
static float z_pos = 0;

void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 480, 0, 640);
}

void display() {
	glTranslatef(x_pos, y_pos, z_pos);
	glColor3f(0.8,1.0,0.4);
	GLfloat verticeY = 500;
	GLfloat verticeX = 100;
	GLfloat y = 0;
	int p = 5;
	int h = 5 + 4*p;
	int lim_sup = h + p;
	int altura_pasto = 5;
	int inicio = h- 15*p;

	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	for(GLfloat x = inicio; x <= lim_sup; x+=0.05) {
		y = -((x - h)*(x - h))/(4*p) + altura_pasto;
		glVertex2f(verticeX + x, verticeY + y);
	}
	glEnd();
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(200, 400);
	glutInitWindowSize(640, 480);
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutCreateWindow("Quadratic Formula");
	init();
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}
