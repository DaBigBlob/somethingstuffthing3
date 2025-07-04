
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
    WndPosDim mainPosDim;
    WndPosDim thtPosDim;
    WNDCLASS* PWndClass;
    int lvl;
    // RECT* HitBOx;
} AppState, *PAppState;

POINT GetCloseButtonCenter(HWND hwnd) {
    RECT windowRect;
    while (GetWindowRect(hwnd, &windowRect) == 0);

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

POINT GetCursorPosNoFail() {
    POINT p;
    while (GetCursorPos(&p) == 0);
    return p;
}

POINT MakeVector(POINT* src, POINT* dst) {
    POINT p = {
        .x = dst->x - src->x,
        .y = dst->y - src->y
    };
    return p;
}

BOOL VectorFits(POINT* d, RECT* r) {
    return (
        (r->left <= d->x) && (d->x <= r->right)
        &&
        (r->top <= d->y) && (d->y <= r->bottom)
    );
}

void spawnThought(AppState* apSt, BOOL once) {
    LONG lvl = ++ apSt->lvl;
    // apSt->HitBOx->bottom = lvl*HARDNESS_GRADIENT;
    // apSt->HitBOx->top = lvl*HARDNESS_GRADIENT;
    // apSt->HitBOx->left = lvl*HARDNESS_GRADIENT;
    // apSt->HitBOx->right = lvl*HARDNESS_GRADIENT;
    while ((apSt->thtHwnd = CreateWindowExA(
        0,
        apSt->PWndClass->lpszClassName,
        "the voice",
        WS_VISIBLE|WS_CAPTION|WS_OVERLAPPED|WS_SYSMENU,
        apSt->mainPosDim.cx/2,
        apSt->mainPosDim.cy/2,
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
        if (hWnd == apSt->mainHwnd) {
            apSt->mainPosDim.x = creaSt->x;
            apSt->mainPosDim.y = creaSt->y;
            apSt->mainPosDim.cx = creaSt->cx;
            apSt->mainPosDim.cy = creaSt->cy;
        } else {
            apSt->thtPosDim.x = creaSt->x;
            apSt->thtPosDim.y = creaSt->y;
            apSt->thtPosDim.cx = creaSt->cx;
            apSt->thtPosDim.cy = creaSt->cy;
        }
        SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)apSt);
    } else {
        apSt = (PAppState) GetWindowLongPtrA(hWnd, GWLP_USERDATA);
    }

    switch (msg) {
        // case WM_MOUSEMOVE: {
        //     int msx = GET_X_LPARAM(lp);
        //     int msy = GET_Y_LPARAM(lp);
        //     if (hWnd == apSt->mainHwnd) {
        //         msx += apSt->mainPosDim.x;
        //         msy += apSt->mainPosDim.y;
        //     } else {
        //         msx += apSt->thtPosDim.x;
        //         msy += apSt->thtPosDim.y;
        //     }

        //     // we assume the close button is 26px each side // TODO: derive instead
        //     #define CB_HSIDE 13
        //     int ICKY_ZONE = (apSt->ickyness)*HARDNESS_GRADIENT;
        //     int cx = (apSt->thtPosDim.x + apSt->thtPosDim.cx - CB_HSIDE);
        //     int cy = (apSt->thtPosDim.y - CB_HSIDE);

        //     int dx = cx - msx;
        //     int dy = cy - msy;

        //     if (
        //         (-ICKY_ZONE <= dx) && (dx <= ICKY_ZONE)
        //         &&
        //         (-ICKY_ZONE <= dy) && (dy <= ICKY_ZONE)
        //     ) {
        //         int mx = (cx + dx - apSt->thtPosDim.cx); //%(apSt->mainPosDim.cx - 50);
        //         int my = (cy + dy); //%(apSt->mainPosDim.cy - 50);
        //         // if (mx < 0) mx += apSt->mainPosDim.cx;
        //         // if (my < 0) my += apSt->mainPosDim.cy;
        //         if (mx < 0) mx = mx + apSt->mainPosDim.cx;
        //         if (my < 0) my = my + apSt->mainPosDim.cx;

        //         if (mx > (apSt->mainPosDim.cx - apSt->thtPosDim.cx)) mx = mx%((apSt->mainPosDim.cx - apSt->thtPosDim.cx));
        //         if (my > (apSt->mainPosDim.cy - apSt->thtPosDim.cy)) my = my%(apSt->mainPosDim.cy - apSt->thtPosDim.cy);

        //         SetWindowPos(apSt->thtHwnd, HWND_TOP, mx, my, CW_USEDEFAULT, CW_USEDEFAULT, SWP_SHOWWINDOW|SWP_NOSIZE);
        //     }
        //     break;
        // }
        case WM_MOUSEMOVE: {
            POINT ms = GetCursorPosNoFail();
            POINT cb = GetCloseButtonCenter(apSt->thtHwnd);
            // POINT v = MakeVector(&ms, &cb);

            // if (VectorFits(&v, apSt->HitBOx)) {
            //     RECT tr;
            //     GetWindowRect(apSt->thtHwnd, &tr);
            //     POINT g = {
            //         // .x = tr.
            //     };
            //     SetWindowPos(apSt->thtHwnd, HWND_TOP, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, SWP_SHOWWINDOW|SWP_NOSIZE);
            // }
            int msx = GET_X_LPARAM(lp);
            int msy = GET_Y_LPARAM(lp);
            if (hWnd == apSt->mainHwnd) {
                msx += apSt->mainPosDim.x;
                msy += apSt->mainPosDim.y;
            } else {
                msx += apSt->thtPosDim.x;
                msy += apSt->thtPosDim.y;
            }

            // we assume the close button is 26px each side // TODO: derive instead
            #define CB_HSIDE 13
            int ICKY_ZONE = (apSt->lvl)*HARDNESS_GRADIENT;
            int cx = (apSt->thtPosDim.x + apSt->thtPosDim.cx - CB_HSIDE);
            int cy = (apSt->thtPosDim.y - CB_HSIDE);

            int dx = cx - msx;
            int dy = cy - msy;

            if (
                (-ICKY_ZONE <= dx) && (dx <= ICKY_ZONE)
                &&
                (-ICKY_ZONE <= dy) && (dy <= ICKY_ZONE)
            ) {
                int mx = (cx + dx - apSt->thtPosDim.cx); //%(apSt->mainPosDim.cx - 50);
                int my = (cy + dy); //%(apSt->mainPosDim.cy - 50);
                // if (mx < 0) mx += apSt->mainPosDim.cx;
                // if (my < 0) my += apSt->mainPosDim.cy;
                if (mx < 0) mx = mx + apSt->mainPosDim.cx;
                if (my < 0) my = my + apSt->mainPosDim.cx;

                if (mx > (apSt->mainPosDim.cx - apSt->thtPosDim.cx)) mx = mx%((apSt->mainPosDim.cx - apSt->thtPosDim.cx));
                if (my > (apSt->mainPosDim.cy - apSt->thtPosDim.cy)) my = my%(apSt->mainPosDim.cy - apSt->thtPosDim.cy);

                SetWindowPos(apSt->thtHwnd, HWND_TOP, mx, my, CW_USEDEFAULT, CW_USEDEFAULT, SWP_SHOWWINDOW|SWP_NOSIZE);
            }
            break;
        }
        case WM_MOVE: {
            int xPos = (int)(short) LOWORD(lp);   // horizontal position 
            int yPos = (int)(short) HIWORD(lp);   // vertical position 
            if (hWnd == apSt->mainHwnd) {
                apSt->mainPosDim.x = xPos;
                apSt->mainPosDim.y = yPos;
            } else {
                apSt->thtPosDim.x = xPos;
                apSt->thtPosDim.y = yPos;
            }
            break;
        }
        case WM_SIZE: {
            int width = LOWORD(lp);
            int height = HIWORD(lp);
            if (hWnd == apSt->mainHwnd) {
                apSt->mainPosDim.cx = width;
                apSt->mainPosDim.cy = height;
            } else {
                apSt->thtPosDim.cx = width;
                apSt->thtPosDim.cy = height;
            }
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            if (hWnd == apSt->mainHwnd) {
                char str[27];
                wsprintfA(str, "MENTAL ILLNESS: %d", apSt->lvl);
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
                "V8,XM:%d,YM:%d,CXM:%d,CYM:%d,XT:%d,YT:%d,CXT:%d,CYT:%d",
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
