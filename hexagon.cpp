#include "hexagon.h"
#include "core.h"
#include "graphic.h"

#include <iostream>

HexagonGame::HexagonGame()
{
	// 初始化棋盘
	//chess_count = CHESS_COUNT; // 棋子数量
	is_quit = false;
	undo_flag = false; // 是否可以撤销
	selected_x = -1;
	selected_y = -1;
	suggestion_x1 = suggestion_y1 = suggestion_x2 = suggestion_y2 = -1;
	// release

	int init_chessboard[9][17] = {
		{-1, -1, -1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1, -1, -1, -1},
		{-1, -1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1, -1, -1},
		{-1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1, -1},
		{-1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1},
		 { 1, -1,  1, -1,  1, -1,  1, -1,  0, -1,  1, -1,  1, -1,  1, -1,  1},
		{-1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1},
		{-1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1, -1},
		{-1, -1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1, -1, -1},
		{-1, -1, -1, -1,  1, -1,  1, -1,  1, -1,  1, -1,  1, -1, -1, -1, -1}
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


	board_copy(chessboard, init_chessboard);
	board_copy(chessboard_backup, init_chessboard);

	chess_count = get_chess_count();
	min_chess_count = chess_count;
}

HexagonGame::~HexagonGame()
{
}

bool HexagonGame::walk_is_valid(int x1, int y1, int x2, int y2)
{
	return in_board(x1, y1) && in_board(x2, y2) &&
		(
			(abs(y1 - y2) == 4 && x1 == x2) ||
			(abs(y1 - y2) == 2 && abs(x1 - x2) == 2)
		) &&
		chessboard[x1][y1] == 1 && chessboard[x2][y2] == 0 && chessboard[(x1 + x2) / 2][(y1 + y2) / 2] == 1;
}

bool HexagonGame::in_board(int x, int y)
{
	return x >= 0 && x < 9 && y >= 0 && y < 17 && chessboard[x][y] != -1;
}

void HexagonGame::board_copy(int dest[9][17], int src[9][17])
{
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 17; j++)
			dest[i][j] = src[i][j];
}

bool HexagonGame::walk(int x1, int y1, int x2, int y2, bool algo)
{
	if (!walk_is_valid(x1, y1, x2, y2))
	{
		return false;
	}
	// 备份
	board_copy(chessboard_backup, chessboard);
	// 走棋
	chessboard[x1][y1] = chessboard[(x1 + x2) / 2][(y1 + y2) / 2] = 0;
	chessboard[x2][y2] = 1;
	// 棋子数量
	chess_count--;
	// 撤销
	undo_flag = true;
	play_sound(PLACE_CHESS);
	return true;
}

bool HexagonGame::is_win()
{
	return chess_count == 1;
}

bool HexagonGame::is_lose()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			if (chessboard[i][j] == 1) // 说明是棋子
			{
				if (walk_is_valid(i, j, i, j + 4) || walk_is_valid(i, j, i, j - 4) || walk_is_valid(i, j, i - 2, j - 2) ||
					walk_is_valid(i, j, i - 2, j + 2) || walk_is_valid(i, j, i + 2, j - 2) || walk_is_valid(i, j, i + 2, j + 2))
				{
					return false;
				}
			}
		}
	}
	return true;
}

void HexagonGame::undo()
{
	if (!undo_flag)
	{
		return;
	}
	chess_count++;
	board_copy(chessboard, chessboard_backup);
	undo_flag = false;
}

int HexagonGame::get_chess_count() {
	int count = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			if (chessboard[i][j] == 1)
			{
				count++;
			}
		}
	}
	return count;
}