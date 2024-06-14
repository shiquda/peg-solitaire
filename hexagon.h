#pragma once

#include "core.h"

class HexagonGame : public Game {
public:
    int chessboard[9][17]; // 棋盘，-1表示非法区域，0表示无子，1表示有子
    int chessboard_backup[9][17]; // 棋盘上一步的备份
	HexagonGame();
	~HexagonGame();
    bool walk(int x1, int y1, int x2, int y2, bool algo = 0);
    void print_graphic(bool show_only = 0);
    bool is_win();
    bool is_lose();
	void undo();

protected:
    bool in_board(int x, int y);
    bool walk_is_valid(int x1, int y1, int x2, int y2);
	void board_copy(int dest[9][17], int src[9][17]);
    int get_chess_count();
};

