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

    _setmode(_fileno(stdout), _O_U8TEXT);  // Для wcout
    _setmode(_fileno(stdin), _O_U8TEXT);   // Для wcin

        Game game;
        game.StartGame();

    while (!game.IsGameOver()) {
        game.RenderUI();
        int choice;
        std::wcin >> choice;
        game.HandlePlayerChoice(choice);
        
    }

    std::wcout << L"\nИгра окончена. Спасибо за участие.\n";
    return 0;
}
