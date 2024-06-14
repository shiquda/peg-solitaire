#pragma once
#pragma comment(lib, "Winmm.lib")


#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

enum Sound
{
	// 1 对应落子音效
	// 2 对应胜利音效
	// 3 对应失败音效
	// 4 对应警告音效
	PLACE_CHESS = 1,
	WIN = 2,
	LOSE = 3,
	ALERT = 4
};

class Clickable {
public:
	int x, y, width, height;

	//void draw();
	void(*callback)();
	bool receive_event(int cx, int cy);
	bool clicked(int cx, int cy);

private:
	
};

class Button : public Clickable {
public:

	const wchar_t* text;
	Button(int x, int y, int width, int height, const wchar_t* text, void(*callback)());
	void draw();
};

class Chess : public Clickable {
public:
	Chess(int x, int y);
	void draw();
};

class Space : public Chess {
public:
	Space(int x, int y);
	void draw();
};

class HexagonChess : public Chess {
public:
	HexagonChess(int x, int y);
	void draw();
};

class HexagonSpace : public HexagonChess {
public:
	HexagonSpace(int x, int y);
	void draw();
};


void main_menu();
//void get_walk_input(int& x1, int& x2, int& y1, int& y2);
//void print_exit();
void clear_screen();
void print_win();
void print_lose(int chess_count);
void wait_for_confirm(bool sound = 1);
void print_algo_limit(bool endgame = false);
void print_undo_limit();
void print_help();
void draw_return_arrow();
void play_sound(Sound type);