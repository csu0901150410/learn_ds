#ifndef __LS_WINDOW_H__
#define __LS_WINDOW_H__

#include <tchar.h>
#include <windows.h>

#include <stdbool.h>

#include "ls_dxf.h"
#include "ls_box.h"

// 窗口过程函数的声明
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 注册
bool RegisterWindowClass(HINSTANCE hInstance, const TCHAR *className);

// 创建
HWND CreateAppWindow(HINSTANCE hInstance, const TCHAR *className, int nCmdShow);

void ls_window_read_dxf(HWND hwnd);

lsBox ls_window_get_window_box(HWND hwnd);

void ls_window_draw_shapes(HWND hwnd, HDC hdc, const lsDxf *dxf);

/**
 * @brief.通用转换
 * 
 * \param entityCenter
 * \param scale
 * \param windowCenter
 * \param windowOrigin
 * \param windowHeight
 * \return lsMatrix
 */
lsMatrix generate_transform_matrix(lsPoint entityCenter, lsReal scale,
    lsPoint windowCenter, lsPoint windowOrigin, lsReal windowHeight);

#endif// __LS_WINDOW_H__