#include "autocontroller.h"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
using namespace std;

string text;
int delay;

Controller control;

void killProcessByName(const wchar_t* processName) {
    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32FirstW(hSnap, &pe32)) {
        do {
            if (_wcsicmp(pe32.szExeFile, processName) == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
                if (hProcess) {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                    wcout << L"Killed " << processName << L" 🗿🔥\n";
                }
            }
        } while (Process32NextW(hSnap, &pe32));
    }
    CloseHandle(hSnap);
}

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
    if (!RegisterHotKey(NULL, 1, MOD_ALT, 0x51)) {
        cerr << "Failed to register hotkey.\n";
        return 1;
    }

    HWND hwnd = FindWindowA(NULL, "Roblox");
    if (!hwnd) {
        MessageBoxA(NULL, "Could not find Roblox, is Roblox running?", "ERROR", MB_ICONERROR);
        return 1;
    }

    cout << "Enter text: ";
    getline(cin, text);
    cout << "Enter delay (ms): ";
    cin >> delay;
    cout << endl;

    printf("HWND id is %p \n", hwnd);
    cout << "Setting Roblox as foreground window" << endl;
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);
    cout << "Typing " << text << endl;

    MSG msg = {0};
    while (true) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_HOTKEY) {
                killProcessByName(L"roblox_spammer.exe");
                return 0;
            }
        }

        if(!isWindowFocused(hwnd)) focusRoblox(hwnd);
        cout << "---------------------------------" << endl;
        sendMessage(text);
        Sleep(delay);
    }

    return 0;
}
