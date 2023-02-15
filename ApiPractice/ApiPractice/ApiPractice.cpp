// ApiPractice.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "ApiPractice.h"
#include "windows.h"
#include <math.h>
#include "resource.h"
#include <commdlg.h>
#include <stdio.h>
#include <CommCtrl.h>
#include <process.h>
#include <time.h>


#define MAX_LOADSTRING 100
#define BSIZE 40
#define UNSELECTED -1
#define THREAD_NUM 10

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

HANDLE hEvent;
HWND G_hWnd;
int G_Max;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
BOOL WINAPI InitializeApplication(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void ThreadProc(void* arg)
{
    HDC hdc;
    int i;
    int xPos = *((int*)arg);
    srand((unsigned)time(0));
    hdc = GetDC(G_hWnd);
    SelectObject(hdc, CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256)));

    for (int i = 0; i < 10; i++)
    {
        int num;
        WaitForSingleObject(hEvent, INFINITE);
        num = rand() % 500;

        if (num > G_Max)
        {
            Sleep(3000);
            G_Max = num;
            Rectangle(hdc, xPos, 0, xPos + 20, num);
        }

        SetEvent(hEvent);
    }

    ReleaseDC(G_hWnd, hdc);
    return;
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
    //MyRegisterClass(hInstance);
    InitializeApplication(hInstance);

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
BOOL WINAPI InitializeApplication(HINSTANCE hInstance)
{
    WNDCLASS wc;

    // Register the frame window class. 

    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APIPRACTICE));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
    wc.lpszMenuName = MAKEINTRESOURCE(IDC_APIPRACTICE);
    wc.lpszClassName = "Window Class Name";

    if (!RegisterClass(&wc))
        return FALSE;

    // Register the MDI child window class. 

    wc.lpfnWndProc = ChildWndProc;
    wc.lpszMenuName = (LPCTSTR)NULL;
    wc.lpszClassName = "Child Window Class Name";

    if (!RegisterClass(&wc))
        return FALSE;

    return TRUE;
}

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
    RegisterClassExW(&wcex);

    wcex.lpfnWndProc = ChildWndProc;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"Child Window Class Name";

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

   /*HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);*/

   HWND hWnd = CreateWindow("Window Class Name", "Main Window Ttile", WS_OVERLAPPEDWINDOW,
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

LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        break;
    case WM_DESTROY:
        break;
    }

    return DefMDIChildProc(hWnd, message, wParam, lParam);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HANDLE hThread[THREAD_NUM];
    static int xPos[THREAD_NUM];
    int i;


    switch (message)
    {
    case WM_CREATE:
        for (i = 0; i < THREAD_NUM; i++)
            xPos[i] = i * 100;
        G_Max = 0;
        G_hWnd = hWnd;
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

        case ID_FILENEW:
            break;



        case ID_EDITCOPY:
            break;

        case ID_COLORDLG:
            break;

        case ID_FILEOPEN:
            break;
        case ID_FILESAVE:
            break;

        case IDM_EXIT:
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

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_LBUTTONDOWN:
        hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
        for (int i = 0; i < THREAD_NUM; i++)
        {
            hThread[i] = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall*)(void*))ThreadProc, (void *)&xPos[i], 0, NULL);
            Sleep(2000);
        }
        
        WaitForMultipleObjects(THREAD_NUM, hThread, TRUE, INFINITE);
        CloseHandle(hEvent);
        break;
    

    
    case WM_LBUTTONUP:
        break;
    case WM_MOUSEMOVE:
        break;
    case WM_CHAR:
        break;
    case WM_KEYDOWN:

        InvalidateRgn(hWnd, NULL, TRUE);
        break;
    case WM_TIMER:
        break;

    case WM_DESTROY:
        for (int i = 0; i < THREAD_NUM; i++)
            CloseHandle(hThread[i]);

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
