#include <pspkernel.h>
#include <pspdebug.h>

void
debug_log(const char *log, ...)
{
}

void
warning(const char *warn, ...)
{
}

void
error(const char *err, ...)
{
}

void
fatal_error(const char *err, ...)
{
    sceKernelExitGame();
}
