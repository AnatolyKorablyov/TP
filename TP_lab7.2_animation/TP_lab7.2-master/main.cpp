#include <SFML/Graphics.hpp>
#include <iostream> 
#include <windows.h>
#include <vector>


using namespace sf;
using namespace std;

static const int WINDOW_WIDTH = 600;
static const int WINDOW_HEIGHT = 800;
static const int HEIGHT = 50;
static const int DEPTH = 20;

void first_move(int &numMove, float &x, float time, RectangleShape &rect) {
	x += float(0.1 * time);
	if (x >= WINDOW_HEIGHT - 30) {
		rect.setFillColor(sf::Color::Green);
		numMove = 1;
	}
}

void second_move(int &numMove, float &x, float &y, float &size, float time, RectangleShape &rect) {
	if (x > WINDOW_HEIGHT / 2)
		size += float(0.001*time);
	else
		size -= float(0.001 * time);
	x -= float(0.12 * time);
	y += float(0.05 * time);
	if (x < 30) {
		rect.setFillColor(sf::Color::Blue);
		numMove = 2;
	}
}

void third_move(float &rotation, float &x, float &time, int count, int &numMove, RectangleShape &rect) {
	if (count % 2 == 0)
		rotation -= float(0.1 * time);
	else
		rotation += float(0.1 * time);
	x += float(0.1 * time);
	if (x >= WINDOW_HEIGHT / 2) {
		rect.setFillColor(sf::Color::Cyan);
		numMove = 3;
	}
}

void fourth_move(float &rotation, float &x, float &y, float &time, int count, int &numMove, RectangleShape &rect) {
	if (count % 2 == 0) {
		x += float(0.1 * time);
		rotation -= 0.1 * time;
	}
	else {
		x -= float(0.1 * time);
		rotation += 0.1 * time;
	}
	y -= float(0.05 * time);
	if (int(rotation) == 720 || int(rotation) == -720) {
		rotation = 0;
		rect.setFillColor(sf::Color::Magenta);
		numMove = 4;
	}
}

void fifth_move(int count, float &x, float &y, int &numMove, float time) {
	if (count % 2 == 0) {
		x -= float(0.1 * time);
		if (int(x) == WINDOW_HEIGHT / 2)
			numMove = 5;
	}
	else {
		x += float(0.1 * time);
		if (int(x) == WINDOW_HEIGHT / 2)
			numMove = 5;
	}
	y -= float(0.037 * time);
}

void sixth_move(float &x, int &numMove, float time, RectangleShape &rect) {
	x -= float(0.1 * time);
	if (x < 30) {
		rect.setFillColor(sf::Color::Red);
		numMove = 0;
	}
}

struct init_shapes {
	float x, y;
	float rotation = 0;
	float size = 1;
	int numMove = 0;

	RectangleShape rect;
	init_shapes(float X, float Y, int num){
		rect.setSize(sf::Vector2f(float(HEIGHT), float(DEPTH)));
		rect.setFillColor(sf::Color::Red);
		x = X;
		y = Y;
		rect.setOrigin(float(HEIGHT / 2), float(DEPTH / 2));
	}
	void update(float time, int count){
		if (numMove == 0) {
			first_move(numMove, x, time, rect);
		}
		else if (numMove == 1) {
			second_move(numMove, x, y, size, time, rect);
		}
		else if (numMove == 2) {
			third_move(rotation, x, time, count, numMove, rect);
		}
		else if (numMove == 3) {
			fourth_move(rotation, x, y, time, count, numMove, rect);
		}
		else if (numMove == 4) {
			fifth_move(count, x, y, numMove, time);
		}
		else if (numMove == 5) {
			sixth_move(x, numMove, time, rect);
		}
		rect.setScale(size, size);
		rect.setRotation(rotation);
		rect.setPosition(x, y);
	}
};


void start_animation(RenderWindow &window, const std::vector<init_shapes*> entities) {
	Clock clock;
	RectangleShape background;
	background.setSize(sf::Vector2f(WINDOW_HEIGHT, WINDOW_WIDTH));
	background.setPosition(0, 0);
	while (window.isOpen()) {
		sf::Event event;
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		window.draw(background);
		int count = 0;
		for (auto i : entities){
			i->update(time, count);
			window.draw(i->rect);
			count += 1;
		}
		window.display();
	}
}

int main() {
	std::vector<init_shapes*>  entities;
	float x = 50, y = 0;
	for (int i = 0; i < 5; i++) {
		y += 50;
		entities.push_back(new init_shapes(x, y, i));
	}

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(WINDOW_HEIGHT, WINDOW_WIDTH), "Animation", sf::Style::Default, settings);

	start_animation(window, entities);

	return 0;
}