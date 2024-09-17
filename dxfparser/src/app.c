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
#include "dxf_parser.h"
#include "ls_dxf.h"

const TCHAR APP_WND_CLASS[] = _T("dxfparser.app.class");

// Ref : http://www.winprog.org/tutorial/start.html

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR pCmdLine, _In_ int nCmdShow)
{
   lsDxf dxfReader;
   ls_dxf_init(&dxfReader, "dxf/bridge.dxf");

   ls_dxf_parse(&dxfReader);

   //while (1)
   //{
   //    int code;
   //    if (!ls_dxf_read_record(&dxfReader, &code))
   //        break;// 读取结束或者读取完毕，退出循环

   //    // 应用过滤，比方说，只打印类型LINE
   //    // see [https://help.autodesk.com/view/ACD/2023/CHS/?guid=GUID-FCEF5726-53AE-4C43-B4EA-C84EB8686A66]
   //    if (0 == code && 0 == strncmp(dxfReader.str, "LINE", 4))
   //    {
   //        ls_log_info("%s", dxfReader.str);
   //    }
   //}
   ls_dxf_deinit(&dxfReader);

   // 注册窗口类
   if (!RegisterWindowClass(hInstance, APP_WND_CLASS))
       return 0;

   HWND hwnd = CreateAppWindow(hInstance, APP_WND_CLASS, nCmdShow);
   if (NULL == hwnd)
      return 0;

   // 消息循环
   MSG msg = {0};
   while (GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

   return 0;
}
