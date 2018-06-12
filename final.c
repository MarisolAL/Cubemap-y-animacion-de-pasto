/*
 * final.c
 *
 *  Created on: 12 jun. 2018
 *      Author: marisol
 */
#include <stdio.h>
#include <GL/glut.h>
#include <ctype.h>
#include <math.h>

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

int tiempoActual, tiempoAnterior;
double delta;
static GLfloat b= 0.04;
static float lim_sup = 0.3;
static int cambio = 0;
int cuenta = 0;


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
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}

/**
 * Rutina que calcula el tiempo pasado desde
 * la ejecucion del programa
 */
void calculaTiempo(){
	tiempoActual = glutGet(GLUT_ELAPSED_TIME);
	delta = (double)(tiempoActual - tiempoAnterior)/1000;
	tiempoAnterior = tiempoActual;

}

/**
 * Se encarga de cambiar la posicion
 * del foco y los limites al graficar parabolas
 * dado el tiempo transcurrido
 */
void animacionFrames(){
	cuenta+=1;
	if(cuenta%1500 == 0){
		cambio = cambio ^ 1;
	}
	if (cambio){
		b = b + 0.0009;
		lim_sup = lim_sup - 0.00001;
	}else{
		b = b - 0.0009;
		lim_sup = lim_sup + 0.00001;
	}
}

/**
 * Se encarga de cambiar los intervalos al graficar pasto
 */
float como_incremento(float actual){
	int caso = (int)actual%10;
	switch(caso){
	case 0:
		return 0.065;

	case 1:
		return 0.09;

	case 2:
		return 0.15;

	case 3:
		return 0.25;
	case 4:
		return 0.13;
	case 5:
			return 0.08;
	case 6:
			return 0.15;
	case 7:
			return 0.09;
	case 8:
			return 0.29;
	case 9:
			return 0.12;
	default: return 0.24;
	}

}

/**
 * Calcula el verde con el que se pintara una hoja de pasto
 */
float calcula_verde(float actual){
	int caso = (int)(actual*10)%10;
	float res = 0.1727451;
	switch(caso){
	case 0:
		return res + 0.051;

	case 1:
		return res + 0.087;

	case 2:
		return res + 0.01457;

	case 3:
		return res + 0.022;
	case 4:
		return res + 0.013;
	case 5:
			return res + 0.03546;
	case 6:
			return res + 0.0133;
	case 7:
			return res + 0.012;
	case 8:
			return res + 0.02784;
	case 9:
			return res + 0.061;

	default: return res;
	}

}
/**
 * Rutina que pinta el cubo,pega las texturas y pinta el pasto.
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

	//glPushAttrib(GL_CURRENT_BIT);

	//glClearColor(0.0f,0.4f,0.f,1.0f );
	glEnable(GL_COLOR_MATERIAL);
	/**GLfloat mat_amb_diff[] = { 0.0f,0.4f,0.f,1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
			mat_amb_diff);*/

	float auxb;
	GLfloat y;
	GLfloat c;
	float limite;
	GLfloat a;
	float x_i;
	float d = abs(D);
	for(float ii = -d*2; ii<d; ii+=0.1){
		float incremento = como_incremento(ii);
		for(x_i = -d; x_i < d; x_i+=incremento){
			glColorMaterial(GL_FRONT, GL_DIFFUSE);
			/* now glColor* changes diffuse reflection */
			//glColor3f(0.1254902 , 0.37647059 , 0.31372549);
			float verde = calcula_verde(x_i);
			glColor3f(0.02 ,verde  , 0.16603922);
			auxb = b + x_i*0.09;
			a = x_i+ 2;
			c = sqrt((x_i - a)*(x_i - a) + auxb*auxb); //Directriz de la parabola
			glLineWidth(1.4);
			limite = lim_sup + x_i;
			glBegin(GL_LINE_STRIP);
			for(GLfloat x = x_i; x <= limite; x+=0.08) {
				y = ((x - a)*(x - a) + auxb*auxb - c*c)/(2*(auxb-c));
				glVertex3f(x+ii, y+D/2, x_i);

			}
			glEnd();
		}
	}
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	/* now glColor* changes diffuse reflection */
	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);


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

/**
 * Se encarga de lo que es la animacion
 */
void animar(){
	calculaTiempo();
	animacionFrames();
	glutPostRedisplay();
}


/**
 * Metodo main
 */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	setUpTexture();
	glutIdleFunc(animar);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	gluLookAt(  0.0, 0.0, 10.0, /* eye point */
			0.0, 0.0, 0.0,  /* punto de referencia */
			0.0, 1.0, 0.0);
	glutMainLoop();
	tiempoActual = tiempoAnterior = 0;
	return 0;
}





