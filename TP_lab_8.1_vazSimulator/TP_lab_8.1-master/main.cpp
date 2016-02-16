#include <SFML/Graphics.hpp>
#include <iostream> 
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>


using namespace sf;
using namespace std;

static const int WINDOW_WIDTH = 600;
static const int WINDOW_HEIGHT = 1360;
static const int LEFT_WHEEL_X = 143;
static const int LEFT_WHEEL_Y = 230;
static const int RIGHT_WHEEL_X = 394;
static const int RIGHT_WHEEL_Y = 230;
static const int RADIUS_WHEEL = 30;
static const float CIRCUMFERENCE = RADIUS_WHEEL * 2 * M_PI;

struct CarSprites {
	Sprite bodyCar;
	Sprite leftWheel;
	Sprite rightWheel;
	float rotation = 0;
	float speed = 0;
	float acceleration = 0.0005;
	sf::Vector2f coordCar = { 0, 200 };

	void set_sprites_origin() {
		leftWheel.setOrigin(RADIUS_WHEEL, RADIUS_WHEEL);
		rightWheel.setOrigin(RADIUS_WHEEL, RADIUS_WHEEL);
	}

	void set_position_car() {
		leftWheel.setRotation(rotation);
		rightWheel.setRotation(rotation);
		bodyCar.setPosition(coordCar);
		leftWheel.setPosition(coordCar.x + LEFT_WHEEL_X, coordCar.y + LEFT_WHEEL_Y);
		rightWheel.setPosition(coordCar.x + RIGHT_WHEEL_X, coordCar.y + RIGHT_WHEEL_Y);
	}

	void calculation_speed_and_rotation(Event event, float time) {
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Right) {
			speed += acceleration * time;
		}
		else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Left) {
			speed -= acceleration * time;
		}
		else {
			if (speed > 0)
				speed -= (acceleration * time) / 2;
			if (speed < 0)
				speed += (acceleration * time) / 2;
		}
		coordCar.x += speed;
		rotation = coordCar.x * 360 / CIRCUMFERENCE;
		set_position_car();
	}
};

void init_background(RectangleShape &background) {
	background.setSize(sf::Vector2f(WINDOW_HEIGHT, WINDOW_WIDTH));
	background.setPosition(0, 0);
}

void init_asphalt(RectangleShape &asphalt) {
	asphalt.setSize(sf::Vector2f(WINDOW_HEIGHT, WINDOW_WIDTH - LEFT_WHEEL_Y - 200));
	asphalt.setPosition(0, 460);
	asphalt.setFillColor(Color::Color(120, 120, 120, 255));
}



void start_move_time(RenderWindow &window, CarSprites &vaz) {

	Clock clock;
	RectangleShape background;
	init_background(background);

	RectangleShape asphalt;
	init_asphalt(asphalt);

	while (window.isOpen()) {
		sf::Event event;
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 700;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		vaz.calculation_speed_and_rotation(event, time);

		window.clear();
		window.draw(background);
		window.draw(asphalt);
		window.draw(vaz.bodyCar);
		window.draw(vaz.leftWheel);
		window.draw(vaz.rightWheel);
		window.display();
	}
}

int main() {

	Texture bodyCarImage, wheelImage;
	bodyCarImage.loadFromFile("vaz_2107.png");
	wheelImage.loadFromFile("wheel.png");
	
	CarSprites vaz;
	vaz.bodyCar.setTexture(bodyCarImage);
	vaz.leftWheel.setTexture(wheelImage);
	vaz.rightWheel.setTexture(wheelImage);

	vaz.set_sprites_origin();

	sf::RenderWindow window(sf::VideoMode(WINDOW_HEIGHT, WINDOW_WIDTH), "Top Gear");
	start_move_time(window, vaz);
	return 0;
}