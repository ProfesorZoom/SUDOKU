#pragma once

#include "Sudoku.h"

//klasa reprezentujaca menu glowne
class Menu{
	bool got_exit_save;
public:
	//Metoda sprawdza czy istnieje plik zapisu
	//Jezeli tak, zwraca true, w przeciwnym wypadku false
	bool checkExitSave();		
	//Metoda wyswietla akcje jakie moze wybrac uzytkownik
	void showActions();	
	//Metoda pobierajaca akcje uzytkownika i inicjujaca instancje gry
	Sudoku* chooseAction();
	//Metoda pobierajaca poziom trudnosci dla nowej gry
	int chooseDifficulty();
	//Metoda tworzaca instancje, bazujaca na wygenerowanej planszy
	Sudoku* newGame();
	//Metoda tworzaca instancje, bazujaca na planszy wczytanej z pliku zapisu
	Sudoku* continueGame();
	//Metoda wychodzaca z gry
	void exitGame();			
};
