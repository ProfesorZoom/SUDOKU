#pragma once
#include <stdlib.h>
#include <stdio.h>

//klasa reprezentujaca pole na planszy sudoku
class Field{
protected:
	//pola odpowiadajace kolejno wspolrzednym x, y na planszy, wartosci faktycznej pola, wartosci wpisanej przez uzytkownika
	int coord_x;
	int coord_y;
	int value;
	int mask_value;
public:
	Field* R_LINK;
	Field(): coord_x(0), coord_y(0), value(0), mask_value(0), R_LINK(NULL){}
	Field(int x, int y, int val, int mask_val) : coord_x(x), coord_y(y), value(val), mask_value(mask_val), R_LINK(NULL){}
	~Field() { delete R_LINK; }
	//metody modyfikujace wartosci pol obiektu
	void setCoordX(int x) { coord_x = x; }
	void setCoordY(int y) { coord_y = y; }
	void setCoords(int x, int y) { coord_x = x; coord_y = y; }
	void setValue(int val) { value = val; }
	virtual void setMaskValue(int m_val) { mask_value = m_val; }
	//metody zwracajace wartosci pol obiektu
	int getCoordX() { return coord_x; }
	int getCoordY() { return coord_y; }
	int getValue() { return value; }
	int getMaskValue() { return mask_value; }
	virtual int getMaskGenValue() { return 0; }
	//metoda wypisujaca wartosc pola maska w konsoli
	virtual void print();
	//metoda szukajaca rekursywnie pola o koordynatach x, y
	//zwraca wskaznik na dane pole
	virtual Field* searchForCoords(int x, int y);
	//metoda wypisujaca wartosci pol obiektu do pliku w postaci zaszyfrowanej
	virtual void printToFile(FILE* output_stream) {}
	//metoda wczytujaca wartosci pol obiektu z pliku
	bool readFromFile(FILE* input_stream);
};
//klasa reprezentujaca pole ze wskazowka na planszy sudoku
class FieldConst : public Field {
public:
	FieldConst() : Field(){}
	FieldConst(int x, int y, int val, int mask_val) : Field(x, y, val, mask_val) {}
	//metoda modyfikujaca pole maski dla obiektu danego podtypu klasy Field
	void setMaskValue(int m_val) { mask_value = value; }
	//metoda zwracajaca wartosc obiektu okreslona przy wygenerowaniu planszy
	int getMaskGenValue() { return mask_value; }
	//metoda wypisujaca wartosc pola maska w konsoli
	void print();
	//metoda wypisujaca wartosci pol obiektu do pliku w postaci zaszyfrowanej
	void printToFile(FILE* output_stream);
};
//klasa reprezentujaca puste pole na planszy sudoku
class FieldVar : public Field {
public:
	FieldVar() : Field() {}
	FieldVar(int x, int y, int val, int mask_val) : Field(x, y, val, mask_val) {}
	//metoda modyfikujaca pole maski dla obiektu danego podtypu klasy Field
	void setMaskValue(int m_val) { mask_value = m_val; }
	//metoda zwracajaca wartosc obiektu okreslona przy wygenerowaniu planszy
	int getMaskGenValue() { return 0; }
	//metoda wypisujaca wartosc pola maska w konsoli
	void print();
	//metoda wypisujaca wartosci pol obiektu do pliku w postaci zaszyfrowanej
	void printToFile(FILE* output_stream);
};
