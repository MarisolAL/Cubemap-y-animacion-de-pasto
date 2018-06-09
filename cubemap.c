/*
 * cubemap.c
 *
 *  Created on: 4 jun. 2018
 *      Author: marisol
 */
#include <stdio.h>
#include <GL/glut.h>
#include <ctype.h>

static float x_pos = 0;
static float y_pos = 0;
static float z_pos = 0;

static float x_ang = 0;
static float y_ang = 0;
static float z_ang = 0;

#define	imageWidth 2048
#define	imageHeight 2048

unsigned char texImage[imageHeight][imageWidth][3];
const char *imagenes[] = {"posx.ppm","negx.ppm","posy.ppm", "negy.ppm", "posz.ppm", "negz.ppm"};

GLuint textures[6];


/**
 * Rutina que lee una imagen PPM dado el nombre de archivo (ruta) que se
 * ingrese como entrada
 */
void readPPMImage(char* filename)
{
	FILE *fp;
	int  ch, i;

	if ((fp = fopen(filename, "r")) == NULL){
		fprintf(stderr, "Cannot open ppm file %s\n",filename);
		exit(1);
	}
	for (i = 0; i < 4; i++){
		while (1){
			if ((ch = fgetc(fp)) != '#') break;
			fgets((char*)texImage, 1024, fp);
			while(isspace(ch)) ch = fgetc(fp);
		}
		while (!isspace(ch)) ch = fgetc(fp);

		if (i < 3){
			while (isspace(ch)) ch = fgetc(fp);
		}
	}
	fread(texImage, 1, imageWidth*imageHeight*3, fp);
	fclose(fp);
}

/**
 * Rutina que se encarga de leer las 6 imagenes y las carga
 * como texturas en textures[]
 */
void setUpTexture(void)
{
	char *x;
	glGenTextures(6, textures);
	for(int i = 0; i< 6; i++){
		GLint texture = textures[i];
		x = (char *)imagenes[i];
		readPPMImage(x);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, texImage);

	}

}

/**
 * Rutina que inicializa lo necesario para el cubemap
 */
void myInit(char *progname)
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(progname);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}

/**
 * Rutina que pinta el cubo y pega las texturas.
 */
void myDisplay(void)
{
	float ancho = 10;
	float D = x_pos - ancho/ 2;
	glPushMatrix();
	glRotatef(z_ang, 0.0, 0.0, 1.0);
	glRotatef(y_ang, 0.0, 1.0, 0.0);
	glRotatef(x_ang, 1.0, 0.0, 0.0);
	glTranslatef(x_pos, y_pos, z_pos);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex3f(-D,-D,-D);
	glTexCoord2f(1,0); glVertex3f(+D,-D,-D);
	glTexCoord2f(1,1); glVertex3f(+D,+D,-D);
	glTexCoord2f(0,1); glVertex3f(-D,+D,-D);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex3f(+D,-D,-D);
	glTexCoord2f(1,0); glVertex3f(+D,-D,+D);
	glTexCoord2f(1,1); glVertex3f(+D,+D,+D);
	glTexCoord2f(0,1); glVertex3f(+D,+D,-D);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textures[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex3f(+D,-D,+D);
	glTexCoord2f(1,0); glVertex3f(-D,-D,+D);
	glTexCoord2f(1,1); glVertex3f(-D,+D,+D);
	glTexCoord2f(0,1); glVertex3f(+D,+D,+D);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex3f(-D,-D,+D);
	glTexCoord2f(1,0); glVertex3f(-D,-D,-D);
	glTexCoord2f(1,1); glVertex3f(-D,+D,-D);
	glTexCoord2f(0,1); glVertex3f(-D,+D,+D);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex3f(-D,+D,-D);
	glTexCoord2f(1,0); glVertex3f(+D,+D,-D);
	glTexCoord2f(1,1); glVertex3f(+D,+D,+D);
	glTexCoord2f(0,1); glVertex3f(-D,+D,+D);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glBegin(GL_QUADS);

	glTexCoord2f(1,1); glVertex3f(+D,-D,-D);
	glTexCoord2f(0,1); glVertex3f(-D,-D,-D);
	glTexCoord2f(0,0); glVertex3f(-D,-D,+D);
	glTexCoord2f(1,0); glVertex3f(+D,-D,+D);
	glEnd();
	glPopMatrix();
	glFlush();
	glDisable(GL_TEXTURE_2D);
}

/**
 * Funcion que define el teclado
 */
void keyboard (unsigned char key, int x, int y) {
	switch (key) {
	case 'x':
	x_pos -= 1;
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
	case 'w':
		x_ang -= 4;
		glutPostRedisplay();
		break;
	case 'W':
		x_ang += 4;
		glutPostRedisplay();
		break;
	case 'e':
		y_ang -= 4;
		glutPostRedisplay();
		break;
	case 'E':
		y_ang += 4;
		glutPostRedisplay();
		break;
	case 'r':
		z_ang -= 4;
		glutPostRedisplay();
		break;
	case 'R':
		z_ang += 4;
		glutPostRedisplay();
		break;
	case 'q':
		exit(0);
		break;
	default: break;
	}
}

/**
 * Funcion que se encarga de adecuar los graficos
 * cuando se cambia el tamano de la ventana
 */
void myReshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)width / (double)height, 0.1, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0.0, 0.0, -3.6);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	setUpTexture();
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutMainLoop();
	return 0;
}


