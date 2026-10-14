#pragma once
#include <iostream>
#include <windows.h>

inline void setCursorPosition(int x, int y) {
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}

inline void fullSc() {
    HWND Hwnd{ GetForegroundWindow() };
    int x{ GetSystemMetrics(SM_CXSCREEN) };
    int y{ GetSystemMetrics(SM_CYSCREEN) };
    LONG winstyle{ GetWindowLong(Hwnd, GWL_STYLE) };
    SetWindowLong(Hwnd, GWL_STYLE, (winstyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
    SetWindowPos(Hwnd, HWND_TOP, 0, 0, x, y, 0);

}

inline void ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

inline void setConsoleColour(unsigned short colour) {
    static const HANDLE hOut{ GetStdHandle(STD_OUTPUT_HANDLE) };
    SetConsoleTextAttribute(hOut, colour);
}

