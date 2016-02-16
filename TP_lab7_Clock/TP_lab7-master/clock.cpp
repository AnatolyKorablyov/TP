#include <SFML/Graphics.hpp>
#include <iostream> 
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>


using namespace sf;
using namespace std;

const int RADIUS = 200;
const int WINDOW_WIDTH = 2 * RADIUS;
const int START_X = RADIUS, START_Y = RADIUS;
const int WINDOW_HEIGHT = 2 * RADIUS;
const int AMOUNT_OF_POINT = 60;
const int ONE_SECOND = 6; // 360 / 60 = 6` shift one second
const int RAD_DIAL = RADIUS - 30; // the radius of the dial
const String NUM_HOUR_ARRAY[] = { "3","4", "5", "6", "7", "8", "9", "10", "11", "12", "1", "2" };

struct init_shapes {
	sf::CircleShape clockBack;
	sf::RectangleShape arrowH;
	sf::RectangleShape arrowM;
	sf::RectangleShape arrowS;
	sf::RectangleShape dot;
};

void arrow_pos(RectangleShape &arrow, float height, float depth) {
	arrow.setRotation(-90);
	arrow.setSize(sf::Vector2f(float(height), float(depth)));
	arrow.setPosition(START_X, START_Y);
	arrow.setOrigin(0, float(depth / 2));
}

void shapes_position(init_shapes &sh) {
	arrow_pos(sh.arrowH, 100, 10);
	sh.arrowH.setFillColor(sf::Color::Black);
	arrow_pos(sh.arrowM, 150, 6);
	sh.arrowM.setFillColor(sf::Color::Red);
	arrow_pos(sh.arrowS, 170, 3);
	sh.arrowS.setFillColor(sf::Color::Green);
	sh.dot.setSize(sf::Vector2f(2, 2));
	sh.dot.setFillColor(sf::Color::Blue);
	sh.clockBack.setRadius(RADIUS);
	sh.clockBack.setPosition(0, 0);
}

void coord_dots(sf::Vector2f (&coordinatesDots)[AMOUNT_OF_POINT]) {
	sf::Vector2f coorDot;
	for (int i = 0; i < AMOUNT_OF_POINT; i++) {
		coorDot.x = START_X + RAD_DIAL * cos(i * ONE_SECOND * float(M_PI) / 180);
		coorDot.y = START_Y + RAD_DIAL * sin(i * ONE_SECOND * float(M_PI) / 180);
		coordinatesDots[i] = { coorDot.x, coorDot.y };
	}
}

void draw_dots(RenderWindow &window, init_shapes &sh, sf::Vector2f (&coorDots)[AMOUNT_OF_POINT]) {
	for (int i = 0; i < AMOUNT_OF_POINT; i++) {
		if (i % 5 == 0) {
			sh.dot.setSize(sf::Vector2f(4, 4));
			sh.dot.setOrigin(4 / 2, 4 / 2);
		}
		else {
			sh.dot.setSize(sf::Vector2f(2, 2));
			sh.dot.setOrigin(2 / 2, 2 / 2);
		}
		sh.dot.setPosition(coorDots[i].x, coorDots[i].y);
		window.draw(sh.dot);
	}
}

void draw_text(RenderWindow &window, Text &text, sf::Vector2f(&coorDots)[AMOUNT_OF_POINT]) {
	sf::Vector2f coordNum = { -10, 10 };
	for (int i = 0; i < 12; i++) {
		text.setString(NUM_HOUR_ARRAY[i]);
		text.setOrigin(coordNum.x, coordNum.y);
		text.setPosition(coorDots[i * 5].x, coorDots[i * 5].y);
		if ((i < 3) || (i > 9))
			coordNum.y -= 4;
		else
			coordNum.y += 4;
		if (i > 6)
			coordNum.x -= 7;
		else
			coordNum.x += 5;
		window.draw(text);
	}
}

void start_move_time(RenderWindow &window, init_shapes &sh, Text text) {
	sf::Vector2f coordDots[AMOUNT_OF_POINT];
	SYSTEMTIME sysTime;
	coord_dots(coordDots);
	while (window.isOpen()) {
		GetSystemTime(&sysTime);
		sh.arrowS.setRotation(float(sysTime.wSecond * 6 - 90));
		sh.arrowM.setRotation(float(sysTime.wMinute * 6 - 90));
		sh.arrowH.setRotation(float((sysTime.wHour + 3) * 30 - 90));
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		window.draw(sh.clockBack);
		draw_dots(window, sh, coordDots);
		draw_text(window, text, coordDots);
		window.draw(sh.arrowH);
		window.draw(sh.arrowM);
		window.draw(sh.arrowS);
		window.display();
	}
}

int main(){
	init_shapes sh;
	shapes_position(sh);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	
	sf::RenderWindow window(sf::VideoMode(WINDOW_HEIGHT, WINDOW_WIDTH), "Analog clock", sf::Style::Default, settings);
	
	Font font;
	font.loadFromFile("Albionic.ttf");
	Text text("", font, 20);
	text.setColor(Color::Red);

	start_move_time(window, sh, text);
	return 0;
}
