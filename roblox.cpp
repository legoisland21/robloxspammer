#include "autocontroller.h"
#include <windows.h>
#include <iostream>
using namespace std;

string text;

Controller control;

bool isWindowFocused(HWND hwnd) {
    return GetForegroundWindow() == hwnd;
}

void sendMessage(const string& text) {
    cout << "Pressing /" << endl;
    control.inputScan(0x35);
    Sleep(100);
    cout << "Clearing chat" << endl;
    control.inputScan(0x0E);
    cout << "Typing string: " << text << endl;
    control.typeString(text, 0);
    Sleep(10);
    cout << "Pressing enter (x10)" << endl;
    for(int i = 0; i != 10; i++) { control.inputScan(0x1C); Sleep(10); }
}

void focusRoblox(HWND hwnd) {
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);
}

int main() {
    HWND hwnd = FindWindowA(NULL, "Roblox");
    if (!hwnd) {
        MessageBoxA(NULL, "Could not find Roblox, is Roblox running?", "ERROR", MB_ICONERROR);
        return 1;
    }

    cout << "Enter text: ";
    getline(cin, text);
    cout << endl;

    printf("HWND id is %p \n", hwnd);
    cout << "Setting Roblox as foreground window" << endl;
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);
    cout << "Typing " << text << endl;

    MSG msg = {0};
    if(!isWindowFocused(hwnd)) focusRoblox(hwnd);
    sendMessage(text);

    return 0;
}