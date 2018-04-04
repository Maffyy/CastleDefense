#include "game.h"
#include <GL\glut.h>
#include <windows.h>
#include <GL\GLU.h>
#include <GL\GL.h>
#include <complex>
#include <iostream>
#include <vector>

using Complex = std::complex<float>;
static Complex pos;
static Complex bullet_pos;
static enum Type {LASER, PARTICLE, VECTOR};

class Cannon {
	float x,y;
	Type Type;
};

class Castle {
	float size;
	float life;
};

struct enemy {
	float x,y;
	float life;
};

static std::vector<enemy> enemies;

int gridX, gridY;
short sDirection = RIGHT;
bool shoot;
GLfloat rotation = 0.0 * 360;
float move = 0;
int posX = 10, posY = 20;
float projX, projY;
float enemyMove = 0.5f;



void initGrid(int x, int y) {
	gridX = x;
	gridY = y;
}
void unit(float, float, float, float, float);


void drawCastle() {
	for (size_t x = 0; x < gridX - 30; x++) {
		for (size_t y = 3; y < gridY - 20; y++) {
			unit((float)x, (float)y, 0.8f, 2.5f, 3.0f);
		}
	}

}
void drawGround() {

	for (size_t x = 0; x < gridX; x++) {
		for (size_t y = 0; y < 3; y++) {
			unit((float)x, (float)y, 0.5f, 0.1, 0.5);
		}
	}
}

void unit(float x, float y, float r, float g, float b) {

	glLineWidth(1.0);
	glColor3f(r, g, b);
	glRectd(x, y, x + 1, y + 1);
}

void drawEnemy() {

	if (40 - enemyMove <= 10) {
		enemyMove = 0;
	}
	enemyMove+= 0.4f;
	unit(40.0f - enemyMove,3.0f,1.0f,1.0f,1.0f);
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
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(posX, posY, 0);
	glRotatef(rotation, 0, 0, 1.0f);
	glTranslatef(-posX, -posY, 0);

	glLineWidth(40);
	glBegin(GL_LINE_LOOP);
	glColor3f(0.8, 1.5, 0.2);
	glVertex2f(posX, posY);
	glVertex2f(posX, posY + 5);

	glEnd();
	
	if (shoot) {
		if (posY - 0.5 + 5 + move > 50) {
			shoot = false;
			move = 0;

		}
		else {
			move++;
			std::cout << posX - 0.5 << std::endl;
			std::cout << posY - 0.5 + 5 + move << std::endl;
			glRectd(posX - 0.5, posY - 0.5 + 5 + move, posX + 0.5, posY + 0.5 + 5 + move);
		}

	}
	glPopMatrix();
	

}
