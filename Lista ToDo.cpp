#include <iostream>
#include <vector>
#include "folder_picker.h"
#include <string>
#include <fstream>
#include <locale>
#include <codecvt>
#include <filesystem>
#include <limits>
#include <windows.h>
#include <cstdlib>  // dla system()

/*

X = NIE ZROBIONE
✓ = ZROBIONE

RZECZY DO ZROBIENIA!!!!
##############################
DODANIE FUNKCJI OZNACZANIA W DANYM PLIKU RZECZY ZROBIONYCH I NIE (X)
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
    cout << "[2] Pokaż zadania z listy" << endl;
    cout << "[3] Usuń zadanie z listy" << endl;
    cout << "[4] Edytuj zadanie z listy" << endl;
    cout << "[5] Zapisz do pliku" << endl;
    cout << "[6] Wczytaj z pliku" << endl;
    cout << "[7] Usuń plik" << endl;
    cout << "[8] Wyjście" << endl;
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
    cout << "Podaj treść nowego zadania: ";
    cin >> noweZadanie;
    lista.insert(lista.begin() + nrZadania, noweZadanie);
    lista.erase(lista.begin() + nrZadania - 1);
}


void dodajZadanie(vector<string>& lista) {
    string zadanie;
    cout << "Podaj treść zadania: ";
    getline(cin, zadanie);
    lista.push_back(zadanie);
    cout << "Dodano zadanie: \"" << zadanie << "\"" << endl;
}

void pokazywanieZadania(const vector<string>& lista) {
    if (lista.empty()) {
        cout << "Brak zadań na liście!" << endl;
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
    cout << "Podaj numer zadania do usunięcia: ";
    cin >> index;
    if (index < 1 || index > lista.size()) {
        cout << "Niepoprawny numer!" << endl;
        return;
    }
    cout << "Usunięto zadanie: \"" << lista[index - 1] << "\"!" << endl;
    lista.erase(lista.begin() + index - 1);
}

void zapiszDoPliku(const vector<string>& lista, string& nazwa, string& path) {
    string sciezka = path + nazwa + ".txt";
    ofstream plik(sciezka, ios::app);
    if (!plik) {
        cout << "Otwieranie pliku nie powiodło się" << endl;
        return;
    }
    for (const string& zadanie : lista) {
        plik << zadanie << endl;
    }
    plik.close();
    cout << "Zadania zostały zapisane do pliku." << endl;
}

void StworzPlik(string& nazwa, const string& path) {
    string sciezka = path + nazwa + ".txt";
    ofstream plik(sciezka, ios::app);
    plik.close();
    cout << "Plik o nazwie " << nazwa << ".txt został stworzony" << endl;
}

void UsunPlik(string& nazwa, const string& path){
    string sciezka = path + nazwa + ".txt";
    try{
        if(fs::remove(sciezka)){
            cout << "Plik " << nazwa + ".txt został usuniety pomyslne." << endl;
        }   else {
            cout << "Plik nie znaleziony." << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Filesystem error: " << e.what() << endl;
    }
}

void wczytywanieZpliku(vector<string>& lista, string& nazwa, string& path) {
    string sciezka = path + nazwa + ".txt";
    ifstream plik(sciezka);

    if (!plik) {
        cout << "Brak pliku lub błąd otwierania" << endl;
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
        cout << "Brak plików w folderze!" << endl;
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
        cout << "Brak zadań na liście!" << endl;
        return;
    }

    if (potwierdzenie("Czy na pewno chcesz usunąć wszystkie zadania z listy?: ")) {
        lista.clear();
        cout << "Wszystkie zadania zostały usuniete." << endl;
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
    // Ustawia kodowanie na UTF-8
    SetConsoleOutputCP(CP_UTF8);

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
            cout << "Co chcesz zrobić?" << endl;
            cout << "[1] Usunąć wszystkie zadania z listy " << endl;
            cout << "[2] Usunąć pojedyncze zadania z listy wybrane przez ciebie" << endl;
            cout << "Podaj cokolwiek aby cofnąć" << endl;
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
            cout << "Podaj nazwe pliku do którego chcesz zapisać zadania: ";
            cin >> nazwaPliku;
            zapiszDoPliku(listaZadan, nazwaPliku, folder);
            break;
        case 6:
            wyswietlaniePlikow(folder);
            cout << "Z którego pliku chcesz załadowac zadania?" << endl;
            cin >> nazwaPliku;
            wczytywanieZpliku(listaZadan, nazwaPliku, folder);
            break;
        case 7:
            wyswietlaniePlikow(folder);
            cout << "Który plik chcesz usunąć?" << endl;
            cout << "Wpisz Exit aby wyjść" << endl;
            cin >> nazwaPliku;
            if (nazwaPliku != "Exit" && nazwaPliku != "exit"){
                UsunPlik(nazwaPliku, folder);
                break;
            } else {
                czyscEkran();
                break;
            }
        case 8:
            cout << "Żegnaj!" << endl;
            return 0;
        default:
            cout << "Proszę podać jedną z opcji" << endl;
        }
    }
}
