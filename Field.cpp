#include "Field.h"
#include <stdio.h>
#include <iostream>
#include <Windows.h>

void Field::print() {
	//wypisanie na ekran wartosci
	if (mask_value != 0)
		std::wcout << mask_value;
	else
		std::wcout << ' ';
}
Field* Field::searchForCoords(int x, int y) {
	//rekurencyjne szukanie w liscie pola o danych koordynatach
	if (coord_x == x && coord_y == y)
		return this;
	else if (R_LINK == NULL)
		return NULL;
	else
		return R_LINK->searchForCoords(x, y);
}
bool Field::readFromFile(FILE* input_stream) {
	//wczytanie danych z pliku
	coord_x = getc(input_stream) - 'A';
	coord_y = getc(input_stream) - 'A';
	value = getc(input_stream) - 'A';
	mask_value = getc(input_stream) - 'A';
	if (coord_x < 1 || coord_x > 9 || coord_y < 1 || coord_y > 9 || value < 1 || value > 9 || mask_value < 0 || mask_value>9) {
		return 0;
	}
	return 1;
}
void FieldConst::print() {
	//wypisanie wartosci na ekranie
	if (mask_value != 0) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
		std::wcout << mask_value;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	}
	else
		std::wcout << ' ';
}
void FieldConst::printToFile(FILE* output_stream) {
	//wypisanie wartosci do pliku
	fputc('c', output_stream);
	fputc(coord_x + 'A', output_stream);
	fputc(coord_y + 'A', output_stream);
	fputc(value + 'A', output_stream);
	fputc(mask_value + 'A', output_stream);
}
void FieldVar::print() {
	//wypisanie wartosci na ekranie
	if (mask_value != 0)
		std::wcout << mask_value;
	else
		std::wcout << ' ';
}
void FieldVar::printToFile(FILE* output_stream) {
	//wypisanie wartosci do pliku
	fputc('v', output_stream);
	fputc(coord_x + 'A', output_stream);
	fputc(coord_y + 'A', output_stream);
	fputc(value + 'A', output_stream);
	fputc(mask_value + 'A', output_stream);
}