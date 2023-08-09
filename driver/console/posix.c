#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include "cgame.h"

static pthread_mutex_t console_usage = {0};

void
debug_log(const char *log, ...)
{
    va_list args;
    va_start(args, log);
    pthread_mutex_lock(&console_usage);
    vfprintf(stdout, log, args);
    fputc('\n', stdout);
    pthread_mutex_unlock(&console_usage);
    va_end(args);
}

void
warning(const char *warn, ...)
{
    va_list args;
    va_start(args, warn);
    pthread_mutex_lock(&console_usage);
    fprintf(stderr, "warning: ");
    vfprintf(stderr, warn, args);
    fputc('\n', stderr);
    pthread_mutex_unlock(&console_usage);
    va_end(args);
}

void
error(const char *err, ...)
{
    va_list args;
    va_start(args, err);
    pthread_mutex_lock(&console_usage);
    fprintf(stderr, "error: ");
    vfprintf(stderr, err, args);
    fputc('\n', stderr);
    pthread_mutex_unlock(&console_usage);
    va_end(args);
}

void
fatal_error(const char *err, ...)
{
    va_list args;
    va_start(args, err);
    pthread_mutex_lock(&console_usage);
    fprintf(stderr, "fatal error: ");
    vfprintf(stderr, err, args);
    fputc('\n', stderr);
    pthread_mutex_unlock(&console_usage);
    va_end(args);
    exit(EXIT_FAILURE);
}
