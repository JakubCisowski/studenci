#include "Funkcje.h"
#include "nvwa/debug_new.h"	// Sprawdzanie wyciekow.

int main(int ile, char* params[])
{
	string plikGrupy = "";
	string plikStatystyki = "";
	bool czyUsuwac = false, czyBilans = false;

	// Wczytywanie przelacznikow podanych przez liste polecen.
	if (wczytajPrzelaczniki(ile, params, plikGrupy, plikStatystyki, czyUsuwac, czyBilans))
	{
		// Tworzenie listy podwieszanej na podstawie pliku.
		Grupa* listaGrup = wczytajDane(plikGrupy);

		// Jesli uzytkownik podal przelacznik -zalUsun, usuwamy uczniow z ocena ponizej 3.
		if (czyUsuwac)
		{
			zalUsun(listaGrup);
		}

		// Jesli uzytkownik podal przelacznik -bilans, wykonujemy bilans grup.
		if (czyBilans)
		{
			bilans(listaGrup);
		}

		// Jesli uzytkownik podal przelacznik -fgrupa, zapisujemy statystyki do pliku.
		if (plikStatystyki != "")
		{
			zapiszStatystyki(listaGrup, plikStatystyki);
		}

		// Zapisujemy stan grup do pliku grupy.
		zapiszGrupy(listaGrup, plikGrupy);

		// Zwalniamy pamiec - usuwamy strukture.
		zwolnijPamiec(listaGrup);

		cout << "Program zostal wykonany pomyslnie, zamykam program." << endl;
	}
	else
	{
		cout << "Nie udalo sie poprawnie wczytac przelacznikow, zamykam program." << endl;
	}

	return 0;
}