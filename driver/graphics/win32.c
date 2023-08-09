#include <windows.h>

// This is being imported from the startup module.
extern HINSTANCE hInstance;
extern volatile BOOL quitting;

// These are being exported to other win32 modules.
WNDCLASS wc;
HWND hWnd;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
                                WPARAM wParam, LPARAM lParam);

void fatal_error(const char *error, ...);

DWORD WINAPI
GraphicsThreadProc(LPVOID lpArg)
{
    DWORD dwStyle, dwExStyle;
    RECT rect;
    INT x, y, width, height;
    MSG msg;

    // Register the window class.
    wc.lpszClassName = TEXT("cgame window");
    wc.lpszMenuName  = NULL;
    wc.lpfnWndProc   = (WNDPROC)WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    if (!RegisterClass(&wc))
        fatal_error("failed to register window class.");

    if (1 /* !fullscreen */)
    {
        dwStyle = WS_OVERLAPPEDWINDOW;
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    }

    // Calculate the initial size and position of the window.
    rect.left   = 0;
    rect.right  = 480;
    rect.top    = 0;
    rect.bottom = 272;
    AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);
    width  = rect.right  - rect.left;
    height = rect.bottom - rect.top; 
    if (1 /* !fullscreen */)
    {
        x = (GetSystemMetrics(SM_CXSCREEN) - width)  / 2;
        y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
    }

    // Create the window.
    hWnd = CreateWindowEx(dwExStyle, wc.lpszClassName, TEXT("cgame"),
                          dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                          x, y, width, height,
                          NULL, NULL, wc.hInstance, NULL);
    if (hWnd == NULL)
    {
        UnregisterClass(wc.lpszClassName, hInstance);
        fatal_error("failed to create window.");
    }

    // Put the window on screen.
    ShowWindow(hWnd, SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);

    // Enter the main message loop.
    msg.message = 0;
    while (!quitting)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                quitting = TRUE;
                continue;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                continue;
            }
        }
    }

    // wglDeleteContext(hRC);
    // ReleaseDC(hWnd, hDC);
    DestroyWindow(hWnd);
    UnregisterClass(wc.lpszClassName, hInstance);

    return msg.wParam;
}

static LRESULT CALLBACK
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
            break;
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
