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
	settextstyle(30, 0, _T("����"));
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
	// ���Ʊ���

	Button start_button(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 200, 100, L"��ʼ��Ϸ��", startGame);
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
	settextstyle(30, 0, _T("����"));
	// ������
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

	// ��ѡ�п�
	if (selected_x != -1 && selected_y != -1)
	{
		setlinecolor(RED);
		setlinestyle(PS_SOLID, 3);
		rectangle(left + selected_y * 50 - 25, upper + selected_x * 50 - 25, left + selected_y * 50 + 25, upper + selected_x * 50 + 25);
	}

	// дʣ�����ӵ�����
	settextcolor(BLUE);
	TCHAR s[100];
	_stprintf_s(s, _T("ʣ������������%d"), chess_count);
	outtextxy(left + BOARD_SIZE * 50 + 100, 50, s);


	if (show_only) return;
	while (1)
	{
		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			int cx = m.x;
			int cy = m.y;
			if (cx >= left - 25 && cx <= left + BOARD_SIZE * 50 && cy >= upper - 25 && cy <= upper + BOARD_SIZE * 50)
			{
				int i = (cy - upper + 25) / 50;
				int j = (cx - left + 25) / 50;
				std::cout << i << " " << j << std::endl;
				select(i, j);
				break;
			}
			//for (int i = 0; i < chesses.size(); i++)
			//{
			//	if (chesses[i].receive_event(cx, cy)) break;
			//}
			//for (int i = 0; i < spaces.size(); i++)
			//{
			//	if (spaces[i].receive_event(cx, cy)) break;
			//}
		}
	}
}	


void clear_screen() {
	// ������Ϊ��ɫ
	setbkcolor(WHITE);
	cleardevice();
}

void print_win() {
	// дʣ�����ӵ�����
	settextcolor(GREEN);
	outtextxy(100 + BOARD_SIZE * 50 + 100, 150, _T("��Ӯ�ˣ�"));
}
void print_lose() {
	settextcolor(RED);
	outtextxy(100 + BOARD_SIZE * 50 + 100, 150, _T("û�п����ƶ������ӣ������ˣ�"));
}
void print_select(Game game) {

}
void print_help() {

}

void wait_for_confirm() {
	Button confirm_button(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 200, 100, L"ȷ��", []() {	});
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