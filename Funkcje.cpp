#include "Funkcje.h"

// Funkcje glowne --------------------------------------------------------------------------------------------------------

bool wczytajPrzelaczniki(int ile, char* params[], string& plikGrupy, string& plikStatystyki, bool& czyUsuwac, bool& czyBilans)
{
	// Jesli podano jakis przelacznik musimy sie upewnic ze podano parametr -fin
	bool jestFin = false;;

	// Iterowanie po kazdym z przelacznikow
	for (int i = 1; i < ile; i++)
	{
		if ((string)params[i] == "-fin")
		{
			// Sprawdzenie czy da sie otworzyc dany plik z grupami
			fstream filestr;
			filestr.open(params[i + 1]);
			if (!filestr.is_open())
			{
				cout << "Podanego pliku z grupami studentow nie udalo sie otworzyc." << endl;
				return false;
			}
			jestFin = true;
			plikGrupy = params[++i];
		}
		else if ((string)params[i] == "-fgrupa")
		{
			// Sprawdzenie czy plik ze statystykami ma poprawna nazwe
			string nazwaPliku = params[i + 1];
			string kropka = ".";
			size_t indeksKropki = nazwaPliku.find(kropka);
			if (indeksKropki != string::npos)
			{
				string rozszerzenie = nazwaPliku.substr(indeksKropki, nazwaPliku.size() - 1);
				if (rozszerzenie != ".txt")
				{
					cout << "Niepoprawna nazwa pliku ze statystykami - rozszerzeniem pliku powinno byc '.txt'." << endl;
					return false;
				}
				plikStatystyki = params[++i];
			}
			else
			{
				cout << "Niepoprawna nazwa pliku ze statystykami - brakuje rozszerzenia pliku." << endl;
				return false;
			}
		}
		else if ((string)params[i] == "-zalUsun")
		{
			czyUsuwac = true;
		}
		else if ((string)params[i] == "-bilans")
		{
			czyBilans = true;
		}
	}

	// Jesli uzytkownik podal jakies przelaczniki, ale nie bylo wsrod nich -fin, zglos blad
	if (!jestFin)
	{
		cout << "Aby przetworzyc dane nalezy uzyc przelacznika -fin \"plik.txt\"." << endl;
		return false;
	}

	return true;
}

Grupa* wczytajDane(string plikGrupy)
{
	string linia = "";
	ifstream plik(plikGrupy);
	Grupa* listaGrup = nullptr;

	vector <Student*> listaStudentow;
	listaStudentow.push_back(nullptr);
	int i = 0;

	// Dodatkowe zabezpieczenie przed niepoprawnym plikiem.
	if (!plik)
	{
		return listaGrup;
	}

	// Iterujemy po pliku z danymi.
	while (getline(plik, linia))
	{
		// Zakladamy ze imiona w pliku wejsciowym rozpoczynaja sie od wielkiej litery.

		// Jezeli dana linia zawiera numer grupy - podwieszamy liste studentow do poprzedniej grupy
		if (linia[0] == 'g')
		{
			if (listaStudentow[i])
			{
				podwiesListe(listaGrup, listaStudentow[i]);
				i++;
				listaStudentow.push_back(nullptr);
			}
			int numerGrupy = stoi(linia.substr(2, linia.size() - 3));
			dodajNaKoniecRekurencyjnie(listaGrup, numerGrupy);
		}
		// Jezeli dana linia zawiera studenta z ocenami.
		else
		{
			int indeksSpacji = linia.find(' ');
			int indeksSrednika = linia.find(';');
			string imieStudenta = linia.substr(0, indeksSpacji);
			string nazwiskoStudenta = linia.substr(indeksSpacji + 1, indeksSrednika - indeksSpacji - 1);
			linia.erase(linia.begin(), linia.begin() + indeksSrednika + 1);

			// Wczytywanie ocen studenta.
			vector<float> ocenyStudenta;
			while (true)
			{
				if (linia == "")
				{
					break;
				}
				else
				{
					indeksSrednika = linia.find(';');
					ocenyStudenta.push_back(stof(linia.substr(0, indeksSrednika)));
					linia.erase(linia.begin(), linia.begin() + indeksSrednika + 1);
				}
			}

			dodajNaKoniecRekurencyjnie(listaStudentow[i], imieStudenta, nazwiskoStudenta, ocenyStudenta);
		}
	}
	// Podwieszanie ostatniej listy studentow
	if (listaStudentow[i])
	{
		podwiesListe(listaGrup, listaStudentow[i]);
		i++;
		listaStudentow.push_back(nullptr);
	}

	plik.close();

	return listaGrup;
}

void zalUsun(Grupa*& pHead)
{
	Grupa* temp = pHead;

	// Iterujemy po grupach studentow.
	while (temp)
	{
		Student* glowaStudentow = temp->pHead;

		// Sprawdzamy pierwszego studenta - jesli nalezy go usunac to ustawiamy pierwszego studenta na kolejnego.
		while (glowaStudentow)
		{
			bool czyUsuniety = false;

			for (float ocena : glowaStudentow->oceny)
			{
				if (ocena < 3)
				{
					czyUsuniety = true;
				}
			}

			if (!czyUsuniety)
			{
				break;
			}
			else
			{
				auto p = glowaStudentow->pNext;
				delete glowaStudentow;
				glowaStudentow = p;
			}
		}

		temp->pHead = glowaStudentow;

		// Jesli cala grupa usunieta, od razu przechodzimy do kolejnej.
		if (!temp->pHead)
		{
			temp = temp->pNext;
			continue;
		}

		// Iterujemy po kolejnych studentach w tej grupie i usuwamy tych ktorzy nie uzyskali zaliczenia.
		while (temp->pHead->pNext)
		{
			bool czyUsuniety = false;

			for (float ocena : temp->pHead->pNext->oceny)
			{
				if (ocena < 3)
				{
					czyUsuniety = true;
				}
			}

			if (!czyUsuniety)
			{
				temp->pHead = temp->pHead->pNext;
			}
			else if (czyUsuniety)
			{
				auto p = temp->pHead->pNext->pNext;
				delete temp->pHead->pNext;
				temp->pHead->pNext = p;
			}
		}

		temp->pHead = glowaStudentow;

		temp = temp->pNext;
	}
}

void bilans(Grupa*& pHead)
{
	int iloscStudentow = policzStudentow(pHead);

	// Dopoki liczba grup sie nie zgadza usuwamy najmniejsza grupe i dodajemy studentow tam gdzie trzeba.
	while (policzGrupy(pHead) != (ceil)((float)(iloscStudentow) / 30))
	{
		// Ustalamy ktora grupa jest najmniejsza.
		Grupa* najmniejsza = najmniejszaGrupa(pHead);

		// Dodajemy studentow z tej grupy do grupy najwiekszej dopoki w tej grupie jest mniej niz 31 studentow.
		Grupa* najwieksza = najwiekszaGrupa(pHead);

		while (rozmiarGrupy(najwieksza) < 30)
		{
			// Jesli najmniejsza grupa ma co najmniej jedenego studenta to usuwamy ostatniego.
			if (najmniejsza->pHead)
			{
				Student* temp = najmniejsza->pHead;
				// Jesli grupa ma conajmniej dwa elementy to usuwamy ostatniego.
				if (temp->pNext)
				{
					while (temp->pNext->pNext)
					{
						temp = temp->pNext;
					}
					dodajStudenta(najwieksza, temp->pNext);
					temp->pNext = nullptr;
				}
				// Jesli grupa ma jeden element to przenosimy go i usuwamy grupe.
				else
				{
					dodajStudenta(najwieksza, najmniejsza->pHead);
					usunGrupe(pHead, najmniejsza);
					break;
				}
			}
			// Jesli grupa nie ma studentow to ja usuwamy
			else
			{
				usunGrupe(pHead, najmniejsza);
				break;
			}
		}
	}
}

void zapiszStatystyki(Grupa* pHead, string plikStatystyki)
{
	ofstream plik(plikStatystyki);

	// Iterujemy po grupach.
	while (pHead)
	{
		int liczbaStudentow = 0;
		float sredniaGrupy = 0;
		float najwyzszaSrednia = 0;

		Student* pHeadStudent = pHead->pHead;

		// Iterujemy po studentach.
		while (pHeadStudent)
		{
			float sredniaStudenta = 0;
			liczbaStudentow++;

			// Iterujemy po ocenach studenta.
			for (float ocena : pHeadStudent->oceny)
			{
				sredniaStudenta += ocena;
			}

			sredniaStudenta /= pHeadStudent->oceny.size();

			if (sredniaStudenta > najwyzszaSrednia)
			{
				najwyzszaSrednia = sredniaStudenta;
			}

			sredniaGrupy += sredniaStudenta;

			pHeadStudent = pHeadStudent->pNext;
		}
		sredniaGrupy /= liczbaStudentow;

		plik << "Grupa nr " << pHead->numer << ": " << endl;
		plik << "Liczba studentow: " << liczbaStudentow << endl;
		plik << "Srednia ocen w grupie: " << sredniaGrupy << endl;
		plik << "Najwyzsza srednia w grupie: " << najwyzszaSrednia << endl;
		plik << endl;

		pHead = pHead->pNext;
	}

	plik.close();
}

void zapiszGrupy(Grupa* pHead, string plikGrupy)
{
	ofstream plik(plikGrupy);

	// Iterujemy po grupach.
	while (pHead)
	{
		plik << "gr" << pHead->numer << ";" << endl;

		Student* pHeadStudent = pHead->pHead;

		// Iterujemy po studentach.
		while (pHeadStudent)
		{
			plik << pHeadStudent->imie << " " << pHeadStudent->nazwisko << ";";

			// Iterujemy po ocenach studenta.
			for (float ocena : pHeadStudent->oceny)
			{
				plik << ocena << ";";
			}

			plik << endl;

			pHeadStudent = pHeadStudent->pNext;
		}

		pHead = pHead->pNext;
	}

	plik.close();
}

void zwolnijPamiec(Grupa*& pHead)
{
	// Iterowanie po liscie grup
	while (pHead)
	{
		// Iterowanie po liscie studentow
		while (pHead->pHead)
		{
			auto p = pHead->pHead->pNext;
			delete pHead->pHead;
			pHead->pHead = p;
		}

		auto p = pHead->pNext;
		delete pHead;
		pHead = p;
	}
}

// Funkcje pomocnicze ----------------------------------------------------------------------------------------------------

void dodajNaKoniecRekurencyjnie(Grupa*& pHead, int numer)
{
	if (not pHead)
	{
		pHead = new Grupa{ numer, nullptr, nullptr };
	}
	else
	{
		dodajNaKoniecRekurencyjnie(pHead->pNext, numer);
	}
}

void dodajNaKoniecRekurencyjnie(Student*& pHead, string imie, string nazwisko, vector<float> oceny)
{
	if (not pHead)
	{
		pHead = new Student{ imie, nazwisko, oceny, nullptr };
	}
	else
	{
		dodajNaKoniecRekurencyjnie(pHead->pNext, imie, nazwisko, oceny);
	}
}

void podwiesListe(Grupa*& pHeadGrupa, Student*& pHeadStudent)
{
	Grupa* temp = pHeadGrupa;

	// Iterujemy az dostaniemy sie do ostatniego elementu listy grup.
	while (temp->pNext)
	{
		temp = temp->pNext;
	}

	// Ustawiamy jego wskaznik na liste studentow.
	temp->pHead = pHeadStudent;
}

int rozmiarGrupy(Grupa* pGrupa)
{
	int rozmiar = 0;

	Student* temp = pGrupa->pHead;

	while (temp)
	{
		rozmiar++;
		temp = temp->pNext;
	}

	return rozmiar;
}

void dodajStudenta(Grupa*& pGrupa, Student* pStudent)
{
	if (pGrupa->pHead)
	{
		Student* temp = pGrupa->pHead;

		while (temp->pNext)
		{
			temp = temp->pNext;
		}

		temp->pNext = pStudent;
		pStudent->pNext = nullptr;
	}
}

int policzGrupy(Grupa* pHead)
{
	int ilosc = 0;
	while (pHead)
	{
		pHead = pHead->pNext;
		ilosc++;
	}
	return ilosc;
}

int policzStudentow(Grupa* pHead)
{
	int ilosc = 0;
	while (pHead)
	{
		Student* temp = pHead->pHead;
		while (temp)
		{
			temp = temp->pNext;
			ilosc++;
		}
		pHead = pHead->pNext;
	}
	return ilosc;
}

Grupa* najmniejszaGrupa(Grupa* pHead)
{
	if (pHead)
	{
		Grupa* najmniejsza = pHead;
		int najmniejszaIlosc = rozmiarGrupy(pHead);

		Grupa* temp = pHead->pNext;

		while (temp)
		{
			if (rozmiarGrupy(temp) < najmniejszaIlosc)
			{
				najmniejsza = temp;
				najmniejszaIlosc = rozmiarGrupy(temp);
			}
			temp = temp->pNext;
		}
		return najmniejsza;
	}
	else
		return nullptr;
}

Grupa* najwiekszaGrupa(Grupa* pHead)
{
	Grupa* temp = pHead;

	if (temp)
	{
		Grupa* najwieksza = nullptr;
		int najwiekszaIlosc = 0;

		while (temp)
		{
			// Jesli rozmiar obecnej jest wiekszy od najwiekszej do tej pory
			// oraz jest mniejszy niz 30 (poniewaz szukamy najliczniejszej grupy ktora nie jest zapelniona)
			// oraz obecna grupa nie jest rownoczesnie najmniejsza (poniewaz wtedy bilans zapisywalby z tej samej do tej samej)
			// to podmieniamy.
			if ((rozmiarGrupy(temp) > najwiekszaIlosc) && (rozmiarGrupy(temp) < 30) && (temp != najmniejszaGrupa(pHead)))
			{
				najwieksza = temp;
				najwiekszaIlosc = rozmiarGrupy(temp);
			}
			temp = temp->pNext;
		}
		return najwieksza;
	}
	else
		return nullptr;
}

void usunGrupe(Grupa*& pHead, Grupa* pGrupa)
{
	Grupa* temp = pHead;

	if (pHead == pGrupa)
	{
		auto p = pHead->pNext;
		delete pHead;
		pHead = p;
		return;
	}

	while (temp->pNext)
	{
		if (temp->pNext == pGrupa)
		{
			auto p = temp->pNext->pNext;
			delete temp->pNext;
			temp->pNext = p;
			return;
		}

		temp = temp->pNext;
	}
}