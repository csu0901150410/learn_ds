#pragma once
#ifndef LS_WINDOW_H
#define LS_WINDOW_H

#include <windows.h>

// ���ڹ��̺���������
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//ע��
void RegisterWindowClass(HINSTANCE hInstance, const wchar_t* className);

//����
HWND CreateAppWindow(HINSTANCE hInstance, const wchar_t* className, int nCmdShow);
#endif 