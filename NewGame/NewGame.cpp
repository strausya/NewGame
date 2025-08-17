// NewGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <thread> 
#include <iostream>
#include "Game.h"

int main() {

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    _setmode(_fileno(stdout), _O_U8TEXT);  // ��� wcout
    _setmode(_fileno(stdin), _O_U8TEXT);   // ��� wcin

        Game game;
        game.StartGame();

    while (!game.IsGameOver()) {
        game.RenderUI();
        int choice;
        std::wcin >> choice;
        game.HandlePlayerChoice(choice);
        void CheckFinalConditions();
        Beep(392, 300);  // ���� (G4)
        Beep(440, 300);  // �� (A4)
        Beep(392, 300);  // ���� (G4)
        Beep(330, 600);  // �� (E4) � ������� ����
                
    }

    std::wcout << L"\n���� ��������. ������� �� �������.\n";
    return 0;
}
