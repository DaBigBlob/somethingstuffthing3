
// written by me to replace all the shit i didnt find definitions for
// kill me

#ifndef _WINDOWS_
#ifndef WINDOWS_H
#define WINDOWS_H

/* Disable all warnings */
#if defined(_MSC_VER)
    #pragma warning(push, 0)
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#include "../lib/window.h"
#include "../lib/windows_base.h"
#include "../lib/gdi.h"
#include "../lib/process.h"
#include "../lib/misc.h"

#define HWND_TOP                        (HWND)0
#define WM_MOUSEMOVE                    0x0200
#define WM_LBUTTONDOWN                  0x0201
#define WM_RBUTTONDOWN                  0x0204
#define SWP_SHOWWINDOW                  0x0040
#define SWP_NOSIZE                      0x0001
#define GWLP_USERDATA                   -21
#define WM_SIZING                       0x0214
#define WM_MOVING                       0x0216
#define IDC_PERSON                      (LPCSTR) MAKEINTRESOURCE(32672)
#define IDC_PIN                         (LPCSTR) MAKEINTRESOURCE(32671)
#define IDC_HAND                        (LPCSTR) MAKEINTRESOURCE(32649)
#define IDC_NO                          (LPCSTR) MAKEINTRESOURCE(32648)
#define IDC_CROSS                       (LPCSTR) MAKEINTRESOURCE(32515)

#define GET_WPARAM(wp, lp)                      (wp)
#define GET_LPARAM(wp, lp)                      (lp)

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

#define GET_WM_ACTIVATE_STATE(wp, lp)           LOWORD(wp)
#define GET_WM_ACTIVATE_FMINIMIZED(wp, lp)      (BOOL)HIWORD(wp)
#define GET_WM_ACTIVATE_HWND(wp, lp)            (HWND)(lp)
#define GET_WM_ACTIVATE_MPS(s, fmin, hwnd)   \
        (WPARAM)MAKELONG((s), (fmin)), (LPARAM)(hwnd)

#define GET_WM_CHARTOITEM_CHAR(wp, lp)          (TCHAR)LOWORD(wp)
#define GET_WM_CHARTOITEM_POS(wp, lp)           HIWORD(wp)
#define GET_WM_CHARTOITEM_HWND(wp, lp)          (HWND)(lp)
#define GET_WM_CHARTOITEM_MPS(ch, pos, hwnd) \
        (WPARAM)MAKELONG((pos), (ch)), (LPARAM)(hwnd)

#define GET_WM_COMMAND_ID(wp, lp)               LOWORD(wp)
#define GET_WM_COMMAND_HWND(wp, lp)             (HWND)(lp)
#define GET_WM_COMMAND_CMD(wp, lp)              HIWORD(wp)
#define GET_WM_COMMAND_MPS(id, hwnd, cmd)    \
        (WPARAM)MAKELONG(id, cmd), (LPARAM)(hwnd)

typedef struct tagCREATESTRUCTA {
    LPVOID    lpCreateParams;
    HINSTANCE hInstance;
    HMENU     hMenu;
    HWND      hwndParent;
    int       cy;
    int       cx;
    int       y;
    int       x;
    LONG      style;
    LPCSTR    lpszName;
    LPCSTR    lpszClass;
    DWORD     dwExStyle;
} CREATESTRUCTA, *LPCREATESTRUCTA;

LONG_PTR WINAPI SetWindowLongPtrA(
    HWND     hWnd,
    int      nIndex,
    LONG_PTR dwNewLong
);

LONG_PTR WINAPI GetWindowLongPtrA(
    HWND hWnd,
    int  nIndex
);

// BOOL GetMessageA(
//   [out]          LPMSG lpMsg,
//   [in, optional] HWND  hWnd,
//   [in]           UINT  wMsgFilterMin,
//   [in]           UINT  wMsgFilterMax
// );
BOOL WINAPI GetMessageA(
    LPMSG lpMsg,
    HWND  hWnd,
    UINT  wMsgFilterMin,
    UINT  wMsgFilterMax
);

// https://github.com/tpn/winsdk-10/blob/master/Include/10.0.10240.0/um/WinUser.h
int WINAPI wsprintfA(
    LPSTR,
    LPCSTR,
    ...
);

BOOL WINAPI WriteConsoleA(
    HANDLE  hConsoleOutput,
    const CHAR    *lpBuffer,
    DWORD   nNumberOfCharsToWrite,
    LPDWORD lpNumberOfCharsWritten,
    LPVOID  lpReserved
);

BOOL WINAPI IsWindow(
    HWND hWnd
);


#if defined(__cplusplus)
}
#endif

/* Enable all warnings */
#if defined(_MSC_VER)
    #pragma warning(pop)
#endif

#endif /* WINDOWS_H */
#endif /* _WINDOWS_ */
