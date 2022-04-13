#include "SudokuDLX.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS

void Node::hideRow() {
	Node* q = this->RLINK;
	//wyrzucamy elementy rzedu z listy nie usuwajac ich ani ich wskaznikow na inne wezly
	while (q != this) {
		q->ULINK->DLINK = q->DLINK;
		q->DLINK->ULINK = q->ULINK;
		q->TOP->LEN--;
		q = q->RLINK;
	}
}
void Node::coverColumn() {
	Node* p = this->DLINK;
	//wyrzucamy elementy rzedu z listy nie usuwajac ich ani ich wskaznikow na inne wezly
	while (p != this) {
		p->hideRow();
		p = p->DLINK;
	}
	this->LLINK->RLINK = this->RLINK;
	this->RLINK->LLINK = this->LLINK;
}
void Node::unHideRow() {
	Node* q = this->LLINK;
	//przywracamy elementy rzedu do listy w kolejnosci odwrotnej
	while (q != this) {
		q->ULINK->DLINK = q;
		q->DLINK->ULINK = q;
		q->TOP->LEN++;
		q = q->LLINK;
	}
}
void Node::unCoverColumn() {
	Node* p;
	//przywracamy elementy kolumny do listy w kolejnosci odwrotnej
	this->LLINK->RLINK = this;
	this->RLINK->LLINK = this;
	p = this->ULINK;
	while (p != this) {
		p->unHideRow();
		p = p->ULINK;
	}
}
SudokuDLX::~SudokuDLX() {
	//usuwanie elementow listy
	Node* ptr = list->RLINK;
	Node* temp;
	while (ptr != list) {
		Node* vert = ptr->DLINK;
		while (vert != ptr) {
			temp = vert->DLINK;
			delete vert;
			vert = temp;
		}
		temp = ptr->RLINK;
		delete ptr;
		ptr = temp;
	}
	delete list;
}
void SudokuDLX::clearGrid() {
	//wstawiamy we wszystkie pola tablicy 0
	for (int i = 0; i < SIZE_DOKU; ++i) {
		for (int j = 0; j < SIZE_DOKU; ++j) {
			grid[i][j] = 0;
		}
	}
}
void SudokuDLX::buildLinkedList() {
	//tworzenie naglowka listy
	list = new Node;
	list->LLINK = list;
	list->RLINK = list;
	list->DLINK = list;
	list->ULINK = list;
	list->LEN = -1;
	list->TOP = list;
	Node* ptr = list;
	//tworzenie naglowkow kolumn
	for (int i = 0; i < ITEMS_NUMBER; ++i) {
		ptr->RLINK = new Node;
		ptr->RLINK->LLINK = ptr;
		ptr = ptr->RLINK;
		ptr->LEN = 0;
		ptr->ULINK = ptr;
		ptr->DLINK = ptr;
		ptr->RLINK = list;
		ptr->RLINK->LLINK = ptr;
		ptr->TOP = NULL;
	}
	//tworzenie wezlow reprezentujacych 1 w macierzy 0,1
	for (int i = 0; i < OPTIONS_NUMBER; ++i) {
		Node* col = list->RLINK;
		Node* prev = NULL;
		for (int j = 0; j < ITEMS_NUMBER; ++j, col = col->RLINK) {
			if (matrix[i][j]) {
				ptr = col;
				while (ptr->DLINK != col) {
					ptr = ptr->DLINK;
				}
				ptr->DLINK = new Node;
				ptr->DLINK->ULINK = ptr;
				ptr = ptr->DLINK;
				//przypisanie odpowiednich opcji
				ptr->row = i / SIZE_SQUARE + 1;
				ptr->column = (i / SIZE_DOKU) % SIZE_DOKU + 1;
				ptr->number = i % SIZE_DOKU + 1;
				ptr->LEN = -1;
				//przypisanie odpowiedniej lokacji w liscie
				if (prev == NULL) {
					prev = ptr;
					prev->RLINK = ptr;
				}
				ptr->LLINK = prev;
				ptr->RLINK = prev->RLINK;
				ptr->RLINK->LLINK = ptr;
				prev->RLINK = ptr;
				ptr->TOP = col;
				ptr->DLINK = col;
				col->LEN++;
				col->ULINK = ptr;
				prev = ptr;
			}
		}
	}
}
void SudokuDLX::mapSolutionToGrid() {
	//interpretujemy kroki rozwiazania jako plansze
	for (int i = 0; solution[i] != NULL; ++i) {
		grid[solution[i]->row - 1][solution[i]->column - 1] = solution[i]->number;
	}
}
void SudokuDLX::algorithmX() {
	//jezeli w liscie zostala sama glowa znaleziono rozwiazanie
	if (list->RLINK == list) {
		++is_solved;	//ilosc rozwiazan danego sudoku
		return;
	}
	//szukamy rozwiazania

	Node* column = list->RLINK, * ptr = column;
	//wybieramy deterministycznie kolumne o najmniejszej liczbie opcji
	column = list->RLINK, ptr = column->RLINK;
	while (ptr != list) {
		if (ptr->LEN < column->LEN)
			column = ptr;
		ptr = ptr->RLINK;
	}
	//przyslaniamy wybrana kolumne
	column->coverColumn();
	//przyslaniamy rekurencyjnie kolejne rzedy i kolumny
	Node* vertical = column->DLINK;
	while (vertical != column) {
		solution[solution_index++] = vertical;	//zapisujemy adres opcji z przyslanianej kolumny jako potencjalny krok w rozwiazaniu
		Node* horizontal = vertical->RLINK;
		//przyslaniamy kolumny wspolne rzedu (opcji) wybranego wczesniej
		while (horizontal != vertical) {
			horizontal->TOP->coverColumn();		
			horizontal = horizontal->RLINK;
		}
		algorithmX();	//rekursja
		if (to_be_generated && is_solved) {		//jezeli generujemy wypelnione sudoku mozemy sie zatrzymac na pierwszym znalezionym rozwiazaniu
			return;
		}
		else if (is_solved > 1) {	//w wypadku sprawdzenia ilosci rozwiazan sudoku sprawdzamy czy jest wiecej niz jedno
			return;
		}
		//cofamy operacje przyslaniania (backtracking)
		solution[solution_index--] = NULL;
		horizontal = vertical->LLINK;
		while (horizontal != vertical) {
			horizontal->TOP->unCoverColumn();
			horizontal = horizontal->LLINK;
		}
		vertical = vertical->DLINK;
	}
	column->unCoverColumn();

}
void SudokuDLX::modifyListByGrid() {
	//modyfikujemy liste o znane wskazowki
	//przyslaniamy kolumny o warunki spelnione przez wskazowki
	for (int i = 0; i < SIZE_DOKU; ++i) {
		for (int j = 0; j < SIZE_DOKU; ++j) {
			if (grid[i][j]) {
				Node* column = list->RLINK;
				while (column != list) {
					Node* ptr = column->DLINK;
					while (ptr != column) {
						if (ptr->number == grid[i][j] && (ptr->row - 1) == i && (ptr->column - 1) == j) {
							column->coverColumn();
							solution[solution_index] = ptr;
							++solution_index;
							Node* temp = ptr->RLINK;
							while (temp != ptr) {
								temp->TOP->coverColumn();
								temp = temp->RLINK;
							}
						}
						ptr = ptr->DLINK;
					}
					column = column->RLINK;
				}
			}
		}
	}
}
void SudokuDLX::fillSparseMatrix() {
	//Wypelnienie macierzy 0,1 
	//Ograniczenie 1. - rzad, kolumna
	for (int i = 0; i < OPTIONS_NUMBER; ++i) {
		matrix[i][i / SIZE_DOKU] = 1;
	}
	//Ograniczenie 2. - rzad, liczba
	for (int i = 0; i < OPTIONS_NUMBER; ++i) {
		matrix[i][SIZE_SQUARE + i / SIZE_SQUARE * SIZE_DOKU + i % SIZE_DOKU] = 1;
	}
	//Ograniczenie 3. - kolumna, liczba
	for (int i = 0; i < OPTIONS_NUMBER; ++i) {
		matrix[i][SIZE_SQUARE * 2 + i % SIZE_SQUARE] = 1;
	}
	//Ograniczenie 4. - pudelko(3x3), libcza
	for (int i = 0; i < OPTIONS_NUMBER; ++i) {
		matrix[i][SIZE_SQUARE * 3 + i / (SIZE_SQUARE * SIZE_SQRT) * SIZE_DOKU * SIZE_SQRT + i / (SIZE_DOKU * SIZE_SQRT) % SIZE_SQRT * SIZE_DOKU + i % SIZE_DOKU] = 1;
	}
}
void SudokuDLX::solveSudoku() {
	//wykonujemy kroki potrzebne do rozwiazania algorytmu X
	is_solved = 0;
	if (!matrix[0][0])
		this->fillSparseMatrix();			//Tworzymy macierz (0,1) do opisania problemu
	if (list == NULL) {
		this->buildLinkedList();			//Tworzymy liste potrzebna do funkcjonowania algorytmu X
	}
	this->modifyListByGrid();			//Upraszczamy liste o spelnione warunki
	this->algorithmX();					//Generujemy sudoku przez algorytm X				
}
void SudokuDLX::restoreList() {
	//przywracamy liste z algorytmu X na podstawie krokow rozwiazania
	while (solution[1]!= NULL){
		Node* temp = solution[--solution_index];
		solution[++solution_index] = NULL;
		solution_index--;
		Node* column = temp->TOP;
		Node* horizontal = temp->LLINK;
		while (horizontal != temp) {
			horizontal->TOP->unCoverColumn();
			horizontal = horizontal->LLINK;
		}
		column->unCoverColumn();
	}
}
void SudokuDLX::clearMask() {
	//wypelniamy maske zerami
	for (int i = 0; i < SIZE_DOKU; ++i) {
		for (int j = 0; j < SIZE_DOKU; ++j) {
			mask[i][j] = 0;
		}
	}
}
bool SudokuDLX::checkSolvability(Field* board) {
	//przechowujemy tymczasowo tablice juz istniejaca aby metoda solveSudoku jej nie zmodyfikowa³a
	int grid_temp[SIZE_DOKU][SIZE_DOKU];
	for (int i = 0; i < SIZE_DOKU; ++i) {
		for (int j = 0; j < SIZE_DOKU; ++j) {
			grid_temp[i][j] = grid[i][j];
		}
	}
	clearGrid();	//czyscimy plansze
	//wczytujemy plansze z podanej tablicy
	for (int i = 0; i < SIZE_DOKU; ++i) {
		for (int j = 0; j < SIZE_DOKU; ++j) {
			grid[i][j] = board->searchForCoords(i + 1, j + 1)->getMaskGenValue();
		}
	}
	this->solveSudoku(); //szukamy ilosci rozwiazan sudoku
	this->restoreList();
	for (int i = 0; i < SIZE_DOKU; ++i) {
		for (int j = 0; j < SIZE_DOKU; ++j) {
			grid[i][j] = grid_temp[i][j];
		}
	}
	if (is_solved != 1) { //jezeli sudoku nie ma unikalnego rozwiazania zwracamy false
		return 0;
	}
	else {	//jezeli sudoku ma unikalne rozwiazanie zwracamy true
		return 1;
	}
}
int SudokuDLX::generateMask(int difficulty) {
	//generujemy maske na podstawie poziomu trudnosci
	this->clearMask();
	int counter = 0;
	for (int i = 0; i < SIZE_DOKU; ++i) {
		for (int j = 0; j < SIZE_DOKU; ++j) {
			if (rand() % (difficulty + 1) == 0) {
				mask[i][j] = 1;
				++counter;
			}
		}
	}
	return counter;
}
Field* SudokuDLX::generateSudoku(int difficulty) {
	to_be_generated = 1;
	Field* doku_grid = new Field;
	//Losujemy pierwszy rzad, zeby generacja sudoku poszla sprawniej
	for (int j = 0; j < SIZE_DOKU; ) {
		bool existance_flag = 0;
		int k = rand() % SIZE_DOKU + 1;
		for (int i = 0; i < j; ++i) {
			if (k == grid[0][i]) {
				existance_flag = 1;
			}
		}
		if (!existance_flag) {
			grid[0][j] = k;
			++j;
		}
	}
	this->solveSudoku();				//Rozwiazujemy raz aby uzyskac pelna plansze
	this->mapSolutionToGrid();			//Interpretujemy rozwiazanie jako plansze sudoku
	this->restoreList();				//Przywracamy liste, abysmy mogli ja usunac i nie zostawic smieci
	to_be_generated = 0;
	//wygenerowanie prawidlowej maski i sprawdzenie czy dla niej plansza ma unikalne rozwiazanie
	bool gen_failed = 1;
	int grid_temp[SIZE_DOKU][SIZE_DOKU];
	while (gen_failed) {
		if (this->generateMask(difficulty) < 17) {	//nie istnieje znane sudoku (o standardowych zasadach) majace mniej niz 17 wskazowek
			continue;
		}
		doku_grid = this->convertGridToPuzzle();
		if (this->checkSolvability(doku_grid) == 1) {
			gen_failed = 0;
		}
		else {
			gen_failed = 1;
			delete doku_grid;
		}
	}
	return doku_grid;	//zwracamy plansze
}
Field* SudokuDLX::convertGridToPuzzle(){
	//tworzymy liste typu Field* zawierajaca gotowe do rozwiazania sudoku
	Field* ptr = NULL;
	for (int i = 0; i < SIZE_DOKU; ++i) {
		for (int j = 0; j < SIZE_DOKU; ++j) {
			if (mask[i][j] == 1) {
				if (puzzle == NULL || ptr == NULL)
					puzzle = ptr = new FieldConst(i + 1, j + 1, grid[i][j], grid[i][j]);
				else {
					ptr->R_LINK = new FieldConst(i + 1, j + 1, grid[i][j], grid[i][j]);
					ptr = ptr->R_LINK;
				}
			}
			else {
				if (puzzle == NULL || ptr == NULL)
					puzzle = ptr = new FieldVar(i + 1, j + 1, grid[i][j], 0);
				else {
					ptr->R_LINK = new FieldVar(i + 1, j + 1, grid[i][j], 0);
					ptr = ptr->R_LINK;
				}
			}
		}
	}
	return puzzle;
}


