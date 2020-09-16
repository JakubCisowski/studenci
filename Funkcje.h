#ifndef FUNKCJE_H
#define FUNKCJE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Struktury.h"

using namespace std;

/**
	Funkcja wczytujaca przelaczniki podane przez linie polecen.
	@param ile Dlugosc tablicy z przelacznikami.
	@param params Tablica przelacznikow i ich argumentow.
	@param plikGrupy Plik wejsciowy opisujacy grupy studentow.
	@param plikStatystyki Plik ze statystykami na temat grup.
	@param czyUsuwac Zmienna okreslajaca czy uzytkownik chce usunac studentow ktorzy nie zaliczyli.
	@param czyBilans Zmienna okreslajaca czy uzytkownik chce przeprowadzic bilans grup.
	@return Zwraca true jesli udalo sie poprawnie wczytac przelaczniki.
*/
bool wczytajPrzelaczniki(int ile, char* params[], string& plikGrupy, string& plikStatystyki, bool& czyUsuwac, bool& czyBilans);

/**
	Funkcja wczytajaca dane z podanego pliku wejsciowego.
	@param plikGrupy Plik wejsciowy opisujacy grupy studentow.
	@return Nowoutworzona lista grup.
*/
Grupa* wczytajDane(string plikGrupy);

/**
	Funkcja dodajaca za pomoca rekurencji element na koniec listy grup.
	@param pHead Wskaznik na pierwszy element listy grup.
	@param numer Numer grupy ktora dodajemy.
*/
void dodajNaKoniecRekurencyjnie(Grupa*& pHead, int numer);

/**
	Funkcja dodajaca za pomoca rekurencji element na koniec listy studentow.
	@param pHead Wskaznik na pierwszy element listy studentow.
	@param imie Imie studenta.
	@param nazwisko Nazwisko studenta.
	@param oceny Wektor ocen studenta.
*/
void dodajNaKoniecRekurencyjnie(Student*& pHead, string imie, string nazwisko, vector<float> oceny);

/**
	Funkcja podwieszajaca liste studentow do ostatniej grupy z listy grup.
	@param pHeadGrupa Lista grup.
	@param pHeadStudent Lista studentow w danej grupie.
*/
void podwiesListe(Grupa*& pHeadGrupa, Student*& pHeadStudent);

/**
	Funkcja zapisujaca statystyki na temat grup do pliku o nazwie podanej jako przelacznik.
	@param pHead Wskaznik na pierwszy element listy grup.
	@param plikStatystyki Plik ze statystykami na temat grup o naziwe podanej jako przelacznik.
*/
void zapiszStatystyki(Grupa* pHead, string plikStatystyki);

/**
	Funkcja zapisujaca stan grup do pliku wejsciowego.
	@param pHead Wskaznik na pierwszy element listy grup.
	@param plikGrupy Plik wejsciowy o nazwie podanej jako przelacznik.
*/
void zapiszGrupy(Grupa* pHead, string plikGrupy);

/**
	Funkcja usuwajaca z grup stundetow ktorzy nie uzyskali zaliczenia (mieli ocene ponizej 3).
	@param pHead Wskaznik na pierwszy element listy grup.
*/
void zalUsun(Grupa*& pHead);

/**
	Funkcja zmniejszajaca ilosc grup oraz przenoszaca studentow tak, by zmniejszyc te ilosc.
	@param pHead Wskaznik na pierwszy element listy grup.
*/
void bilans(Grupa*& pHead);

/**
	Funkcja zliczajaca rozmiar grupy studentow.
	@param pGrupa Wskaznik na grupe ktora chcemy zliczyc.
	@return Rozmiar podanej grupy.
*/
int rozmiarGrupy(Grupa* pGrupa);

/**
	Funkcja dodajaca studenta do podanej grupy studentow.
	@param pGrupa Wskaznik na grupe do ktorej chcemy dodac studenta.
	@param pStudent Wskaznik na studenta ktorego chcemy dodac do grupy.
*/
void dodajStudenta(Grupa*& pGrupa, Student* pStudent);

/**
	Funkcja wyznaczajaca ilosc grup.
	@param pHead Wskaznik na pierwszy element listy grup.
	@return Ilosc grup.
*/
int policzGrupy(Grupa* pHead);

/**
	Funkcja wyznaczajaca ilosc studentow.
	@param pHead Wskaznik na pierwszy element listy grup.
	@return Ilosc studentow.
*/
int policzStudentow(Grupa* pHead);

/**
	Funkcja zwracajaca grupe z najmniejsza iloscia studentow.
	@param pHead Wskaznik na pierwszy element listy grup.
	@return Wskaznik na najmniej liczna grupe.
*/
Grupa* najmniejszaGrupa(Grupa* pHead);

/**
	Funkcja zwracajaca grupe z najwieksza iloscia studentow ponizej 30 i nie jest jednoczesnie najmniejsza z grup.
	@param pHead Wskaznik na pierwszy element listy grup.
	@return Wskaznik na najliczniejsza grupe ponizej 30 studentow nie bedaca jednoczesnie najmniejsza.
*/
Grupa* najwiekszaGrupa(Grupa* pHead);

/**
	Funkcja usuwajaca grupe z listy grup.
	@param pHead Wskaznik na pierwszy element listy grup.
	@param pGrupa Wskaznik na grupe ktora chcemy usunac.
*/
void usunGrupe(Grupa*& pHead, Grupa* pGrupa);

/**
	Funkcja usuwajaca z pamieci liste grup.
	@param pHead Wskaznik na pierwszy element listy grup.
*/
void zwolnijPamiec(Grupa*& pHead);

#endif