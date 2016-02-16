#include <SFML/Graphics.hpp>
#include <iostream> 
#include <windows.h>


using namespace sf;
using namespace std;

static const int WINDOW_WIDTH = 600;
static const int WINDOW_HEIGHT = 800;

void init_background(RectangleShape &background) {
	background.setSize(sf::Vector2f(WINDOW_HEIGHT, WINDOW_WIDTH));
	background.setPosition(0, 0);
}

void set_properties(ConvexShape &convex) {
	convex.setOrigin(130, 50);
	convex.setPosition(490, 100);
	convex.setFillColor(Color::Blue);
}

void init_pendulum(ConvexShape &anker, ConvexShape &leftPallet, ConvexShape &rightPallet, ConvexShape &weight){
	anker.setPointCount(3);
	anker.setPoint(0, sf::Vector2f(130, 50));
	anker.setPoint(1, sf::Vector2f(230, 110));
	anker.setPoint(2, sf::Vector2f(30, 110));
	set_properties(anker);

	leftPallet.setPointCount(3);
	leftPallet.setPoint(0, sf::Vector2f(50, 110));
	leftPallet.setPoint(1, sf::Vector2f(30, 150));
	leftPallet.setPoint(2, sf::Vector2f(30, 110));
	set_properties(leftPallet);

	rightPallet.setPointCount(3);
	rightPallet.setPoint(0, sf::Vector2f(230, 110));
	rightPallet.setPoint(1, sf::Vector2f(230, 150));
	rightPallet.setPoint(2, sf::Vector2f(210, 110));
	set_properties(rightPallet);

	weight.setPointCount(6);
	weight.setPoint(0, sf::Vector2f(130, 50));
	weight.setPoint(1, sf::Vector2f(130, 310));
	weight.setPoint(2, sf::Vector2f(150, 330));
	weight.setPoint(3, sf::Vector2f(130, 350));
	weight.setPoint(4, sf::Vector2f(110, 330));
	weight.setPoint(5, sf::Vector2f(125, 310));
	set_properties(weight);
}

void init_gears(Sprite &blackGear, Sprite &blueGear) {
	blackGear.setPosition(150, 150);
	blackGear.setOrigin(150, 150);
	blueGear.setPosition(327, 295);
	blueGear.setOrigin(150, 150);
}

void pendulum_rotation(ConvexShape &anker, ConvexShape &leftPallet, ConvexShape &rightPallet, ConvexShape &weight, float rotation){
	anker.setRotation(rotation);
	leftPallet.setRotation(rotation);
	rightPallet.setRotation(rotation);
	weight.setRotation(rotation);
}

void calculation_speed(float &speed, float &acceleration, float &rotation, float &rotGear, bool &downhill) {
	if (int(speed * 1000) == 0) {
		speed = acceleration * 2;
		downhill = true;
	}
	if (int(rotation) == 0 && downhill) {
		if (speed > 0)
			rotGear -= acceleration;
		speed += acceleration * 8;
		acceleration = -acceleration;
		downhill = false;
	}
	rotGear += acceleration * 100;
	speed += acceleration;
	rotation += speed;
}

void start_move_time(RenderWindow &window, Sprite blackGear, Sprite blueGear) {
	float rotation = 30, rotGear = 0;
	float speed = -0.001;
	float acceleration = -0.001;
	bool downhill = true;

	init_gears(blackGear, blueGear);
	RectangleShape background;
	init_background(background);

	ConvexShape anker;
	ConvexShape leftPallet;
	ConvexShape rightPallet;
	ConvexShape weight;
	
	init_pendulum(anker, leftPallet, rightPallet, weight);

	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		blackGear.setRotation(-rotGear);
		blueGear.setRotation(rotGear);

		pendulum_rotation(anker, leftPallet, rightPallet, weight, rotation);
		calculation_speed(speed, acceleration, rotation, rotGear, downhill);

		window.clear();
		window.draw(background);
		window.draw(anker);
		window.draw(leftPallet);
		window.draw(rightPallet);
		window.draw(weight);
		window.draw(blackGear);
		window.draw(blueGear);
		window.display();
	}
}

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	Texture blackGearImage, blueGearImage;
	Sprite blackGear, blueGear;
	blackGearImage.loadFromFile("black_gear.png");
	blackGear.setTexture(blackGearImage);
	blueGearImage.loadFromFile("blue_gear.png");
	blueGear.setTexture(blueGearImage);

	sf::RenderWindow window(sf::VideoMode(WINDOW_HEIGHT, WINDOW_WIDTH), "Top Gear", sf::Style::Default, settings);
	start_move_time(window, blackGear, blueGear);
	return 0;
}