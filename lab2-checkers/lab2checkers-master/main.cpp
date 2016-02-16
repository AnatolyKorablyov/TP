#include <SFML/Graphics.hpp>
#include <sstream>
#include "Config.h"
#include <iostream>

using namespace sf;

struct pair {
	int one;
	int two;
};
struct motion {
	int start_x, start_y, mov_x, mov_y, num_spr;
	bool isMov;
};
struct ret_check {
	bool one;
	int two[5][3];
};
struct inicialization {
	int number_el, bot_move[13][3], moved_bot[24][5], const_pos[3];
	int num_x_start, num_y_start, num_x_end, num_y_end;
	int coor_ceil_x, coor_ceil_y, count, number, dX, dY;
	int mass_cut[6][3], count_bot_move, count_cut, count_res;
};
//отрисовка карты
void arrangement_map(Sprite mapsprite[size][size], Texture & maptexture, float cord_x, float cord_y) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			mapsprite[i][j].setTexture(maptexture);
			mapsprite[i][j].setPosition(cord_x, cord_y);
			cord_x += 88;
			if (cord_x == const_coords[8]) {
				cord_x = const_coords[0];
				cord_y += 88;
			}
		}
	}
}
// функция расстановки шашек
void arrangement_of_checkers(Sprite king_checker[13], Texture & king_texture, Sprite checker[13], Texture & ch_texture, float cord_x, float cord_y) {
	for (int i = 0; i < 12; i++) {
		king_checker[i].setTexture(king_texture);
		king_checker[i].setPosition(border, border);
		checker[i].setTexture(ch_texture);
		checker[i].setPosition(cord_x, cord_y);
		cord_x += length + length;
		if (cord_x == const_coords[size]) {
			cord_x = const_coords[1];;
			cord_y += length;
		}
		if (cord_x == const_coords[size + 1]) {
			cord_x = const_coords[0];
			cord_y += length;
		}
	}
}
//функция определения ячейки
pair coordinate(float line_x) {
	int coord_line = -1;
	int coor = border, count = 0;
	while ((const_coords[count] <= line_x) && (count < size))
		count += 1;
	if (count <= size) {
		coord_line = count - 1;
		coor = const_coords[count - 1];
	}
	pair result;
	result.one = coord_line;
	result.two = coor;
	return result;
}
// определение номера спрайта, и плавное движение мыши
motion move_object(Sprite obje[13], int position_x, int position_y) {
	motion result;
	bool ch_bol = false;
	for (char i = 0; i < 12; i++)
		if (obje[i].getGlobalBounds().contains(position_x, position_y))//и при этом координата курсора попадает в спрайт
		{
			result.start_x = obje[i].getPosition().x;
			result.start_y = obje[i].getPosition().y;
			result.mov_x = position_x - obje[i].getPosition().x;//делаем разность между позицией курсора и спрайта.для корректировки нажатия
			result.mov_y = position_y - obje[i].getPosition().y;//тоже самое по игреку
			result.isMov = true;//можем двигать спрайт
			result.num_spr = i;
			ch_bol = true;
		}
	if (ch_bol == false) {
		result.start_x = -1;
		result.start_y = -1;
		result.mov_x = -1;
		result.mov_y = -1;
		result.isMov = false;
		result.num_spr = -1;
	}
	return result;
}
//проверка безопасного хода
bool check_safe_move(int num_x, int num_y, int chess_board[size][size]) {
	int const_checker[3];
	bool safe = false;
	const_checker[0] = 1;
	const_checker[1] = 3;
	if ((chess_board[num_y - 1][num_x + 1] != const_checker[0]) && (chess_board[num_y - 1][num_x + 1] != const_checker[1]) &&
		(chess_board[num_y - 1][num_x - 1] != const_checker[0]) && (chess_board[num_y - 1][num_x - 1] != const_checker[1]))
		safe = true;
	return safe;
}
ret_check check_move(bool & white_move, bool & queen, int num_x, int num_y, int chess_board[size][size]) {
	int const_checker[3];
	int count = 0;
	int address_move[5][3];
	bool cut = false;
	bool check_up = false, check_down = false;
	if (white_move) {
		const_checker[0] = black_checker;
		const_checker[1] = black_king_checker;
		check_down = true;
	}
	else {
		const_checker[0] = white_checker;
		const_checker[1] = white_king_checker;
		check_up = true;
	}
	if (queen) {
		check_down = true;
		check_up = true;
	}
	if ((num_y != 0) && (num_x != 7) && check_up) {
		if (((chess_board[num_y - 1][num_x + 1] == const_checker[0]) || (chess_board[num_y - 1][num_x + 1] == const_checker[1])) && (num_y > 1) && (num_x < 6)) {
			if (chess_board[num_y - 2][num_x + 2] == 0) {
				if (cut == false)
					count = 0;
				address_move[count][0] = num_y - 2;
				address_move[count][1] = num_x + 2;
				count += 1;
				cut = true;
			}
		}
		if ((chess_board[num_y - 1][num_x + 1] == 0) && (cut == false)) {
			address_move[count][0] = num_y - 1;
			address_move[count][1] = num_x + 1;
			count += 1;
		}
	}
	if ((num_y != 0) && (num_x != 0) && check_up) {
		if (((chess_board[num_y - 1][num_x - 1] == const_checker[0]) || (chess_board[num_y - 1][num_x - 1] == const_checker[1])) && (num_y > 1) && (num_x > 1)) {
			if (chess_board[num_y - 2][num_x - 2] == 0) {
				if (cut == false)
					count = 0;
				address_move[count][0] = num_y - 2;
				address_move[count][1] = num_x - 2;
				count += 1;
				cut = true;
			}
		}
		if ((chess_board[num_y - 1][num_x - 1] == 0) && (cut == false)) {
			address_move[count][0] = num_y - 1;
			address_move[count][1] = num_x - 1;
			count += 1;
		}
	}
	if ((num_y != 7) && (num_x != 7) && check_down) {
		if (((chess_board[num_y + 1][num_x + 1] == const_checker[0]) || (chess_board[num_y + 1][num_x + 1] == const_checker[1])) && (num_y < 6) && (num_x < 6)) {
			if (chess_board[num_y + 2][num_x + 2] == 0) {
				if (cut == false)
					count = 0;
				address_move[count][0] = num_y + 2;
				address_move[count][1] = num_x + 2;
				count += 1;
				cut = true;
			}
		}
		if ((chess_board[num_y + 1][num_x + 1] == 0) && (cut == false)) {
			address_move[count][0] = num_y + 1;
			address_move[count][1] = num_x + 1;


			count += 1;
		}
	}
	if ((num_y != 7) && (num_x != 0) && check_down) {
		if (((chess_board[num_y + 1][num_x - 1] == const_checker[0]) || (chess_board[num_y + 1][num_x - 1] == const_checker[1])) && (num_y < 6) && (num_x > 1)) {
			if (chess_board[num_y + 2][num_x - 2] == 0) {
				if (cut == false)
					count = 0;
				address_move[count][0] = num_y + 2;
				address_move[count][1] = num_x - 2;
				count += 1;
				cut = true;
			}
		}
		if ((chess_board[num_y + 1][num_x - 1] == 0) && (cut == false)) {
			address_move[count][0] = num_y + 1;
			address_move[count][1] = num_x - 1;
			count += 1;
		}
	}
	for (char i = count; (i < 4); i++) {
		address_move[i][0] = -1;
		address_move[i][1] = -1;
	}
	ret_check result;
	result.one = cut;
	for (char i = 0; i < 4; i++) {
		result.two[i][0] = address_move[i][0];
		result.two[i][1] = address_move[i][1];
	}
	result.two[4][0] = -1;
	result.two[4][1] = -1;
	return result;
}
//Отрисовка ходов, если ходит бот, запись всех допустимых ходов в массив
void draw_move(bool white_move, int(&const_pos)[3], bool &cut_mode, int(&bot_move)[13][3], int(&mass_cut)[6][3], int &count_bot_move, int &count_cut, Sprite mapsprite[size][size], int chess_board[size][size]) {
	int count_res;
	count_bot_move = 0, count_cut = 0;
	int checker = const_pos[0], king_checker = const_pos[1];
	bool check, cut, queen;
	cut_mode = false;
	ret_check res;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			check = false;
			if (chess_board[i][j] == checker) {
				check = true;
				queen = false;
			}
			if (chess_board[i][j] == king_checker) {
				check = true;
				queen = true;
			}
			if (check) {
				if (!white_move) {
					bot_move[count_bot_move][0] = j; bot_move[count_bot_move][1] = i;
					count_bot_move += 1;
				}
				else {
					res = check_move(white_move, queen, j, i, chess_board);
					cut = res.one;
					if (cut) {
						cut_mode = true;
						mass_cut[count_cut][0] = j; mass_cut[count_cut][1] = i;
						count_cut += 1;
					}
					count_res = 0;
					while ((res.two[count_res][0] != -1) && (res.two[count_res][1] != -1)) {
						mapsprite[res.two[count_res][0]][res.two[count_res][1]].setColor(Color::Red);
						count_res += 1;
					}
				}
			}
		}
}
// Обработка ходов
void consider_move(int &count_bot_move, int(&moved_bot)[24][5], int bot_move[13][3], int chess_board[size][size]) {
	int count = 0, count_res;
	ret_check res;
	bool safe_mode = false, queen, white_move = false, safe;
	for (int i = 0; i < count_bot_move; i++) {
		if (chess_board[bot_move[i][1]][bot_move[i][0]] == black_king_checker)
			queen = true;
		else
			queen = false;
		res = check_move(white_move, queen, bot_move[i][0], bot_move[i][1], chess_board);
		if (res.one) {
			moved_bot[0][4] = 2;
			moved_bot[0][0] = bot_move[i][0]; moved_bot[0][1] = bot_move[i][1];
			moved_bot[0][2] = res.two[0][1]; moved_bot[0][3] = res.two[0][0];
			count += 1;
		}
		count_res = 0;
		while ((res.two[count_res][0] != -1) && (res.two[count_res][1] != -1)) {
			safe = check_safe_move(res.two[count_res][0], res.two[count_res][1], chess_board);
			if (safe)
				moved_bot[count][4] = 1;
			else
				moved_bot[count][4] = -1;
			moved_bot[count][0] = bot_move[i][0]; moved_bot[count][1] = bot_move[i][1];
			moved_bot[count][2] = res.two[count_res][1]; moved_bot[count][3] = res.two[count_res][0];
			count += 1;
			count_res += 1;
		}
	}
}
void cut_enemy(bool &white_move, bool &queen, int x_start, int y_start, int x_end, int y_end, int &count_cut,
	int(&chess_board)[size][size], Sprite enemysprite[13], Sprite king_enemysprite[13]) { // удаление вражеской шашки с поля
	int che_num, const_enemy[3];
	ret_check res;
	motion answer;
	che_num = chess_board[y_start + (y_end - y_start) / 2][x_start + (x_end - x_start) / 2];
	chess_board[y_start + (y_end - y_start) / 2][x_start + (x_end - x_start) / 2] = 0;
	if (!white_move) {
		count_cut += 1;
		const_enemy[0] = white_checker; const_enemy[1] = white_king_checker;
	}
	else {
		count_cut += 1;
		const_enemy[0] = black_checker; const_enemy[1] = black_king_checker;
	}
	if (che_num == const_enemy[0]) {
		answer = move_object(enemysprite, const_coords[x_start + (x_end - x_start) / 2], const_coords[y_start + (y_end - y_start) / 2]);
		enemysprite[answer.num_spr].setPosition(border, border);
	}
	if (che_num == const_enemy[1]) {
		answer = move_object(king_enemysprite, const_coords[x_start + (x_end - x_start) / 2], const_coords[y_start + (y_end - y_start) / 2]);
		king_enemysprite[answer.num_spr].setPosition(border, border);
	}
	res = check_move(white_move, queen, x_end, y_end, chess_board);
	if (!res.one)
		if (!white_move)
			white_move = true;
		else
			white_move = false;
}
//движение бота
bool bot_move_start(int moved_bot[24][5], int(&chess_board)[size][size], Sprite blacksprite[13], Sprite king_blacksprite[13]) {
	inicialization var;
	motion answer;
	bool queen = false;
	var.num_x_start = moved_bot[0][0]; var.num_y_start = moved_bot[0][1];
	var.num_x_end = moved_bot[0][2]; var.num_y_end = moved_bot[0][3];
	if (chess_board[var.num_y_start][var.num_x_start] == black_checker) {
		answer = move_object(blacksprite, const_coords[var.num_x_start], const_coords[var.num_y_start]);
		if (const_coords[var.num_y_end] < const_coords[1]) { //преобразование шашки в дамку
			king_blacksprite[answer.num_spr].setPosition(const_coords[var.num_x_end], const_coords[var.num_y_end]);
			blacksprite[answer.num_spr].setPosition(border, border);
			chess_board[var.num_y_end][var.num_x_end] = black_king_checker;
			queen = true;
		}
		else {
			blacksprite[answer.num_spr].setPosition(const_coords[var.num_x_end], const_coords[var.num_y_end]);
			chess_board[var.num_y_end][var.num_x_end] = black_checker;
		}
	}
	else {
		answer = move_object(king_blacksprite, const_coords[var.num_x_start], const_coords[var.num_y_start]);
		king_blacksprite[answer.num_spr].setPosition(const_coords[var.num_x_end], const_coords[var.num_y_end]);
		chess_board[var.num_y_end][var.num_x_end] = black_king_checker;
		queen = true;
	}
	chess_board[var.num_y_start][var.num_x_start] = 0;
	return queen;
}

void player_move(bool &white_move, bool queen, bool cut, int num, int &number_el, int num_x_end, int num_x_start, int num_y_start, int coor_ceil_x, int coor_ceil_y, int(&count_cut_black), int num_y_end, Sprite whitesprite[13], Sprite king_whitesprite[13], Sprite blacksprite[13], Sprite king_blacksprite[13], int(&chess_board)[size][size]) {
	if (queen == false) {
		if (coor_ceil_y >= const_coords[size - 1]) { // преобразование шашки в дамку
			chess_board[num_y_end][num_x_end] = white_king_checker;
			whitesprite[num].setPosition(border, border);
			king_whitesprite[num].setPosition(coor_ceil_x, coor_ceil_y);
		}
		else {
			whitesprite[num].setPosition(coor_ceil_x, coor_ceil_y);
			chess_board[num_y_end][num_x_end] = white_checker;
		}
		chess_board[num_y_start][num_x_start] = 0;
		if (cut) {
			cut_enemy(white_move, queen, num_x_start, num_y_start, num_x_end, num_y_end, count_cut_black,
				chess_board, blacksprite, king_blacksprite);
			if (white_move)
				number_el = num;
		}
		else
			white_move = false;
	}
	else {
		king_whitesprite[num].setPosition(coor_ceil_x, coor_ceil_y);
		chess_board[num_y_start][num_x_start] = 0;
		chess_board[num_y_end][num_x_end] = white_king_checker;
		if (cut) {
			cut_enemy(white_move, queen, num_x_start, num_y_start, num_x_end, num_y_end, count_cut_black,
				chess_board, blacksprite, king_blacksprite);
			if (white_move)
				number_el = num;
		}
		else
			white_move = false;
	}
}
// назначение констант
void inp_const_pos(bool white_move, int(&const_pos)[3]) {
	if (white_move) {
		const_pos[0] = white_checker;
		const_pos[1] = white_king_checker;
	}
	else {
		const_pos[0] = black_checker;
		const_pos[1] = black_king_checker;
	}
}
// возврат к "чистому" полю
void draw_white_wall(Sprite mapsprite[size][size]) {
	for (char i = 0; i < 8; i++)
		for (char j = 0; j < 8; j++)
			mapsprite[i][j].setColor(Color::White);
}
//отрисовка хода "рубки"
void draw_move_cut(Sprite mapsprite[size][size], int(&count_cut), bool queen, int(&mass_cut)[6][3], int chess_board[size][size]) {
	ret_check res;
	int count_res;
	bool white_move = true;
	draw_white_wall(mapsprite);
	for (int i = 0; i < count_cut;i++) {
		res = check_move(white_move, queen, mass_cut[i][0], mass_cut[i][1], chess_board);
		count_res = 0;
		while ((res.two[count_res][0] != -1) && (res.two[count_res][1] != -1)) {
			mapsprite[res.two[count_res][0]][res.two[count_res][1]].setColor(Color::Red);
			count_res += 1;
		}
	}
}
//ход бота
void bot_step(bool &white_move, int count_bot_move, int &count, int &count_cut_white, int(&moved_bot)[24][5], int(&bot_move)[13][3], int(&chess_board)[size][size], Sprite whitesprite[13], Sprite king_whitesprite[13], Sprite blacksprite[13], Sprite king_blacksprite[13]) {
	bool queen;
	int num_x_start, num_y_start, num_x_end, num_y_end, number;
	consider_move(count_bot_move, moved_bot, bot_move, chess_board); // Обработка ходов
	if (moved_bot[0][4] == 2) {
		num_x_start = moved_bot[0][0]; num_y_start = moved_bot[0][1];
		num_x_end = moved_bot[0][2]; num_y_end = moved_bot[0][3];
		queen = bot_move_start(moved_bot, chess_board, blacksprite, king_blacksprite);
		cut_enemy(white_move, queen, num_x_start, num_y_start, num_x_end, num_y_end, count_cut_white,
			chess_board, whitesprite, king_whitesprite);
	}
	else {
		number = 0;
		while ((moved_bot[number][4] != 1) && (number < count)) {
			number += 1;
		}
		queen = bot_move_start(moved_bot, chess_board, blacksprite, king_blacksprite);
		white_move = true;
	}
}
//определение номера нажатого спрайта
void num_sprite(bool white_move, bool &queen, bool &isMove, int pos_x, int pos_y, int &coord_x, int &coord_y, int &dX, int &dY, int &num, Sprite whitesprite[13], Sprite king_whitesprite[13]) {
	motion answer;
	if (white_move) {
		answer = move_object(whitesprite, pos_x, pos_y);
		isMove = answer.isMov;
		if (isMove != true) {
			answer = move_object(king_whitesprite, pos_x, pos_y);
			isMove = answer.isMov;
			if (isMove == true) {
				coord_x = answer.start_x; coord_y = answer.start_y;
				dX = answer.mov_x; dY = answer.mov_y;
				num = answer.num_spr;
				queen = true;
			}
		}
		else {
			coord_x = answer.start_x; coord_y = answer.start_y;
			dX = answer.mov_x; dY = answer.mov_y;
			num = answer.num_spr;
			queen = false;
		}
	}
}
//отрисовка хода отдельного спрайта
void draw_sprite_move(bool white_move, bool queen, bool cut_mode, bool &isMove, int count_cut, int num_x_start, int num_y_start, int mass_cut[6][3], int chess_board[size][size], Sprite mapsprite[size][size]) {
	ret_check res;
	if (cut_mode) {
		int count = 0;
		isMove = false;
		for (int i = 0; i < count_cut;i++) {
			if ((mass_cut[i][0] == num_x_start) && (mass_cut[i][1] == num_y_start))
				isMove = true;
		}
	}
	if (isMove) {
		res = check_move(white_move, queen, num_x_start, num_y_start, chess_board);
		int count_res = 0;
		while ((res.two[count_res][0] != -1) && (res.two[count_res][1] != -1)) {
			mapsprite[res.two[count_res][0]][res.two[count_res][1]].setColor(Color::Red);
			count_res += 1;
		}
	}
}
//определение начальной и конечной клетки 
void check_cell(bool white_move, bool queen, bool &cut, int coord_x, int coord_y, int pos_x, int pos_y, int &coor_ceil_x, int &coor_ceil_y, int &num_x_start, int &num_y_start, int &num_x_end, int &num_y_end, int chess_board[size][size]) {
	pair ret;
	ret_check res;
	ret = coordinate(coord_x); num_x_start = ret.one;
	ret = coordinate(coord_y); num_y_start = ret.one;
	res = check_move(white_move, queen, num_x_start, num_y_start, chess_board);
	cut = res.one;
	ret = coordinate(pos_x); num_x_end = ret.one; coor_ceil_x = ret.two;
	ret = coordinate(pos_y); num_y_end = ret.one; coor_ceil_y = ret.two;
}
void start_game(bool white_move, int(&chess_board)[size][size], Sprite whitesprite[13], Sprite king_whitesprite[13], Sprite blacksprite[13], Sprite king_blacksprite[13], Sprite mapsprite[size][size], Sprite mapsprite_wall, Text text) {
	RenderWindow window(sf::VideoMode(screen_size_x, screen_size_y), "checkers");
	motion answer;
	pair ret;
	ret_check res;
	inicialization var;
	int num = -1;
	bool move = false;
	bool isMove = false, cut, safe, cut_mode = false;
	bool queen = false;
	int count_cut_white = 0, count_cut_black = 0;
	int coord_x, coord_y;
	while (window.isOpen()) {
		Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
		Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			inp_const_pos(white_move, var.const_pos);
			move = false;
			if (!isMove) { //рисуем допустимые ходы для белых или массив ходов для черных
				draw_move(white_move, var.const_pos, cut_mode, var.bot_move, var.mass_cut, var.count_bot_move, var.count_cut, mapsprite, chess_board);
			}
			if (cut_mode) { // рисуем ходы только чтобы "срубить", для игрока
				draw_move_cut(mapsprite, var.count_cut, queen, var.mass_cut, chess_board);
			}
			if (!white_move) { //Бот
				bot_step(white_move, var.count_bot_move, var.count, count_cut_white, var.moved_bot, var.bot_move, chess_board, whitesprite, king_whitesprite, blacksprite, king_blacksprite);
			} //БОТ
			if ((event.type == Event::MouseButtonPressed) && (event.key.code == Mouse::Left)) {//если нажата клавиша мыши//а именно левая
				draw_white_wall(mapsprite);
				isMove = false;
				num_sprite(white_move, queen, isMove, pos.x, pos.y, coord_x, coord_y, var.dX, var.dY, num, whitesprite, king_whitesprite);
				ret = coordinate(coord_x); var.num_x_start = ret.one;
				ret = coordinate(coord_y); var.num_y_start = ret.one;
				res = check_move(white_move, queen, var.num_x_start, var.num_y_start, chess_board);
				draw_sprite_move(white_move, queen, cut_mode, isMove, var.count_cut, var.num_x_start, var.num_y_start, var.mass_cut, chess_board, mapsprite);
			}
			if ((event.type == Event::MouseButtonReleased) && (isMove) && (event.key.code == Mouse::Left)) {//если отпустили клавишу//а именно левую
				isMove = false;
				move = false;
				var.count_res = 0;
				while ((res.two[var.count_res][0] != -1) && (res.two[var.count_res][1] != -1)) {
					mapsprite[res.two[var.count_res][0]][res.two[var.count_res][1]].setColor(Color::White);
					var.count_res += 1;
				}
				check_cell(white_move, queen, cut, coord_x, coord_y, pos.x, pos.y, var.coor_ceil_x, var.coor_ceil_y, var.num_x_start, var.num_y_start, var.num_x_end, var.num_y_end, chess_board);
				for (char l = 0; l < 4; l++)
					if ((res.two[l][0] == var.num_y_end) && (res.two[l][1] == var.num_x_end)) {
						move = true;
					}
				if (move) {
					player_move(white_move, queen, cut, num, var.number_el, var.num_x_end, var.num_x_start, var.num_y_start, var.coor_ceil_x, var.coor_ceil_y, count_cut_black, var.num_y_end, whitesprite, king_whitesprite, blacksprite, king_blacksprite, chess_board);
				}
				else {
					if (white_move) {
						if (queen)
							king_whitesprite[num].setPosition(coord_x, coord_y);
						else
							whitesprite[num].setPosition(coord_x, coord_y);
					}
				}
				move = false;
				num = -1;
			}
			if ((isMove) && (white_move)) {
				if (queen)
					king_whitesprite[num].setPosition(pos.x - var.dX, pos.y - var.dY);
				else
					whitesprite[num].setPosition(pos.x - var.dX, pos.y - var.dY);
			}
		}
		window.clear();
		window.draw(mapsprite_wall);
		for (char i = 0; i < 8; i++)
			for (char j = 0; j < 8; j++) {
				if (((i % 2 == 0) && (j % 2 == 0)) || ((i % 2 != 0) && (j % 2 != 0)))
					window.draw(mapsprite[i][j]);
			}
		for (char i = 0; i < 12; i++) {
			window.draw(king_whitesprite[i]);
			window.draw(king_blacksprite[i]);
			window.draw(whitesprite[i]);
			window.draw(blacksprite[i]);
		}
		if (count_cut_white == 12) {
			text.setPosition(299, 350);//задаем позицию текста, центр камеры
			text.setString("BOT WIN");//задает строку тексту
			window.draw(text);//рисую этот текст
		}
		if (count_cut_black == 12) {
			text.setPosition(299, 350);//задаем позицию текста, центр камеры
			text.setString("Player WIN");//задает строку тексту
			window.draw(text);//рисую этот текст
			white_move = true;
		}
		window.display();
	}
}
int main() {
	Font font;//шрифт 
	font.loadFromFile("images/CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 40);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text.setColor(Color::Blue);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
	bool white_move = true;
	Texture whitetexture, blacktexture, king_whitetexture, king_blacktexture, maptexture, maptexture_wall;
	whitetexture.loadFromFile("images/white.png");
	blacktexture.loadFromFile("images/black.png");
	maptexture.loadFromFile("images/mapp.png");
	maptexture_wall.loadFromFile("images/checkers_780.png");
	king_whitetexture.loadFromFile("images/king_white.png");
	king_blacktexture.loadFromFile("images/king_black.png");
	int chess_board[size][size] =
	{
		{ 2, 5, 2, 5, 2, 5, 2, 5 },
		{ 5, 2, 5, 2, 5, 2, 5, 2 },
		{ 2, 5, 2, 5, 2, 5, 2, 5 },
		{ 5, 0, 5, 0, 5, 0, 5, 0 },
		{ 0, 5, 0, 5, 0, 5, 0, 5 },
		{ 5, 1, 5, 1, 5, 1, 5, 1 },
		{ 1, 5, 1, 5, 1, 5, 1, 5 },
		{ 5, 1, 5, 1, 5, 1, 5, 1 }
	};
	int coord_x = const_coords[0], coord_y = const_coords[0]; // 35? 35
	Sprite whitesprite[13], blacksprite[13], king_whitesprite[13], king_blacksprite[13], mapsprite[size][size], mapsprite_wall;
	// отрисовка фона
	mapsprite_wall.setTexture(maptexture_wall);
	mapsprite_wall.setPosition(0, 0);
	// отрисовка квадратов-фона
	arrangement_map(mapsprite, maptexture, coord_x, coord_y);
	// отрисовка шашек
	coord_x = const_coords[0]; coord_y = const_coords[0];
	arrangement_of_checkers(king_whitesprite, king_whitetexture, whitesprite, whitetexture, coord_x, coord_y);
	coord_x = const_coords[1]; coord_y = const_coords[5]; // 123? 475
	arrangement_of_checkers(king_blacksprite, king_blacktexture, blacksprite, blacktexture, coord_x, coord_y);
	// основа
	start_game(white_move, chess_board, whitesprite, king_whitesprite, blacksprite, king_blacksprite, mapsprite, mapsprite_wall, text);
	return 0;
}