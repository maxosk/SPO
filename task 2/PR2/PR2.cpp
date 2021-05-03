#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

//gcc pr2.c -o pr2 -lgdi32 -lcomdlg32

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void OpenDialogText(HWND);
void SaveDialogText(HWND);
void LoadFileText(LPSTR);
void SaveFileText(LPSTR);

int var = 0;

void OpenDialogBin(HWND);
void SaveDialogBin(HWND);
void LoadFileBin(LPSTR);
void SaveFileBin(LPSTR);

HWND editBin;
HWND editText;
HWND staticBin;
HWND staticText;
HWND listHere;
HWND parent;



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {

    MSG  msg;
    WNDCLASS wc = { 0 };
    wc.lpszClassName = TEXT("Opendialog");
    wc.hInstance = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(0, IDC_ARROW);


    RegisterClass(&wc);
    parent = CreateWindow(wc.lpszClassName, TEXT("Opendialog"),
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        150, 150, 750, 540, 0, 0, hInstance, 0);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg) {

    case WM_CREATE:

        CreateWindowW(L"Static", L"Бинарный файл",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            230, 10, 200, 25,
            hwnd, (HMENU)0, NULL, NULL);

        staticBin = CreateWindowEx(WS_EX_RIGHTSCROLLBAR, TEXT("edit"), NULL,
            WS_VISIBLE | WS_CHILD | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE,
            30, 40, 260, 180,
            hwnd, (HMENU)11, NULL, NULL);
        EnableWindow(staticBin, FALSE);
        editBin = CreateWindowEx(WS_EX_RIGHTSCROLLBAR, TEXT("edit"), NULL,
            WS_VISIBLE | WS_CHILD | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE,
            300, 40, 260, 180,
            hwnd, (HMENU)12, NULL, NULL);

        CreateWindowW(L"Static", L"Меню",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            580, 10, 200, 25,
            hwnd, (HMENU)0, NULL, NULL);

        listHere = CreateWindowW(WC_LISTBOXW, NULL,
            WS_VISIBLE | WS_CHILD | LBS_NOTIFY,
            580, 40, 130, 50, hwnd, (HMENU)5, NULL, NULL
        );
        SendMessageW(listHere, LB_ADDSTRING, 0, (LPARAM)L"BYTE");
        SendMessageW(listHere, LB_ADDSTRING, 0, (LPARAM)L"CHAR");
        SendMessageW(listHere, LB_ADDSTRING, 0, (LPARAM)L"DWORD");


        CreateWindowW(L"button", L"Открыть",
            WS_VISIBLE | WS_CHILD,
            580, 150, 80, 25, hwnd, (HMENU)1, NULL, NULL
        );

        CreateWindowW(L"button", L"Сохранить",
            WS_VISIBLE | WS_CHILD,
            580, 180, 80, 25, hwnd, (HMENU)2, NULL, NULL
        );

        CreateWindowW(L"Static", L"Текстовый файл",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            230, 270, 200, 25,
            hwnd, (HMENU)0, NULL, NULL);
        staticText = CreateWindowEx(WS_EX_RIGHTSCROLLBAR, TEXT("edit"), NULL,
            WS_VISIBLE | WS_CHILD | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE,
            30, 300, 260, 180,
            hwnd, (HMENU)21, NULL, NULL);
        EnableWindow(staticText, FALSE);
        editText = CreateWindowEx(WS_EX_RIGHTSCROLLBAR, TEXT("edit"), NULL,
            WS_VISIBLE | WS_CHILD | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE,
            300, 300, 260, 180,
            hwnd, (HMENU)22, NULL, NULL);

        CreateWindowW(L"button", L"Открыть",
            WS_VISIBLE | WS_CHILD,
            580, 395, 80, 25, hwnd, (HMENU)3, NULL, NULL
        );

        CreateWindowW(L"button", L"Сохранить",
            WS_VISIBLE | WS_CHILD,
            580, 425, 80, 25, hwnd, (HMENU)4, NULL, NULL
        );

        break;

    case WM_COMMAND:
        if (wParam == 1) {
            OpenDialogBin(hwnd);
        }
        else if (wParam == 2) {
            SaveDialogBin(hwnd);
        }if (wParam == 3) {
            OpenDialogText(hwnd);
        }
        else if (wParam == 4) {
            SaveDialogText(hwnd);
        }
        else if (LOWORD(wParam) == 5) {
            if (HIWORD(wParam) == LBN_SELCHANGE)
                var = (int)SendMessageW(listHere, LB_GETCURSEL, 0, 0);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void OpenDialogText(HWND hwnd) {

    OPENFILENAME ofn;
    TCHAR szFile[MAX_PATH];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.hwndOwner = hwnd;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = TEXT("TXT files(*.txt)\0*.txt\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrFileTitle = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
        LoadFileText(ofn.lpstrFile);

}

void SaveDialogText(HWND hwnd) {

    OPENFILENAME ofn;
    TCHAR szFile[MAX_PATH];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.hwndOwner = hwnd;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = TEXT("TXT files(*.txt)\0*.txt\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrFileTitle = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST;

    if (GetSaveFileName(&ofn))
        SaveFileText(ofn.lpstrFile);

}

void LoadFileText(LPSTR file) {

    HANDLE hFile;
    DWORD dwSize;
    DWORD dw;

    LPBYTE lpBuffer = NULL;
    char buffer[100];

    hFile = CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    dwSize = GetFileSize(hFile, NULL);
    lpBuffer = (LPBYTE)HeapAlloc(GetProcessHeap(),
        HEAP_GENERATE_EXCEPTIONS, dwSize + 1);
    ReadFile(hFile, (LPWSTR)lpBuffer, dwSize, &dw, NULL);
    CloseHandle(hFile);
    lpBuffer[dwSize] = 0;
    SetWindowText(editText, (LPSTR)lpBuffer);
    int n, i = 0;
    while (i < dwSize) {
        n += sprintf(buffer + n, "%x ", (unsigned char)lpBuffer[i]);
        i++;
    }

    SetWindowText(staticText, (LPSTR)buffer);



    HeapFree(GetProcessHeap(), 0, lpBuffer);
}

void SaveFileText(LPSTR file) {

    HANDLE hFile;
    DWORD dw;
    DWORD dwto;
    wchar_t buf[300];

    hFile = CreateFile(file, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    int len = GetWindowTextLength(editText) + 1;
    GetWindowText(editText, (LPSTR)buf, len);
    dwto = wcslen(buf) * sizeof(wchar_t);
    WriteFile(hFile, (LPVOID)buf, dwto, &dw, NULL);
    CloseHandle(hFile);
};

void OpenDialogBin(HWND hwnd) {

    OPENFILENAME ofn;
    TCHAR szFile[MAX_PATH];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.hwndOwner = hwnd;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = TEXT("BINARY files(*.bin)\0*.bin\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrFileTitle = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
        LoadFileBin(ofn.lpstrFile);

}

void SaveDialogBin(HWND hwnd) {

    OPENFILENAME ofn;
    TCHAR szFile[MAX_PATH];
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.hwndOwner = hwnd;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = TEXT("BINARY files(*.bin)\0*.bin\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrFileTitle = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST;

    if (GetSaveFileName(&ofn))
        SaveFileBin(ofn.lpstrFile);

}


void LoadFileBin(LPSTR file) {

    HANDLE hFile;
    DWORD dwSize;
    DWORD dw;

    LPBYTE lpBuffer = NULL;


    hFile = CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    dwSize = GetFileSize(hFile, NULL);
    lpBuffer = (LPBYTE)HeapAlloc(GetProcessHeap(),
        HEAP_GENERATE_EXCEPTIONS, dwSize + 1);
    ReadFile(hFile, (LPWSTR)lpBuffer, dwSize, &dw, NULL);
    CloseHandle(hFile);
    lpBuffer[dwSize] = 0;
    char buf[300];
    int k, l = 0;
    if (var == 0) {
        while (l < dwSize) {
            k += sprintf(buf + k, "%x ", (unsigned char)lpBuffer[l]);
            l++;
        }
        SetWindowText(editBin, (LPSTR)buf);
    }if (var == 1) {
        while (l < dwSize) {
            k += sprintf(buf + k, "%c ", (unsigned char)lpBuffer[l]);
            l++;
        }
        SetWindowText(editBin, (LPSTR)buf);
    }if (var == 2) {
        SetWindowText(editBin, (LPSTR)lpBuffer);
    }

    char buffer[300];
    int n, i = 0;
    while (i < dwSize) {
        n += sprintf(buffer + n, "%x ", (unsigned char)lpBuffer[i]);
        i++;
    }
    SetWindowText(staticBin, (LPSTR)buffer);
    HeapFree(GetProcessHeap(), 0, lpBuffer);
}

void SaveFileBin(LPSTR file) {

    HANDLE hFile;
    DWORD dw;
    DWORD dwto;
    wchar_t buf[300];

    hFile = CreateFile(file, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    int len = GetWindowTextLength(editBin) + 1;
    GetWindowText(editBin, (LPSTR)buf, len);
    dwto = wcslen(buf) * sizeof(wchar_t);
    WriteFile(hFile, (LPVOID)buf, dwto, &dw, NULL);
    CloseHandle(hFile);
};
