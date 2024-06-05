#pragma	once

#define BOARD_SIZE 7
#define CHESS_COUNT 32

class Game {
public:
    // ��Ա����
    int chessboard[BOARD_SIZE][BOARD_SIZE]; // ���̣�-1��ʾ�Ƿ�����0��ʾ���ӣ�1��ʾ����
    int chessboard_backup[BOARD_SIZE][BOARD_SIZE]; // ������һ���ı���
    int chess_count; // ��������
    int step; // ����
    bool undo_flag; // �Ƿ���Գ���
	int selected_x, selected_y; // �Ƿ�ѡ��

    Game();
    ~Game();
    bool walk(int x1, int y1, int x2, int y2);
    bool is_win();
    bool is_lose();
    void print_graphic(bool show_only = 0);
    bool undo();
	void select(int x, int y);

private:
    bool in_board(int x, int y);
    bool walk_is_valid(int x1, int y1, int x2, int y2);
    void board_copy(int dest[BOARD_SIZE][BOARD_SIZE], int src[BOARD_SIZE][BOARD_SIZE]);
};


void startGame();