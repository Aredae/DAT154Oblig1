// Oblig1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Oblig1.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
COLORREF grey = RGB(220, 220, 220);
COLORREF red = RGB(255, 0, 0);
COLORREF green = RGB(0, 255, 0);
COLORREF yellow = RGB(255, 255, 0);
COLORREF lights[3] = { grey,grey,grey };
COLORREF lights2[3] = { grey,grey,grey };
static int pwe;
static int pns;
WCHAR pweS[20] ;
WCHAR pnsS[20] ;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    probability(HWND, UINT, WPARAM, LPARAM);
class Car {
public:
    int x, y;
    COLORREF color;
    Car(int _x, int _y, COLORREF _color) :x(_x), y(_y), color(_color) {}
    void draw(HDC hdc) {
        HBRUSH brush = CreateSolidBrush(color);
        HGDIOBJ org = SelectObject(hdc, brush);
        Rectangle(hdc, x, y - 5, x + 10, y + 5);
        SelectObject(hdc, org);
        DeleteObject(brush);
    }
};
class CarList {
public:
    Car* t[1000];
    int m_i;
    CarList() { m_i = 0; }
    ~CarList() {
        Clear();
    }
    void push(Car* pCar) {
        t[m_i++] = pCar;
    }
    void draw(HDC hdc) {
        for (int k = 0; k < m_i; k++) {
            t[k]->draw(hdc);
        }
    }
    void moveWE(int i, int dx, int y) {
        t[i]->x += dx;
        t[i]->y = y;
    }
    void moveNS(int i, int dy, int x) {
        t[i]->y += dy;
        t[i]->x = x;
    }

    void Clear() {
        for (int k = 0; k < m_i; k++) {
            delete t[k];
        }
        m_i = 0;
    }
    int getSize() {
        return m_i;
    }
    Car* getThisCar(int i) {
        return t[i];
    }
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OBLIG1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OBLIG1));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OBLIG1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OBLIG1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

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
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int state = 0;
    RECT screen;
    static CarList xlist;
    static CarList ylist;
    static bool NSgreen;
    static bool WEgreen;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
            // TODO: Add any drawing code that uses hdc here...

            GetClientRect(hWnd, &screen);
            HBRUSH hb = CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(hdc, hb);
            //lys 1 og 2
            Rectangle(hdc, screen.right / 2-100, screen.bottom / 2+30, screen.right / 2-40 , screen.bottom / 2 + 190);
            Rectangle(hdc, screen.right / 2 + 50, screen.bottom / 2-200, screen.right / 2 + 110, screen.bottom / 2 -40);
            //vei 1 og 2
            Rectangle(hdc, 0, screen.bottom / 2 - 20, screen.right, screen.bottom / 2 + 20);
            Rectangle(hdc, screen.right / 2 - 20, 0, screen.right / 2 + 20, screen.bottom);
            DeleteObject(hb);
            
            int distance = 0;

            for (int i = 0; i < 3; i++) {
                hb = CreateSolidBrush(lights[i]);
                SelectObject(hdc, hb);
                Ellipse(hdc, screen.right / 2 - 95, screen.bottom / 2 + 35 + distance, screen.right / 2 - 45, screen.bottom / 2 + 80 + distance);
                DeleteObject(hb);
                
                hb = CreateSolidBrush(lights2[i]);
                SelectObject(hdc, hb);
                Ellipse(hdc, screen.right / 2 + 55, screen.bottom / 2 - 195 + distance, screen.right / 2 + 105, screen.bottom / 2 - 150 + distance);
                distance += 50;
                DeleteObject(hb);
            }
            hb = CreateSolidBrush(RGB(0,0,255));
            SelectObject(hdc, hb);
            xlist.draw(hdc);
            ylist.draw(hdc);
            DeleteObject(hb);
            hb = CreateSolidBrush(red);
            SelectObject(hdc, hb);
            swprintf_s(pweS, L"%d",pwe);
            swprintf_s(pnsS, L"%d", pns);
            TextOut(hdc, 50, 50, L"Sannsynlighet for nye biler fra vest per sekund: ", 49);
            TextOut(hdc, 400, 50, pweS, lstrlen(pweS));
            TextOut(hdc, 430, 50, L"%", 1);
            TextOut(hdc, 50, 70, L"Sannsynlighet for nye biler fra nord per sekund: ", 49);
            TextOut(hdc, 400, 70, pnsS, lstrlen(pnsS));
            TextOut(hdc, 430, 70, L"%", 1);
            DeleteObject(hb);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
    {
        static bool fTimer = false;
        if (!fTimer) {
            SetTimer(hWnd, 0, 1000, 0);
            SetTimer(hWnd, 1, 1000, 0);
            SetTimer(hWnd, 2, 100, 0);
        }
        else {
            KillTimer(hWnd, 0);
            KillTimer(hWnd, 1);
            KillTimer(hWnd, 2);
            InvalidateRect(hWnd, 0, true);
        }

        fTimer = !fTimer;
        break;

    }
    case WM_KEYDOWN:{
        switch (wParam) {
            case VK_SPACE: {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, probability);
                InvalidateRect(hWnd, 0, 0);



            }

        }
    }

    case WM_TIMER:
    {
        switch (wParam)
        {
        case 0:
        {
            if (state == 4) {
                state = 0;
            }
            if (state == 0) {
                lights[0] = red;
                lights[1] = grey;
                lights[2] = grey;

                lights2[0] = grey;
                lights2[1] = grey;
                lights2[2] = green;
                WEgreen = true;
            }
            else if (state == 1) {
                lights[0] = red;
                lights[1] = yellow;
                lights[2] = grey;

                lights2[0] = grey;
                lights2[1] = yellow;
                lights2[2] = grey;
                WEgreen = false;
            }
            else if (state == 2) {
                lights[0] = grey;
                lights[1] = grey;
                lights[2] = green;
                NSgreen = true;

                lights2[0] = red;
                lights2[1] = grey;
                lights2[2] = grey;
            }
            else if (state == 3) {
                lights[0] = grey;
                lights[1] = yellow;
                lights[2] = grey;
                NSgreen = false;

                lights2[0] = red;
                lights2[1] = yellow;
                lights2[2] = grey;
            }
            state++;
            InvalidateRect(hWnd, 0, true);

            break;
        }
        case 1:
        {
            GetClientRect(hWnd, &screen);
            if (pwe > rand() % 100) {
                xlist.push(new Car(screen.left, screen.bottom / 2, RGB(rand() % 255, rand() % 255, rand() % 255)));
            }
            if(pns > rand()%100){
                ylist.push(new Car(screen.left / 2 - 5, screen.top, RGB(rand() % 255, rand() % 255, rand() % 255)));
            }
            break;
        }
        case 2:
        {
            GetClientRect(hWnd, &screen);
            int xstop = screen.right / 2 - 20;
            for (int i = 0; i < xlist.getSize(); i++) {
                if (!WEgreen && (xlist.getThisCar(i)->x < xstop) && (xlist.getThisCar(i)->x > xstop - 11)) {
                    xstop -= 10;
                }
                else {
                    xlist.moveWE(i, 10, screen.bottom / 2);
                }
            }
            int ystop = screen.bottom / 2 - 10;
            for (int i = 0; i < ylist.getSize(); i++) {
                if (!NSgreen && (ylist.getThisCar(i)->y < ystop) && (ylist.getThisCar(i)->y > ystop - 11)) {
                    ystop -= 10;
                }
                else {
                    ylist.moveNS(i, 10, screen.right / 2 - 5);
                }
            }
                
            
            InvalidateRect(hWnd, 0, true);
            break;
        }
        }



    }
        
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

INT_PTR CALLBACK probability(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG: {
        SetWindowText(hDlg, L"Select Probability");
        HWND static1 = GetDlgItem(hDlg, IDC_STATIC);
        SetWindowText(static1, L"Type probability of cars coming W -> E:");
        HWND static2 = GetDlgItem(hDlg, IDC_STATIC2);
        SetWindowText(static1, L"Type probability of cars coming N -> S:");
        return (INT_PTR)TRUE;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            pwe = GetDlgItemInt(hDlg, IDC_EDIT1,0,0);
            pns = GetDlgItemInt(hDlg, IDC_EDIT2, 0, 0);
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
