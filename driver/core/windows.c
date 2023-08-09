#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

// These are being exported to other win32 modules.
HINSTANCE hInstance;
volatile BOOL quitting = FALSE;

// String buffer for Message Boxes
static char mbText[256];

DWORD WINAPI GraphicsThreadProc(LPVOID lpArg);

INT WINAPI
WinMain(HINSTANCE _hInstance, HINSTANCE hPrevInstance,
        PSTR lpCmdLine, INT nCmdShow)
{
    HANDLE gThread;
    DWORD gThreadID;
    MSG msg;

    hInstance = _hInstance;

    // Create the graphics thread.
    gThread = CreateThread(NULL, 0, GraphicsThreadProc, NULL, 0, &gThreadID);
    if (gThread == NULL)
        fatal_error("failed to create graphics thread.");

    WaitForSingleObject(gThread, INFINITE);

    return EXIT_SUCCESS;
}
