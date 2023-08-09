#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/time.h>
#include <pthread.h>
#include <X11/Xlib.h>

#include "cgame.h"

static struct timeval now, last_time;

volatile bool game_is_running = true;

void *graphics_thread_proc(void *arg);
void setup_timing(void);
void update_timing(void);

void
idle_proc(void)
{
    sleep(0);
}

int
main(int argc, char *argv[])
{
    int err;
    pthread_t gthread;

    XInitThreads();

    err = pthread_create(&gthread, NULL, &graphics_thread_proc, NULL);
    if (err != 0)
    {
        fatal_error("failed to create graphics thread.");
        exit(EXIT_FAILURE);
    }

    gettimeofday(&last_time, NULL);
    while (game_is_running)
    {
        update_scene();
        last_time = now;
        sleep(0);
    }

    pthread_join(gthread, NULL);

    return 0;
}

void
exit_game(void)
{
    game_is_running = false;
}

float
getdt(void)
{
    struct timeval res;
    gettimeofday(&now, NULL);
    timersub(&now, &last_time, &res);
    return (float)res.tv_usec / 1000000;
}
