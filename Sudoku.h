#pragma once
#include "Field.h"
#include "SudokuDLX.h"
#include <Windows.h>

//klasa reprezentujaca rozgrywke
class Sudoku{
	//lsita pol na planszy sudoku
	Field* grid;
	int difficulty;
public:
	Sudoku(): grid(NULL), difficulty(1){}
	Sudoku(int _difficulty): grid(NULL), difficulty(_difficulty){}
	~Sudoku() { delete grid; }
	//metoda generujaca siatke
	void generateGrid();
	//metoda zapisujaca stan gry.
	void exitSave();
	//metoda wczytujaca zapis gry, 
	// wczytanie pomyslne = 1, niepomyslne = 0
	bool loadExitSave();
	//metoda sprawdzajaca czy obecna plansza w ogole jest plansza sudoku
	//uzywana przy sprawdzeniu ingerencji w plik z zapisem
	// plansza jest poprawna = 1, plansza nie jest poprawna = 0
	bool checkValidity();
	//metoda sprawdzajaca czy obecna plansza ma unikalne rozwiazanie
	// ma unikalne rozwiazanie = 1, nie ma = 0
	bool checkSolvability();
	//metoda sprawdzajaca czy sudoku zostalo rozwiazane poprawnie
	// zostalo rozwiazane poprawnie = 1, nie zostalo rozwiazane poprawnie = 0
	bool checkSolution();
	//metoda sprawdzajaca czy cala plansza jest wypelniona
	// jest wypelniona = 1, nie jest wypelniona = 0
	bool checkCompletion();
	//metoda rozwiazujaca sudoku automatycznie
	// zostala rozwiazana = 1, nie zostala rozwiazana = 0
	bool autoSolve();
	//glowna metoda sterujaca gra sudoku
	void solveSudoku();
	//metoda wypisujaca siatke sudoku i obecne wartosci na ekran
	void printSudoku();
	//metoda wypisujaca plansze, przyjmujaca jako argumenty znaki unicode odpowiadajace poszczegolnym elementom siatki
	template <typename T>
	void printGrid(const wchar_t* corner_NW, const wchar_t* corner_NE, const wchar_t* corner_SW, const wchar_t* corner_SE,
		const wchar_t* frame_horizontal, const wchar_t* frame_vertical, const wchar_t* frame_cross,
		const wchar_t* frame_cross_N, const wchar_t* frame_cross_S, const wchar_t* frame_cross_W, const wchar_t* frame_cross_E,
		const wchar_t* middle_vertical, const wchar_t* middle_horizontal, const wchar_t* middle_cross, T* ptr);
	//metoda szukajaca odpowiedniego elementu na liscie pol
	Field* searchForCoords(int _x, int y);
};

//Funkcje do wypisania planszy
template <typename T>
void printDataRow(const wchar_t* first, const wchar_t* cross_section, const wchar_t* cross_section_box, const wchar_t* last, T*& ptr);
void printSpacerRow(const wchar_t* first, const wchar_t* cross_section, const wchar_t* cross_section_box, const wchar_t* last, const wchar_t* btwn);
void printExtremeRow(const wchar_t* corner_W, const wchar_t* corner_E, const wchar_t* frame_cross, const wchar_t* frame_horizontal);
void printThreeTimes(const wchar_t* character);
//Funkcje do poruszanai kursorem po planszy
void moveCursorUp(COORD& cursor_coords);
void moveCursorDown(COORD& cursor_coords);
void moveCursorLeft(COORD& cursor_coords);
void moveCursorRight(COORD& cursor_coords);
void backCursor(COORD& cursor_coords);
void moveCursorToCoords(COORD& cursor_coords);

//funkcja rysujaca na ekranie  siatke z odpowiadajacymi jej danymi
// wypisze liste dowolnego typu T pod warunkiem ze:
// posiada on publiczna metode print()
// w liscie jest co najmniej 81 elementow
// dla elementow szerszych niz 1 pole w konsoli dojdzie do znieksztalcen
template <typename T>
void Sudoku::printGrid(const wchar_t* corner_NW, const wchar_t* corner_NE, const wchar_t* corner_SW, const wchar_t* corner_SE,
	const wchar_t* frame_horizontal, const wchar_t* frame_vertical, const wchar_t* frame_cross,
	const wchar_t* frame_cross_N, const wchar_t* frame_cross_S, const wchar_t* frame_cross_W, const wchar_t* frame_cross_E,
	const wchar_t* middle_vertical, const wchar_t* middle_horizontal, const wchar_t* middle_cross, T* ptr) {
	printExtremeRow(corner_NW, corner_NE, frame_cross_N, frame_horizontal);
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			printDataRow(frame_vertical, middle_vertical, frame_vertical, frame_vertical, ptr);
			printSpacerRow(frame_vertical, middle_cross, frame_vertical, frame_vertical, middle_horizontal);
		}
		printDataRow(frame_vertical, middle_vertical, frame_vertical, frame_vertical, ptr);
		printSpacerRow(frame_cross_W, frame_horizontal, frame_cross, frame_cross_E, frame_horizontal);
	}
	for (int j = 0; j < 2; ++j) {
		printDataRow(frame_vertical, middle_vertical, frame_vertical, frame_vertical, ptr);
		printSpacerRow(frame_vertical, middle_cross, frame_vertical, frame_vertical, middle_horizontal);
	}
	printDataRow(frame_vertical, middle_vertical, frame_vertical, frame_vertical, ptr);
	printExtremeRow(corner_SW, corner_SE, frame_cross_S, frame_horizontal);
}
//funkcja rysujaca na ekranie wiersz siatki z odpowiadajacymi mu danymi
// wypisze liste dowolnego typu T pod warunkiem ze:
// posiada on publiczna metode print()
// w liscie jest co najmniej 81 elementow
// dla elementow szerszych niz 1 pole w konsoli dojdzie do znieksztalcen
template <typename T>
void printDataRow(const wchar_t* first, const wchar_t* cross_section, const wchar_t* cross_section_box, const wchar_t* last, T*& ptr) {
	std::wcout << first;
	for (int k = 0; k < 2; ++k) {
		for (int j = 0; j < 2; ++j) {
			std::wcout << " ";
			ptr->print();
			ptr = ptr->R_LINK;
			std::wcout << " ";
			std::wcout << cross_section;
		}
		std::wcout << " ";
		ptr->print();
		ptr = ptr->R_LINK;
		std::wcout << " ";
		std::wcout << cross_section_box;
	}
	for (int j = 0; j < 2; ++j) {
		std::wcout << " ";
		ptr->print();
		ptr = ptr->R_LINK;
		std::wcout << " ";
		std::wcout << cross_section;
	}
	std::wcout << " ";
	ptr->print();
	ptr = ptr->R_LINK;
	std::wcout << " " << last << std::endl;
}