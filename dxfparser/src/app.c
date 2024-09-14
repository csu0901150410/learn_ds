// 程序入口点
#include "ls_window.h"

#include <stdio.h>
#include <stdlib.h>

#include <tchar.h>

#include "ls_log.h"
#include "dxf_parser.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    test_read_dxf("dxf/bridge.dxf");
    ls_log_info("hello world\n");

    //// 注册窗口类
    //const wchar_t CLASS_NAME[] = L"Sample Window Class";

    //RegisterWindowClass(hInstance, CLASS_NAME);

    //HWND hwnd = CreateAppWindow(hInstance, CLASS_NAME, nCmdShow);
    //if (hwnd == NULL)
    //{
    //    return 0;
    //}

    //// 消息循环
    //MSG msg = {0};
    //while (GetMessage(&msg, NULL, 0, 0))
    //{
    //    TranslateMessage(&msg);
    //    DispatchMessage(&msg);
    //}

    return 0;
}
