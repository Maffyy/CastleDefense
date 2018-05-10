#include <iostream>
#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <SFML\OpenGL.hpp>
#include <vector>
#include <cstdlib>
#include <thread>

#include "DrawMethods.h"

using namespace sf;


const float PI = 3.14159265;



class Enemy {
public:
	RectangleShape shape;
	Vector2f dir;
	float maxSpeed;

	Enemy(float width = 100.f, float height = 100.f)
		: maxSpeed(5.f) {
		this->shape.setSize(Vector2f(width, height));
		this->shape.setFillColor(Color(255,255,255));
	}
};

class Boss : public Enemy {

};
class Minion : public Enemy {

};


class Bullet {
public:
	CircleShape shape;
	Vector2f currVelocity;
	float maxSpeed;

	Bullet(float radius = 5.f)
		: currVelocity(0.f, 0.f), maxSpeed(15.f) {
		this->shape.setRadius(radius);
		this->shape.setFillColor(Color::Red);
	}

};

class Laser {
public:
	RectangleShape shape;
	float fading;
	int alpha;

	Laser(float width = 5.f)
		: fading(0.05f) {
		this->alpha = 255;
		this->shape.setFillColor(Color(77, 204, 74));
	}

	void fadeLaser() {
		alpha -= 10;
		this->shape.setFillColor(Color(77, 204, 74, alpha));
	}
};

class Bomb {
public:
	CircleShape shape;
	Vector2f currVelocity;
	float maxSpeed;
	float radius;

	Bomb()
		: currVelocity(0.f, 0.f), maxSpeed(5.f), radius(10.f) {
		this->shape.setRadius(radius);
		this->shape.setFillColor(Color::Yellow);
	}
};

class Cannon {

};

class Castle {

};

int main() {
	sf::RenderWindow window(VideoMode(1000.f, 750.f), "Castle Defense", Style::Default);
	window.setFramerateLimit(60);

	Bullet b;
	Bomb bomb;
	Laser laser;
	Enemy enemy;
	std::vector<Bullet> bullets;
	std::vector<Enemy> enemies;
	std::vector<Laser> lasers;
	std::vector<Bomb> bombs;

	// Logic

	Vector2f cannonHead;
	Vector2f mousePosWindow;
	Vector2f aimDir;
	Vector2f aimDirNorm;

	int spawnCounter = 40;
	int gun_type = 0;

	//Textures

	Texture enemy_texture;
	Texture castle_texture;
	Texture ground_texture;
	Texture background_texture;
	Texture cannon_texture;

	//ground_texture.setRepeated(true);

	if (!enemy_texture.loadFromFile("cat.png")) {
		std::cout << "Enemy texture was not loaded!" << std::endl;
	}
	if (!castle_texture.loadFromFile("castle.PNG")) {
		std::cout << "Castle texture was not loaded!" << std::endl;
	}
	if (!ground_texture.loadFromFile("ground.png")) {
		std::cout << "Ground texture was not loaded!" << std::endl;
	}
	if (!background_texture.loadFromFile("platformer_sky_background_by_nathan_jahromi-d92pf1o.png")) {
		std::cout << "Background texture was not loaded!" << std::endl;
	}
	if (!cannon_texture.loadFromFile("cannon.png")) {
		std::cout << "Cannon texture was not loaded!" << std::endl;
	}
	
	
	// Draw Objects
	RectangleShape background(Vector2f(window.getSize().x,window.getSize().y));
	background.setPosition(Vector2f(0,0));
	background.setTexture(&background_texture);

	RectangleShape ground(Vector2f(1000.f, 25.f));
	ground.setPosition(Vector2f(0.f, window.getSize().y - ground.getSize().y));
	ground.setFillColor(Color::White);
	ground.setTexture(&ground_texture);
	

	RectangleShape castle(Vector2f(200.f, 200.f));
	castle.setPosition(0.f, window.getSize().y - ground.getSize().y - castle.getSize().y);
	//castle.setFillColor(Color::White);
	castle.setTexture(&castle_texture);

	RectangleShape cannon(Vector2f(100.f, 40.f));
	cannon.setOrigin(Vector2f(0, cannon.getSize().y / 2));
	cannon.setPosition(Vector2f(castle.getSize().x - 100, window.getSize().y - ground.getSize().y - castle.getSize().y));
	cannon.setFillColor(Color::White);
	cannon.setTexture(&cannon_texture);


	
	

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {

			if (event.type == Event::Closed) {
				window.close();
			}

			if (event.type == Event::MouseWheelMoved) {
				//	std::cout << "delta:" << event.mouseWheel.delta << std::endl;
				gun_type = gun_type + event.mouseWheel.delta;
				gun_type %= 3;
				if (gun_type < 0) {
					gun_type = 2;
				}
				std::cout << "gun: " << gun_type << std::endl;
			}

		}

		//Update




		mousePosWindow = Vector2f(Mouse::getPosition(window));

		// Cannon rotation
		float dx = cannon.getPosition().x - mousePosWindow.x;
		//std::cout << "dx: " << dx << std::endl;
		float dy = cannon.getPosition().y - mousePosWindow.y;
		float rotation = (atan2(dy, dx)) * 180 / PI;
		cannon.setRotation(rotation + 180);

		cannonHead = cannon.getTransform().transformPoint(Vector2f(cannon.getSize().x, cannon.getSize().y / 2));


		//Cannon head coordinates
		//std::cout << "x: " << cannonHead.x << std::endl;
		//std::cout << "y: " << cannonHead.y  << std::endl;

		aimDir = mousePosWindow - cannonHead;
		aimDirNorm = aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));


		// Enemy spawn

		if (spawnCounter < 40) {
			spawnCounter++;
		}
		else {
			enemy.shape.setPosition(Vector2f(window.getSize().x, ground.getPosition().y - enemy.shape.getSize().y));
			enemy.shape.setTexture(&enemy_texture);
			enemy.dir = Vector2f(-(rand() % 5), 0.f);
			enemies.push_back(Enemy(enemy));
			spawnCounter = 0;
		}

		//Shooting

		// Machine Gun
		if (gun_type == 0 && Mouse::isButtonPressed(Mouse::Left)) {

			b.shape.setPosition(cannonHead);
			b.currVelocity = aimDirNorm * b.maxSpeed;
			bullets.push_back(Bullet(b));
		}

		// Bomb
		if (gun_type == 1 && Mouse::isButtonPressed(Mouse::Left)) {
			bomb.shape.setPosition(cannonHead);
			bomb.currVelocity = aimDirNorm * b.maxSpeed;
			bombs.push_back(Bomb(bomb));
		}

		// Laser
		if (gun_type == 2 && Mouse::isButtonPressed(Mouse::Left)) {
			laser.shape.setPosition(cannonHead);
			laser.shape.setRotation(cannon.getRotation());

			float length = 0.f;
			float param = 90 - abs(abs(rotation) - 90);
			std::cout << param << std::endl;

			float b = (window.getSize().x - cannonHead.x) / cos(param* PI / 180);
			std::cout << b << std::endl;

			if (rotation + 180 >= 180) {
				length = sqrt(pow(window.getSize().x - cannonHead.x, 2) + pow(cannonHead.y, 2));
			}
			else {
				//std::cout << window.getSize().x - cannonHead.x << " " << window.getSize().y - ground.getSize().y - cannonHead.y << std::endl;
				length = sqrt(pow(window.getSize().x - cannonHead.x, 2) + pow(window.getSize().y - ground.getSize().y - cannonHead.y, 2));
			}

			//	std::cout << "rotation: " << rotation + 180 << std::endl;
			//	std::cout << "length: " << length << std::endl;
			//	std::cout << "y: " << cannonHead.y << std::endl;
			laser.shape.setSize(Vector2f(length, cannon.getSize().y / 3));


			lasers.push_back(Laser(laser));
		}



		for (size_t i = 0; i < enemies.size(); i++) {
			enemies[i].shape.move(enemies[i].dir);
			if (enemies[i].shape.getPosition().x < castle.getSize().x) {
				enemies.erase(enemies.begin() + i);
			}
		}

		for (size_t i = 0; i < bombs.size(); i++) {
			bombs[i].currVelocity.y += 0.5f;
			bombs[i].shape.move(bombs[i].currVelocity);
			//bombs[i].radius +=1.5f;
			//bombs[i].shape.setRadius(bombs[i].radius);

			if (bombs[i].shape.getPosition().x < 0 || bombs[i].shape.getPosition().x > window.getSize().x
				|| bombs[i].shape.getPosition().y < 0 ||
				bombs[i].shape.getPosition().y > window.getSize().y - ground.getSize().y) {

				bombs.erase(bombs.begin() + i);
			}
			else {
				for (size_t j = 0; j < enemies.size(); j++) {
					if (bombs[i].shape.getGlobalBounds().intersects(enemies[j].shape.getGlobalBounds())) {
						bombs.erase(bombs.begin() + i);
						enemies.erase(enemies.begin() + j);
					}
				}
			}
		}

		for (size_t i = 0; i < lasers.size(); i++) {
			lasers[i].fadeLaser();

			for (size_t j = 0; j < enemies.size(); j++) {
				if (lasers[i].shape.getGlobalBounds().intersects(enemies[j].shape.getGlobalBounds())) {
					std::cout << lasers[i].shape.getGlobalBounds().left + lasers[i].shape.getGlobalBounds().width << std::endl;
					enemies.erase(enemies.begin() + j);
				}
			}


			if (lasers[i].alpha <= 0) {
				lasers.erase(lasers.begin() + i);
			}


		}


		for (size_t i = 0; i < bullets.size(); i++) {
			bullets[i].shape.move(bullets[i].currVelocity);

			if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > window.getSize().x
				|| bullets[i].shape.getPosition().y < 0 ||
				bullets[i].shape.getPosition().y > window.getSize().y - 25.f) {

				bullets.erase(bullets.begin() + i);
			}
			else {
				for (size_t j = 0; j < enemies.size(); j++) {
					if (bullets[i].shape.getGlobalBounds().intersects(enemies[j].shape.getGlobalBounds())) {
						bullets.erase(bullets.begin() + i);
						enemies.erase(enemies.begin() + j);
					}
				}
			}
		}





		//Draw

		window.clear();
		window.draw(background);
		window.draw(ground);
		window.draw(castle);
		window.draw(cannon);

		for (size_t i = 0; i < bombs.size(); i++) {
			window.draw(bombs[i].shape);
		}

		for (size_t i = 0; i < lasers.size(); i++) {
			window.draw(lasers[i].shape);
		}

		for (size_t i = 0; i < bullets.size(); i++) {
			window.draw(bullets[i].shape);
		}

		for (size_t i = 0; i < enemies.size(); i++) {
			window.draw(enemies[i].shape);
		}

		window.display();
	}
	return 0;
}