#include "game.h"
#include <GL\glut.h>
#include <gl\GLU.h>
#include <GL\GL.h>
#include <stdio.h>
#include "SOIL.h"
#include <iostream>


int gridX, gridY;
short sDirection = RIGHT;
bool shoot;
GLfloat rotation = 0.0 * 360;

int posX = 10, posY = 20;

void initGrid(int x, int y) {

	gridX = x;
	gridY = y;
}
void unit(int, int, float, float, float);


void drawCastle() {
	for (size_t x = 0; x < gridX - 30; x++) {
		for (size_t y = 3; y < gridY - 20; y++) {
			unit(x, y, 0.8f, 2.5f, 3.0f);
		}
	}

}

void drawGround() {

	for (size_t x = 0; x < gridX; x++) {
		for (size_t y = 0; y < 3; y++) {
			unit(x, y, 0.5f, 0.1, 0.5);
		}
	}
}

void unit(int x, int y, float r, float g, float b) {

	glLineWidth(1.0);
	glColor3f(r, g, b);
	glRectd(x, y, x + 1, y + 1);
}

void drawProjectile() {
	
	if (shoot) {

		glBegin(GL_POINT);
		
		glEnd;
		
	}
	
}

void drawCannon() {


	if (sDirection == RIGHT) {
		rotation -= 20.00f;
		sDirection = DEFAULT;
	}
	else if (sDirection == LEFT) {
		rotation += 20.00f;
		sDirection = DEFAULT;
	}

	glPushMatrix();
	glTranslatef(posX,posY,0);
	glRotatef(rotation,0,0,1.0f);
	glTranslatef(-posX,-posY,0);

	glLineWidth(40);
	glBegin(GL_LINE_LOOP);
		glColor3f(0.8, 1.5, 0.2);
		glVertex2f(posX, posY);
		glVertex2f(posX,posY+5);

	glEnd();
	glLineWidth(40);

	drawProjectile();

	glPopMatrix();
	

}
