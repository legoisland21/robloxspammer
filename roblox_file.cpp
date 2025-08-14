#include "autocontroller.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <windows.h>
using namespace std;

string filename;
bool repeat;
vector<string> lines;
string line;

Controller control;
atomic<bool> running(true);

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
    inputSlashScan();
    Sleep(100);
    sendBackspace();
    control.typeString(text, 0);
    Sleep(10);
    for(int i = 0; i != 10; i++) { inputEnterScan(); Sleep(10); }
}

void focusRoblox(HWND hwnd) {
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);
}

void hotkeyListener() {
    RegisterHotKey(NULL, 1, MOD_ALT, 0x51);
    MSG msg = {};
    while (running) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_HOTKEY && msg.wParam == 1) {
                running = false;
                return;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(50));
    }
    UnregisterHotKey(NULL, 1);
}

int main() {
    thread hotkeyThread(hotkeyListener);

    HWND hwnd = FindWindowA(NULL, "Roblox");
    if (!hwnd) {
        cerr << "Could not find Roblox, is Roblox running?" << endl;
        running = false;
        hotkeyThread.join();
        return 1;
    }

    cout << "Enter filename to load lines from: ";
    getline(cin, filename);

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Can't open file: " << filename << endl;
        running = false;
        hotkeyThread.join();
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
        while(running) {
            for(const auto& l : lines) {
                if(!running) break;
                if(!isWindowFocused(hwnd)) focusRoblox(hwnd);
                sendMessage(l);
                Sleep(delay_ms);
            }
        }
    } else {
        for(const auto& l : lines) {
            if(!running) break;
            if(!isWindowFocused(hwnd)) focusRoblox(hwnd);
            sendMessage(l);
            Sleep(delay_ms);
        }
    }

    running = false;
    hotkeyThread.join();
    return 0;
}
