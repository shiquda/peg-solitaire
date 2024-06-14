#include <cmath>
#include <iostream>
//#include <cmath>
//#include <graphics.h>

#include "core.h"
#include "graphic.h"
#include "hexagon.h"

Game::Game(bool endgame)
{	
	// 初始化棋盘
	//chess_count = CHESS_COUNT; // 棋子数量
	is_quit = false;
	undo_flag = false; // 是否可以撤销
	is_endgame = endgame;
	selected_x = -1; 
	selected_y = -1;
	suggestion_x1 = suggestion_y1 = suggestion_x2 = suggestion_y2 = -1;
	// release

	int init_chessboard[BOARD_SIZE][BOARD_SIZE] = {
			{-1, -1, 1, 1, 1, -1, -1},
			{-1, -1, 1, 1, 1, -1, -1},
			{1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 0, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1},
			{-1, -1, 1, 1, 1, -1, -1},
			{-1, -1, 1, 1, 1, -1, -1},
		};

	int init_endgame_chessboard[BOARD_SIZE][BOARD_SIZE] = {
			{-1, -1, 0, 0, 0, -1, -1},
			{-1, -1, 0, 0, 1, -1, -1},
			{0, 1, 1, 0, 1, 0, 1},
			{1, 0, 0, 1, 0, 0, 1},
			{1, 0, 1, 0, 1, 1, 0},
			{-1, -1, 1, 0, 0, -1, -1},
			{-1, -1, 0, 1, 1, -1, -1},
	};


	 //test
	//int init_chessboard[BOARD_SIZE][BOARD_SIZE]{ 0 };
	//init_chessboard[0][1] = init_chessboard[0][2] = init_chessboard[0][5] = init_chessboard[0][6] = 1;
	//int init_chessboard[BOARD_SIZE][BOARD_SIZE] = {
	//	{-1, -1, 1, 0, 0, -1, -1},
	//	{-1, -1, 0, 0, 0, -1, -1},
	//	{1, 1, 0, 0, 0, 0, 0},
	//	{1, 0, 0, 0, 0, 0, 0},
	//	{1, 0, 1, 0, 0, 0, 0},
	//	{-1, -1, 1, 0, 0, -1, -1},
	//	{-1, -1, 0, 0, 0, -1, -1},
	//};

	if (endgame)
	{
		board_copy(chessboard, init_endgame_chessboard);
		board_copy(chessboard_backup, init_endgame_chessboard);
	}
	else 
	{
		board_copy(chessboard, init_chessboard);
		board_copy(chessboard_backup, init_chessboard);
	}

	chess_count = get_chess_count();
	min_chess_count = chess_count;
	loadimage(&img, _T("./src/game.jpg"), WINDOW_WIDTH, WINDOW_HEIGHT);
}

Game::~Game()
{
}

int Game::get_chess_count() {
	int count = 0;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (chessboard[i][j] == 1)
			{
				count++;
			}
		}
	}
	return count;
}

void Game::board_copy(int dest[BOARD_SIZE][BOARD_SIZE], int src[BOARD_SIZE][BOARD_SIZE])
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			dest[i][j] = src[i][j];
		}
	}
}

void Game::undo()
{
	if (!undo_flag)
	{
		return;
	}
	chess_count++;
	board_copy(chessboard, chessboard_backup);
	undo_flag = false;
}



bool Game::in_board(int x, int y) {
	return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && chessboard[x][y] != -1;
}

bool Game::walk(int x1, int y1, int x2, int y2, bool algo)
{
	if (!walk_is_valid(x1, y1, x2, y2))
	{
		return false;
	}
	if(!algo) board_copy(chessboard_backup, chessboard);
	chessboard[x1][y1] = 0;
	chessboard[x2][y2] = 1;
	chessboard[(x1 + x2) / 2][(y1 + y2) / 2] = 0;
	chess_count--;
	if (!algo) undo_flag = true;
	if (!algo) min_chess_count = min(min_chess_count, chess_count);
	if (!algo) play_sound(PLACE_CHESS);
	return true;
}

inline bool Game::walk_is_valid(int x1, int y1, int x2, int y2)
{
	return in_board(x1, y1) && in_board(x2, y2) && 
		((x1 == x2 && abs(y1 - y2) == 2) ||
		(y1 == y2 && abs(x1 - x2) == 2)) &&
		chessboard[x1][y1] == 1 && chessboard[x2][y2] == 0 && chessboard[(x1+x2)/2][(y1+y2)/2] == 1;
}

bool Game::is_win()
{
	return chess_count == 1;
}

bool Game::is_lose()
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (chessboard[i][j] == 1) // 说明是棋子
			{
				if (walk_is_valid(i, j, i - 2, j) || walk_is_valid(i, j, i + 2, j) || walk_is_valid(i, j, i, j - 2) || walk_is_valid(i, j, i, j + 2))
				{
					return false;
				}
			}
		}
	}
	return true;
}

void Game::select(int x, int y)
{
	if (selected_x == x && selected_y == y)
	{
		selected_x = selected_y = -1;
		std::cout << "取消选中" << '\n';
	}
	else
	if (selected_x == -1 && selected_y == -1)
	{
		selected_x = x;
		selected_y = y;
		std::cout << "选中" << x << " " << y << '\n';
	}
	else
	{
		if (walk(selected_x, selected_y, x, y)) // 如果走棋成功
		{
			selected_x = selected_y = -1;
			std::cout << "走棋成功" << '\n';
			return;
		}
		else
		{
			std::cout << "非法操作" << '\n';
			selected_x = x;
			selected_y = y;
		}
	}
}

void wait() {
	char c;
	while (1)
	{
		std::cin >> c;
		if (c == 't')
		{
			break;
		}
	}
}

void start_game() {
	Game game;
	while (1)
	{

		game.print_graphic();
		if (game.is_win())
		{
			game.print_graphic(1);
			print_win();
			wait_for_confirm(0);
			break;
		}
		else if (game.is_lose()) {
			game.print_graphic(1);
			print_lose(game.chess_count);
			wait_for_confirm(0);
			break;
		}
		else if (game.is_quit) // 用户选择退出
		{
			break;
		}
	}
}


void start_endgame() {
	Game game(true);
	while (1)
	{
		game.print_graphic();
		if (game.is_win())
		{
			game.print_graphic(1);
			print_win();
			wait_for_confirm(0);
			break;
		}
		else if (game.is_lose()) {
			game.print_graphic(1);
			print_lose(game.chess_count);
			wait_for_confirm(0);
			break;
		}
		else if (game.is_quit) // 用户选择退出
		{
			break;
		}
	}
}

void start_hexagon_game() {
	HexagonGame game;
	while (1)
	{
		game.print_graphic();
		if (game.is_win())
		{
			game.print_graphic(1);
			play_sound(WIN);
			print_win();
			wait_for_confirm(0);
			break;
		}
		else if (game.is_lose()) {
			game.print_graphic(1);
			play_sound(LOSE);
			print_lose(game.chess_count);
			wait_for_confirm(0);
			break;
		}
		else if (game.is_quit) // 用户选择退出
		{
			break;
		}
	}
}

