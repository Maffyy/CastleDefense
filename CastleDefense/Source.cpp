#include <GL\glut.h>
#include "game.h"


#define COLUMNS 40
#define ROWS 40
#define FPS 10

extern short sDirection;
extern bool shoot;

void timer_callback(int);
void display_callback();
void reshape_callback(int, int);
void keyboard_callback(unsigned char, int, int);
void special_keyboard_callback(int, int, int);

void init() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	initGrid(COLUMNS, ROWS);
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1000, 750);
	glutCreateWindow("CastleDefense");
	glutDisplayFunc(display_callback);
	glutReshapeFunc(reshape_callback);
	glutTimerFunc(0, timer_callback, 0);
	glutKeyboardFunc(keyboard_callback);
	glutSpecialFunc(special_keyboard_callback);
	init();
	glutMainLoop();

	return 0;
}
int index = 0;
void display_callback() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawGround();
	drawCastle();
	drawCannon();
	drawEnemy();
	glutSwapBuffers();
}

void reshape_callback(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, COLUMNS, 0.0, ROWS, -1.0, 1.0);	//orthographic projection
	glMatrixMode(GL_MODELVIEW);
}

void timer_callback(int) {

	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, timer_callback, 0);
}

void keyboard_callback(unsigned char key, int, int) {

	switch (key) {
	case 32:
		shoot = true;
	default:
		break;
	}
}
void special_keyboard_callback(int key, int, int) {


	switch (key) {
	case GLUT_KEY_LEFT:
		sDirection = LEFT;
		break;
	case GLUT_KEY_RIGHT:
		sDirection = RIGHT;
		//cout << "ahoj";
		break;
	default:
		break;
	}

}
