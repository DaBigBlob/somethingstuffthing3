
#include "../lib/window.h"
#include "../lib/windows_base.h"
#include "../lib/gdi.h"
#include "../lib/process.h"
#include "../lib/misc.h"
#include "../lib/windows.h"

#define HARDNESS_GRADIENT 30

const char* chThts[] = {
    "Im gonna do it.",
    "Kidnapper? I barely know 're!"
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
    int ickyness;
} AppState, *PAppState;

void spawnThought(AppState* apSt, BOOL once) {
    apSt->ickyness ++;
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
    UpdateWindow(apSt->thtHwnd);
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
        case WM_MOUSEMOVE: {
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
            int ICKY_ZONE = (apSt->ickyness)*HARDNESS_GRADIENT;
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
                char str[20];
                wsprintfA(str, "MENTAL ILLNESS: %d", apSt->ickyness);
                TextOutA(hdc, 10, 10, str, lstrlenA(str));
            } else {
                const char* text = chThts[(apSt->ickyness)%cntCnThts];
                TextOutA(hdc, 10, 10, text, lstrlenA(text));
            }

            EndPaint(hWnd, &ps);
            break;
        }
        case WM_RBUTTONDOWN: {
            char* name;
            if (hWnd == apSt->mainHwnd) {
                name = "main window =>";
            } else {
                name = "thought window =>";
            }

            char str[20];
            wsprintfA(str, "%s X:%d, Y:%d", name, GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
            MessageBoxA(hWnd, str, "Ding!", MB_OK|MB_ICONEXCLAMATION);
            break;
        }
        case WM_LBUTTONDOWN: {
            char str[30];
            wsprintfA(
                str,
                "V9XM:%d,YM:%d,CXM:%d,CYM:%d,XT:%d,YT:%d,CXT:%d,CYT:%d",
                apSt->mainPosDim.x,apSt->mainPosDim.y,
                apSt->mainPosDim.cx,apSt->mainPosDim.cy,
                apSt->thtPosDim.x,apSt->thtPosDim.y,
                apSt->thtPosDim.cx,apSt->thtPosDim.cy
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
        .ickyness = 0
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
