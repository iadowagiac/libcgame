#include <string.h>

#include "cgame.h"

int camera_count = 0;
struct camera *cameras[10];

void
register_camera(struct camera *camera)
{
    if (camera_count == sizeof(cameras) / sizeof(*cameras))
        return;
    *(cameras + camera_count++) = camera;
}

void
unregister_camera(struct camera *camera)
{
    if (camera_count == 0)
        return;
    for (int i = 0; i < camera_count; i++)
    {
        if (*(cameras + i) == camera)
        {
            memmove(cameras + i, cameras + i + 1,
                    (--camera_count - i) * sizeof(struct camera));
            return;
        }
    }
}
