#include <stdbool.h>

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>

#define printf pspDebugScreenPrintf

#define VERS 1
#define REVS 0

PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
PSP_HEAP_THRESHOLD_SIZE_KB(0);

volatile bool game_is_running = true;

static int
exit_callback(int arg1, int arg2, void *common)
{
    game_is_running = false;
    return 0;
}

static int
callback_update_thread_proc(SceSize args, void *argp)
{
    int exitcb = sceKernelCreateCallback("Exit Callback",
                                         exit_callback, NULL);
    sceKernelRegisterExitCallback(exitcb);
    sceKernelSleepThreadCB();
    return 0;
}

int
main(void)
{
    int cbuthread = 0;

    cbuthread = sceKernelCreateThread("Callback Update Thread",
                                      callback_update_thread_proc,
                                      0x11, 0xFA0, THREAD_ATTR_USER, 0);

    if (cbuthread < 0)
        sceKernelExitGame();

    sceKernelStartThread(cbuthread, 0, 0);

    pspDebugScreenInit();

    while (game_is_running)
    {
        pspDebugScreenSetXY(0, 0);
        printf("hello, world!\n");
        sceDisplayWaitVblankStart();
    }

    sceKernelExitGame();
    return 0;
}

void
exit_game(void)
{
    game_is_running = false;
}
