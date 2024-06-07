#include "core.h"
#include "graphic.h"

#include <cmath>
#include <iostream>
#include <cmath>

Game::Game()
{
	// 初始化棋盘
	//chess_count = CHESS_COUNT; // 棋子数量
	step = 0; // 步数
	undo_flag = false; // 是否可以撤销
	selected_x = -1; 
	selected_y = -1;
	suggestion_x1 = suggestion_y1 = suggestion_x2 = suggestion_y2 = -1;
	min_chess_count = CHESS_COUNT + 1;
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
	 //test
	//int init_chessboard[BOARD_SIZE][BOARD_SIZE]{ 0 };
	//init_chessboard[0][1] = init_chessboard[0][2] = init_chessboard[0][5] = init_chessboard[0][6] = 1;


	board_copy(chessboard, init_chessboard);
	board_copy(chessboard_backup, init_chessboard);
	chess_count = get_chess_count();
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
	step--;
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
	step++;
	if (!algo) undo_flag = true;
	return true;
}

bool Game::walk_is_valid(int x1, int y1, int x2, int y2)
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
		std::cout << "取消选中" << std::endl;
	}
	else
	if (selected_x == -1 && selected_y == -1)
	{
		selected_x = x;
		selected_y = y;
		std::cout << "选中" << x << " " << y << std::endl;
	}
	else
	{
		if (walk(selected_x, selected_y, x, y)) // 如果走棋成功
		{
			selected_x = selected_y = -1;
			std::cout << "走棋成功" << std::endl;
			return;
		}
		else
		{
			std::cout << "非法操作" << std::endl;
			selected_x = x;
			selected_y = y;
		}
	}
}

void Game::suggestion() // 最优走棋算法
{
	min_chess_count = std::min(chess_count, min_chess_count);
	int min_chess_count_copy = min_chess_count;
	//std::cout << "min_chess_count: " << min_chess_count << std::endl;
	//std::cout << "x1 " << suggestion_x1 << " y1 " << suggestion_y1 << " x2 " << suggestion_x2 << " y2 " << suggestion_y2 << std::endl;
	if (min_chess_count == 1) return;

	int chessboard_copy[BOARD_SIZE][BOARD_SIZE];
	board_copy(chessboard_copy, chessboard);
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			// 深度优先搜索
			if (chessboard[i][j] == 1)
			{
				if (walk_is_valid(i, j, i - 2, j))
				{
					walk(i, j, i - 2, j, true);
					suggestion();
					if (min_chess_count < min_chess_count_copy) {
						suggestion_x1 = i;
						suggestion_y1 = j;
						suggestion_x2 = i - 2;
						suggestion_y2 = j;
						//std::cout << "min_chess_count changed: " << min_chess_count << "at " << chess_count <<
						//	", walk:" << suggestion_x1 << ' ' << suggestion_y1 << ' ' << suggestion_x2 << ' ' << suggestion_y2 << std::endl;
					}
					board_copy(chessboard, chessboard_copy);
					step--;
					chess_count++;
					min_chess_count_copy = min_chess_count;
					if (min_chess_count == 1) break;
				}
				else if (walk_is_valid(i, j, i + 2, j))
				{
					walk(i, j, i + 2, j, true);
					suggestion();
					if (min_chess_count < min_chess_count_copy) {
						suggestion_x1 = i;
						suggestion_y1 = j;
						suggestion_x2 = i + 2;
						suggestion_y2 = j;
						//std::cout << "min_chess_count changed: " << min_chess_count << "at " << chess_count <<
						//	", walk:" << suggestion_x1 << ' ' << suggestion_y1 << ' ' << suggestion_x2 << ' ' << suggestion_y2 << std::endl;
					}
					board_copy(chessboard, chessboard_copy);
					step--;
					chess_count++;
					min_chess_count_copy = min_chess_count;
					if (min_chess_count == 1) break;
				}
				else if (walk_is_valid(i, j, i, j - 2))
				{
					walk(i, j, i, j - 2, true);
					suggestion();
					if (min_chess_count < min_chess_count_copy) {
						suggestion_x1 = i;
						suggestion_y1 = j;
						suggestion_x2 = i;
						suggestion_y2 = j - 2;
						//std::cout << "min_chess_count changed: " << min_chess_count << "at " << chess_count <<
						//	", walk:" << suggestion_x1 << ' ' << suggestion_y1 << ' ' << suggestion_x2 << ' ' << suggestion_y2 << std::endl;
					}
					board_copy(chessboard, chessboard_copy);
					step--;
					chess_count++;
					min_chess_count_copy = min_chess_count;
					if (min_chess_count == 1) break;
				}
				else if (walk_is_valid(i, j, i, j + 2))
				{
					walk(i, j, i, j + 2, true);
					suggestion();
					if (min_chess_count < min_chess_count_copy) {
						suggestion_x1 = i;
						suggestion_y1 = j;
						suggestion_x2 = i;
						suggestion_y2 = j + 2;
						//std::cout << "min_chess_count changed: " << min_chess_count << "at " << chess_count <<
						//	", walk:" << suggestion_x1 << ' ' << suggestion_y1 << ' ' << suggestion_x2 << ' ' << suggestion_y2 << std::endl;
					}
					board_copy(chessboard, chessboard_copy);
					step--;
					chess_count++;
					if (min_chess_count == 1) break;
				}
			}
		}
	}
	board_copy(chessboard, chessboard_copy);
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

void startGame() {
	Game game;
	while (1)
	{

		game.print_graphic();
		if (game.is_win())
		{
			game.print_graphic(1);
			print_win();
			wait_for_confirm();
			break;
		}
		else if (game.is_lose()) {
			game.print_graphic(1);
			print_lose();
			wait_for_confirm();
			break;
		}

	}

}

