#pragma once
#ifndef LS_WINDOW_H
#define LS_WINDOW_H

#include <windows.h>

// 窗口过程函数的声明
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 注册
void RegisterWindowClass(HINSTANCE hInstance, const wchar_t *className);

// 创建
HWND CreateAppWindow(HINSTANCE hInstance, const wchar_t *className, int nCmdShow);
#endif