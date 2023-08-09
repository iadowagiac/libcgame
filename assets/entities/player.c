#include <stddef.h>
#include <math.h>
#include <cgame.h>

#include "../assets.h"

struct en_player
{
    ENTITY_HEADER
    float angle;
    struct transform transform;
    struct mesh *mesh;
};

/*
static const struct component_array_element component_array[] =
{
    { COMPONENT_TRANSFORM, offsetof(struct en_player, transform) },
}
*/

extern struct material material;

struct mesh *get_cube(int x, int y, int z,
                      int w, int h, int d);

static void
handle_event(struct en_player *player, int event, void *message)
{
    float dt;
    switch (event)
    {
        case EVENT_SPAWN:
            player->angle     = 0.0;
            player->transform = *(struct transform *)message;
            player->mesh      = get_cube(0, 0, 0, 2, 2, 2);
            break;
        case EVENT_UPDATE:
            dt = getdt();
            player->angle += 10.0 * dt;
            player->angle = fmod(player->angle, 360.0);
            player->transform.rotation = (struct vector3)
            {
                player->angle, player->angle, player->angle
            };
            break;
        case EVENT_RENDER:
            render_mesh(player->mesh, &player->transform, 1, &material);
            break;
    }
}

const struct entity_type en_player =
{
    .entity_size  = sizeof(struct en_player),
    .tag          = TAG_PLAYER,
    .layers       = ~0,
    .handle_event = handle_event,
/*
    .component_count = lengthof(component_array),
    .components = component_array,
*/
};
