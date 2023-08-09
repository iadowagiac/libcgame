#include <stdbool.h>
#include <stddef.h>

#include "cgame.h"
#include "assets/assets.h"

extern bool graphics_initialized;

extern int camera_count;
extern struct camera *cameras[];
extern struct entity *first_entity;

extern int scene_count;
extern const struct scene *scenes[];

static struct scene scene;

bool scene_loaded = false;

bool
load_scene(int i)
{
    if (!graphics_initialized)
        return false;
    if (i >= scene_count)
        return false;
    scene = **(scenes + i);
    scene.on_load_scene();
    scene_loaded = true;
    return true;
}

void
unload_scene(void)
{
    scene_loaded = false;
}

void
update_scene(void)
{
    if (!scene_loaded && !load_scene(1))
        return;
    for (struct entity *e = first_entity; e != NULL; e = e->next)
        e->handle_event(e, EVENT_UPDATE, NULL);
}

void
render_scene(void)
{
    if (!scene_loaded)
        return;
    for (int i = 0; i < camera_count; i++)
    {
        struct camera *camera = *(cameras + i);
        setup_camera(camera);
        for (struct entity *e = first_entity; e != NULL; e = e->next)
        {
            if (e->layers & camera->layer_mask)
                e->handle_event(e, EVENT_RENDER, NULL);
        }
        // wait_for_message(RENDERING_DONE);
        swap_buffers();
        // wait_for_message(VERTICAL_RETRACE);
   }
}
