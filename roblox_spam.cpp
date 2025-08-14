#include "autocontroller.h"
#include <iostream>
using namespace std;
Controller control;

void sendBackspace() {
    INPUT inputs[2] = {};
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wScan = 0x0E;
    inputs[0].ki.dwFlags = KEYEVENTF_SCANCODE;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wScan = 0x0E;
    inputs[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

    SendInput(2, inputs, sizeof(INPUT));
}

void inputSlashScan() {
    INPUT input[2] = {};

    input[0].type = INPUT_KEYBOARD;
    input[0].ki.dwFlags = KEYEVENTF_SCANCODE;
    input[0].ki.wScan = 0x35;

    input[1].type = INPUT_KEYBOARD;
    input[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
    input[1].ki.wScan = 0x35;

    SendInput(2, input, sizeof(INPUT));
}

void inputEnterScan() {
    INPUT input[2] = {};

    input[0].type = INPUT_KEYBOARD;
    input[0].ki.dwFlags = KEYEVENTF_SCANCODE;
    input[0].ki.wScan = 0x1C;

    input[1].type = INPUT_KEYBOARD;
    input[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
    input[1].ki.wScan = 0x1C;

    SendInput(2, input, sizeof(INPUT));
}

void sendMessage(const string& text) {
    cout << "Pressing /" << endl;
    inputSlashScan();
    Sleep(100);
    cout << "Clearing chat" << endl;
    sendBackspace();
    cout << "Typing string: " << text << endl;
    control.typeString(text, 0);
    Sleep(10);
    cout << "Pressing enter (x10)" << endl;
    for(int i = 0; i != 10; i++) { inputEnterScan(); Sleep(10); }
}

int main() {
    string text;
    int delay;
    cout << "Enter text: ";
    getline(cin, text);
    cout << "Enter delay: ";
    cin >> delay;
    cout << endl;

    HWND hwnd = FindWindowA(NULL, "Roblox");
    if(hwnd) {
        printf("HWND id is %p \n", hwnd);
        cout << "Setting Roblox as foreground window" << endl;
        SetForegroundWindow(hwnd);
        cout << "Focusing Roblox" << endl;
        SetFocus(hwnd);
        cout << "Typing " << text << endl;
        while(true) {
            sendMessage(text);
            Sleep(delay);
        }
    } else {
        MessageBoxA(NULL, "Could not find Roblox, is Roblox running?", "ERROR", MB_ICONERROR);
        return 1;
    }
    return 0;
}