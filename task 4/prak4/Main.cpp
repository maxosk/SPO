#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>

#include <tchar.h>
//gcc s2.c -o pr3 -lgdi32 -lcomdlg32

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc2(HWND, UINT, WPARAM, LPARAM);


HINSTANCE ghInstance;

HWND parent;
HWND child;
UINT_PTR timer;

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
        550, 150, 400, 200, NULL, NULL, hInstance, NULL);

    ghInstance = hInstance;

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam,
    LPARAM lParam) {

    HDC hDeviceContext;
    PAINTSTRUCT paintStruct;
    RECT rectPlace;
    HFONT hFont;
    static char text[2] = { ' ','\0' };
    boolean f = TRUE;

    switch (msg) {

    case WM_CREATE:
        timer = SetTimer(hwnd, 1, 2500, NULL);
        SetTimer(hwnd, 2, 5000, NULL);

        break;

    case WM_PAINT:
        hDeviceContext = BeginPaint(hwnd, &paintStruct);
        GetClientRect(hwnd, &rectPlace);
        SetTextColor(hDeviceContext, RGB(0, 0, 0));
        hFont = CreateFont(100, 40, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Impact"));
        SelectObject(hDeviceContext, hFont);
        DrawText(hDeviceContext, _T("6ext"), 1, &rectPlace, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        EndPaint(hwnd, &paintStruct);
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_HOME:case VK_END:case VK_PRIOR:
        case VK_NEXT:case VK_LEFT:case VK_RIGHT:
        case VK_UP:case VK_DOWN:case VK_DELETE:
        case VK_SHIFT:case VK_SPACE:case VK_CONTROL:
        case VK_CAPITAL:case VK_MENU:case VK_TAB:
        case VK_BACK:case VK_RETURN:
            break;
        default:
            text[0] = (char)wParam;
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }break;
    case WM_LBUTTONDOWN:
        text[0] = ' ';
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    case WM_TIMER:
    {
        switch (wParam) {
        case 1:
        {
            WNDCLASSW w = { 0 };
            w.lpszClassName = L"Window2";
            w.hInstance = ghInstance;
            w.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
            w.lpfnWndProc = WndProc2;
            w.hCursor = LoadCursor(0, IDC_ARROW);
            RegisterClassW(&w);
            child = CreateWindowW(L"Window2", L"Window2",
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                150, 150, 400, 200, parent, NULL,
                ghInstance, NULL);
            SetFocus(child);
            KillTimer(hwnd, timer);
        }
        break;
        case 2:
            SetFocus(child);
            break;
        }
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK WndProc2(HWND hwnd, UINT msg, WPARAM wParam,
    LPARAM lParam) {

    HDC hDeviceContext;
    PAINTSTRUCT paintStruct;
    RECT rectPlace;
    HFONT hFont;
    static char text[2] = { ' ','\0' };

    switch (msg) {

    case WM_CREATE:
        SetTimer(hwnd, 3, 5000, NULL);
        break;

    case WM_PAINT:
        hDeviceContext = BeginPaint(hwnd, &paintStruct);
        GetClientRect(hwnd, &rectPlace);
        SetTextColor(hDeviceContext, RGB(0, 0, 0));
        hFont = CreateFont(100, 40, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Impact"));
        SelectObject(hDeviceContext, hFont);
        DrawText(hDeviceContext, _T("text") , 1, &rectPlace, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        EndPaint(hwnd, &paintStruct);
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_HOME:case VK_END:case VK_PRIOR:
        case VK_NEXT:case VK_LEFT:case VK_RIGHT:
        case VK_UP:case VK_DOWN:case VK_DELETE:
        case VK_SHIFT:case VK_SPACE:case VK_CONTROL:
        case VK_CAPITAL:case VK_MENU:case VK_TAB:
        case VK_BACK:case VK_RETURN:
            break;
        default:
            text[0] = (char)wParam;
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }break;
    case WM_LBUTTONDOWN:
        text[0] = ' ';
        InvalidateRect(hwnd, NULL, TRUE);
        break;

    case WM_TIMER:
    {
        SetFocus(parent);
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}
