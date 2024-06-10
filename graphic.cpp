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

	settextstyle(30, 0, _T("黑体"));
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
	// 设计主菜单
	clear_screen();

	// 绘制棋盘和棋子背景

	// 绘制黑色网格
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
	// 绘制标题：孔明棋
	settextstyle(50, 0, _T("宋体"));
	settextcolor(GREEN);
	outtextxy(WINDOW_WIDTH / 2 - 100 , 100, _T("孔明棋"));



	Button start_button(WINDOW_WIDTH / 2 - 200 , WINDOW_HEIGHT / 2, 150, 50, L"经典模式", start_game);
	Button endgame_start_button(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 150, 50, L"残局模式", start_endgame);
	Button help_button(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 100, 150, 50, L"帮助", print_help);
	Button quit_button(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 200, 150, 50, L"退出", []() { exit(0); });
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
	settextstyle(30, 0, _T("黑体"));
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


		
	// 写剩余棋子的数量
	settextcolor(BLUE);
	TCHAR s[100];
	_stprintf_s(s, _T("剩余棋子数量：%d"), chess_count);
	outtextxy(left + BOARD_SIZE * 50 + 100, 50, s);

	// 画重置按钮
	Button reset_button(left + BOARD_SIZE * 50 + 200, 300, 100, 50, L"重置", []() {});
	reset_button.draw();

	// 画撤销按钮
	Button undo_button(left + BOARD_SIZE * 50 + 200, 400, 100, 50, L"撤销", [](){});
	undo_button.draw();

	// 画算法按钮
	Button algorithm_button(left + BOARD_SIZE * 50 + 200, 500, 100, 50, L"提示", []() {});
	algorithm_button.draw();

	// 画建议走法
	if (suggestion_x1 != -1 && suggestion_y1 != -1 && suggestion_x2 != -1 && suggestion_y2 != -1)
	{
		setlinecolor(GREEN);
		setlinestyle(PS_SOLID, 3);
		rectangle(left + suggestion_y1 * 50 - 25, upper + suggestion_x1 * 50 - 25, left + suggestion_y1 * 50 + 25, upper + suggestion_x1 * 50 + 25);
		rectangle(left + suggestion_y2 * 50 - 25, upper + suggestion_x2 * 50 - 25, left + suggestion_y2 * 50 + 25, upper + suggestion_x2 * 50 + 25);
		suggestion_x1 = suggestion_y1 = suggestion_x2 = suggestion_y2 = -1;
	}

	// 画圆形箭头返回按钮
	draw_return_arrow();

	// 画选中框
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
					std::cout << "当前无法撤销！" << std::endl;
					wait_for_confirm();
					break;
				}
			}
			// 监听算法
			else if (cx >= left + BOARD_SIZE * 50 + 200 - 50 && cx <= left + BOARD_SIZE * 50 + 200 + 50 && cy >= 500 - 50 && cy <= 500 + 50)
			{
				if (is_endgame)
				{
					print_algo_limit(is_endgame);
					std::cout << "残局无法使用提示功能！" << std::endl;
					wait_for_confirm();
					break;
				}
				if (chess_count < 18)
				{
					min_chess_count = chess_count;
					int min_chess_count_backup = min_chess_count;
					suggestion(1);
					std::cout << "当前最佳剩余棋子数：" << min_chess_count << std::endl;
					min_chess_count = min_chess_count_backup;
					std::cout << suggestion_x1 << " " << suggestion_y1 << " " << suggestion_x2 << " " << suggestion_y2 << std::endl;
				}
				else
				{
					print_algo_limit();
					std::cout << "当前棋子数过多，无法计算！" << std::endl;
					wait_for_confirm();
				}
				break;
			}
			// 重置
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
			// 返回
			else if (cx >= 20 && cx <= 80 && cy >= 20 && cy <= 80)
			{
				is_quit = 1;
				break;
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
void print_lose(int chess_count) {
	settextcolor(RED);
	outtextxy(100 + BOARD_SIZE * 50 + 100, 150, _T("没有可以移动的棋子，您输了！"));
	// 根据棋子数量给出评价
	if (chess_count <= 3)
	{
		outtextxy(100 + BOARD_SIZE * 50 + 100, 200, _T("非常好，就差一点！"));
	}
	else if (chess_count <= 6)
	{
		outtextxy(100 + BOARD_SIZE * 50 + 100, 200, _T("评价：不错哦！"));
	}
	else if (chess_count <= 9)
	{
		outtextxy(100 + BOARD_SIZE * 50 + 100, 200, _T("评价：一般，继续努力！"));
	}
	else
	{
		outtextxy(100 + BOARD_SIZE * 50 + 100, 200, _T("评价：下次加油哦！"));
	}
}

void print_algo_limit(bool endgame){
	settextcolor(RED);
	if (endgame)
	{
		outtextxy(100 + BOARD_SIZE * 50 + 100, 150, _T("残局无法使用提示功能！"));
	}
	else {
		outtextxy(100 + BOARD_SIZE * 50 + 100, 150, _T("当前棋子数过多，不允许提示！"));
	}
	
}

void print_undo_limit() {
	settextcolor(RED);
	outtextxy(100 + BOARD_SIZE * 50 + 100, 150, _T("当前无法撤销！"));
}

void print_help() {
	/*
	你的目标是尽可能地将棋盘上的所有棋子都移出棋盘，使得最后只剩下一个棋子。
	每次使用鼠标点击，选定一颗棋子，选中的棋子会有红色标记。
	选中棋子后，跳过直接相邻的一颗棋子，跳到空的孔里，被跳过的棋子被移出棋盘。
	每次移动可以是上下左右四个方向，但不能是斜向。
	如果没有可以移动的棋子，游戏结束。
	在游戏过程中，可以点击右侧的“撤销”按钮，撤销上一步的操作，但是该功能仅可连续使用一次。
	当你的棋子数少于一定数量时，可以点击右侧的“提示”按钮，提示当前最佳的移动方案。
	*/
	clear_screen();
	settextcolor(BLACK);
	settextstyle(20, 0, _T("黑体"));
	outtextxy(100, 100, _T("你的目标是尽可能地将棋盘上的所有棋子都移出棋盘，使得最后只剩下一个棋子。"));
	outtextxy(100, 150, _T("每次使用鼠标点击，选定一颗棋子，选中的棋子会有红色标记。"));
	outtextxy(100, 200, _T("选中棋子后，跳过直接相邻的一颗棋子，跳到空的孔里，被跳过的棋子被移出棋盘。"));
	outtextxy(100, 250, _T("每次移动可以是上下左右四个方向，但不能是斜向。"));
	outtextxy(100, 300, _T("如果没有可以移动的棋子，游戏结束。"));
	outtextxy(100, 350, _T("在游戏过程中，可以点击右侧的“撤销”按钮，撤销上一步的操作，该功能仅可连续使用一次。"));
	outtextxy(100, 400, _T("当你的棋子数少于一定数量时，可以点击右侧的“提示”按钮，提示当前最佳的移动方案。"));

	// 画圆形箭头返回按钮
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
	// 画圆形箭头返回按钮
	setfillcolor(BLUE);
	fillcircle(45, 48, 25);

	// 绘制箭头
	setfillcolor(WHITE);
	POINT arrow[7] = { {60, 45}, {40, 45}, {40, 40}, {30, 50}, {40, 60}, {40, 55}, {60, 55} };
	fillpolygon(arrow, 7);
}

void wait_for_confirm() {
	Button confirm_button(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 100, 50, L"确定", []() {	});
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