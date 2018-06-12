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

static GLfloat b= 2;
static float lim_sup = 1.5;
static float x_pos = 0;
static float y_pos = 0;
static float z_pos = 0;
static int cambio = 0;
int cuenta = 0;


void init(char *progname) {
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(progname);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	//gluOrtho2D(0, 480, 0, 640);
}

void calculaTiempo(){
	tiempoActual = glutGet(GLUT_ELAPSED_TIME);
	delta = (double)(tiempoActual - tiempoAnterior)/1000;
	tiempoAnterior = tiempoActual;

}

void animacionFrames(){
	cuenta+=1;
	if(cuenta%1500 == 0){
		cambio = cambio ^ 1;
	}
	if (cambio){
		b = b + 0.0003;
		lim_sup = lim_sup - 0.00001;
	}else{
		b = b - 0.0003;
		lim_sup = lim_sup + 0.00001;
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0,0.4,0);
	float auxb;
	GLfloat y;
	GLfloat c;
	float limite;
	GLfloat a;
	int x_i;
	glPushMatrix();
	glTranslatef(x_pos, y_pos, z_pos);
	for(x_i = -1; x_i < 3; x_i++){
		auxb = b + x_i*0.05;
		a = x_i+ 2;
		c = sqrt((x_i - a)*(x_i - a) + auxb*auxb); //Directriz de la parabola
		glLineWidth(1.5);
		limite = lim_sup + x_i;
		glBegin(GL_LINE_STRIP);
		for(GLfloat x = x_i; x <= limite; x+=0.1) {
			y = ((x - a)*(x - a) + auxb*auxb - c*c)/(2*(auxb-c));
			glVertex3f(x, y, 0.0f);

		}
		glEnd();
	}

	glPopMatrix();
	glutSwapBuffers();


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

void animar(){
	calculaTiempo();
	animacionFrames();
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	init(argv[0]);
	glutIdleFunc(animar);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);

	tiempoActual = tiempoAnterior = 0;
	glutMainLoop();
	return 0;

}
