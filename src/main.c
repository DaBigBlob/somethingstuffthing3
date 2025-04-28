
#include "../lib/window.h"
#include "../lib/windows_base.h"
#include "../lib/gdi.h"
#include "../lib/process.h"
#include "../lib/misc.h"
#include "../lib/windows.h"

LRESULT CALLBACK Wndproc(
    HWND hWnd,
    UINT msg,
    WPARAM wp,
    LPARAM lp
){
    switch (msg) {
        case WM_CREATE:
            MessageBoxA(hWnd, "Window created!", "Ding!", MB_OK|MB_ICONEXCLAMATION);
            break;
        case WM_MOUSEMOVE:
            SetWindowPos(hWnd, HWND_TOP, GET_X_LPARAM(lp), GET_Y_LPARAM(lp), 20, 20, SWP_SHOWWINDOW|SWP_NOSIZE);
            // MessageBoxA(hWnd, "Window created!", "Ding!", MB_OK|MB_ICONEXCLAMATION);
            break;
        case WM_LBUTTONDOWN:
            SetWindowPos(hWnd, HWND_TOP, GET_X_LPARAM(lp), GET_Y_LPARAM(lp), 20, 20, SWP_SHOWWINDOW|SWP_NOSIZE);
            MessageBoxA(hWnd, "Your mother is fat!", "Ding!", MB_OK|MB_ICONEXCLAMATION);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcA(hWnd, msg, wp, lp);
}

int WINAPI WinMain() {
    HINSTANCE hInstance = GetModuleHandleA(0);
    WNDCLASS wc = {
        .style = 0,
        .lpfnWndProc = Wndproc,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = hInstance,
        .hIcon = LoadIconA(
            hInstance,
            (LPCSTR)32512 // application icon
        ),
        .hCursor = LoadCursorA(
            hInstance,
            (LPSTR)0x07f00 // mouse cursor-arrow
        ),
        .hbrBackground = (void *)5, // std window color
        .lpszClassName = "intrusive thoughts"
    };
    if (RegisterClassA(&wc) == 0) {
        ExitProcess(1);
        return 1;
    }

    HWND mainWn;
    if ((mainWn = CreateWindowExA(
        0,
        wc.lpszClassName,
        wc.lpszClassName,
        WS_VISIBLE|WS_MAXIMIZE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        hInstance,
        0
    )) == 0) {
        ExitProcess(1);
        return 1;
    }
    ShowWindow(mainWn, SW_SHOW);
    UpdateWindow(mainWn);

    MSG msg;
    while (GetMessageA(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    ExitProcess(0);
    return 0;
}
