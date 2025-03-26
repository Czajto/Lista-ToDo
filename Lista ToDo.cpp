#include <iostream>
#include <vector>
#include "folder_picker.h"
#include <string>
#include <fstream>
#include <filesystem>
#include <limits>
#include <cstdlib>  // dla system()

/*

X = NIE ZROBIONE
✓ = ZROBIONE

RZECZY DO ZROBIENIA!!!!
##############################
MOZLIWOSC EDYTOWANIA PODANYCH ZADAN Z LISTY (X)
WSZYSTKO MA LADNIE SIE WYSWIETLAC (X)
DODANIE FUNKCJI OZNACZANIA W DANYM PLIKU RZECZY ZROBIONYCH I NIE (X)
FUNKCJA KTORA CZYSCI DANE ZADANIA Z PLIKU (X)
FUNKCJA KTORA USUWA PLIKI Z KOMPUTERA (X)

MOZLIWOSC CZYSCZENIA OBECNEJ LISTY Z FUNKCJA "CZY NA PEWNO?" (✓)
DODAWANIE A NIE NADPISYWANIE DO ISTNIEJACYCH PLIKOW Z ZADANIAMI (✓)
############################
*/

using namespace std;
namespace fs = filesystem;

// Deklaracja funkcji potwierdzenia
bool potwierdzenie(const string& komunikat);
void StworzPlik(const string& nazwa, const string& sciezka);


void czyscEkran() {
#ifdef _WIN32
    system("cls");  // dla Windows
#else
    system("clear");  // dla Linux/macOS
#endif
}

int wybor() {
    int wybor;
    cout << "[1] Dodaj zadanie do listy" << endl;
    cout << "[2] Pokaz zadania z listy" << endl;
    cout << "[3] Usun zadanie z listy" << endl;
    cout << "[4] Edytuj zadanie z listy" << endl;
    cout << "[5] Zapisz do pliku" << endl;
    cout << "[6] Wczytaj z pliku" << endl;
    cout << "[7] Wyjscie" << endl;
    cin >> wybor;
    cin.ignore();
    return wybor;
}

void edytujZadanie(vector<string>& lista) {
    if (lista.empty()) { 
        return; 
    }
    int nrZadania;
    string noweZadanie;

    cout << "Wybierz zadanie do edycji: ";
    cin >> nrZadania;
    cout << "Twoje zadania do edycji to: " << lista[nrZadania - 1] << endl;
    cout << "Podaj tresc nowego zadania: ";
    cin >> noweZadanie;
    lista.insert(lista.begin() + nrZadania, noweZadanie);
    lista.erase(lista.begin() + nrZadania - 1);
}


void dodajZadanie(vector<string>& lista) {
    string zadanie;
    cout << "Podaj tresc zadania: ";
    getline(cin, zadanie);
    lista.push_back(zadanie);
    cout << "Dodano zadanie: \"" << zadanie << "\"" << endl;
}

void pokazywanieZadania(const vector<string>& lista) {
    if (lista.empty()) {
        cout << "Brak zadan na liscie!" << endl;
        return;
    }

    cout << "Twoje zadania:" << endl;
    for (size_t i = 0; i < lista.size(); i++) {
        cout << i + 1 << ". " << lista[i] << "." << endl;
    }
    cout << endl;
}

void usuwanieZadania(vector<string>& lista) {
    if (lista.empty()) {
        return;
    }

    size_t index;
    cout << "Podaj numer zadania do usuniecia: ";
    cin >> index;

    if (index < 1 || index > lista.size()) {
        cout << "Niepoprawny numer!" << endl;
        return;
    }

    cout << "Usunieto zadanie: \"" << lista[index - 1] << "\"!" << endl;
    lista.erase(lista.begin() + index - 1);
}

void zapiszDoPliku(const vector<string>& lista, string& nazwa, string& path) {
    string sciezka = path + nazwa + ".txt";
    ofstream plik(sciezka, ios::app);

    if (!plik) {
        cout << "Otwieranie pliku nie powiodlo sie" << endl;
        return;
    }

    for (const string& zadanie : lista) {
        plik << zadanie << endl;
    }

    plik.close();
    cout << "Zadania zostaly zapisane do pliku." << endl;
}

void StworzPlik(string& nazwa, const string& path) {
    string sciezka = path + nazwa + ".txt";
    ofstream plik(sciezka, ios::app);
    plik.close();
    cout << "Plik o nazwie " << nazwa << ".txt zostal stworzony" << endl;
}

void wczytywanieZpliku(vector<string>& lista, string& nazwa, string& path) {
    string sciezka = path + nazwa + ".txt";
    ifstream plik(sciezka);

    if (!plik) {
        cout << "Brak pliku lub blad otwierania" << endl;
        return;
    }

    lista.clear();
    string zadanie;
    while (getline(plik, zadanie)) {
        lista.push_back(zadanie);
    }

    plik.close();
    cout << "Zadania wczytane z pliku: \"" << nazwa << ".txt\"" << endl;
}

void wyswietlaniePlikow(const string& sciezka) {

    if (fs::is_empty(sciezka)) {
        string nazwa;
        cout << "Brak plikow w folderze!" << endl;
        cout << "Podaj nazwe pliku: ";
        cin >> nazwa;
        StworzPlik(nazwa, sciezka);
    }

    cout << "Pliki w folderze: " << sciezka << endl;
    for (const auto& plik : fs::directory_iterator(sciezka)) {
        cout << " - " << plik.path().stem() << endl;
    }
}
void wyczyscCalaListe(vector<string>& lista) {

    if (lista.empty()) {
        cout << "Brak zadan na liscie!" << endl;
        return;
    }

    if (potwierdzenie("Czy na pewno chcesz usunac wszystkie zadania z listy?: ")) {
        lista.clear();
        cout << "Wszystkie zadania zostaly usuniete." << endl;
    }
    else {
        cout << "Anulowano operacje." << endl;
    }

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool potwierdzenie(const string& komunikat) {
    char decyzja;
    cout << komunikat << " (T/N): ";
    cin >> decyzja;
    decyzja = toupper(decyzja);
    return (decyzja == 'T');
}

int main() {
    setlocale(LC_ALL, "");

    string nazwaPliku;
    vector<string> listaZadan;

    cout << "Proszę wybrać folder do którego beda zapisywane i pobierane pliki!" << endl;

    string folder = choose_folder();
    string wyborUsuniecia;

    if (!folder.empty()) {
        cout << "Wybrano folder: " << folder << endl;
    }
    else {
        cout << "Anulowano wybór folderu" << endl;
    }

    while (true) {
        int mainWybor = wybor();
        czyscEkran();

        switch (mainWybor) {
        case 1:
            dodajZadanie(listaZadan);
            break;
        case 2:
            pokazywanieZadania(listaZadan);
            break;
        case 3:
            cout << "Co chcesz zrobic?" << endl;
            cout << "[1] Usunac wszystkie zadania z listy " << endl;
            cout << "[2] Usunac pojedyncze zadania z listy wybrane przez ciebie" << endl;
            cout << "Podaj cokolwiek aby cofnac" << endl;
            cin >> wyborUsuniecia;
            if (wyborUsuniecia == "1") {
                wyczyscCalaListe(listaZadan);
            }
            else if (wyborUsuniecia == "2") {
                pokazywanieZadania(listaZadan);
                usuwanieZadania(listaZadan);
            }
            break;

        case 4:
            pokazywanieZadania(listaZadan);
            edytujZadanie(listaZadan);
            break;
        case 5:               // Zapisz do pliku
            wyswietlaniePlikow(folder);
            cout << "Podaj nazwe pliku do ktorego chcesz zapisac zadania: ";
            cin >> nazwaPliku;
            zapiszDoPliku(listaZadan, nazwaPliku, folder);
            break;
        case 6:
            wyswietlaniePlikow(folder);
            cout << "Z ktorego pliku chcesz zaladowac zadania?" << endl;
            cin >> nazwaPliku;
            wczytywanieZpliku(listaZadan, nazwaPliku, folder);
            break;
        case 7:
            cout << "Zegnaj!" << endl;
            return 0;
        default:
            cout << "Prosze podac jedna z opcji" << endl;
        }
    }
}
