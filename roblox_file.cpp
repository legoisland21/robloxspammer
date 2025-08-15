#include "autocontroller.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

string filename;
bool repeat;
vector<string> lines;
string line;

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
        cerr << "Could not find Roblox, is Roblox running?" << endl;
        return 1;
    }

    cout << "Enter filename to load lines from: ";
    getline(cin, filename);

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Can't open file: " << filename << endl;
        return 1;
    }

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

    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    if(repeat) {
        while(true) {
            for(const auto& l : lines) {
                if(!isWindowFocused(hwnd)) focusRoblox(hwnd);
                sendMessage(l);
                Sleep(delay_ms);
            }
        }
    } else {
        for(const auto& l : lines) {
            if(!isWindowFocused(hwnd)) focusRoblox(hwnd);
            sendMessage(l);
            Sleep(delay_ms);
        }
    }
    return 0;
}
