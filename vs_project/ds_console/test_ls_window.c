// ������ڵ�
#include "ls_window.h"
#include <stdio.h>
#include <stdlib.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // ע�ᴰ����
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    RegisterWindowClass(hInstance, CLASS_NAME);

    HWND hwnd = CreateAppWindow(hInstance, CLASS_NAME, nCmdShow);
    if (hwnd == NULL)
    {
        return 0;
    }

    // ��Ϣѭ��
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
