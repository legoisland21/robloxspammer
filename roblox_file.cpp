#include "autocontroller.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

Controller control;

bool isWindowFocused(HWND hwnd) {
    return GetForegroundWindow() == hwnd;
}

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

void focusRoblox(HWND hwnd) {
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);
}

int main() {
    string filename;
    bool repeat;
    cout << "Enter filename to load lines from: ";
    getline(cin, filename);

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Can't open file: " << filename << endl;
        return 1;
    }

    vector<string> lines;
    string line;
    while(getline(file, line)) {
        if(!line.empty()) lines.push_back(line);
    }
    file.close();

    int delay_ms = 0;
    cout << "Input delay (ms): ";
    cin >> delay_ms;
    cin.ignore();

    cout << "Repeat file? (true/false): ";
    cin >> repeat;
    cin.ignore();

    HWND hwnd = FindWindowA(NULL, "Roblox");
    if (!hwnd) {
        cerr << "Could not find Roblox, is Roblox running?" << endl;
        return 1;
    }

    printf("HWND id is %p \n", hwnd);
    cout << "Setting Roblox as foreground window" << endl;
    SetForegroundWindow(hwnd);
    cout << "Focusing Roblox" << endl;
    SetFocus(hwnd);

    if(repeat) {
        cout << "Repeating entire file..." << endl;
        while(true) {
            for(const auto& l : lines) {
                if(!isWindowFocused(hwnd)) focusRoblox(hwnd);
                cout << "---------------------------------" << endl;
                cout << "Sending line: " << l << endl;
                sendMessage(l);
                Sleep(delay_ms);
            }
        }
    } else {
        for(const auto& l : lines) {
            cout << "Sending line: " << l << endl;
            sendMessage(l);
            Sleep(delay_ms);
        }
    }

    cout << "Finished sending all lines." << endl;
    return 0;
}
