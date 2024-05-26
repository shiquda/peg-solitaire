#include "core.h"
#include "graphic.h"


class Game
{
public:
	// ��Ա����
	int chessboard[BOARD_SIZE][BOARD_SIZE]; // ���̣�-1��ʾ�Ƿ�����0��ʾ���ӣ�1��ʾ����
	int chessboard_backup[BOARD_SIZE][BOARD_SIZE]; // ������һ���ı���
	int chess_count = CHESS_COUNT; // ��������
	int step = 0; // ����

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
	// ��ʼ������
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