
#include "../lib/window.h"
#include "../lib/windows_base.h"
#include "../lib/gdi.h"
#include "../lib/process.h"
#include "../lib/misc.h"
#include "../lib/windows.h"

typedef struct _tagAppState {
    HWND mainHwnd;
    HWND thtHwnd;
} AppState, *PAppState;

LRESULT CALLBACK MainWndProc(
    HWND hWnd,
    UINT msg,
    WPARAM wp,
    LPARAM lp
){
    switch (msg) {
        case WM_CREATE: {
            MessageBoxA(hWnd, "Window created!", "Ding!", MB_OK|MB_ICONEXCLAMATION);
            break;
        }
        case WM_MOUSEMOVE: {
            // SetWindowPos(hWnd, HWND_TOP, GET_X_LPARAM(lp), GET_Y_LPARAM(lp), CW_USEDEFAULT, CW_USEDEFAULT, SWP_SHOWWINDOW|SWP_NOSIZE);
            // MessageBoxA(hWnd, "Window created!", "Ding!", MB_OK|MB_ICONEXCLAMATION);
            break;
        }
        case WM_LBUTTONDOWN: {
            char str[20];
            wsprintfA(str, "X:%d, Y:%d", GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
            
            SetWindowPos(hWnd, HWND_TOP, GET_X_LPARAM(lp), GET_Y_LPARAM(lp), CW_USEDEFAULT, CW_USEDEFAULT, SWP_SHOWWINDOW|SWP_NOSIZE);
            MessageBoxA(hWnd, str, "Ding!", MB_OK|MB_ICONEXCLAMATION);
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProcA(hWnd, msg, wp, lp);
}

int WINAPI WinMain() {
    HINSTANCE hInstance = GetModuleHandleA(0);
    WNDCLASS MainWinClass = {
        .style = 0,
        .lpfnWndProc = MainWndProc,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = hInstance,
        .hIcon = 0,
        .hCursor = LoadCursorA(
            hInstance,
            (LPCTSTR) MAKEINTRESOURCE(32672) // mouse cursor-arrow
        ),
        .hbrBackground = (void *)5, // std window color
        .lpszClassName = "intrusive thoughts"
    };
    while (RegisterClassA(&MainWinClass) == 0);

    AppState apSt;

    while ((apSt.thtHwnd = CreateWindowExA(
        0,
        MainWinClass.lpszClassName,
        "the voice",
        WS_VISIBLE|WS_CAPTION,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        hInstance,
        0
    )) == 0);

    while ((apSt.mainHwnd = CreateWindowExA(
        0,
        MainWinClass.lpszClassName,
        MainWinClass.lpszClassName,
        WS_VISIBLE|WS_MAXIMIZE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        hInstance,
        0
    )) == 0);

    MSG msg;
    while (GetMessageA(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    ExitProcess(0);
    return 0;
}
