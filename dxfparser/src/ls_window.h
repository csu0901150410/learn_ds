#ifndef __LS_WINDOW_H__
#define __LS_WINDOW_H__

#include <tchar.h>
#include <windows.h>

#include <stdbool.h>

// 窗口过程函数的声明
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 注册
bool RegisterWindowClass(HINSTANCE hInstance, const TCHAR *className);

// 创建
HWND CreateAppWindow(HINSTANCE hInstance, const TCHAR *className, int nCmdShow);

#endif// __LS_WINDOW_H__