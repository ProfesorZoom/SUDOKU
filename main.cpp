#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <time.h>
#include "Menu.h"
#include "Sudoku.h"

using namespace std;

int main() {

	_setmode(_fileno(stdout), _O_U16TEXT);		//Ustawienie trybu wyswietlania konsoli na unicode
	srand(time(NULL));							//Inicjalizacja ziarna generatora liczb losowych
	while (true) {
		Menu* menu = new Menu;				//tworzenie nowej instancji menu
		menu->showActions();				//wyswietlenie menu
		Sudoku* doku = new Sudoku;			//utworzenie instancji sudoku
		if ((doku = menu->chooseAction()) == NULL) {		//Wybranie akcji z menu
			delete menu;
			continue;
		}
		//wyswietlenie sudoku i glowna metoda sterujaca rozgrywka
		doku->printSudoku();
		doku->solveSudoku();
	}
	return 0;
}
