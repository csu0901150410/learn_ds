/**
 * @file app.c
 * @author lhy (lianghongyu.csu@gmail.com)
 * @brief 窗口函数
 * @version 0.1
 * @date 2024-09-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <tchar.h>
#include <windows.h>

#include "ls_window.h"
#include "ls_log.h"
#include "ls_dxf.h"

const TCHAR APP_WND_CLASS[] = _T("dxfparser.app.class");

// Ref : http://www.winprog.org/tutorial/start.html

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR pCmdLine, _In_ int nCmdShow)
{
    // 注册窗口类
    if (!RegisterWindowClass(hInstance, APP_WND_CLASS))
        return 0;

    HWND hwnd = CreateAppWindow(hInstance, APP_WND_CLASS, nCmdShow);
    if (NULL == hwnd)
        return 0;

    // 消息循环
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0) // 这个函数可能会返回-1/0/正整数
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
