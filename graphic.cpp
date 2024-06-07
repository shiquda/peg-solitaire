#include <graphics.h>
#include <vector>
#include <iostream>

#include "core.h"
#include "graphic.h"

bool Clickable::clicked(int cx, int cy)
{
	return cx >= x - width / 2 && cy >= y  - height / 2 && cx <= x + width/2 && cy<= y + height / 2;
}

bool Clickable::receive_event(int cx, int cy)
{
	if (clicked(cx, cy)) {
		callback();
		return true;
	}
	return false;
}

Button::Button(int x, int y, int width, int height, const wchar_t* text, void(*callback)())
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->callback = callback;
	this->text = text;
}

void Button::draw()
{
	// Set the background and border of the button
	setfillcolor(BLUE);
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 1);
	fillrectangle(x - width / 2, y - height / 2, x + width / 2, y + height / 2);

	// Set the text style and color
	settextstyle(30, 0, _T("宋体"));
	settextcolor(GREEN);

	// Display the text at the specified position
	outtextxy(x - width / 2 + 10, y - height / 2 + 10, text);
}

Chess::Chess(int x, int y) {
	this->x = x;
	this->y = y;
}

void Chess::draw() {
	const int r = 20;
	setfillcolor(BLACK);
	fillcircle(x, y, r);
	rectangle(x - 25, y - 25, x + 25, y + 25);
}

Space::Space(int x, int y) : Chess(x, y) {

}

void Space::draw() {
	const int r = 25;
	setfillcolor(WHITE);
	fillrectangle(x - r, y - r, x + r, y + r);
}

void main_menu() {
	// 绘制背景

	Button start_button(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 200, 100, L"开始游戏！", startGame);
	start_button.draw();

	while (1)
	{
		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			int cx = m.x;
			int cy = m.y;
			if (start_button.receive_event(cx, cy)) {
				break;
			}
		}
	}
}

void Game::print_graphic(bool show_only) {
	const int left = 100;
	const int upper = 100;
	clear_screen();
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 1);
	settextstyle(30, 0, _T("宋体"));
	// 画棋盘
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (chessboard[i][j] == 1)
			{
				Chess chess(left + j * 50, upper + i * 50);
				chess.draw();
			}
			else if (chessboard[i][j] == 0)
			{
				Space space(left + j * 50, upper + i * 50);
				space.draw();
			}
		}
	}

	// 画选中框
	if (selected_x != -1 && selected_y != -1)
	{
		setlinecolor(RED);
		setlinestyle(PS_SOLID, 3);
		rectangle(left + selected_y * 50 - 25, upper + selected_x * 50 - 25, left + selected_y * 50 + 25, upper + selected_x * 50 + 25);
	}

	// 写剩余棋子的数量
	settextcolor(BLUE);
	TCHAR s[100];
	_stprintf_s(s, _T("剩余棋子数量：%d"), chess_count);
	outtextxy(left + BOARD_SIZE * 50 + 100, 50, s);

	// 画撤销按钮
	Button undo_button(left + BOARD_SIZE * 50 + 200, 400, 200, 50, L"撤销", [](){});
	undo_button.draw();

	// 测试算法
	int min_chess_count_backup = min_chess_count;
	suggestion();
	min_chess_count = min_chess_count_backup;
	std::cout << min_chess_count << std::endl;
	std::cout << suggestion_x1 << " " << suggestion_y1 << " " << suggestion_x2 << " " << suggestion_y2 << std::endl;

	if (show_only) return;
	while (1)
	{
		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			int cx = m.x;
			int cy = m.y;
			// 选中棋子
			if (cx >= left - 25 && cx <= left + BOARD_SIZE * 50 && cy >= upper - 25 && cy <= upper + BOARD_SIZE * 50)
			{
				int i = (cy - upper + 25) / 50;
				int j = (cx - left + 25) / 50;
				std::cout << i << " " << j << std::endl;
				select(i, j);
				break;
			}
			// 监听撤销
			else if (cx >= left + BOARD_SIZE * 50 + 200 - 100 && cx <= left + BOARD_SIZE * 50 + 200 + 100 && cy >= 400 - 50 && cy <= 400 + 50)
			{
				if (undo_flag)
				{
					undo();
					break;
				}
			}
		}
	}
}	


void clear_screen() {
	// 背景变为白色
	setbkcolor(WHITE);
	cleardevice();
}

void print_win() {
	// 写剩余棋子的数量
	settextcolor(GREEN);
	outtextxy(100 + BOARD_SIZE * 50 + 100, 150, _T("您赢了！"));
}
void print_lose() {
	settextcolor(RED);
	outtextxy(100 + BOARD_SIZE * 50 + 100, 150, _T("没有可以移动的棋子，您输了！"));
}
void print_select(Game game) {

}
void print_help() {

}

void wait_for_confirm() {
	Button confirm_button(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 200, 100, L"确定", []() {	});
	confirm_button.draw();
	while (1)
	{
		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			int cx = m.x;
			int cy = m.y;
			if (confirm_button.receive_event(cx, cy)) {
				break;
			}
		}
	}
}