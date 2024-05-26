#include "core.h"
#include "graphic.h"


class Game
{
public:
	// 成员变量
	int chessboard[BOARD_SIZE][BOARD_SIZE]; // 棋盘，-1表示非法区域，0表示有子，1表示有子
	int chessboard_backup[BOARD_SIZE][BOARD_SIZE]; // 棋盘上一步的备份
	int chess_count = CHESS_COUNT; // 棋子数量
	int step = 0; // 步数

	Game();
	~Game();
	bool walk(int x, int y);
	bool walk_is_valid(int x, int y);
	bool is_win();
	void print_graphic();
	void quit();

private:

};

Game::Game()
{
	// 初始化棋盘
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if ((i >= 2 && i <= 4) || (j >= 2 && j <= 4))
			{
				chessboard[i][j] = 1;
			}
			else
			{
				chessboard[i][j] = -1;
			}
		}
	}
	chessboard[(BOARD_SIZE + 1) / 2][(BOARD_SIZE + 1) / 2] = 0;
}

Game::~Game()
{
}

bool Game::walk(int x, int y)
{
	
}

bool Game::walk_is_valid(int x, int y)
{

}

bool Game::is_win()
{
	return chess_count == 1;
}

void Game::print_graphic()
{

}

void startGame() {
	
}