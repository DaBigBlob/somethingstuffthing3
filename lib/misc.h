/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Copyright (c) Arvid Gerstmann. All rights reserved.
 */
#ifndef _WINDOWS_
#ifndef WINDOWS_MISC_H
#define WINDOWS_MISC_H
typedef long long __int64;

/* Disable all warnings */
#if defined(_MSC_VER)
    #pragma warning(push, 0)
#endif

#include <stdarg.h>

#ifndef WINDOWS_BASE_H
#include "windows_base.h"
#endif
#if defined(__cplusplus)
extern "C" {
#endif

/* MMRESULT: */
typedef UINT MMRESULT;
#define TIMERR_BASE                     96
#define TIMERR_NOERROR                  (0)
#define TIMERR_NOCANDO                  (TIMERR_BASE+1)
#define TIMERR_STRUCT                   (TIMERR_BASE+33)

/* Code Pages: */
#define CP_INSTALLED                0x00000001
#define CP_SUPPORTED                0x00000002
#define CP_ACP                      0
#define CP_OEMCP                    1
#define CP_MACCP                    2
#define CP_THREAD_ACP               3
#define CP_SYMBOL                   42
#define CP_UTF7                     65000
#define CP_UTF8                     65001

/* Format: */
#define FORMAT_MESSAGE_ALLOCATE_BUFFER  0x00000100
#define FORMAT_MESSAGE_ARGUMENT_ARRAY   0x00002000
#define FORMAT_MESSAGE_FROM_SYSTEM      0x00001000
#define FORMAT_MESSAGE_IGNORE_INSERTS   0x00000200
#define FORMAT_MESSAGE_FROM_HMODULE     0x00000800
#define FORMAT_MESSAGE_FROM_STRING      0x00000400

/* Language ID: */
#define MAKELANGID(p, s)                ((((WORD)(s)) << 10) | (WORD  )(p))
#define PRIMARYLANGID(lgid)             ((WORD)(lgid) & 0x3ff)
#define SUBLANGID(lgid)                 ((WORD)(lgid) >> 10)

#define LANG_NEUTRAL                    0x00
#define LANG_INVARIANT                  0x7f
#define LANG_ENGLISH                    0x09
#define LANG_GERMAN                     0x07

#define SUBLANG_NEUTRAL                 0x00
#define SUBLANG_DEFAULT                 0x01
#define SUBLANG_SYS_DEFAULT             0x02
#define SUBLANG_CUSTOM_DEFAULT          0x03
#define SUBLANG_CUSTOM_UNSPECIFIED      0x04
#define SUBLANG_UI_CUSTOM_DEFAULT       0x05
#define SUBLANG_ENGLISH_US              0x01
#define SUBLANG_ENGLISH_UK              0x02
#define SUBLANG_GERMAN                  0x01


/* ========================================================================== */
/* Structures: */
/* Time: */
typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME;
typedef PSYSTEMTIME LPSYSTEMTIME;

typedef struct _TIME_ZONE_INFORMATION {
    LONG       Bias;
    WCHAR      StandardName[32];
    SYSTEMTIME StandardDate;
    LONG       StandardBias;
    WCHAR      DaylightName[32];
    SYSTEMTIME DaylightDate;
    LONG       DaylightBias;
} TIME_ZONE_INFORMATION, *PTIME_ZONE_INFORMATION;
typedef PTIME_ZONE_INFORMATION LPTIME_ZONE_INFORMATION;


/* ========================================================================== */
/* Functions: */
/*
 * I could've implemented them in assembly, they're like 4 lines, but this is
 * avoiding the cost of a function call, if optimizations are turned on.
 */
#ifdef WIN32_BYTESWAP_MACROS
#define _byteswap_ulong(x)      (((unsigned long)(x) << 24) | \
                                (((unsigned long)(x) << 8) & 0xff0000) | \
                                (((unsigned long)(x) >> 8) & 0xff00) | \
                                ((unsigned long)(x) >> 24))
#define _byteswap_uint64(x)     (((unsigned __int64)(x) << 56) | \
                                (((unsigned __int64)(x) << 40) & 0xff000000000000ULL) | \
                                (((unsigned __int64)(x) << 24) & 0xff0000000000ULL) | \
                                (((unsigned __int64)(x) << 8)  & 0xff00000000ULL) | \
                                (((unsigned __int64)(x) >> 8)  & 0xff000000ULL) | \
                                (((unsigned __int64)(x) >> 24) & 0xff0000ULL) | \
                                (((unsigned __int64)(x) >> 40) & 0xff00ULL) | \
                                ((unsigned __int64)(x)  >> 56))
#else
unsigned short   __cdecl _byteswap_ushort(unsigned short   Number);
unsigned long    __cdecl _byteswap_ulong (unsigned long    Number);
// unsigned __int64 __cdecl _byteswap_uint64(unsigned __int64 Number);
#endif

unsigned int _rotl(unsigned int value, int shift);
// unsigned __int64 _rotl64(unsigned __int64 value, int shift);
unsigned char _BitScanForward(unsigned long * Index, unsigned long Mask);
// unsigned char _BitScanForward64(unsigned long * Index, unsigned __int64 Mask);


/* ========================================================================== */
/* Code Pages: */
UINT WINAPI GetACP();

/* ========================================================================== */
/* UTF-16 <-> UTF-8 conversion Functions: */
int WINAPI WideCharToMultiByte(
    UINT        CodePage,
    DWORD       dwFlags,
    LPCWSTR     lpWideCharStr,
    int         cchWideChar,
    LPSTR       lpMultiByteStr,
    int         cbMultiByte,
    LPCSTR      lpDefaultChar,
    LPBOOL      lpUsedDefaultChar);
int WINAPI MultiByteToWideChar(
    UINT        CodePage,
    DWORD       dwFlags,
    LPCSTR      lpMultiByteStr,
    int         cbMultiByte,
    LPWSTR      lpWideCharStr,
    int         cchWideChar);
int WINAPI lstrlenA(
        LPCSTR  lpString);
int WINAPI lstrlenW(
        LPCWSTR lpString);
LPCSTR WINAPI lstrcpyA(
        LPCSTR  lpString1,
        LPCSTR  lpString2);
LPCWSTR WINAPI lstrcpyW(
        LPCWSTR lpString1,
        LPCWSTR lpString2);

/* ========================================================================== */
/* Time: */
void WINAPI GetSystemTime(
        LPSYSTEMTIME lpSystemTime);
void WINAPI GetLocalTime(
        LPSYSTEMTIME lpSystemTime);
BOOL WINAPI QueryProcessCycleTime(
        HANDLE      hProcess,
        PULONG64    CycleTime);
BOOL WINAPI SystemTimeToFileTime(
  const SYSTEMTIME *    lpSystemTime,
        LPFILETIME      lpFileTime);
BOOL WINAPI FileTimeToSystemTime(
  const FILETIME *      lpFileTime,
        LPSYSTEMTIME    lpSystemTime);
LONG WINAPI CompareFileTime(
  const FILETIME *      lpFileTime1,
  const FILETIME *      lpFileTime2);
void WINAPI GetSystemTimeAsFileTime(
        LPFILETIME lpSystemTimeAsFileTime);
BOOL WINAPI SystemTimeToTzSpecificLocalTime(
        LPTIME_ZONE_INFORMATION lpTimeZone,
  const SYSTEMTIME * lpUniversalTime,
        LPSYSTEMTIME lpLocalTime);
DWORD timeGetTime(void);

/* ========================================================================== */
/* Environment: */
BOOL WINAPI SetEnvironmentVariableA(
        LPCSTR  lpName,
        LPCSTR  lpValue);
BOOL WINAPI SetEnvironmentVariableW(
        LPCWSTR lpName,
        LPCWSTR lpValue);
DWORD WINAPI GetEnvironmentVariableA(
        LPCSTR  lpName,
        LPCSTR  lpBuffer,
        DWORD   nSize);
DWORD WINAPI GetEnvironmentVariableW(
        LPCWSTR lpName,
        LPCWSTR lpBuffer,
        DWORD   nSize);

/* ========================================================================== */
/* Misc Functions: */
BOOL WINAPI DisableThreadLibraryCalls(
        HMODULE hModule);
DWORD WINAPI GetLastError(void);
VOID WINAPI SetLastError(
        DWORD dwErrCode);
void WINAPI Sleep(
        DWORD dwMilliseconds);
DWORD WINAPI SleepEx(
        DWORD dwMilliseconds,
        BOOL bAlertable);
HMODULE WINAPI GetModuleHandleA(
        LPCSTR lpModuleName);
HMODULE WINAPI GetModuleHandleW(
        LPCWSTR lpModuleName);
DWORD WINAPI FormatMessageA(
        DWORD   dwFlags,
        LPCVOID lpSource,
        DWORD   dwMessageId,
        DWORD   dwLanguageId,
        LPSTR   lpBuffer,
        DWORD   nSize,
        va_list *Arguments);
DWORD WINAPI FormatMessageW(
        DWORD   dwFlags,
        LPCVOID lpSource,
        DWORD   dwMessageId,
        DWORD   dwLanguageId,
        LPWSTR  lpBuffer,
        DWORD   nSize,
        va_list *Arguments);


/* ========================================================================== */
/* Timer Functions: */
DWORD WINAPI GetTickCount(void);
ULONGLONG WINAPI GetTickCount64(void);
BOOL WINAPI QueryPerformanceFrequency(
        LARGE_INTEGER *lpFrequency);
BOOL WINAPI QueryPerformanceCounter(
        LARGE_INTEGER *lpPerformanceCount);


/* ========================================================================== */
/* Multi Media Timer:                                                         */
#define TIMERR_NOERROR        (0)                  /* no error */
#define TIMERR_NOCANDO        (TIMERR_BASE+1)      /* request not completed */
#define TIMERR_STRUCT         (TIMERR_BASE+33)     /* time struct size */

typedef struct timecaps_tag {
    UINT    wPeriodMin;     /* minimum period supported  */
    UINT    wPeriodMax;     /* maximum period supported  */
} TIMECAPS, *PTIMECAPS, *NPTIMECAPS, *LPTIMECAPS;
typedef UINT MMRESULT;

MMRESULT WINAPI timeGetDevCaps(LPTIMECAPS ptc, UINT cbtc);
MMRESULT WINAPI timeBeginPeriod(UINT uPeriod);
MMRESULT WINAPI timeEndPeriod(UINT uPeriod);


/* ========================================================================== */
/* DLL Functions: */
HMODULE WINAPI LoadLibraryA(
        LPCSTR lpFileName);
HMODULE WINAPI LoadLibraryW(
        LPCWSTR lpFileName);
HMODULE WINAPI LoadLibraryExA(
        LPCSTR lpLibFileName,
        HANDLE hFile,
        DWORD  dwFlags);
HMODULE WINAPI LoadLibraryExW(
        LPCWSTR lpLibFileName,
        HANDLE  hFile,
        DWORD   dwFlags);
FARPROC WINAPI GetProcAddress(
        HMODULE hModule,
        LPCSTR lProcName);
PROC WINAPI wglGetProcAddress(
        LPCSTR lpszProc);
BOOL WINAPI FreeLibrary(
        HMODULE hModule);

/* ========================================================================== */
/* Libc Replacements: */
PVOID SecureZeroMemory(
        PVOID  ptr,
        SIZE_T cnt);

VOID __stosb(
        PBYTE  Destination,
        BYTE  Value,
        SIZE_T Count);

VOID __stosw(
        PWORD   Destination,
        WORD   Value,
        SIZE_T Count);

VOID __stosd(
        PDWORD Destination,
        DWORD Value,
        SIZE_T Count);

VOID __stosq(
        PDWORD64 Destination,
        DWORD64 Value,
        SIZE_T Count);

// FORCEINLINE PVOID SecureZeroMemory(
//         PVOID ptr,
//         SIZE_T cnt)
// {
//     volatile char* vptr = (volatile char*)ptr;

// #if defined(_M_AMD64)
//     __stosb((PBYTE)((DWORD64)vptr), 0, cnt);
// #else
//     while (cnt)
//     {
// #   if !defined(_M_CEE) && (defined(_M_ARM) || defined(_M_ARM64))
//         __iso_volatile_store8(vptr, 0);
// #   else
//         * vptr = 0;
// #   endif

//         vptr++;
//         cnt--;
//     }

// #endif // _M_AMD64

//     return ptr;
// }

// _NODISCARD _Check_return_
// int __cdecl memcmp(
//     _In_reads_bytes_(_Size) void const* _Buf1,
//     _In_reads_bytes_(_Size) void const* _Buf2,
//     _In_                    size_t      _Size
// );

// _CRT_INSECURE_DEPRECATE_MEMORY(memmove_s)
// _VCRTIMP void* __cdecl memmove(
//     _Out_writes_bytes_all_opt_(_Size) void* _Dst,
//     _In_reads_bytes_opt_(_Size)       void const* _Src,
//     _In_                              size_t      _Size
// );

// void* __cdecl memcpy(
//     _Out_writes_bytes_all_(_Size) void* _Dst,
//     _In_reads_bytes_(_Size)       void const* _Src,
//     _In_                          size_t      _Size
// );

// void* __cdecl memset(
//     _Out_writes_bytes_all_(_Size) void* _Dst,
//     _In_                          int    _Val,
//     _In_                          size_t _Size
// );


#define RtlEqualMemory(Destination,Source,Length) (!memcmp((Destination),(Source),(Length)))
#define RtlMoveMemory(Destination,Source,Length) memmove((Destination),(Source),(Length))
#define RtlCopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#define RtlFillMemory(Destination,Length,Fill) memset((Destination),(Fill),(Length))
#define RtlZeroMemory(Destination,Length) memset((Destination),0,(Length))

#define MoveMemory RtlMoveMemory
#define CopyMemory RtlCopyMemory
#define FillMemory RtlFillMemory
#define ZeroMemory RtlZeroMemory

UINT WINAPI GetDoubleClickTime(VOID);
BOOL WINAPI SetDoubleClickTime(UINT);


/* ========================================================================== */
/* Locales: */
#define LOCALE_NAME_INVARIANT               L""

#define LCMAP_LOWERCASE                     0x00000100
#define LCMAP_UPPERCASE						0x00000200

typedef struct _nlsversioninfo {
  DWORD dwNLSVersionInfoSize;
  DWORD dwNLSVersion;
  DWORD dwDefinedVersion;
} NLSVERSIONINFO, *LPNLSVERSIONINFO;

int WINAPI LCMapStringEx(
  LPCWSTR          lpLocaleName,
  DWORD            dwMapFlags,
  LPCWSTR          lpSrcStr,
  int              cchSrc,
  LPWSTR           lpDestStr,
  int              cchDest,
  LPNLSVERSIONINFO lpVersionInformation,
  LPVOID           lpReserved,
  LPARAM           sortHandle
);

/* ========================================================================== */
/* Resources: */
DECLARE_HANDLE(HRSRC);
typedef HANDLE HGLOBAL;

HRSRC WINAPI FindResourceA(
  HMODULE hModule,
  LPCSTR  lpName,
  LPCSTR  lpType
);

DWORD WINAPI SizeofResource(
  HMODULE hModule,
  HRSRC   hResInfo
);

HGLOBAL WINAPI LoadResource(
  HMODULE hModule,
  HRSRC   hResInfo
);

LPVOID WINAPI LockResource(
  HGLOBAL hResData
);

#if defined(__cplusplus)
}
#endif

/* Enable all warnings */
#if defined(_MSC_VER)
    #pragma warning(pop)
#endif

#endif /* WINDOWS_MISC_H */
#endif /* _WINDOWS_ */

