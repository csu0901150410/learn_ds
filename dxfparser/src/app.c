// 程序入口点
#include "ls_window.h"

#include <stdio.h>
#include <stdlib.h>

#include <tchar.h>

#include "ls_log.h"
#include "dxf_parser.h"

#include "ls_dxf.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    lsDxf dxfReader;
    ls_dxf_init(&dxfReader, "dxf/bridge.dxf");

    while (1)
    {
        int code;
        if (!ls_dxf_read_record(&dxfReader, &code))
            break;// 读取结束或者读取完毕，退出循环

        // // 应用过滤，比方说，只打印组码为0的数据
        // if (0 == code)
        // {
        //     // 组码0之后是表示图元类型的字符串
        //     // see [https://help.autodesk.com/view/ACD/2023/CHS/?guid=GUID-3F0380A5-1C15-464D-BC66-2C5F094BCFB9]
        //     ls_log_info("%s", dxfReader.str);
        // }

        // 应用过滤，比方说，只打印类型LINE
        // see [https://help.autodesk.com/view/ACD/2023/CHS/?guid=GUID-FCEF5726-53AE-4C43-B4EA-C84EB8686A66]
        if (0 == code && 0 == strncmp(dxfReader.str, "LINE", 4))
        {
            ls_log_info("%s", dxfReader.str);
        }
    }
    ls_dxf_deinit(&dxfReader);


    // test_read_dxf("dxf/bridge.dxf");
    // ls_log_info("hello world\n");

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
