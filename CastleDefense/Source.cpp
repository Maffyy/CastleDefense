#include <GL\glut.h>
#include <GL\GL.h>


using namespace std;

void display_callback();
void reshape_callback();

void init() {
	glClearColor(1.0,1.0,1.0,1.0);
}

int main(int argc,char **argv) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1000, 750);
	glutCreateWindow("CastleDefense");
	glutDisplayFunc(display_callback);
	glutReshapeFunc(reshape_callback);
	init();
	glutMainLoop();
	
	return 0;
}

void display_callback() {
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

void reshape_callback(int w,int h) {
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

}