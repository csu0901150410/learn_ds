#include "ls_window.h"
#include "ls_shapes.h"

// ���ڹ��̺����������ڵ���Ϣ
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:  // ������������Ϣ
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:    // ����滭��Ϣ
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		lsPoint  point, start, end, center, LT_point, RB_point, LT_point_ell, RB_point_ell;

		point= ls_point_create(50, 200);
		start = ls_point_create(100, 200);
		end = ls_point_create(200, 300);
		LT_point = ls_point_create(400, 200);
		RB_point = ls_point_create(570, 300);

		LT_point_ell = ls_point_create(400, 370);
		RB_point_ell = ls_point_create(370, 400);

		center = ls_point_create(650, 200);
		int radius = 100;

		lsPoint points[] = {{100, 100}, {150, 50}, {200, 100}, {175, 150}, {125, 150}};
		int numPionts = 5;

		lsLineSegment line = *ls_line_segment_create(start, end);

		//����һ�����
		draw_point(hdc, point , RGB(255, 0, 0));

		//�����߶�
		draw_line(hdc, line, RGB(0, 255, 0));

		// ���ƾ���
		draw_rect(hdc, LT_point, RB_point, RGB(0, 0, 255));

		// ����Բ��
		void draw_cricle(hdc, center, radius, RGB��255��0��0��);

		// ������Բ
		void draw_ellipss(hdc, LT_point_ell, RB_point_ell, RGB��255��0��255��);

		// ���ƶ����
		void draw_polygon(hdc, points, numPionts, RGB��0��255��255��);

		EndPaint(hwnd, &ps);

		return 0;
	}

	// ����Ĭ�ϵĴ��ڹ���
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

// ע�ᴰ����
void RegisterWindowClass(HINSTANCE hInstance, const wchar_t* className)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = className;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	if (!RegisterClass(&wc))
	{
		OutputDebugString(L"RegisterClass failed");
	}
	RegisterClass(&wc);
}

// ����Ӧ�ó��򴰿�
HWND CreateAppWindow(HINSTANCE hInstance, const wchar_t* className, int nCmdShow)
{
	HWND hwnd = CreateWindowEx(
		0,
		className,
		L"My First Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd != NULL)
	{
		ShowWindow(hwnd, nCmdShow);
		UpdateWindow(hwnd);
	}
	else
	{
		OutputDebugString(L"CreateWindowEx failed");
	}
	return hwnd;
}