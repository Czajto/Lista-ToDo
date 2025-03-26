#include "folder_picker.h"
#include <windows.h>
#include <shlobj.h>
#include <ole2.h>
#include <string>
#include <iostream>

// Funkcja konwertująca std::wstring na std::string
std::string wstring_to_string(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string str(size_needed - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], size_needed, NULL, NULL);
    return str;
}

std::string choose_folder() {
    // Inicjalizowanie COM
    if (CoInitializeEx(NULL, COINIT_APARTMENTTHREADED) != S_OK) {
        std::cerr << "Błąd inicjalizacji COM!" << std::endl;
        return "";
    }

    BROWSEINFOW bi;
    ZeroMemory(&bi, sizeof(bi));
    bi.hwndOwner = NULL;
    bi.lpszTitle = L"Wybierz folder";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);
    std::string selectedPath = "";

    if (pidl != NULL) {
        wchar_t path[MAX_PATH];
        if (SHGetPathFromIDListW(pidl, path)) {
            selectedPath = wstring_to_string(path); // Konwersja wchar_t -> std::string
        }
        CoTaskMemFree(pidl);
    }

    CoUninitialize();
    return selectedPath + "\\";
}
