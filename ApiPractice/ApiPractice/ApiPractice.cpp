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

#define MAX_LOADSTRING 100
#define BSIZE 40
#define UNSELECTED -1

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void MakeColumn(HWND& hWnd)
{
    char name[2][30] = { "이름", "전화번호" };
    LVCOLUMN lvCol = {0,};
    HWND hList;
    int i;
    hList = GetDlgItem(hWnd, IDC_LIST_MEMBER);
    lvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvCol.fmt = LVCFMT_LEFT;

    for (i = 0; i < 2; i++)
    {
        lvCol.cx = 90;
        lvCol.pszText = name[i];
        lvCol.iSubItem = i;
        SendMessage(hList, LVM_INSERTCOLUMN, (WPARAM)1, (LPARAM)&lvCol);
    }
}

void InsertData(HWND& hWnd)
{
    LVITEM item;
    HWND hList;
    char name[20];
    char phone[20];

    int count = 0;

    int i;
    GetDlgItemText(hWnd, IDC_EDIT_NAME, name, 20);
    SetDlgItemText(hWnd, IDC_EDIT_NAME, "");

    if (strcmp(name, "") == 0)
        return;


    GetDlgItemText(hWnd, IDC_EDIT_PHONE, phone, 20);
    SetDlgItemText(hWnd, IDC_EDIT_PHONE, "");

    hList = GetDlgItem(hWnd, IDC_LIST_MEMBER);
    count = ListView_GetItemCount(hList);
    item.mask = LVIF_TEXT;
    item.iItem = count;
    item.iSubItem = 0;
    item.pszText = name;
    ListView_InsertItem(hList, &item);
    ListView_SetItemText(hList, count, 1, phone);

}

int SelectItem(HWND& hWnd, LPARAM lParam)
{
    LPNMLISTVIEW nlv;
    HWND hList;
    char name[20], phone[20];
    hList = GetDlgItem(hWnd, IDC_LIST_MEMBER);
    nlv = (LPNMLISTVIEW)lParam;
    ListView_GetItemText(hList, nlv->iItem, 0, name, 20);
    SetDlgItemText(hWnd, IDC_EDIT_NAME, name);
    ListView_GetItemText(hList, nlv->iItem, 1, phone, 20);
    SetDlgItemText(hWnd, IDC_EDIT_PHONE, phone);
    return nlv->iItem;
}

void ModifyItem(HWND& hWnd, int selection)
{
    LVITEM item;
    HWND hList;
    char name[20];
    char phone[20];

    hList = GetDlgItem(hWnd, IDC_LIST_MEMBER);

    GetDlgItemText(hWnd, IDC_EDIT_NAME, name, 20);
    GetDlgItemText(hWnd, IDC_EDIT_PHONE, phone, 20);

    if (strcmp(name, "") == 0)
        return;

    ListView_SetItemText(hList, selection, 0, name);
    ListView_SetItemText(hList, selection, 1, phone);

    SetDlgItemText(hWnd, IDC_EDIT_NAME, "");
    SetDlgItemText(hWnd, IDC_EDIT_PHONE, "");
}

void DeleteItem(HWND& hWnd, int selection)
{
    static HWND hList;
    hList = GetDlgItem(hWnd, IDC_LIST_MEMBER);
    ListView_DeleteItem(hList, selection);
    SetDlgItemText(hWnd, IDC_EDIT_NAME, "");
    SetDlgItemText(hWnd, IDC_EDIT_PHONE, "");
    return;
}

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

void TextPrint(HDC hdc, int x, int y, char text[])
{
    SetTextColor(hdc, RGB(255, 255, 255));

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            TextOut(hdc, x + i, y + j, text, strlen(text));
        }
    }

    SetTextColor(hdc, RGB(0, 0, 0));
    TextOut(hdc, x, y, text, strlen(text));
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

INT_PTR CALLBACK Dlg6_lProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LPNMHDR hdr;
    HWND hList;
    HDC hdc;

    static int selection;

    switch (message)
    {
    case WM_INITDIALOG:
        selection = UNSELECTED;
        MakeColumn(hWnd);
        break;

    case WM_NOTIFY:
        hdr = (LPNMHDR)lParam;
        hList = GetDlgItem(hWnd, IDC_LIST_MEMBER);
        if (hdr->hwndFrom == hList && hdr->code == LVN_ITEMCHANGING)
            selection = SelectItem(hWnd, lParam);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON_INSERT:
            InsertData(hWnd);
            break;

        

        case IDC_BUTTON_MODIFY:
            if (selection == UNSELECTED)
                break;

            ModifyItem(hWnd, selection);
            selection = UNSELECTED;
            break;

        case IDC_BUTTON_DELETE:
            if (selection == UNSELECTED)
                break;
            
            DeleteItem(hWnd, selection);
            selection = UNSELECTED;
            break;


        case IDOK:
            EndDialog(hWnd, 0);
            break;
        case IDCANCEL:
            EndDialog(hWnd, 0);
            break;
        }

        break;
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    static HDC hdc, mem1dc, mem2dc;
    PAINTSTRUCT ps;
    static HBITMAP hBit1, hBit2, oldBit1, oldBit2;
    static RECT rectView;
    static int yPos;
    char word[] = "테스트테스트123";

    HWND hDlg = NULL;

    switch (message)
    {
    case WM_CREATE:
        yPos = -30;
        GetClientRect(hWnd, &rectView);
        SetTimer(hWnd, 1, 70, NULL);
        hBit2 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_RUMINE));
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

        case ID_6_3_DIALOG:

            if (IsWindow(hDlg) == false)
            {
                hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hDlg, Dlg6_lProc);
                ShowWindow(hDlg, SW_SHOW);
            }
            break;

        case ID_EDITCOPY:
            break;

        case ID_COLORDLG:
            break;

        case ID_FILENEW:
            MessageBox(hWnd, "새파일?", "새 파일 선택", MB_OKCANCEL);
            break;
        case ID_FILEOPEN:
            break;
        case ID_FILESAVE:
            break;

        case IDM_EXIT:
            DestroyWindow(hDlg);
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        GetClientRect(hWnd, &rectView);
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        mem1dc = CreateCompatibleDC(hdc);
        oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
        BitBlt(hdc, 0, 0, 1024, 768, mem1dc, 0, 0, SRCCOPY);
        SelectObject(mem1dc, oldBit1);
        DeleteDC(mem2dc);
        EndPaint(hWnd, &ps);
    }
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

        yPos += 5;
        if (yPos > rectView.bottom)
            yPos = -30;

        hdc = GetDC(hWnd);
        if (hBit1 == NULL)
            hBit1 = CreateCompatibleBitmap(hdc, 1024, 768);
        
        mem1dc = CreateCompatibleDC(hdc);
        mem2dc = CreateCompatibleDC(mem1dc);
        oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
        oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit2);
        BitBlt(mem1dc, 0, 0, 1024, 768, mem2dc, 0, 0, SRCCOPY);
        SetBkMode(mem1dc, TRANSPARENT);
        TextPrint(mem1dc, 200, yPos, word);
        SelectObject(mem2dc, oldBit2);
        DeleteDC(mem2dc);
        SelectObject(mem1dc, oldBit1);
        DeleteDC(mem1dc);
        ReleaseDC(hWnd, hdc);
        InvalidateRgn(hWnd, NULL, TRUE);
        break;

    case WM_DESTROY:
        if (hBit1)
            DeleteObject(hBit1);

        DeleteObject(hBit2);

        KillTimer(hWnd, 1);
        DeleteDC(mem1dc);
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
