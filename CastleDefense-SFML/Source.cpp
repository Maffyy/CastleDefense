#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <math.h>
#include <vector>

using namespace sf;

const float PI = 3.14159265;

class Castle {
public:
	RectangleShape shape;
	int hp;
	Castle(float width = 200.f, float height = 200.f) {
		this->hp = 100;
		this->shape.setSize(Vector2f(width, height));
	}
};

struct GameState;
class Bullet;
class Laser;
class Bomb;

template<class Elem>
void RemoveFrom(std::vector<Elem>& list, int i) {
	if (!list.empty()) {
		list[i] = list.back();
		list.pop_back();
	}
}

class Enemy {
public:
	RectangleShape shape;
	Vector2f dir;
	int hp;
	float maxSpeed;
	bool collision;

	Enemy(float width = 100.f, float height = 100.f)
		: maxSpeed(5.f), hp(100) {
		this->shape.setSize(Vector2f(width, height));
		this->shape.setFillColor(Color(255, 255, 255));
		this->collision = false;
	}

	void UpdateMove(Castle& castle, RenderWindow& window, Text& castle_hp) {
		shape.move(dir);
		if (shape.getPosition().x < castle.shape.getSize().x) {
			castle.hp -= 5;
			collision = true;
		}
		castle_hp.setString("Castle health: " + std::to_string(castle.hp));

		if (castle.hp <= 0) {
			window.close();
		}
	}

};
class Boss : public Enemy {
public:
	Boss(float width = 110.f, float height = 200.f) {
		this->maxSpeed = 2.f;
		this->hp = 300;
		this->shape.setSize(Vector2f(width, height));
		this->shape.setFillColor(Color(255, 0, 0));
		this->collision = false;
	}
};
class Sprinter : public Enemy {
public:
	Sprinter(float width = 100.f, float height = 50.f) {
		this->hp = 10;
		this->maxSpeed = 10.f;
		this->shape.setSize(Vector2f(width, height));
		this->shape.setFillColor(Color(0, 255, 0));
		this->collision = false;
	}
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

	void Fade() {
		alpha -= 10;
		this->shape.setFillColor(Color(77, 204, 74, alpha));
	}
	void CollisionWithEnemy(std::vector<Enemy>& enemies, int& killed, Sound& sound) {

		for (size_t i = 0; i < enemies.size(); i++) {
			if (shape.getGlobalBounds().intersects(enemies[i].shape.getGlobalBounds())) {
				enemies[i].hp -= 100;
				if (enemies[i].hp <= 0) {
					RemoveFrom(enemies, i);
					killed++;
					sound.play();
				}

			}
		}
	}

};

class Bomb {
public:
	CircleShape shape;
	Vector2f currVelocity;
	float maxSpeed;
	float radius;

	Bomb()
		: currVelocity(0.f, 0.f), maxSpeed(12.f), radius(10.f) {
		this->shape.setRadius(radius);
		this->shape.setFillColor(Color::Yellow);
	}
};

class Cannon {
public:
	Vector2f cannonHead;
	Vector2f aimDir;
	Vector2f aimDirNorm;
	Vector2f mousePosWindow;
	RectangleShape shape;
	float rotation;

	void Rotation() {
		float dx = shape.getPosition().x - mousePosWindow.x;
		float dy = shape.getPosition().y - mousePosWindow.y;
		float rotation = (atan2(dy, dx)) * 180 / PI;
		shape.setRotation(rotation + 180);

		cannonHead = shape.getTransform().transformPoint(Vector2f(shape.getSize().x, shape.getSize().y / 2));

		aimDir = mousePosWindow - cannonHead;
		aimDirNorm = aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
	}
};

struct GameState {

	float windowWidth;
	float windowHeight;

	SoundBuffer soundbuffer;
	Sound sound;

	Castle castle;
	Cannon cannon;

	std::vector<Bullet> bullets;
	std::vector<Enemy> enemies;
	std::vector<Laser> lasers;
	std::vector<Bomb> bombs;

	int bullet_counter;
	int bomb_counter;
	bool laser_fired;

	int bullet_reload;
	int bomb_reload;
	int laser_reload;

	int spawnCounter;
	int gun_type;
	int killed;

	Font font;
	Text chosen_gun;
	Text killed_enemies;
	Text castle_hp;

	Texture enemy_texture;
	Texture castle_texture;
	Texture ground_texture;
	Texture background_texture;
	Texture cannon_texture;

	RectangleShape background;
	RectangleShape ground;

};

void Init(GameState& gameState, RenderWindow& window) {

	gameState.windowWidth = 1000.f;
	gameState.windowHeight = 750.f;

	gameState.bullet_counter = 0;
	gameState.bomb_counter = 0;
	gameState.laser_fired = false;

	gameState.bullet_reload = 0;
	gameState.bomb_reload = 0;
	gameState.laser_reload = 0;

	gameState.spawnCounter = 40;
	gameState.gun_type = 0;
	gameState.killed = 0;

	if (!gameState.soundbuffer.loadFromFile("meow.wav")) {
		std::cout << "Sound has not been loaded!" << std::endl;
	}

	gameState.sound.setBuffer(gameState.soundbuffer);
	gameState.sound.setVolume(2.f);

	if (!gameState.font.loadFromFile("arial.ttf")) {
		std::cout << "Font has not been loaded!" << std::endl;
	}

	gameState.chosen_gun.setFont(gameState.font);
	gameState.chosen_gun.setString("Weapon: ");
	gameState.chosen_gun.setCharacterSize(20);
	gameState.chosen_gun.setPosition(Vector2f(770, 20));
	gameState.chosen_gun.setStyle(Text::Bold);

	gameState.killed_enemies.setFont(gameState.font);
	gameState.killed_enemies.setString("Score: ");
	gameState.killed_enemies.setCharacterSize(20);
	gameState.killed_enemies.setPosition(Vector2f(600, 20));
	gameState.killed_enemies.setStyle(Text::Bold);

	gameState.castle_hp.setFont(gameState.font);
	gameState.castle_hp.setString("Castle health: " + gameState.castle.hp);
	gameState.castle_hp.setCharacterSize(20);
	gameState.castle_hp.setPosition(20, 20);
	gameState.castle_hp.setStyle(Text::Bold);

	if (!gameState.enemy_texture.loadFromFile("cat.png")) {
		std::cout << "Enemy texture was not loaded!" << std::endl;
	}
	if (!gameState.castle_texture.loadFromFile("castle.PNG")) {
		std::cout << "Castle texture was not loaded!" << std::endl;
	}
	if (!gameState.ground_texture.loadFromFile("ground.png")) {
		std::cout << "Ground texture was not loaded!" << std::endl;
	}
	if (!gameState.background_texture.loadFromFile("platformer_sky_background_by_nathan_jahromi-d92pf1o.png")) {
		std::cout << "Background texture was not loaded!" << std::endl;
	}
	if (!gameState.cannon_texture.loadFromFile("cannon.png")) {
		std::cout << "Cannon texture was not loaded!" << std::endl;
	}

	gameState.background.setSize(Vector2f(window.getSize().x, window.getSize().y));
	gameState.background.setPosition(Vector2f(0, 0));
	gameState.background.setTexture(&gameState.background_texture);

	gameState.ground.setSize(Vector2f(1000.f, 25.f));
	gameState.ground.setPosition(Vector2f(0.f, gameState.windowHeight - gameState.ground.getSize().y));
	gameState.ground.setFillColor(Color::White);
	gameState.ground.setTexture(&gameState.ground_texture);

	gameState.castle.shape.setPosition(0.f, gameState.windowHeight - gameState.ground.getSize().y - gameState.castle.shape.getSize().y);
	gameState.castle.shape.setTexture(&gameState.castle_texture);


	gameState.cannon.shape.setSize(Vector2f(100.f, 40.f));
	gameState.cannon.shape.setOrigin(Vector2f(0, gameState.cannon.shape.getSize().y / 2));
	gameState.cannon.shape.setPosition(Vector2f(gameState.castle.shape.getSize().x - 100, window.getSize().y - gameState.ground.getSize().y - gameState.castle.shape.getSize().y));
	gameState.cannon.shape.setFillColor(Color::White);
	gameState.cannon.shape.setTexture(&gameState.cannon_texture);

}

void SwitchGun(int& gun_type, Text& chosen_gun) {
	switch (gun_type) {
	case 0:
		chosen_gun.setString("Weapon: Machine Gun");
		break;
	case 1:
		chosen_gun.setString("Weapon: Cannon");
		break;
	case 2:
		chosen_gun.setString("Weapon: Laser");
		break;
	default:
		break;
	}
}

void EnemySpawn(int& spawnCounter, Enemy& enemy, std::vector<Enemy>& enemies,
	float windowWidth, RectangleShape& ground, Texture& enemy_texture) {

	if (spawnCounter < 40) {
		spawnCounter++;
	}
	else {
		int choose_enemy = rand() % 3;
		if (choose_enemy == 0) {
			std::cout << "Enemy" << std::endl;
			Enemy enemy;
			enemy.shape.setPosition(Vector2f(windowWidth, ground.getPosition().y - enemy.shape.getSize().y));
			enemy.shape.setTexture(&enemy_texture);
			int speed = enemy.maxSpeed;
			enemy.dir = Vector2f(-(rand() % speed), 0.f);
			enemies.push_back(Enemy(enemy));
			spawnCounter = 0;
		}
		if (choose_enemy == 1) {
			std::cout << "Boss" << std::endl;
			Boss boss;
			boss.shape.setPosition(Vector2f(windowWidth, ground.getPosition().y - boss.shape.getSize().y));
			boss.shape.setTexture(&enemy_texture);
			boss.dir = Vector2f(-boss.maxSpeed, 0.f);
			enemies.push_back(Boss(boss));
			spawnCounter = 0;
		}
		if (choose_enemy == 2) {
			std::cout << "Sprinter" << std::endl;
			Sprinter sprinter;
			sprinter.shape.setPosition(Vector2f(windowWidth, ground.getPosition().y - sprinter.shape.getSize().y));
			sprinter.shape.setTexture(&enemy_texture);
			sprinter.dir = Vector2f(-sprinter.maxSpeed, 0.f);
			enemies.push_back(Sprinter(sprinter));
			spawnCounter = 0;
		}
	}
}


void ShootLaser(std::vector<Laser>& lasers, bool& laser_fired, int& laser_reload, Cannon& cannon,
	float windowWidth, float windowHeight, RectangleShape& ground) {

	if (!laser_fired) {
		Laser laser;
		laser.shape.setPosition(cannon.cannonHead);
		laser.shape.setRotation(cannon.shape.getRotation());

		float length = 0.f;
		float param = 90 - abs(abs(cannon.rotation) - 90);

		float b = (windowWidth - cannon.cannonHead.x) / cos(param* PI / 180);

		if (cannon.rotation + 180 >= 180) {
			length = sqrt(pow(windowWidth - cannon.cannonHead.x, 2) + pow(cannon.cannonHead.y, 2));
		}
		else {
			length = sqrt(pow(windowWidth - cannon.cannonHead.x, 2) + pow(windowHeight - ground.getSize().y - cannon.cannonHead.y, 2));
		}

		laser.shape.setSize(Vector2f(length, cannon.shape.getSize().y / 3));
		lasers.push_back(Laser(laser));
		laser_fired = true;
	}

}

void ShootBomb(std::vector<Bomb>& bombs, int& bomb_counter, int& bomb_reload, Cannon& cannon) {
	if (bomb_counter != 60) {
		Bomb bomb;
		bomb.shape.setPosition(cannon.cannonHead);
		bomb.currVelocity = cannon.aimDirNorm * bomb.maxSpeed;
		bombs.push_back(Bomb(bomb));
		bomb_counter++;
	}

}

void ShootBullet(std::vector<Bullet>& bullets, int& bullet_counter, int& bullet_reload, Cannon& cannon) {
	if (bullet_counter != 150) {
		Bullet bullet;
		bullet_counter++;
		bullet.shape.setPosition(cannon.cannonHead);
		bullet.currVelocity = cannon.aimDirNorm * bullet.maxSpeed;
		bullets.push_back(Bullet(bullet));
	}
}

void Shoot(GameState& gameState) {

	switch (gameState.gun_type) {
	case 0:
		ShootBullet(gameState.bullets, gameState.bullet_counter, gameState.bomb_reload, gameState.cannon);
		break;
	case 1:
		ShootBomb(gameState.bombs, gameState.bomb_counter, gameState.bomb_reload, gameState.cannon);
		break;
	case 2:
		ShootLaser(gameState.lasers, gameState.laser_fired, gameState.laser_reload, gameState.cannon, gameState.windowWidth,
			gameState.windowHeight, gameState.ground);
		break;
	default:
		break;
	}

	if (gameState.laser_fired) {
		gameState.laser_reload++;
	}
	if (gameState.laser_reload == 100) {
		gameState.laser_reload = 0;
		gameState.laser_fired = false;
	}
	if (gameState.bomb_counter == 60) {
		gameState.bomb_reload++;
	}
	if (gameState.bomb_reload == 150) {
		gameState.bomb_reload = 0;
		gameState.bomb_counter = 0;
	}
	if (gameState.bullet_counter == 150) {
		gameState.bullet_reload++;
	}
	if (gameState.bullet_reload == 300) {
		gameState.bullet_counter = 0;
		gameState.bullet_reload = 0;
	}
}

void DetectBulletCollisions(std::vector<Enemy>& enemies, std::vector<Bullet>& bullets,
	RenderWindow& window, int& killed, Sound& sound) {

	for (size_t i = 0; i < bullets.size(); i++) {
		bullets[i].shape.move(bullets[i].currVelocity);

		if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > window.getSize().x
			|| bullets[i].shape.getPosition().y < 0 ||
			bullets[i].shape.getPosition().y > window.getSize().y - 25.f) {

			RemoveFrom(bullets, i);
		}
		else {
			for (size_t j = 0; j < enemies.size(); j++) {
				if (bullets[i].shape.getGlobalBounds().intersects(enemies[j].shape.getGlobalBounds())) {
					RemoveFrom(bullets, i);
					enemies[j].hp -= 20;
					if (enemies[j].hp <= 0) {
						RemoveFrom(enemies, j);
						killed++;
						sound.play();
						break;
					}

				}
			}
		}
	}
}

void DetectLaserCollisions(std::vector<Enemy>& enemies, std::vector<Laser>& lasers, Sound& sound,
	int& killed) {
	for (size_t i = 0; i < lasers.size(); i++) {
		lasers[i].Fade();
		lasers[i].CollisionWithEnemy(enemies, killed, sound);
		if (lasers[i].alpha <= 0) {
			RemoveFrom(lasers, i);
		}
	}
}

void DetectBombCollisions(std::vector<Enemy>& enemies, std::vector<Bomb>& bombs, int& killed,
	Sound& sound, RenderWindow& window, RectangleShape& ground) {

	for (size_t i = 0; i < bombs.size(); i++) {
		bombs[i].currVelocity.y += 0.2f;
		bombs[i].shape.move(bombs[i].currVelocity);

		if (bombs[i].shape.getPosition().x < 0 || bombs[i].shape.getPosition().x > window.getSize().x
			|| bombs[i].shape.getPosition().y < 0 ||
			bombs[i].shape.getPosition().y > window.getSize().y - ground.getSize().y) {

			RemoveFrom(bombs, i);
		}
		else {
			for (size_t j = 0; j < enemies.size(); j++) {
				if (bombs[i].shape.getGlobalBounds().intersects(enemies[j].shape.getGlobalBounds())) {
					RemoveFrom(bombs, i);
					enemies[j].hp -= 40;
					if (enemies[j].hp <= 0) {
						RemoveFrom(enemies, j);
						killed++;
						sound.play();
					}
				}
			}
		}
	}
}

void Update(GameState& gameState, RenderWindow& window) {

	SwitchGun(gameState.gun_type, gameState.chosen_gun);

	// Cannon rotation
	gameState.cannon.mousePosWindow = Vector2f(Mouse::getPosition(window));
	gameState.cannon.Rotation();


	// Enemy spawn

	Enemy enemy;
	EnemySpawn(gameState.spawnCounter, enemy, gameState.enemies, gameState.windowWidth,
		gameState.ground, gameState.enemy_texture);

	//Shooting
	if (Mouse::isButtonPressed(Mouse::Left)) {
		Shoot(gameState);
	}


	// Enemy collision

	for (size_t i = 0; i < gameState.enemies.size(); i++) {
		gameState.enemies[i].UpdateMove(gameState.castle, window, gameState.castle_hp);
		if (gameState.enemies[i].collision == true) {
			RemoveFrom(gameState.enemies, i);
		}
	}
	DetectBulletCollisions(gameState.enemies, gameState.bullets, window, gameState.killed, gameState.sound);
	DetectLaserCollisions(gameState.enemies, gameState.lasers, gameState.sound, gameState.killed);
	DetectBombCollisions(gameState.enemies, gameState.bombs, gameState.killed, gameState.sound, window, gameState.ground);


	gameState.killed_enemies.setString("Score: " + std::to_string(gameState.killed));

}
void Draw(GameState& gameState, RenderWindow& window) {

	window.clear();
	window.draw(gameState.background);
	window.draw(gameState.ground);
	window.draw(gameState.castle.shape);
	window.draw(gameState.cannon.shape);

	for (size_t i = 0; i < gameState.bombs.size(); i++) {
		window.draw(gameState.bombs[i].shape);
	}

	for (size_t i = 0; i < gameState.lasers.size(); i++) {
		window.draw(gameState.lasers[i].shape);
	}

	for (size_t i = 0; i < gameState.bullets.size(); i++) {
		window.draw(gameState.bullets[i].shape);
	}

	for (size_t i = 0; i < gameState.enemies.size(); i++) {
		window.draw(gameState.enemies[i].shape);
	}

	window.draw(gameState.killed_enemies);
	window.draw(gameState.castle_hp);
	window.draw(gameState.chosen_gun);

}

int main() {

	GameState gameState;

	sf::RenderWindow window(VideoMode(1000.f, 750.f), "Castle Defense", Style::Default);
	window.setFramerateLimit(60);

	//Init
	Init(gameState, window);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {

			if (event.type == Event::Closed) {
				window.close();
			}

			if (event.type == Event::MouseWheelMoved) {
				gameState.gun_type = gameState.gun_type + event.mouseWheel.delta;
				gameState.gun_type %= 3;
				if (gameState.gun_type < 0) {
					gameState.gun_type = 2;
				}
			}
		}
		//Update
		Update(gameState, window);
		//Draw
		Draw(gameState, window);

		window.display();
	}
	return 0;
}