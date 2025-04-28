
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
    PAppState apSt;
    if (msg == WM_CREATE) {
        apSt = (((LPCREATESTRUCTA) lp)->lpCreateParams);
        SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)apSt);
    } else {
        apSt = (PAppState) GetWindowLongPtrA(hWnd, GWLP_USERDATA);
    }

    switch (msg) {
        case WM_MOUSEMOVE: {
            if (hWnd != apSt->mainHwnd) break;
            // SetWindowPos(hWnd, HWND_TOP, GET_X_LPARAM(lp), GET_Y_LPARAM(lp), CW_USEDEFAULT, CW_USEDEFAULT, SWP_SHOWWINDOW|SWP_NOSIZE);
            // MessageBoxA(hWnd, "Window created!", "Ding!", MB_OK|MB_ICONEXCLAMATION);
            SetWindowPos(apSt->thtHwnd, HWND_TOP, GET_X_LPARAM(lp), GET_Y_LPARAM(lp), CW_USEDEFAULT, CW_USEDEFAULT, SWP_SHOWWINDOW|SWP_NOSIZE);
            break;
        }
        case WM_LBUTTONDOWN: {
            if (hWnd != apSt->mainHwnd) break;

            char str[20];
            wsprintfA(str, "X:%d, Y:%d", GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
            MessageBoxA(hWnd, str, "Ding!", MB_OK|MB_ICONEXCLAMATION);
            break;
        }
        // UpdateWindow(hWnd);
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

    while ((apSt.mainHwnd = CreateWindowExA(
        0,
        MainWinClass.lpszClassName,
        MainWinClass.lpszClassName,
        WS_VISIBLE|WS_MAXIMIZE,
        0,
        0,
        100,
        100,
        0,
        0,
        hInstance,
        &apSt
    )) == 0);
    ShowWindow(apSt.mainHwnd, SW_SHOWMAXIMIZED);

    while ((apSt.thtHwnd = CreateWindowExA(
        0,
        MainWinClass.lpszClassName,
        "the voice",
        WS_VISIBLE|WS_CAPTION,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        apSt.mainHwnd,
        0,
        hInstance,
        &apSt
    )) == 0);

    MSG msg;
    while (GetMessageA(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    ExitProcess(0);
    return 0;
}
