
#include "../lib/window.h"
#include "../lib/windows_base.h"
#include "../lib/gdi.h"
#include "../lib/process.h"
#include "../lib/misc.h"
#include "../lib/windows.h"

const char* chThts[] = {
    "Turning the alarm off and sleep.",
    "Computer hard. Become Amish.",
    "Do an AI startup.",
    "Invest in Apple, Inc.",
    "Kidnapper? I barely know 'er.",
    "The knives are speaking to me.",
    "TBD"
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
    int captionHeight;
    while ((captionHeight = GetSystemMetrics(SM_CYCAPTION)) == 0);

    POINT p;
    p.x = windowRect.right - frameWidth - (buttonWidth / 2);
    p.y = windowRect.top + frameWidth + (captionHeight / 2);

    return p;
}

void spawnThought(AppState* apSt, BOOL once) {
    int buttonWidth;
    while ((buttonWidth = GetSystemMetrics(SM_CXSIZE)) == 0);
    int frameWidth;
    while ((frameWidth = GetSystemMetrics(SM_CXFRAME)) == 0);
    int captionHeight;
    while ((captionHeight = GetSystemMetrics(SM_CYCAPTION)) == 0);

    LONG lvl = ++ apSt->lvl;
    apSt->hb = lvl*buttonWidth;

    HDC hdc = GetDC(apSt->thtHwnd);
    const char* nxt_tht = chThts[(apSt->lvl)%cntCnThts];
    SIZE tbs;
    while (GetTextExtentPoint32A(hdc, nxt_tht, lstrlenA(nxt_tht), &tbs) == 0);
    ReleaseDC(apSt->thtHwnd, hdc);
    tbs.cx = tbs.cy + tbs.cx + frameWidth*2;
    tbs.cy = (tbs.cy)*2 + frameWidth + captionHeight;

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
        tbs.cx,
        tbs.cy,
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

                int px = mr.right - (tr.right-tr.left);
                int py = mr.bottom - (tr.bottom-tr.top);
                
                if (gx < 0) gx = gx + px;
                else if (gx >= px) gx = gx%px;

                if (gy < 0) gy = gy + py;
                else if (gy >= py) gy = gy%py;

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
                TextOutA(hdc, 0, 0, text, lstrlenA(text));
            }

            EndPaint(hWnd, &ps);
            break;
        }
        case WM_RBUTTONDOWN: {
            MessageBoxA(hWnd, "Holy sheet bro. Damn.", "!", MB_OK|MB_ICONEXCLAMATION);
            break;
        }
        case WM_LBUTTONDOWN: {
            MessageBoxA(hWnd, "Woah there buddy. Calm down.", "?", MB_OK|MB_ICONEXCLAMATION);
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
