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
	setfillcolor(BLUE);
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 1);
	fillrectangle(x - width / 2, y - height / 2, x + width / 2, y + height / 2);

	settextstyle(30, 0, _T("����"));
	settextcolor(GREEN);

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
	// ������˵�
	clear_screen();

	// �������̺����ӱ���

	// ���ƺ�ɫ����
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 1);
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			line(50 + j * 50, 50 + i * 50, 50 + j * 50 + 50, 50 + i * 50);
			line(50 + j * 50, 50 + i * 50, 50 + j * 50, 50 + i * 50 + 50);
		}
	}
	// ���Ʊ��⣺������
	settextstyle(50, 0, _T("����"));
	settextcolor(GREEN);
	outtextxy(WINDOW_WIDTH / 2 - 100 , 100, _T("������"));



	Button start_button(WINDOW_WIDTH / 2 - 200 , WINDOW_HEIGHT / 2, 150, 50, L"����ģʽ", start_game);
	Button endgame_start_button(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 150, 50, L"�о�ģʽ", start_endgame);
	Button help_button(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 100, 150, 50, L"����", print_help);
	Button quit_button(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 200, 150, 50, L"�˳�", []() { exit(0); });
	start_button.draw();
	endgame_start_button.draw();
	help_button.draw();
	quit_button.draw();

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
			if (endgame_start_button.receive_event(cx, cy)) {
				break;
			}
			if (help_button.receive_event(cx, cy)) {
				break;
			}
			if (quit_button.receive_event(cx, cy)) {
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


		
	// дʣ�����ӵ�����
	settextcolor(BLUE);
	TCHAR s[100];
	_stprintf_s(s, _T("ʣ������������%d"), chess_count);
	outtextxy(left + BOARD_SIZE * 50 + 100, 50, s);

	// �����ð�ť
	Button reset_button(left + BOARD_SIZE * 50 + 200, 300, 100, 50, L"����", []() {});
	reset_button.draw();

	// ��������ť
	Button undo_button(left + BOARD_SIZE * 50 + 200, 400, 100, 50, L"����", [](){});
	undo_button.draw();

	// ���㷨��ť
	Button algorithm_button(left + BOARD_SIZE * 50 + 200, 500, 100, 50, L"��ʾ", []() {});
	algorithm_button.draw();

	// �������߷�
	if (suggestion_x1 != -1 && suggestion_y1 != -1 && suggestion_x2 != -1 && suggestion_y2 != -1)
	{
		setlinecolor(GREEN);
		setlinestyle(PS_SOLID, 3);
		rectangle(left + suggestion_y1 * 50 - 25, upper + suggestion_x1 * 50 - 25, left + suggestion_y1 * 50 + 25, upper + suggestion_x1 * 50 + 25);
		rectangle(left + suggestion_y2 * 50 - 25, upper + suggestion_x2 * 50 - 25, left + suggestion_y2 * 50 + 25, upper + suggestion_x2 * 50 + 25);
		suggestion_x1 = suggestion_y1 = suggestion_x2 = suggestion_y2 = -1;
	}

	// ��Բ�μ�ͷ���ذ�ť
	draw_return_arrow();

	// ��ѡ�п�
	if (selected_x != -1 && selected_y != -1)
	{
		setlinecolor(RED);
		setlinestyle(PS_SOLID, 3);
		rectangle(left + selected_y * 50 - 25, upper + selected_x * 50 - 25, left + selected_y * 50 + 25, upper + selected_x * 50 + 25);
	}



	if (show_only) return;
	while (1)
	{
		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			int cx = m.x;
			int cy = m.y;
			// ѡ������
			if (cx >= left - 25 && cx <= left + BOARD_SIZE * 50 && cy >= upper - 25 && cy <= upper + BOARD_SIZE * 50)
			{
				int i = (cy - upper + 25) / 50;
				int j = (cx - left + 25) / 50;
				std::cout << i << " " << j << std::endl;
				select(i, j);
				break;
			}
			// ��������
			else if (cx >= left + BOARD_SIZE * 50 + 200 - 50 && cx <= left + BOARD_SIZE * 50 + 200 + 50 && cy >= 400 - 50 && cy <= 400 + 50)
			{
				if (undo_flag)
				{
					undo();
					break;
				}
				else
				{
					print_undo_limit();
					std::cout << "��ǰ�޷�������" << std::endl;
					wait_for_confirm();
					break;
				}
			}
			// �����㷨
			else if (cx >= left + BOARD_SIZE * 50 + 200 - 50 && cx <= left + BOARD_SIZE * 50 + 200 + 50 && cy >= 500 - 50 && cy <= 500 + 50)
			{
				if (is_endgame)
				{
					print_algo_limit(is_endgame);
					std::cout << "�о��޷�ʹ����ʾ���ܣ�" << std::endl;
					wait_for_confirm();
					break;
				}
				if (chess_count < 18)
				{
					min_chess_count = chess_count;
					int min_chess_count_backup = min_chess_count;
					suggestion(1);
					std::cout << "��ǰ���ʣ����������" << min_chess_count << std::endl;
					min_chess_count = min_chess_count_backup;
					std::cout << suggestion_x1 << " " << suggestion_y1 << " " << suggestion_x2 << " " << suggestion_y2 << std::endl;
				}
				else
				{
					print_algo_limit();
					std::cout << "��ǰ���������࣬�޷����㣡" << std::endl;
					wait_for_confirm();
				}
				break;
			}
			// ����
			else if (cx >= left + BOARD_SIZE * 50 + 200 - 50 && cx <= left + BOARD_SIZE * 50 + 200 + 50 && cy >= 300 - 50 && cy <= 300 + 50)
			{
				if (is_endgame)
				{
					start_endgame();
				}
				else 
				{
					start_game();
				}
				is_quit = 1;
			 	break;
			}
			// ����
			else if (cx >= 20 && cx <= 80 && cy >= 20 && cy <= 80)
			{
				is_quit = 1;
				break;
			}
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
void print_lose(int chess_count) {
	settextcolor(RED);
	outtextxy(100 + BOARD_SIZE * 50 + 100, 150, _T("û�п����ƶ������ӣ������ˣ�"));
	// ��������������������
	if (chess_count <= 3)
	{
		outtextxy(100 + BOARD_SIZE * 50 + 100, 200, _T("�ǳ��ã��Ͳ�һ�㣡"));
	}
	else if (chess_count <= 6)
	{
		outtextxy(100 + BOARD_SIZE * 50 + 100, 200, _T("���ۣ�����Ŷ��"));
	}
	else if (chess_count <= 9)
	{
		outtextxy(100 + BOARD_SIZE * 50 + 100, 200, _T("���ۣ�һ�㣬����Ŭ����"));
	}
	else
	{
		outtextxy(100 + BOARD_SIZE * 50 + 100, 200, _T("���ۣ��´μ���Ŷ��"));
	}
}

void print_algo_limit(bool endgame){
	settextcolor(RED);
	if (endgame)
	{
		outtextxy(100 + BOARD_SIZE * 50 + 100, 150, _T("�о��޷�ʹ����ʾ���ܣ�"));
	}
	else {
		outtextxy(100 + BOARD_SIZE * 50 + 100, 150, _T("��ǰ���������࣬��������ʾ��"));
	}
	
}

void print_undo_limit() {
	settextcolor(RED);
	outtextxy(100 + BOARD_SIZE * 50 + 100, 150, _T("��ǰ�޷�������"));
}

void print_help() {
	/*
	���Ŀ���Ǿ����ܵؽ������ϵ��������Ӷ��Ƴ����̣�ʹ�����ֻʣ��һ�����ӡ�
	ÿ��ʹ���������ѡ��һ�����ӣ�ѡ�е����ӻ��к�ɫ��ǡ�
	ѡ�����Ӻ�����ֱ�����ڵ�һ�����ӣ������յĿ�������������ӱ��Ƴ����̡�
	ÿ���ƶ����������������ĸ����򣬵�������б��
	���û�п����ƶ������ӣ���Ϸ������
	����Ϸ�����У����Ե���Ҳ�ġ���������ť��������һ���Ĳ��������Ǹù��ܽ�������ʹ��һ�Ρ�
	���������������һ������ʱ�����Ե���Ҳ�ġ���ʾ����ť����ʾ��ǰ��ѵ��ƶ�������
	*/
	clear_screen();
	settextcolor(BLACK);
	settextstyle(20, 0, _T("����"));
	outtextxy(100, 100, _T("���Ŀ���Ǿ����ܵؽ������ϵ��������Ӷ��Ƴ����̣�ʹ�����ֻʣ��һ�����ӡ�"));
	outtextxy(100, 150, _T("ÿ��ʹ���������ѡ��һ�����ӣ�ѡ�е����ӻ��к�ɫ��ǡ�"));
	outtextxy(100, 200, _T("ѡ�����Ӻ�����ֱ�����ڵ�һ�����ӣ������յĿ�������������ӱ��Ƴ����̡�"));
	outtextxy(100, 250, _T("ÿ���ƶ����������������ĸ����򣬵�������б��"));
	outtextxy(100, 300, _T("���û�п����ƶ������ӣ���Ϸ������"));
	outtextxy(100, 350, _T("����Ϸ�����У����Ե���Ҳ�ġ���������ť��������һ���Ĳ������ù��ܽ�������ʹ��һ�Ρ�"));
	outtextxy(100, 400, _T("���������������һ������ʱ�����Ե���Ҳ�ġ���ʾ����ť����ʾ��ǰ��ѵ��ƶ�������"));

	// ��Բ�μ�ͷ���ذ�ť
	draw_return_arrow();

	while (1)
	{
		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			int cx = m.x;
			int cy = m.y;
			if (cx >= 20 && cx <= 80 && cy >= 20 && cy <= 80)
			{
				break;
			}
		}
	}
}

void draw_return_arrow() {
	// ��Բ�μ�ͷ���ذ�ť
	setfillcolor(BLUE);
	fillcircle(45, 48, 25);

	// ���Ƽ�ͷ
	setfillcolor(WHITE);
	POINT arrow[7] = { {60, 45}, {40, 45}, {40, 40}, {30, 50}, {40, 60}, {40, 55}, {60, 55} };
	fillpolygon(arrow, 7);
}

void wait_for_confirm() {
	Button confirm_button(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 100, 50, L"ȷ��", []() {	});
	confirm_button.draw();
	while (1)
	{
		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			int cx = m.x;
			int cy = m.y;
			//std::cout << cx << ' ' << cy << std::endl;
			if (cx >= WINDOW_WIDTH / 2 - 50 && cx <= WINDOW_WIDTH / 2 + 50 && cy >= WINDOW_HEIGHT / 2 - 25 && cy <= WINDOW_HEIGHT / 2 + 25) {
				break;
			}
		}
	}
}