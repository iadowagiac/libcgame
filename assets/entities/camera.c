#include <cgame.h>
#include "../assets.h"

struct en_camera
{
    ENTITY_HEADER
    struct camera camera;
};

/*
static const struct component_array_element component_array[] =
{
    { COMPONENT_TRANSFORM, offsetof(struct en_camera, transform) },
    { COMPONENT_CAMERA,    offsetof(struct en_camera, camera)    },
}
*/

static void
handle_event(struct en_camera *entity, int event, void *message)
{
    switch (event)
    {
        case EVENT_SPAWN:
            entity->camera.clear_color  = RGBA32(0.8, 0.8, 0.8, 1.0);
            entity->camera.fov          = 40.0;
            entity->camera.ortho.left   = -10.0;
            entity->camera.ortho.right  =  10.0;
            entity->camera.ortho.top    = -10.0;
            entity->camera.ortho.bottom =  10.0;
            entity->camera.ortho.near   =  1.0;
            entity->camera.ortho.far    =  1000.0;
            entity->camera.layer_mask   = ~0;
            register_camera(&entity->camera);
            break;
        case EVENT_DESPAWN:
            unregister_camera(&entity->camera);
            break;
    }
}

const struct entity_type en_camera =
{
    .entity_size  = sizeof(struct en_camera),
    .tag          = TAG_CAMERA,
    .layers       = ~0,
    .handle_event = handle_event,
/*
    .component_count = lengthof(component_array),
    .components = component_array,
*/
};
