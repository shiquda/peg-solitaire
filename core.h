#pragma	once

#include <unordered_map>
#include <string>
#include <graphics.h>
#include <cmath>

#define BOARD_SIZE 7
#define CHESS_COUNT 32

struct State
{
    int x1, y1, x2, y2;
    int min_chess_count;
};


class Game {
public:
    // ��Ա����
    int chessboard[BOARD_SIZE][BOARD_SIZE]; // ���̣�-1��ʾ�Ƿ�����0��ʾ���ӣ�1��ʾ����
    int chessboard_backup[BOARD_SIZE][BOARD_SIZE]; // ������һ���ı���
    int chess_count; // ��������    
    bool undo_flag; // �Ƿ���Գ���
	int selected_x, selected_y; // �Ƿ�ѡ��
	int suggestion_x1, suggestion_y1, suggestion_x2, suggestion_y2; // �����߷�
	int min_chess_count; // ������������
	bool is_quit; // �û�ѡ���˳�
	bool is_endgame; // �Ƿ��ǲо�
    IMAGE img;


    Game(bool endgame = false);
    ~Game();
    virtual bool walk(int x1, int y1, int x2, int y2, bool algo = 0);
    virtual bool is_win();
    virtual bool is_lose();
    virtual void print_graphic(bool show_only = 0);
    virtual void undo();
	void select(int x, int y);
    void suggestion(bool first = 0);

protected:
    virtual bool in_board(int x, int y);
    virtual bool walk_is_valid(int x1, int y1, int x2, int y2);
    virtual void board_copy(int dest[BOARD_SIZE][BOARD_SIZE], int src[BOARD_SIZE][BOARD_SIZE]);
    virtual int get_chess_count();
    std::unordered_map<unsigned long long, State> memo;
    unsigned long long board_to_number(int board[BOARD_SIZE][BOARD_SIZE]);
    void fill_record(int board[BOARD_SIZE][BOARD_SIZE], int x1, int y1, int x2, int y2, int min_chess_count);

    // ����ӵ���ת�ͷ�ת��������
    void rotate_board_90(int board[BOARD_SIZE][BOARD_SIZE], int new_board[BOARD_SIZE][BOARD_SIZE]);
    void rotate_board_180(int board[BOARD_SIZE][BOARD_SIZE], int new_board[BOARD_SIZE][BOARD_SIZE]);
    void rotate_board_270(int board[BOARD_SIZE][BOARD_SIZE], int new_board[BOARD_SIZE][BOARD_SIZE]);
    void flip_board(int board[BOARD_SIZE][BOARD_SIZE], int new_board[BOARD_SIZE][BOARD_SIZE]);
};



void start_game();
void start_endgame();
void start_hexagon_game();