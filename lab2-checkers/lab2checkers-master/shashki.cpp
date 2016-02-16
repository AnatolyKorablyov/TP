#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>
using namespace sf;
int num_x, num_y, coor_ceil_x, coor_ceil_y, coord_line, coor;
int address_cell[5], count, number;
int carved_white[13], carved_black[13], count_carv_black, count_carv_white;
bool white_move = true, move = false, king = false, draw;
struct pair{
	int one;
	int two;
};
pair coordinate(int line_x) {
	if (35 <= line_x)
		if (line_x < 123) {
			coord_line = 1;
			coor = 35;
		}
	if (123 <= line_x)
		if (line_x < 211) {
			coord_line = 2;
			coor = 123;
		}
	if (211 <= line_x)
		if (line_x < 299) {
			coord_line = 3;
			coor = 211;
		}
	if (299 <= line_x)
		if (line_x < 387) {
			coord_line = 4;
			coor = 299;
		}
	if (387 <= line_x)
		if (line_x < 475) {
			coord_line = 5;
			coor = 387;
		}
	if (475 <= line_x)
		if (line_x < 563) {
			coord_line = 6;
			coor = 475;
		}
	if (563 <= line_x)
		if (line_x < 651) {
			coord_line = 7;
			coor = 563;
		}
	if (651 <= line_x)
		if (line_x <= 739) {
			coord_line = 8;
			coor = 651;
		}
	pair result;
	result.one = coord_line;
	result.two = coor;
	return result;
}
int main() {
	RenderWindow window(sf::VideoMode(780, 780), "game");
	Clock clock; //создаем переменную времени, т.о. привязка ко времени(а не загруженности/мощности процессора).
	Texture whitetexture, blacktexture, mapstexture;
	whitetexture.loadFromFile("images/white.png");
	blacktexture.loadFromFile("images/black.png");
	mapstexture.loadFromFile("Images/checkers_780.png");
	Sprite mapssprite;
	mapssprite.setTexture(mapstexture);
	mapssprite.setPosition(0, 0);
	int mass[89];
	mass[11] = 2; mass[21] = 5; mass[31] = 2; mass[41] = 5; mass[51] = 2; mass[61] = 5; mass[71] = 2; mass[81] = 5;
	mass[12] = 5; mass[22] = 2; mass[32] = 5; mass[42] = 2; mass[52] = 5; mass[62] = 2; mass[72] = 5; mass[82] = 2;
	mass[13] = 2; mass[23] = 5; mass[33] = 2; mass[43] = 5; mass[53] = 2; mass[63] = 5; mass[73] = 2; mass[83] = 5;
	mass[14] = 5; mass[24] = 0; mass[34] = 5; mass[44] = 0; mass[54] = 5; mass[64] = 0; mass[74] = 5; mass[84] = 0;
	mass[15] = 0; mass[25] = 5; mass[35] = 0; mass[45] = 5; mass[55] = 0; mass[65] = 5; mass[75] = 0; mass[85] = 5;
	mass[16] = 5; mass[26] = 1; mass[36] = 5; mass[46] = 1; mass[56] = 5; mass[66] = 1; mass[76] = 5; mass[86] = 1;
	mass[17] = 1; mass[27] = 5; mass[37] = 1; mass[47] = 5; mass[57] = 1; mass[67] = 5; mass[77] = 1; mass[87] = 5;
	mass[18] = 5; mass[28] = 1; mass[38] = 5; mass[48] = 1; mass[58] = 5; mass[68] = 1; mass[77] = 5; mass[88] = 1;
	Sprite whitesprite[13], blacksprite[13];
	int coord_x = 35, coord_y = 35;
	int dX, dY;
	char num;
	bool isMove, cut;
	for (char i = 0; i < 12; i++) { // цикл расстановки белых шашек
		carved_white[i] = 0;
		carved_black[i] = 0;
		whitesprite[i].setTexture(whitetexture);
		whitesprite[i].setPosition(coord_x, coord_y);
		coord_x += 176;
		if (coord_x == 739) {
			coord_x = 123;
			coord_y += 88;
		}
		if (coord_x == 827) {
			coord_x = 35;
			coord_y += 88;
		}
	}
	coord_x = 123; coord_y = 475;
	for (char i = 0; i < 12; i++) { // цикл расстановки черных шашек
		blacksprite[i].setTexture(blacktexture);
		blacksprite[i].setPosition(coord_x, coord_y);
		coord_x += 176;
		if (coord_x == 739) {
			coord_x = 123;
			coord_y += 88;
		}
		if (coord_x == 827) {
			coord_x = 35;
			coord_y += 88;
		}
	}
	num = -1;
	for (char i = 0; i < 4; i++)
		address_cell[i] = 0;
	white_move = true;
	move = false;
	count_carv_white = 0;
	count_carv_black = 0;
	while (window.isOpen()) {
		Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
		Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed)//если нажата клавиша мыши
				if (event.key.code == Mouse::Left) {//а именно левая
					if (white_move) {
						for (char i = 0; i < 12; i++)
							if (whitesprite[i].getGlobalBounds().contains(pos.x, pos.y))//и при этом координата курсора попадает в спрайт
							{
								coord_x = whitesprite[i].getPosition().x;
								coord_y = whitesprite[i].getPosition().y;
								dX = pos.x - whitesprite[i].getPosition().x;//делаем разность между позицией курсора и спрайта.для корректировки нажатия
								dY = pos.y - whitesprite[i].getPosition().y;//тоже самое по игреку
								isMove = true;//можем двигать спрайт
								num = i;
								move = true;
							}
					}
					else
						for (char i = 0; i < 12; i++)
							if (blacksprite[i].getGlobalBounds().contains(pos.x, pos.y))//и при этом координата курсора попадает в спрайт
							{
								coord_x = blacksprite[i].getPosition().x;
								coord_y = blacksprite[i].getPosition().y;
								dX = pos.x - blacksprite[i].getPosition().x;//делаем разность между позицией курсора и спрайта.для корректировки нажатия
								dY = pos.y - blacksprite[i].getPosition().y;//тоже самое по игреку
								isMove = true;//можем двигать спрайт
								num = i;
								move = true;
							}
					pair res;
					res = coordinate(pos.x);
					num_x = res.one; coor_ceil_x = res.two;
					res = coordinate(pos.y);
					num_y = res.one; coor_ceil_y = res.two;
					count = 0;
					cut = false;
					if (white_move) { //проверка на допустимые ходы
						if (num_y != 8) {
							if (num_x != 8) {
								if (mass[(num_x + 1) * 10 + (num_y + 1)] == 1)
									if (num_y < 7)
										if (num_x < 7)
											if (mass[(num_x + 2) * 10 + (num_y + 2)] == 0) {
												address_cell[count] = (num_x + 2) * 10 + (num_y + 2);
												count += 1;
												cut = true;
											}
								if (cut == false)
									if (mass[(num_x + 1) * 10 + (num_y + 1)] == 0) {
										address_cell[count] = (num_x + 1) * 10 + (num_y + 1);
										count += 1;
									}
							}
							if (num_x != 1) {
								if (mass[(num_x - 1) * 10 + (num_y + 1)] == 1) {
									if (num_y < 7)
										if (num_x > 2)
											if (mass[(num_x - 2) * 10 + (num_y + 2)] == 0) {
												address_cell[count] = (num_x - 2) * 10 + (num_y + 2);
												count += 1;
												cut = true;
											}
								}
								if (cut == false)
									if (mass[(num_x - 1) * 10 + (num_y + 1)] == 0) {
										address_cell[count] = (num_x - 1) * 10 + (num_y + 1);
										count += 1;
									}

							}
						}
					}
					else {
						if (num_y != 1) {
							if (num_x != 8) {
								if (mass[(num_x + 1) * 10 + (num_y - 1)] == 2) {
									if (num_y > 2)
										if (num_x < 7)
											if (mass[(num_x + 2) * 10 + (num_y - 2)] == 0) {
												address_cell[count] = (num_x + 2) * 10 + (num_y - 2);
												count += 1;
												cut = true;
											}
								}
								if (cut == false)
									if (mass[(num_x + 1) * 10 + (num_y - 1)] == 0) {
										address_cell[count] = (num_x + 1) * 10 + (num_y - 1);
										count += 1;
									}
							}
							if (num_x != 1) {
								if (mass[(num_x - 1) * 10 + (num_y - 1)] == 2) {
									if (num_y > 2)
										if (num_x > 2)
											if (mass[(num_x - 2) * 10 + (num_y - 2)] == 0) {
												address_cell[count] = (num_x - 2) * 10 + (num_y - 2);
												count += 1;
												cut = true;
											}
								}
								if (cut == false)
									if (mass[(num_x - 1) * 10 + (num_y - 1)] == 0) {
										address_cell[count] = (num_x - 1) * 10 + (num_y - 1);
										count += 1;
									}
							}
						}
					}
					for (char i = count; (i < 4); i++) {
						address_cell[count] = 0;
					}
				}
			if (move) {
				if (event.type == Event::MouseButtonReleased)//если отпустили клавишу
					if (event.key.code == Mouse::Left) {//а именно левую
						isMove = false;
						move = false;
						//сюда проверку на ячейку
						pair res;
						res = coordinate(pos.x);
						num_x = res.one; coor_ceil_x = res.two;
						res = coordinate(pos.y);
						num_y = res.one; coor_ceil_y = res.two;
						number = num_x * 10 + num_y;
						for (char i = 0; i < 4; i++) {
							if ((num_x * 10 + num_y) == address_cell[i])
								move = true;
						}
						if (num != -1) {
							if (move) {
								if (white_move) {
									whitesprite[num].setPosition(coor_ceil_x, coor_ceil_y);
									white_move = false;
									mass[num_x * 10 + num_y] = 2;
									if (cut)
										for (char i = 0; i < 12; i++)
											if (blacksprite[i].getGlobalBounds().contains(coord_x + ((coor_ceil_x - coord_x) / 2), coord_y + ((coor_ceil_y - coord_y) / 2))) {
												carved_black[count_carv_black] = i;
												count_carv_black += 1;
												res = coordinate(coord_x + ((coor_ceil_x - coord_x) / 2));
												num_x = res.one; coor_ceil_x = res.two;
												res = coordinate(coord_y + ((coor_ceil_y - coord_y) / 2));
												num_y = res.one; coor_ceil_y = res.two;
												mass[num_x * 10 + num_y] = 0;
											}
								}
								else {
									blacksprite[num].setPosition(coor_ceil_x, coor_ceil_y);
									white_move = true;
									mass[num_x * 10 + num_y] = 1;
									if (cut)
										for (char i = 0; i < 12; i++)
											if (whitesprite[i].getGlobalBounds().contains(coord_x + ((coor_ceil_x - coord_x) / 2), coord_y + ((coor_ceil_y - coord_y) / 2))) {
												carved_white[count_carv_white] = i;
												count_carv_white += 1;
												res = coordinate(coord_x + ((coor_ceil_x - coord_x) / 2));
												num_x = res.one; coor_ceil_x = res.two;
												res = coordinate(coord_y + ((coor_ceil_y - coord_y) / 2));
												num_y = res.one; coor_ceil_y = res.two;
												mass[num_x * 10 + num_y] = 0;
											}
								}
								res = coordinate(coord_x);
								num_x = res.one; coor_ceil_x = res.two;
								res = coordinate(coord_y);
								num_y = res.one; coor_ceil_y = res.two;
								mass[num_x * 10 + num_y] = 0;
								number = num_x * 10 + num_y;
								std::cout << "move = " << number;
							}
							else
								if (white_move)
									whitesprite[num].setPosition(coord_x, coord_y);
								else
									blacksprite[num].setPosition(coord_x, coord_y);
						}
						move = false;
					}
				if (isMove) {
					if (white_move) {
						whitesprite[num].setPosition(pos.x - dX, pos.y - dY);
					}
					else {
						blacksprite[num].setPosition(pos.x - dX, pos.y - dY);
					}
				}
			}
		}
		window.clear();
		window.draw(mapssprite);
		for (char i = 0; i < 12; i++) {
			draw = true;
			for (char j = 0; j < count_carv_white; j++) {
				if (carved_white[j] == i)
					draw = false;
			}
			if (draw)
				window.draw(whitesprite[i]);
			draw = true;
			for (char j = 0; j < count_carv_black; j++) {
				if (carved_black[j] == i)
					draw = false;
			}
			if (draw)
				window.draw(blacksprite[i]);
		}
		window.display();
	}
	return 0;
}