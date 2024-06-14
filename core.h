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
    // 成员变量
    int chessboard[BOARD_SIZE][BOARD_SIZE]; // 棋盘，-1表示非法区域，0表示无子，1表示有子
    int chessboard_backup[BOARD_SIZE][BOARD_SIZE]; // 棋盘上一步的备份
    int chess_count; // 棋子数量    
    bool undo_flag; // 是否可以撤销
	int selected_x, selected_y; // 是否选中
	int suggestion_x1, suggestion_y1, suggestion_x2, suggestion_y2; // 建议走法
	int min_chess_count; // 最少棋子数量
	bool is_quit; // 用户选择退出
	bool is_endgame; // 是否是残局
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

    // 新添加的旋转和翻转函数声明
    void rotate_board_90(int board[BOARD_SIZE][BOARD_SIZE], int new_board[BOARD_SIZE][BOARD_SIZE]);
    void rotate_board_180(int board[BOARD_SIZE][BOARD_SIZE], int new_board[BOARD_SIZE][BOARD_SIZE]);
    void rotate_board_270(int board[BOARD_SIZE][BOARD_SIZE], int new_board[BOARD_SIZE][BOARD_SIZE]);
    void flip_board(int board[BOARD_SIZE][BOARD_SIZE], int new_board[BOARD_SIZE][BOARD_SIZE]);
};



void start_game();
void start_endgame();
void start_hexagon_game();