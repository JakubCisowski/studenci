#ifndef STRUKTURY_H
#define STRUKTURY_H
#include "Funkcje.h"
using namespace std;

/** Lista jednokierunkowa studentow w danej grupie */
struct Student
{
	string imie;		 ///< Imie studenta.
	string nazwisko;	 ///< Nazwisko studenta.
	vector<float> oceny; ///< Wektor ocen studenta.
	Student* pNext;		 ///< Wskaznik na nastepny element listy studentow.
};

/** Lista jednokierunkowa podwieszana grup studentow */
struct Grupa
{
	int numer;		///< Numer grupy.
	Student* pHead; ///< Wskaznik na pierwszy element listy studentow.
	Grupa* pNext;	///< Wskaznik na nastepny element listy grup.
};

#endif
