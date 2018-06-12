/*
 * pasto.c
 *
 *  Created on: 9 jun. 2018
 *      Author: marisol
 */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

int tiempoActual, tiempoAnterior;
double delta;
//static int x_i = 100;
int pastos[] = {100,150,200};
static GLfloat b= 300;
static GLfloat a = 100*1.5;
static int lim_sup = 100*1.5;
static float x_pos = 0;
static float y_pos = 0;
static float z_pos = -0.1;


void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	//glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 480, 0, 640);
	//glClear(GL_COLOR_BUFFER_BIT);
}

void calculaTiempo(){
	tiempoActual = glutGet(GLUT_ELAPSED_TIME);
	//printf("%d\n", tiempoActual);
	delta = (double)(tiempoActual - tiempoAnterior)/1000;
	tiempoAnterior = tiempoActual;

}

void animacionFrames(){
	int par = (tiempoActual/300)%2;
	if(lim_sup> 2*(a)){
		par = 1;

	}
	if (par){
		b = b + delta*100;
		lim_sup = lim_sup - tiempoActual/20000;
	}else{
		b = b - delta*100;
		lim_sup = lim_sup + tiempoActual/20000;
	}
	if(lim_sup >= 2*a){
		printf("%d\n", tiempoActual);
	}

}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0,0.4,0);
	GLfloat y;

	glPushMatrix();
	glTranslatef(x_pos, y_pos, z_pos);
	int x_i = pastos[0];
	GLfloat c = sqrt((x_i - a)*(x_i - a) + b*b); //Directriz de la parabola

	float incremento = (lim_sup-x_i)/32;
	glBegin(GL_LINE_STRIP);
	for(GLfloat x = x_i; x <= lim_sup; x+=incremento) {
		y = ((x - a)*(x - a) + b*b - c*c)/(2*(b-c));
		glVertex3f(x, y, z_pos);

	}
	glEnd();
	glutSolidSphere(1, 100, 50);
	glPopMatrix();
	glutSwapBuffers();





}

void animar(){
	calculaTiempo();
	animacionFrames();
	glutPostRedisplay();
}

/**
 * Funcion que define el teclado
 */
void keyboard (unsigned char key, int x, int y) {
	switch (key) {
	case 'x':
		x_pos -= 0.5;
		glutPostRedisplay();
		break;
	case 'X':
		x_pos += 0.5;
		glutPostRedisplay();
		break;
	case 'y':
		y_pos -= 0.5;
		glutPostRedisplay();
		break;
	case 'Y':
		y_pos += 0.5;
		glutPostRedisplay();
		break;
	case 'z':
		z_pos -= 0.5;
		glutPostRedisplay();
		break;
	case 'Z':
		z_pos += 0.5;
		glutPostRedisplay();
		break;
	case 'q':
		exit(0);
		break;
	default: break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(200, 400);
	glutInitWindowSize(640, 480);
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutCreateWindow("Quadratic Formula");
	init();
	glutIdleFunc(animar);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);

	tiempoActual = tiempoAnterior = 0;
	glutMainLoop();
	return 0;

}
