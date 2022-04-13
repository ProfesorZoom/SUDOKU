#define _CRT_SECURE_NO_WARNINGS

#include "Sudoku.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <conio.h>
#include <Windows.h>

//metoda wywolujaca instancje generatora
void Sudoku::generateGrid() {
	SudokuDLX* gen = new SudokuDLX();			//tworzymy nowa instancje SudokuDLX w celu skorzystania z generatora
	grid = gen->generateSudoku(difficulty);
	delete gen;
}
//metoda zapisujaca dane do pliku wyjsciowego
void Sudoku::exitSave() {
	int ch;
	Field* ptr = grid;
	FILE* exit_save;
	//sprawdzenie czy da sie stworzyc plik ze stanem gry
	while ((exit_save = fopen("exitsave.sav", "w")) == NULL) {
		system("cls");
		std::wcout << L"Nie udalo sie stworzyc pliku z zapisem." << std::endl << L"P - probuj ponownie. K - kontynuuj bez zapisu.";
		ch = _getch();
		if (ch == 'k') {
			system("cls");
			exit(0);
		}
		else if (ch == 'p') {
			system("cls");
			continue;
		}
	}
	//zapis pol sudoku do pliku ze stanem gry
	while (ptr != NULL) {
		ptr->printToFile(exit_save);
		ptr = ptr->R_LINK;
	}
	fclose(exit_save);
}
//metoda wczytujaca dane z pliku
bool Sudoku::loadExitSave() {
	int ch;
	bool read_correct_type = 1;
	Field* ptr = grid;
	FILE* load_save = fopen("exitsave.sav", "r");	//otworzenie pliku z zapisem
	//sprawdzenie czy plik nie jest pusty oraz czy ma poprawna zawartosc
	if ((ch = getc(load_save)) == EOF || !(ch == 'c' || ch == 'v'))  {
		system("cls");
		std::wcout << L"Nie udalo sie odczytac pliku z zapisem." << std::endl << L"M - powrot do menu. Esc - wyjdz z gry.";
		fclose(load_save);
		std::remove("exitsave.sav");
		ch = _getch();
		if (ch == 27) {
			system("cls");
			exit(0);
		}
		else if (ch == 'm') {
			system("cls");
			return 0;
		}
	}
	else {
		ungetc(ch, load_save);
	}
	//petla wczytujaca dane z pliku do listy typu Field
	for (int i = 0; i < SIZE_SQUARE; ++i) {
		ch = getc(load_save);
		//sprawdzenie czy w pliku jest dobry kod typu dziedziczonego po Field
		if (!(ch == 'c' || ch == 'v') || !read_correct_type) {
			system("cls");
			std::wcout << L"Nieprawidlowe dane w pliku z zapisem." << std::endl << L"M - powrot do menu. Esc - wyjdz z gry.";
			fclose(load_save);
			std::remove("exitsave.sav");
			ch = _getch();
			if (ch == 27) {
				system("cls");
				exit(0);
			}
			else if (ch == 'm') {
				system("cls");
				return 0;
			}
		}
		//sprawdzenie i przypisanie odpowiedniego typu zgodnie z danymi z pliku ze stanem gry
		if (grid == NULL) {
			if (ch == 'c') {
				grid = ptr = new FieldConst();
			}
			else if (ch == 'v') {
				grid = ptr = new FieldVar();
			}
		}
		else {
			if (ch == 'c') {
				ptr->R_LINK = new FieldConst();
				ptr = ptr->R_LINK;
			}
			else if (ch == 'v') {
				ptr->R_LINK = new FieldVar();
				ptr = ptr->R_LINK;
			}
		}
		read_correct_type = ptr->readFromFile(load_save);	//wczytanie odpowiednich wartosci do utworzonego wezla listy
	}
	fclose(load_save);
	//sprawdzenie czy pobrane z pliku dane sa poprawne
	if (!(this->checkValidity())) {
		system("cls");
		std::wcout << L"Nieprawidlowe dane w pliku z zapisem." << std::endl << L"M - powrot do menu. Esc - wyjdz z gry.";
		std::remove("exitsave.sav");
		ch = _getch();
		if (ch == 27) {
			system("cls");
			exit(0);
		}
		else if (ch == 'm') {
			system("cls");
			return 0;
		}
	}
	return 1;
}
//metoda sprawdzajaca czy sudoku zawiera odpowiednia ilosc odpowiednich liczb
// wywolywana w celu sprawdzenia czy plik z zapisem nie byl zmodyfikowany w niewlasciwy sposob
bool Sudoku::checkValidity() {
	Field* ptr = grid;
	int amount[SIZE_DOKU] = { 0 };
	while (ptr != NULL) {
		amount[ptr->getValue() - 1]++;	//zliczamy ilosc wystapien cyfr
		ptr = ptr->R_LINK;
	}
	for (int i = 0; i < SIZE_DOKU; ++i) {
		if (amount[i] > SIZE_DOKU) {	//jezeli ilosc cyfr jest wieksza od 9 plansza nie jest poprawna
			return 0;
		}
	}
	if (!(this->checkSolvability()))
		return 0;
	return 1;
}
//metoda sprawdzajaca czy obecne sudoku da sie rozwiazac
bool Sudoku::checkSolvability() {
	SudokuDLX* checker = new SudokuDLX;				//tworzymy nowa instancje SudokuDLX w celu sprawdzenia czy istnieje rozwiazanie
	bool is_solvable = checker->checkSolvability(grid);		//sprawdzamy czy istnieje rozwiazanie
	delete checker;
	return is_solvable;
}
//metoda sprawdzajaca czy wszystkie pola sa poprawnie zapelnione
bool Sudoku::checkSolution() {
	Field* ptr = grid;
	bool is_solved = 1;
	while (ptr != NULL) {
		if (ptr->getValue() != ptr->getMaskValue())
			is_solved = 0;
		ptr = ptr->R_LINK;
	}
	return is_solved;
}
//metoda sprawdzajaca czy wszystkie pola sa zapelnione
bool Sudoku::checkCompletion() {
	Field* ptr = grid;
	bool is_completed = 1;
	while (ptr != NULL) {
		if (ptr->getMaskValue() == 0)
			is_completed = 0;
		ptr = ptr->R_LINK;
	}
	return is_completed;
}
//metoda rozwiazujaca sudoku automatycznie
bool Sudoku::autoSolve() {
	COORD cursor_coordinates{ 2, 1 };
	Field* ptr = grid;
	while (ptr != NULL) {
		ptr->setMaskValue(ptr->getValue());
		cursor_coordinates = { (short)ptr->getCoordY() * 4 - 2, (short)ptr->getCoordX() * 2 - 1 };
		moveCursorToCoords(cursor_coordinates);
		ptr->print();
		ptr = ptr->R_LINK;
	}
	return 1;
}
//glowna metoda sterujaca gra
void Sudoku::solveSudoku() {
	bool is_auto_solved = 0;
	int ch = 1;
	Field* ptr;
	//ustawiamy odpowiedni rozmiar i pozycje kursora
	COORD cursor_coordinates{ 2, 1 }, temp{ 0, 30 };
	CONSOLE_CURSOR_INFO cursor{ 99, TRUE };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor_coordinates);
	//petla wykonujaca sie dopoki sudoku nie jest rozwiazane poprawnie
	while (!(this->checkCompletion() && (this->checkSolution()))) {
		ch = _getch();		//pobranie znaku od uzytkownika
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), temp);
		std::wcout << L"                                                                                  " << std::endl;
		backCursor(cursor_coordinates);
		//wywolanie odpowiednich funkcji/metod w zaleznosci od wejscia od uzytkownika
		if (ch == 'w') {
			if (cursor_coordinates.Y != 1)
				moveCursorUp(cursor_coordinates);
		}
		else if (ch == 's') {
			if (cursor_coordinates.Y != 17)
				moveCursorDown(cursor_coordinates);
		}
		else if (ch == 'a') {
			if (cursor_coordinates.X != 2)
				moveCursorLeft(cursor_coordinates);
		}
		else if (ch == 'd') {
			if (cursor_coordinates.X != 34)
				moveCursorRight(cursor_coordinates);
		}
		else if (ch >= '0' && ch <= '9') {
			//zmiana wartosci w polu na planszy
			ptr = this->searchForCoords((cursor_coordinates.Y + 1) / 2, (cursor_coordinates.X + 2) / 4);
			ptr->setMaskValue(ch - '0');
			ptr->print();
			backCursor(cursor_coordinates);
		}
		else if (ch == 27) {
			//wyjscie z gry
			temp = { 0, 0 };
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), temp);
			system("cls");
			exit(0);
		}
		else if (ch == 'm') {
			//powrot do menu
			temp = { 0, 0 };
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), temp);
			system("cls");
			return;
		}
		else if (ch == 'r') {
			//automatyczne rozwiazanie
			is_auto_solved = this->autoSolve();
		}
		else if (ch == 'p') {
			//wyjscie do menu i zapisanie stanu gry
			this->exitSave();
			system("cls");
			return;
		}
		if (this->checkCompletion() && !(this->checkSolution())) {
			//przy wypelnionej planszy wyswietlenie informacji o niepoprawnych wartosciach w polach
			temp = { 0, 30 };
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), temp);
			std::wcout << L"Cos jest nie tak. Sprawdz swoje rozwiazanie." << std::endl;
			backCursor(cursor_coordinates);
		}
	}
	if (this->checkSolution()) {
		//koniec wlasciwej rozgrywki, wyswietlenie wiadomosci koncowych i wybor opcji
		temp = { 0, 30 };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), temp);
		if (is_auto_solved)
			std::wcout << L"Nacisnij esc by opuscic gre, M by wrocic do menu." << std::endl;
		else
			std::wcout << L"Gratulacje! Rozwiazales poprawnie sudoku!" << std::endl << L"Nacisnij esc by opuscic gre, M by wrocic do menu." << std::endl;
		remove("exitsave.sav");
		backCursor(cursor_coordinates);
		while (1) {
			ch = _getch(); {
				if (ch == 27) {
					//wyjdz z gry
					system("cls");
					exit(0);
				}
				else if (ch == 'm')
					//wyjdz do menu
					break;
			}
		}
	}
	system("cls");
}
//Metoda wyswietlajaca plansze w oparciu o wygenerowany uklad cyfr
void Sudoku::printSudoku() {
	printGrid<Field>(L"\u2554", L"\u2557", L"\u255A", L"\u255D", L"\u2550", L"\u2551", L"\u256C", L"\u2566", L"\u2569", L"\u2560", L"\u2563", L"\u2502", L"\u2500", L"\u253C", grid);
	std::wcout << L"Sterowanie: " << std::endl << L"WASD\t-Ruch po planszy" << std::endl << L"1-9\t-Wstawienie danej cyfry w pole z kursorem" << std::endl;
	std::wcout << L"0\t-Usuniecie cyfry z pola" << std::endl << L"Esc\t-Wyjscie bez zapisu" << std::endl << L"P\t-Zapis i powrot do menu" << std::endl;
	std::wcout << L"M\t-Powrot do menu" << std::endl << L"R\t-Rozwiaz automatycznie" << std::endl;
}
//metoda szukajaca w liscie pola o odpowiednich koordynatach
Field* Sudoku::searchForCoords(int x, int y) {
	return grid->searchForCoords(x, y);
}

//Funkcje do poczatkowego narysowania planszy
void printSpacerRow(const wchar_t* first, const wchar_t* cross_section, const wchar_t* cross_section_box, const wchar_t* last, const wchar_t* btwn) {
	std::wcout << first;
	for (int k = 0; k < 2; ++k) {
		for (int j = 0; j < 2; ++j) {
			printThreeTimes(btwn);
			std::wcout << cross_section;
		}
		printThreeTimes(btwn);
		std::wcout << cross_section_box;
	}
	for (int j = 0; j < 2; ++j) {
		printThreeTimes(btwn);
		std::wcout << cross_section;
	}
	printThreeTimes(btwn);
	std::wcout << last << std::endl;
}
void printExtremeRow(const wchar_t* corner_W, const wchar_t* corner_E, const wchar_t* frame_cross, const wchar_t* frame_horizontal) {
	printSpacerRow(corner_W, frame_horizontal, frame_cross, corner_E, frame_horizontal);
}
void printThreeTimes(const wchar_t* character) {
	for (int i = 0; i < 3; ++i) {
		std::wcout << character;
	}
}
//Funkcje ruszajace kursorem po planszy
void moveCursorUp(COORD& cursor_coords) {
	cursor_coords.Y -= 2;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor_coords);
}
void moveCursorDown(COORD& cursor_coords) {
	cursor_coords.Y += 2;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor_coords);
}
void moveCursorLeft(COORD& cursor_coords){
	cursor_coords.X -= 4;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor_coords);
}
void moveCursorRight(COORD& cursor_coords){
	cursor_coords.X += 4;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor_coords);
}
void backCursor(COORD& cursor_coords) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor_coords);
}
void moveCursorToCoords(COORD& cursor_coords) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor_coords);
}
