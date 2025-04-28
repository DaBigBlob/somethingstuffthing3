
#include "../lib/window.h"
#include "../lib/windows_base.h"
#include "../lib/gdi.h"
#include "../lib/process.h"
#include "../lib/misc.h"
#include "../lib/windows.h"

#define HARDNESS_GRADIENT 30

const char* chThts[] = {
    "00 end myself",
    "11 end myself end myself end myself",
    "22 end myself end myself",
    "33 end"
};
const int cntCnThts = sizeof(chThts)/sizeof(chThts[0]);

typedef struct tagWndPosDim {
    int       cy;
    int       cx;
    int       y;
    int       x;
} WndPosDim, *PWndPosDim;

typedef struct tagAppState {
    HWND mainHwnd;
    HWND thtHwnd;
    WNDCLASS* PWndClass;
    int lvl;
    LONG hb;
} AppState, *PAppState;

POINT GetCursorPosNoFail() {
    POINT p;
    while (GetCursorPos(&p) == 0);
    return p;
}

RECT GetWindowRectNoFail(HWND hwnd) {
    RECT r;
    while (GetWindowRect(hwnd, &r) == 0);
    return r;
}

POINT GetCloseButtonCenter(HWND hwnd) {
    RECT windowRect = GetWindowRectNoFail(hwnd);
    int buttonWidth;
    while ((buttonWidth = GetSystemMetrics(SM_CXSIZE)) == 0);
    int frameWidth;
    while ((frameWidth = GetSystemMetrics(SM_CXFRAME)) == 0);
    int frameHeight;
    while ((frameHeight = GetSystemMetrics(SM_CYFRAME)) == 0);
    int captionHeight;
    while ((captionHeight = GetSystemMetrics(SM_CYCAPTION)) == 0);

    POINT p;
    p.x = windowRect.right - frameWidth - (buttonWidth / 2);
    p.y = windowRect.top + frameHeight + (captionHeight / 2);

    return p;
}

void spawnThought(AppState* apSt, BOOL once) {
    LONG lvl = ++ apSt->lvl;
    apSt->hb = lvl*HARDNESS_GRADIENT;

    RECT mr;
    GetWindowRect(apSt->mainHwnd, &mr);
    POINT p = {
        .x = (mr.right-mr.left)/2,
        .y = (mr.bottom-mr.top)/2,
    };
    while ((apSt->thtHwnd = CreateWindowExA(
        0,
        apSt->PWndClass->lpszClassName,
        "the voice",
        WS_VISIBLE|WS_CAPTION|WS_OVERLAPPED|WS_SYSMENU,
        p.x,
        p.y,
        150,
        90,
        apSt->mainHwnd,
        0,
        apSt->PWndClass->hInstance,
        apSt
    )) == 0) {
        if (once) return;
    };
    InvalidateRect(apSt->thtHwnd, NULL, TRUE);
    UpdateWindow(apSt->thtHwnd);
    InvalidateRect(apSt->mainHwnd, NULL, TRUE);
    UpdateWindow(apSt->mainHwnd);
}

LRESULT CALLBACK MainWndProc(
    HWND hWnd,
    UINT msg,
    WPARAM wp,
    LPARAM lp
){
    PAppState apSt;
    if (msg == WM_CREATE) {
        LPCREATESTRUCTA creaSt = (LPCREATESTRUCTA) lp;
        apSt = creaSt->lpCreateParams;
        SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)apSt);
    } else {
        apSt = (PAppState) GetWindowLongPtrA(hWnd, GWLP_USERDATA);
    }

    switch (msg) {
        case WM_MOUSEMOVE: {
            POINT ms = GetCursorPosNoFail();
            POINT cb = GetCloseButtonCenter(apSt->thtHwnd);
            RECT tr = GetWindowRectNoFail(apSt->thtHwnd);
            RECT mr = GetWindowRectNoFail(apSt->mainHwnd);

            int ICKY_ZONE = apSt->hb;
            int dx = cb.x - ms.x;
            int dy = cb.y - ms.y;

            if (
                (-ICKY_ZONE <= dx) && (dx <= ICKY_ZONE)
                &&
                (-ICKY_ZONE <= dy) && (dy <= ICKY_ZONE)
            ) {
                int gx = (cb.x + dx - (cb.x - tr.left));
                int gy = (cb.y + dy);

                int mcx = mr.right-mr.left;
                int mcy = mr.bottom-mr.top;
                
                if (gx < 0) gx = gx + mcx;
                if (gy < 0) gy = gy + mcy;

                // int 
                if (gx > (mr.right-tr.left)) gx = gx%(mr.right-tr.left);
                if (gy > (mr.bottom-tr.top)) gy = gy%(mr.bottom-tr.top);

                SetWindowPos(apSt->thtHwnd, HWND_TOP, gx, gy, CW_USEDEFAULT, CW_USEDEFAULT, SWP_SHOWWINDOW|SWP_NOSIZE);
            }
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            if (hWnd == apSt->mainHwnd) {
                char str[27];
                wsprintfA(str, "MENTAL ILLNESS LEVEL: %d", apSt->lvl);
                TextOutA(hdc, 10, 10, str, lstrlenA(str));
            } else {
                const char* text = chThts[(apSt->lvl)%cntCnThts];
                TextOutA(hdc, 10, 10, text, lstrlenA(text));
            }

            EndPaint(hWnd, &ps);
            break;
        }
        case WM_RBUTTONDOWN: {
            char* name;
            if (hWnd == apSt->mainHwnd) {
                name = "main window p =>";
            } else {
                name = "thought window p =>";
            }

            POINT p;
            GetCursorPos(&p);

            char str[20];
            wsprintfA(str, "%s X:%d, Y:%d", name, p.x, p.y);
            MessageBoxA(hWnd, str, "Ding!", MB_OK|MB_ICONEXCLAMATION);
            break;
        }
        case WM_LBUTTONDOWN: {
            RECT mr;
            RECT tr;
            GetWindowRect(apSt->mainHwnd, &mr);
            GetWindowRect(apSt->thtHwnd, &tr);
            char str[30];
            wsprintfA(
                str,
                "V9,XM:%d,YM:%d,CXM:%d,CYM:%d,XT:%d,YT:%d,CXT:%d,CYT:%d",
                mr.left,mr.top,
                mr.right-mr.left,mr.bottom-mr.top,
                tr.left,tr.top,
                tr.right-tr.left,tr.bottom-tr.top
            );
            MessageBoxA(hWnd, str, "Ding!", MB_OK|MB_ICONEXCLAMATION);
            break;
        }
        // UpdateWindow(hWnd);
        case WM_DESTROY: {
            if (hWnd == apSt->mainHwnd) {
                PostQuitMessage(0);
                return 0;
            }
            else spawnThought(apSt, TRUE); // TODO: add this back for release
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
            IDC_CROSS
        ),
        .hbrBackground = (void *)5, // std window color
        .lpszClassName = "intrusive thoughts"
    };
    while (RegisterClassA(&MainWinClass) == 0);

    AppState apSt = {
        .PWndClass = &MainWinClass,
        .lvl = 0
    };

    while ((apSt.mainHwnd = CreateWindowExA(
        0,
        MainWinClass.lpszClassName,
        MainWinClass.lpszClassName,
        WS_VISIBLE|WS_CAPTION|WS_MAXIMIZE,
        0,
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        hInstance,
        &apSt
    )) == 0);
    ShowWindow(apSt.mainHwnd, SW_SHOWMAXIMIZED);

    spawnThought(&apSt, FALSE);

    MSG msg;
    while (GetMessageA(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    ExitProcess(0);
    return 0;
}
