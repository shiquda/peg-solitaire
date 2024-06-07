#pragma	once

#define BOARD_SIZE 7
#define CHESS_COUNT 32

class Game {
public:
    // 成员变量
    int chessboard[BOARD_SIZE][BOARD_SIZE]; // 棋盘，-1表示非法区域，0表示无子，1表示有子
    int chessboard_backup[BOARD_SIZE][BOARD_SIZE]; // 棋盘上一步的备份
    int chess_count; // 棋子数量
    int step; // 步数
    bool undo_flag; // 是否可以撤销
	int selected_x, selected_y; // 是否选中
	int suggestion_x1, suggestion_y1, suggestion_x2, suggestion_y2; // 建议走法
	int min_chess_count; // 最少棋子数量

    Game();
    ~Game();
    bool walk(int x1, int y1, int x2, int y2, bool algo = 0);
    bool is_win();
    bool is_lose();
    void print_graphic(bool show_only = 0);
    void undo();
	void select(int x, int y);
    void suggestion();

private:
    bool in_board(int x, int y);
    bool walk_is_valid(int x1, int y1, int x2, int y2);
    void board_copy(int dest[BOARD_SIZE][BOARD_SIZE], int src[BOARD_SIZE][BOARD_SIZE]);
    int get_chess_count();
};


void startGame();