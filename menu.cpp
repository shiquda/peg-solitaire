#include <iostream>
#include <conio.h>

#include "core.h"

using namespace std;

void menu() {
	clearScren();
	int choice;
	choice = _getch();
	switch (choice) {
	case '1':
		startGame();
		break;
	case '2':
		print_exit();
		exit(0);
		break;
	}
}

void clearScren() {
	system("cls");
}

void print_menu() {
	cout << "1. ��ʼ��Ϸ" << endl;
	cout << "2. �˳���Ϸ" << endl;
	cout << "���������ѡ��";
}

void print_exit()
{
	cout << "��л���棡" << endl;
}