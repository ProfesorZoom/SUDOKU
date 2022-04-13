#pragma once

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "Sudoku.h"

#define _CRT_SECURE_NO_WARNINGS
#define MAX_RECORD 1024
#define SIZE_DOKU 9

const int ITEMS_NUMBER = SIZE_DOKU * SIZE_DOKU * 4;
const int OPTIONS_NUMBER = SIZE_DOKU * SIZE_DOKU * SIZE_DOKU;
const int SIZE_SQUARE = SIZE_DOKU * SIZE_DOKU;
const int SIZE_SQRT = sqrt(SIZE_DOKU);

//struktura reprezentujaca wêze³ listy dynamicznej potrzebnej w algorytmie X
struct Node {
	//wskazniki na odpowiednie inne wezly
	Node* LLINK;	
	Node* RLINK;
	Node* ULINK;
	Node* DLINK;
	Node* TOP;		//wskaznik na naglowek kolumny
	int LEN;		//uzywane przez naglowki kolumn: ilosc wezlow polaczonych pionowo w kolumnie
	int column;		//kolumna w sudoku
	int row;		//rzad w sudoku
	int number;		//cyfra w sudoku
	Node() :LLINK(NULL), RLINK(NULL), ULINK(NULL), DLINK(NULL), TOP(NULL), LEN(0), column(-1), row(-1), number(-1) {}
	//metody odpowiadajace za przyslanianie kolumn i rzedow
	void hideRow();
	void coverColumn();
	void unHideRow();
	void unCoverColumn();
};
//klasa reprezentujaca elementy programu korzystajace z algorytmu X w celu:
//wygenerowania nowej planszy sudoku
//sprawdzenia czy dane sudoku ma rozwiazanie
class SudokuDLX{
	//pola odpowiadajace za poprawna prace algorytmu:
	int is_solved;								//przechowuje ilosc rozwiazan danej planszy sudoku
	bool matrix[OPTIONS_NUMBER][ITEMS_NUMBER];	//macierz 0,1 potrzebna do utworzenia listy dla algorytmu X
	int solution_index;							//index wezlow bedacych krokami w rozwiazaniu, potrzebny do sterowania algorytmem
	int grid[SIZE_DOKU][SIZE_DOKU];				//tablica do przechowywania bezposredniego wartosci wygenerowanych pol planszy
	bool mask[SIZE_DOKU][SIZE_DOKU];			//maska uzywana przy generacji do przyslonienia pelnej planszy, aby uzyskac plansze czesciowo zapelniona
	bool to_be_generated;						//zmienna kontrolna algorytmu X
public:
	//pole zawierajace adres pierwszego elementu listy wyjsciowej rozwiazania
	Field* puzzle;
	//lista wezlow bedacych krokami w rozwiazaniu
	Node* solution[MAX_RECORD] = { NULL };
	//naglowek listy potrzebnej do wykonania algorytmu X
	Node* list;
	SudokuDLX() : is_solved(0), solution_index(0), list(NULL), to_be_generated(0){}
	~SudokuDLX();
	//metoda wstawiajaca 0 do tablicy wartosci pol planszy
	//potrzebna przy generacji planszy
	void clearGrid();
	//metoda budujaca z macierzy 0,1 liste potrzebna algorytmowi X
	void buildLinkedList();
	//metoda interpretujaca elementy listy krokow rozwiazania jako plansza sudoku
	void mapSolutionToGrid();
	//rekursywny algorytm dla problemów typu "Exact Cover"
	//macierz 0,1 reprezentuje odpowiednie warunki(kolumny) i opcje(wiersze)
	//algorytm x szuka takich opcji aby wszystkie warunki by³y spe³nione (1)
	//robi to poprzez przykrywanie kolumn i wierszy listy dwuwymiarowej
	//wykonujac operacje na polach na inne elementy listy
	//kroki ktore algorytm podejmie sa zarazem rozwiazaniem problemu
	void algorithmX();
	//metoda modyfikujaca liste, pomniejszajac ja o poczatkowe warunki w planszy(grid)
	void modifyListByGrid();
	//metoda wypelniajaca macierz 0,1 tak, by odpowiadala ona warunkom i opcjom dla sudoku
	void fillSparseMatrix();
	//metoda rozwiazujaca sudoku, ktorego wartosci sa w tablicy grid
	void solveSudoku();
	//metoda przywracajaca polaczenia wezlow listy dla algorytmu dlx po jej wykonaniu aby mozliwe bylo pozniejsze usuniecie jej
	void restoreList();
	//metoda zerujaca maske potrzebna do ponownego wygenerowania maski w przypadku gdy maska nie zapewnia unikalnego rozwiazania sudoku
	void clearMask();
	//metoda sprawdzajaca czy plansza sudoku podana poprzez wskaznik na pierwszy element listy typu Field ma dokladnie 1 rozwiazanie
	//zwraca 1 jezeli tak, 0 jezeli nie
	bool checkSolvability(Field* board);
	//metoda generujaca maske pelnej planszy sudoku
	//difficulty przyjmuje wartosci 1-latwy 2-sredni 3-trudny
	//zwraca ilosc pol bedacych wskazowka
	int generateMask(int difficulty);
	//metoda generujaca sudoku dla podanego poziomu trudnosci difficulty
	//difficulty przyjmuje wartosci 1-latwy 2-sredni 3-trudny
	//zwraca wskaznik na pierwszy element wygenerowanej listy
	Field* generateSudoku(int difficulty);
	//metoda tworzaca liste dynamiczna pol planszy sudoku na podstawie tablicy grid
	//zwraca wskaznik na pierwszy element listy
	Field* convertGridToPuzzle();
};



