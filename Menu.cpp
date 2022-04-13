#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include "Menu.h"
#include <conio.h>
#include <Windows.h>
#include "Sudoku.h"

bool Menu::checkExitSave() {
	FILE* save_ptr;
	if ((save_ptr = fopen("exitsave.sav", "r")) != NULL) {
		got_exit_save = 1;
		fclose(save_ptr);
	}
	else {
		got_exit_save = 0;
	}
	return got_exit_save;
}
void Menu::showActions() {
	COORD cursor_coordinates{ 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor_coordinates);
	std::wcout << L"Sterowanie menu: W/S - wybor opcji, Enter - zatwierdzenie." << std::endl;
	if (checkExitSave()) {
		std::wcout << L" Kontynuuj" << std::endl;
	}
	std::wcout << L" Nowa gra" << std::endl;
	std::wcout << L" Wyjdz z gry" << std::endl;
}
Sudoku* Menu::chooseAction() {
	checkExitSave();
	//ustawiamy pozycje i rozmiar kursora
	CONSOLE_CURSOR_INFO cursor{99, TRUE};
	COORD cursor_coordinates{ 0, 1 };
	int ch = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor_coordinates);
	//czekamy na nacisniecie odpowiedniego przycisku przez uzytkownika
	while (ch != 13) {
		ch = _getch();
		//przesuwamy kursor w gore lub w dol
		if (ch == 'w') {
			if (cursor_coordinates.Y != 1)
				cursor_coordinates.Y--;
		}
		else if (ch == 's') {
			if (cursor_coordinates.Y != (2 + got_exit_save))
				cursor_coordinates.Y++;
		}
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor_coordinates);
	}
	if (got_exit_save) {
		switch (cursor_coordinates.Y) {
		case 1:
			return continueGame();
			break;
		case 2:
			return newGame();
			break;
		case 3:
			exitGame();
			break;
		}
	}
	else {
		switch (cursor_coordinates.Y) {
		case 1:
			return newGame();
			break;
		case 2:
			exitGame();
			break;
		}
	}
}
int Menu::chooseDifficulty() {
	int difficulty = 1;
	int ch;
	system("cls");
	std::wcout << L"Podaj poziom trudnosci: 1 - latwy, 2 - sredni, 3 - trudny" << std::endl;
	while (true) {
		//pobieramy od uzytkownika odpowiedni klawisz
		ch = _getch();
		if (ch == '1') {
			difficulty = 1;
			break;
		}
		else if (ch == '2') {
			difficulty = 2;
			break;
		}
		else if (ch == '3') {
			difficulty = 3;
			break;
		}
	}
	return difficulty;
}
Sudoku* Menu::continueGame() {
	COORD cursor_coordinates{ 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor_coordinates);
	system("cls");
	Sudoku* sudoku_n = new Sudoku();	
	if (!(sudoku_n->loadExitSave()))	//wywolujemy i sprawdzamy metode wczytujaca z pliku
		return NULL;
	else
		return sudoku_n;
}
Sudoku* Menu::newGame() {
	int difficulty = this->chooseDifficulty();
	COORD cursor_coordinates{ 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor_coordinates);
	system("cls");
	Sudoku* sudoku_n = new Sudoku(difficulty);
	sudoku_n->generateGrid();	//wywolujemy metode generujaca plansze
	return sudoku_n;
}
void Menu::exitGame() {
	COORD cursor_coordinates{ 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor_coordinates);
	system("cls");
	exit(0);
}