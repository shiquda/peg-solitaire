#include "core.h"

#include <iostream>

unsigned long long Game::board_to_number(int board[BOARD_SIZE][BOARD_SIZE]) {
	unsigned long long result = 0;
	unsigned long long pow = 1;
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			int value = board[i][j];
			if (value == -1)
				value = 2;  // 将 -1 映射到 2
			result += value * pow;
			pow *= 3;
		}
	}
	return result;
}

void Game::fill_record(int board[BOARD_SIZE][BOARD_SIZE], int x1, int y1, int x2, int y2, int min_chess_count) {
	// 辅助函数：计算棋盘状态并保存到memo
	auto save_to_memo = [&](int b[BOARD_SIZE][BOARD_SIZE], int x1, int y1, int x2, int y2) {
		memo[board_to_number(b)] = { x1, y1, x2, y2, min_chess_count };
	};

	int board_90[BOARD_SIZE][BOARD_SIZE];
	int board_180[BOARD_SIZE][BOARD_SIZE];
	int board_270[BOARD_SIZE][BOARD_SIZE];
	int board_flip[BOARD_SIZE][BOARD_SIZE];
	int board_90_flip[BOARD_SIZE][BOARD_SIZE];
	int board_180_flip[BOARD_SIZE][BOARD_SIZE];
	int board_270_flip[BOARD_SIZE][BOARD_SIZE];

	// 旋转和翻转
	rotate_board_90(board, board_90);
	rotate_board_180(board, board_180);
	rotate_board_270(board, board_270);
	flip_board(board, board_flip);
	flip_board(board_90, board_90_flip);
	flip_board(board_180, board_180_flip);
	flip_board(board_270, board_270_flip);

	// 计算变换后的坐标
	int new_x1_90 = y1, new_y1_90 = BOARD_SIZE - 1 - x1;
	int new_x2_90 = y2, new_y2_90 = BOARD_SIZE - 1 - x2;

	int new_x1_180 = BOARD_SIZE - 1 - x1, new_y1_180 = BOARD_SIZE - 1 - y1;
	int new_x2_180 = BOARD_SIZE - 1 - x2, new_y2_180 = BOARD_SIZE - 1 - y2;

	int new_x1_270 = BOARD_SIZE - 1 - y1, new_y1_270 = x1;
	int new_x2_270 = BOARD_SIZE - 1 - y2, new_y2_270 = x2;

	int new_x1_flip = x1, new_y1_flip = BOARD_SIZE - 1 - y1;
	int new_x2_flip = x2, new_y2_flip = BOARD_SIZE - 1 - y2;

	int new_x1_90_flip = BOARD_SIZE - 1 - new_x1_90, new_y1_90_flip = new_y1_90;
	int new_x2_90_flip = BOARD_SIZE - 1 - new_x2_90, new_y2_90_flip = new_y2_90;

	int new_x1_180_flip = BOARD_SIZE - 1 - new_x1_180, new_y1_180_flip = new_y1_180;
	int new_x2_180_flip = BOARD_SIZE - 1 - new_x2_180, new_y2_180_flip = new_y2_180;

	int new_x1_270_flip = BOARD_SIZE - 1 - new_x1_270, new_y1_270_flip = new_y1_270;
	int new_x2_270_flip = BOARD_SIZE - 1 - new_x2_270, new_y2_270_flip = new_y2_270;

	// 保存所有状态到memo
	save_to_memo(board, x1, y1, x2, y2);
	save_to_memo(board_90, new_x1_90, new_y1_90, new_x2_90, new_y2_90);
	save_to_memo(board_180, new_x1_180, new_y1_180, new_x2_180, new_y2_180);
	save_to_memo(board_270, new_x1_270, new_y1_270, new_x2_270, new_y2_270);
	save_to_memo(board_flip, new_x1_flip, new_y1_flip, new_x2_flip, new_y2_flip);
	save_to_memo(board_90_flip, new_x1_90_flip, new_y1_90_flip, new_x2_90_flip, new_y2_90_flip);
	save_to_memo(board_180_flip, new_x1_180_flip, new_y1_180_flip, new_x2_180_flip, new_y2_180_flip);
	save_to_memo(board_270_flip, new_x1_270_flip, new_y1_270_flip, new_x2_270_flip, new_y2_270_flip);
}

// 旋转和翻转函数实现
void Game::rotate_board_90(int board[BOARD_SIZE][BOARD_SIZE], int new_board[BOARD_SIZE][BOARD_SIZE]) {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			new_board[j][BOARD_SIZE - 1 - i] = board[i][j];
		}
	}
}

void Game::rotate_board_180(int board[BOARD_SIZE][BOARD_SIZE], int new_board[BOARD_SIZE][BOARD_SIZE]) {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			new_board[BOARD_SIZE - 1 - i][BOARD_SIZE - 1 - j] = board[i][j];
		}
	}
}

void Game::rotate_board_270(int board[BOARD_SIZE][BOARD_SIZE], int new_board[BOARD_SIZE][BOARD_SIZE]) {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			new_board[BOARD_SIZE - 1 - j][i] = board[i][j];
		}
	}
}

void Game::flip_board(int board[BOARD_SIZE][BOARD_SIZE], int new_board[BOARD_SIZE][BOARD_SIZE]) {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			new_board[i][BOARD_SIZE - 1 - j] = board[i][j];
		}
	}
}


void Game::suggestion(bool first) {
	const bool debug = 1;
	min_chess_count = std::min(chess_count, min_chess_count);



	//if (debug) std::cout << "min_chess_count: " << min_chess_count << '\n';
	if (min_chess_count == 1) return;
	int min_chess_count_copy = min_chess_count;
	//suggestion_x1 = suggestion_y1 = suggestion_x2 = suggestion_y2 = -1;

	// 计算当前局面对应的数字
	unsigned long long board_num = board_to_number(chessboard);
	if (memo.find(board_num) != memo.end()) { // 已有记录
		min_chess_count = std::min(memo[board_num].min_chess_count, min_chess_count);
		if (first) {
			suggestion_x1 = memo[board_num].x1;
			suggestion_y1 = memo[board_num].y1;
			suggestion_x2 = memo[board_num].x2;
			suggestion_y2 = memo[board_num].y2;
		}
		return;
	}

	bool flag = false; // 是否能继续递归
	//int chessboard_copy[BOARD_SIZE][BOARD_SIZE];
	//board_copy(chessboard_copy, chessboard);	
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (chessboard[i][j] == 1) {
				if (walk_is_valid(i, j, i - 2, j)) {
					walk(i, j, i - 2, j, true);
					suggestion();
					if (min_chess_count < min_chess_count_copy) {
						flag = 1;
						suggestion_x1 = i;
						suggestion_y1 = j;
						suggestion_x2 = i - 2;
						suggestion_y2 = j;
						if (debug) std::cout << "min_chess_count changed: " << min_chess_count << " at " << chess_count <<
							", walk:" << suggestion_x1 << ' ' << suggestion_y1 << ' ' << suggestion_x2 << ' ' << suggestion_y2 << '\n';
					}
					// 还原
					chessboard[i][j] = chessboard[i - 1][j] = 1;
					chessboard[i - 2][j] = 0;

					chess_count++;
					min_chess_count_copy = min_chess_count;
					if (min_chess_count == 1) break;
				}
				if (walk_is_valid(i, j, i + 2, j)) {
					walk(i, j, i + 2, j, true);
					suggestion();
					if (min_chess_count < min_chess_count_copy) {
						flag = 1;
						suggestion_x1 = i;
						suggestion_y1 = j;
						suggestion_x2 = i + 2;
						suggestion_y2 = j;
						if (debug) std::cout << "min_chess_count changed: " << min_chess_count << " at " << chess_count <<
							", walk:" << suggestion_x1 << ' ' << suggestion_y1 << ' ' << suggestion_x2 << ' ' << suggestion_y2 << '\n';
					}
					// 还原
					chessboard[i][j] = chessboard[i + 1][j] = 1;
					chessboard[i + 2][j] = 0;

					chess_count++;
					min_chess_count_copy = min_chess_count;
					if (min_chess_count == 1) break;
				}
				if (walk_is_valid(i, j, i, j - 2)) {
					walk(i, j, i, j - 2, true);
					suggestion();
					if (min_chess_count < min_chess_count_copy) {
						flag = 1;
						suggestion_x1 = i;
						suggestion_y1 = j;
						suggestion_x2 = i;
						suggestion_y2 = j - 2;
						if (debug) std::cout << "min_chess_count changed: " << min_chess_count << " at " << chess_count <<
							", walk:" << suggestion_x1 << ' ' << suggestion_y1 << ' ' << suggestion_x2 << ' ' << suggestion_y2 << '\n';
					}
					// 还原
					chessboard[i][j] = chessboard[i][j - 1] = 1;
					chessboard[i][j - 2] = 0;

					chess_count++;
					min_chess_count_copy = min_chess_count;
					if (min_chess_count == 1) break;
				}
				if (walk_is_valid(i, j, i, j + 2)) {
					walk(i, j, i, j + 2, true);
					suggestion();
					if (min_chess_count < min_chess_count_copy) {
						flag = 1;
						suggestion_x1 = i;
						suggestion_y1 = j;
						suggestion_x2 = i;
						suggestion_y2 = j + 2;
						if (debug) std::cout << "min_chess_count changed: " << min_chess_count << " at " << chess_count <<
							", walk:" << suggestion_x1 << ' ' << suggestion_y1 << ' ' << suggestion_x2 << ' ' << suggestion_y2 << '\n';
					}
					// 还原
					chessboard[i][j] = chessboard[i][j + 1] = 1;
					chessboard[i][j + 2] = 0;
					chess_count++;
					min_chess_count_copy = min_chess_count;
					if (min_chess_count == 1) break;
				}
			}
		}
	}


	//board_copy(chessboard, chessboard_copy);
	if (flag) // 此次还能走
	{
		fill_record(chessboard, suggestion_x1, suggestion_y1, suggestion_x2, suggestion_y2, min_chess_count);
	}
}