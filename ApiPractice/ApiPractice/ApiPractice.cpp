﻿// ApiPractice.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "ApiPractice.h"
#include "windows.h"
#include <math.h>
#include "resource.h"
#include <commdlg.h>

#define MAX_LOADSTRING 100
#define BSIZE 40

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


double LengthPts(int x1, int y1, int x2, int y2)
{
    return (sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
}

BOOL InCircle(int x, int y, int mx, int my)
{
    if (LengthPts(x, y, mx, my) < BSIZE)
        return TRUE;

    return FALSE;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_APIPRACTICE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APIPRACTICE));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APIPRACTICE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_APIPRACTICE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int answer = 0;
    OPENFILENAME OFN;
    char str[100], lpstrFile[100] = "";
    char filter[] = "Every File(*.*) \0*.*\0Text File\0*.txt;*.doc\0";
    HDC hdc;

    PAINTSTRUCT ps;
    CHOOSEFONT FONT;
    CHOOSECOLOR COLOR;
    static COLORREF temp[16], color;
    HFONT hFont, OldFont;
    static LOGFONT LogFont;
    HBRUSH hBrush, OldBrush;

    static HMENU hMenu, hSubMenu;

    switch (message)
    {
    case WM_CREATE:
        hMenu = GetMenu(hWnd);
        hSubMenu = GetSubMenu(hMenu, 2);
        EnableMenuItem(hSubMenu, ID_COLORDLG, MF_GRAYED);
        EnableMenuItem(hSubMenu, ID_FONTDLG, MF_GRAYED);
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;

        case ID_COLORDLG:
            for (int i = 0; i < 16; i++)
                temp[i] = RGB(rand() % 256, rand() % 256, rand() % 256);

            memset(&COLOR, 0, sizeof(CHOOSECOLOR));
            COLOR.lStructSize = sizeof(CHOOSECOLOR);
            COLOR.hwndOwner = hWnd;
            COLOR.lpCustColors = temp;
            COLOR.Flags = CC_FULLOPEN;
            if (ChooseColor(&COLOR) != 0)
            {
                color = COLOR.rgbResult;
                InvalidateRgn(hWnd, NULL, TRUE);
            }
            break;

        case ID_FILENEW:
            MessageBox(hWnd, "새파일?", "새 파일 선택", MB_OKCANCEL);
            break;
        case ID_FILEOPEN:
            memset(&OFN, 0, sizeof(OPENFILENAME));
            OFN.lStructSize = sizeof(OPENFILENAME);
            OFN.hwndOwner = hWnd;
            OFN.lpstrFilter = filter;
            OFN.lpstrFile = lpstrFile;
            OFN.nMaxFile = 100;
            OFN.lpstrInitialDir = ".";
            if (GetOpenFileName(&OFN) != 0)
            {
                wsprintf(str, "%s 파일을 열겠습니카", OFN.lpstrFile);
                MessageBox(hWnd, str, "열기 선택", MB_OK);
            }
            break;
        case ID_FILESAVE:
            memset(&OFN, 0, sizeof(OPENFILENAME));
            OFN.lStructSize = sizeof(OPENFILENAME);
            OFN.hwndOwner = hWnd;
            OFN.lpstrFilter = filter;
            OFN.lpstrFile = lpstrFile;
            OFN.nMaxFile = 100;
            OFN.lpstrInitialDir = ".";
            if (GetSaveFileName(&OFN) != 0)
            {
                wsprintf(str, "%s 파일을 저장하시겠습니카", OFN.lpstrFile);
                MessageBox(hWnd, str, "저장하기 선택", MB_OK);
            }
            break;
            break;

        case IDM_EXIT:

            answer = MessageBox(hWnd, "파일 선택 확인", "끝내기 선택", MB_YESNO);
            if (answer == IDYES)
                DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        hBrush = CreateSolidBrush(color);
        OldBrush = (HBRUSH)SelectObject(hdc, hBrush);
        Ellipse(hdc, 10, 10, 200, 200);
        SelectObject(hdc, OldBrush);
        DeleteObject(hBrush);


        EndPaint(hWnd, &ps);
    }
    break;
    
    

    case WM_LBUTTONDOWN:
        break;
    case WM_LBUTTONUP:
        break;
    case WM_MOUSEMOVE:
        hdc = GetDC(hWnd);
        ReleaseDC(hWnd, hdc);
        break;
    case WM_CHAR:
        hdc = GetDC(hWnd);
        ReleaseDC(hWnd, hdc);
        break;
    case WM_KEYDOWN:

        InvalidateRgn(hWnd, NULL, TRUE);
        break;
    case WM_TIMER:
        hdc = GetDC(hWnd);

        
        

        InvalidateRgn(hWnd, NULL, TRUE);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
