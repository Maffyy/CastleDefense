#include "game.h"
#include <GL\glut.h>
#include <gl\GLU.h>
#include <GL\GL.h>
#include <stdio.h>
#include "SOIL.h"


int gridX, gridY;
short sDirection = RIGHT;

int posX = 20, posY = 20;
GLuint texture[1];

void initGrid(int x, int y) {

	gridX = x;
	gridY = y;
}
void unit(int, int);



int drawCastle() {

	texture[0] = SOIL_load_OGL_texture
	(
	"./22-affinity-flat-castle.jpg/",
	SOIL_LOAD_AUTO,
	SOIL_CREATE_NEW_ID,
	SOIL_FLAG_INVERT_Y
	);

	if (texture[0] == 0) {
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glEnable(GL_TEXTURE_2D);
	 glShadeModel(GL_SMOOTH);                        // Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                   // Black Background
    glClearDepth(1.0f);                         // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    
	glBegin(GL_QUADS);
		// Front Face
    glTexCoord2f(0.0f, 0.0f); glVertex2f(2, 2);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex2f( 2+1, 2);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex2f( 2+1,  2+1);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex2f(2,  2+1);  // Top Left Of The Texture and Quad
    
	glEnd();

	return true;
}
void drawGround() {
	for (size_t x = 0; x < gridX; x++)
	{
		for (size_t y = 0; y < 3; y++)
		{
			unit(x,y);
		}

	}
}


void unit(int x, int y) {
	
	glLineWidth(1.0);
	glColor3f(1.0, 1.0, 1.0);
	
	glBegin(GL_LINE_LOOP);
		glVertex2f(x, y);
		glVertex2f(x + 1, y);
		glVertex2f(x+1, y+1);
		glVertex2f(x, y + 1);
	glEnd();
}

void drawSnake() {

	if (sDirection == UP)
		posY++;
	else if (sDirection == DOWN)
		posY--;
	else if (sDirection == RIGHT)
		posX++;
	else if (sDirection == LEFT)
		posX--;

	glRectd(posX, posY, posX + 1, posY + 1);
}
