#pragma once

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

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


void main_menu();
//void get_walk_input(int& x1, int& x2, int& y1, int& y2);
//void print_exit();
void clear_screen();
void print_win();
void print_lose();
//void print_help();
void wait_for_confirm();
