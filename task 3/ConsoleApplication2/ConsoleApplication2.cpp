#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>

//gcc pr3.c -o pr3 -lgdi32 -lcomdlg32

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DialogProcMod(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK PanelProc(HWND, UINT, WPARAM, LPARAM);

void CreateDialogBox(HWND);
void RegisterDialogClass(HWND);
void CreateDialogBox2(HWND);
void RegisterDialogClass2(HWND);
void AddMenus(HWND);
COLORREF ShowColorDialog(HWND);
void RegisterPanel(void);


COLORREF gColor = RGB(255, 255, 255);

HINSTANCE ghInstance;

HWND parent;
HWND child;
HWND childMod;
HWND textHere;
HWND listHere;
HWND hwndPanel;

wchar_t buf[300];
int sel = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {

    MSG  msg;
    WNDCLASSW wc = { 0 };

    wc.lpszClassName = L"Window";
    wc.hInstance = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    RegisterClassW(&wc);

    parent = CreateWindowW(wc.lpszClassName, L"Window",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        150, 150, 650, 250, NULL, NULL, hInstance, NULL);

    ghInstance = hInstance;

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg) {

    case WM_CREATE:
        //Регистрация модального дитя
    {
        WNDCLASSEXW wc = { 0 };
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.lpfnWndProc = (WNDPROC)DialogProcMod;
        wc.hInstance = ghInstance;
        wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
        wc.lpszClassName = L"DialogClassMod";
        RegisterClassExW(&wc);
    }
    //Регистрация немодального дитя
    {
        WNDCLASSEXW wc2 = { 0 };
        wc2.cbSize = sizeof(WNDCLASSEXW);
        wc2.lpfnWndProc = (WNDPROC)DialogProc;
        wc2.hInstance = ghInstance;
        wc2.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
        wc2.lpszClassName = L"DialogClass";
        RegisterClassExW(&wc2);
    }
    //МЕНЮ
    AddMenus(hwnd);
    //КНОПКИ И ПРОЧЕЕ
    CreateWindowW(L"button", L"Я модальный",
        WS_VISIBLE | WS_CHILD,
        20, 50, 100, 25, hwnd, (HMENU)1, NULL, NULL
    );
    CreateWindowW(L"button", L"Выход",
        WS_VISIBLE | WS_CHILD,
        170, 50, 80, 25, hwnd, (HMENU)2, NULL, NULL
    );

    textHere = CreateWindowW(L"Edit", L"HELP",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        20, 150, 150, 20, hwnd, (HMENU)3, NULL, NULL
    );
    CreateWindowW(L"button", L"Изменить",
        WS_VISIBLE | WS_CHILD,
        180, 150, 80, 25, hwnd, (HMENU)4, NULL, NULL
    );
    listHere = CreateWindowW(WC_LISTBOXW, NULL,
        WS_VISIBLE | WS_CHILD | LBS_NOTIFY,
        300, 10, 200, 170, hwnd, (HMENU)5, NULL, NULL
    );
    for (int i = 0; i < 10; i++) {
        SendMessageW(listHere, LB_ADDSTRING, 0,
            (LPARAM)L"HELP");
    }

    CreateWindowW(L"button", L"Я не модальный",
        WS_VISIBLE | WS_CHILD,
        10, 80, 130, 25, hwnd, (HMENU)6, NULL, NULL
    );
    CreateWindowW(L"button", L"Я сис. мод.",
        WS_VISIBLE | WS_CHILD,
        160, 80, 120, 25, hwnd, (HMENU)7, NULL, NULL
    );
    CreateWindowW(L"button", L"Color",
        WS_VISIBLE | WS_CHILD,
        520, 50, 80, 25,
        hwnd, (HMENU)8, NULL, NULL
    );
    CreateWindowW(TRACKBAR_CLASSW, L"Trackbar Control",
        WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
        10, 10, 250, 30, hwnd, (HMENU)0, NULL, NULL
    );
    RegisterPanel();
    hwndPanel = CreateWindowW(L"Panel", NULL,
        WS_CHILD | WS_VISIBLE,
        520, 90, 80, 80, hwnd, (HMENU)2, NULL, NULL
    );


    break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            childMod = CreateWindowExW(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
                L"DialogClassMod", L"Dialog Box",
                WS_VISIBLE | WS_SYSMENU | WS_CAPTION, 100, 100, 200, 150,
                NULL, NULL, ghInstance, NULL
            );
            EnableWindow(parent, FALSE);
        }
        else if (LOWORD(wParam) == 2) {
            PostQuitMessage(0);
        }
        else if (LOWORD(wParam) == 4) {
            int textLength_WithNUL = GetWindowTextLength(textHere) + 1;
            SendMessageW(textHere, LB_GETTEXT, sel, (LPARAM)buf);
            GetWindowTextW(textHere, buf, textLength_WithNUL);
            SendMessageW(listHere, LB_DELETESTRING, sel, 0);
            SendMessageW(listHere, LB_ADDSTRING, sel, (LPARAM)buf);
        }
        else if (LOWORD(wParam) == 5) {
            if (HIWORD(wParam) == LBN_SELCHANGE) {
                sel = (int)SendMessageW(listHere, LB_GETCURSEL, 0, 0);
                SendMessageW(listHere, LB_GETTEXT, sel, (LPARAM)buf);
                SetWindowTextW(textHere, buf);
            }
        }
        else if (LOWORD(wParam) == 6) {
            child = CreateWindowExW(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
                L"DialogClass", L"Dialog Box",
                WS_VISIBLE | WS_SYSMENU | WS_CAPTION, 100, 100, 200, 150,
                NULL, NULL, ghInstance, NULL
            );
        }
        else if (LOWORD(wParam) == 7) {
            MessageBox(hwnd, TEXT("I'm message box."), TEXT("оу"), MB_OK);
        }
        else if (LOWORD(wParam) == 8) {
            gColor = ShowColorDialog(hwnd);
            InvalidateRect(hwndPanel, NULL, TRUE);
        }
        break;

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK DialogProcMod(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {

    case WM_CREATE:
        CreateWindowW(L"button", L"Ok",
            WS_VISIBLE | WS_CHILD,
            50, 50, 80, 25, hwnd, (HMENU)1, NULL, NULL);
        break;

    case WM_COMMAND:
        EnableWindow(parent, TRUE);
        DestroyWindow(hwnd);
        break;

    case WM_CLOSE:
        EnableWindow(parent, TRUE);
        DestroyWindow(hwnd);
        break;

    }

    return (DefWindowProcW(hwnd, msg, wParam, lParam));
}

LRESULT CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {

    case WM_CREATE:
        CreateWindowW(L"button", L"Ok",
            WS_VISIBLE | WS_CHILD,
            50, 50, 80, 25, hwnd, (HMENU)1, NULL, NULL);
        break;

    case WM_COMMAND:
        DestroyWindow(hwnd);
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    }

    return (DefWindowProcW(hwnd, msg, wParam, lParam));
}

LRESULT CALLBACK PanelProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;

    switch (msg) {

    case WM_PAINT:
    {
        GetClientRect(hwnd, &rect);
        hdc = BeginPaint(hwnd, &ps);
        SetBkColor(hdc, gColor);
        ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, "", 0, NULL);
        EndPaint(hwnd, &ps);
        break;
    }
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void AddMenus(HWND hwnd) {

    HMENU hMenubar;
    HMENU hMenu;

    hMenubar = CreateMenu();
    hMenu = CreateMenu();

    AppendMenuW(hMenu, MF_STRING, 8, L"Вызов цвета");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, 2, L"Ещё один выход");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"Меню");
    SetMenu(hwnd, hMenubar);
}

COLORREF ShowColorDialog(HWND hwnd) {

    CHOOSECOLOR cc;
    static COLORREF crCustClr[16];

    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwnd;
    cc.lpCustColors = (LPDWORD)crCustClr;
    cc.rgbResult = RGB(0, 255, 0);
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
    ChooseColor(&cc);

    return cc.rgbResult;
}

void RegisterPanel(void) {

    WNDCLASSW rwc = { 0 };
    rwc.lpszClassName = L"Panel";
    rwc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    rwc.lpfnWndProc = PanelProc;
    RegisterClassW(&rwc);
}
