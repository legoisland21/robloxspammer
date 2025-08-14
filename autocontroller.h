#pragma once
#include <windows.h>
#include <cmath>
#include <thread>
#include <string>
#include <chrono>

#define LMB 1
#define MMB 2
#define RMB 3

class Controller {
public:
    // Mouse
    void setMousePos(int x, int y) {
        SetCursorPos(x, y);
    }

    void dragMouseTo(int x, int y, float delay) {
        POINT current;
        GetCursorPos(&current);

        int steps = 100;
        float dx = (x - current.x) / (float)steps;
        float dy = (y - current.y) / (float)steps;

        for (int i = 0; i <= steps; ++i) {
            int nx = current.x + std::round(dx * i);
            int ny = current.y + std::round(dy * i);
            SetCursorPos(nx, ny);
            std::this_thread::sleep_for(std::chrono::milliseconds((int)(delay * 1000.0f / steps)));
        }
    }

    void clickMouseButton(WORD button) {
        INPUT input[2] = {};
        input[0].type = INPUT_MOUSE;
        input[1].type = INPUT_MOUSE;

        switch (button) {
            case LMB:
                input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
                break;
            case RMB:
                input[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
                input[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
                break;
            case MMB:
                input[0].mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
                input[1].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
                break;
            default:
                return;
        }

        SendInput(2, input, sizeof(INPUT));
    }

    void holdMouseButton(WORD button) {
        INPUT input = {};
        input.type = INPUT_MOUSE;

        switch (button) {
            case LMB:
                input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                break;
            case RMB:
                input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
                break;
            case MMB:
                input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
                break;
            default:
                return;
        }

        SendInput(1, &input, sizeof(INPUT));
    }

    void releaseMouseButton(WORD button) {
        INPUT input = {};
        input.type = INPUT_MOUSE;

        switch (button) {
            case LMB:
                input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
                break;
            case RMB:
                input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
                break;
            case MMB:
                input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
                break;
            default:
                return;
        }

        SendInput(1, &input, sizeof(INPUT));
    } 

    void scrollVertical(int amount) {
        INPUT input = {};
        input.type = INPUT_MOUSE;
        input.mi.mouseData = amount * WHEEL_DELTA;
        input.mi.dwFlags = MOUSEEVENTF_WHEEL;
        SendInput(1, &input, sizeof(INPUT));
    } // Mouse

    // Keyboard
    void inputKey(WORD vk) {
        INPUT input[2] = {};
        input[0].type = INPUT_KEYBOARD;
        input[0].ki.wVk = vk;

        input[1].type = INPUT_KEYBOARD;
        input[1].ki.wVk = vk;
        input[1].ki.dwFlags = KEYEVENTF_KEYUP;

        SendInput(2, input, sizeof(INPUT));
    }
    void typeString(const std::string& text, int delayMs = 50) {
        for (char ch : text) {
            SHORT vk = VkKeyScanA(ch);
            if (vk == -1) continue;

            bool shift = vk & 0x0100;
            BYTE vkCode = vk & 0xFF;

            if (shift) {
                INPUT shiftDown = {};
                shiftDown.type = INPUT_KEYBOARD;
                shiftDown.ki.wVk = VK_SHIFT;
                SendInput(1, &shiftDown, sizeof(INPUT));
            }

            INPUT input[2] = {};
            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = vkCode;

            input[1].type = INPUT_KEYBOARD;
            input[1].ki.wVk = vkCode;
            input[1].ki.dwFlags = KEYEVENTF_KEYUP;

            SendInput(2, input, sizeof(INPUT));

            if (shift) {
                INPUT shiftUp = {};
                shiftUp.type = INPUT_KEYBOARD;
                shiftUp.ki.wVk = VK_SHIFT;
                shiftUp.ki.dwFlags = KEYEVENTF_KEYUP;
                SendInput(1, &shiftUp, sizeof(INPUT));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }
    }

    void typeString(const char* cstr, int delayMs = 50) {
        typeString(std::string(cstr), delayMs);
    }

    void holdKey(WORD vk) {
        INPUT input = {};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = vk;
        SendInput(1, &input, sizeof(INPUT));
    }

    void releaseKey(WORD vk) {
        INPUT input = {};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = vk;
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
    }

    void sendRawKeyDown(WORD scanCode) {
        INPUT input = {};
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = scanCode;
        input.ki.dwFlags = KEYEVENTF_SCANCODE;
        SendInput(1, &input, sizeof(INPUT));
    }

    void sendKeyUp(WORD scanCode) {
        INPUT input = {};
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = scanCode;
        input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
    } // Keyboard
};