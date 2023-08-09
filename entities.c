#include <stdlib.h>

#include "cgame.h"

extern int entity_type_count;
extern const struct entity_type *entity_types[];

struct entity *first_entity = NULL,
              *last_entity = NULL;

bool
spawn_entity(int entity_type,
             float position_x, float position_y, float position_z,
             float rotation_x, float rotation_y, float rotation_z,
             float scale)
{
    struct entity *entity;
    struct transform transform;
    entity = malloc(entity_types[entity_type]->entity_size);
    if (entity == NULL)
        return false;
    entity->entity_type  = entity_type;
    entity->tag          = entity_types[entity_type]->tag;
    entity->layers       = entity_types[entity_type]->layers;
    entity->handle_event = entity_types[entity_type]->handle_event;
    if (first_entity != NULL)
    {
        last_entity->next = entity;
        entity->previous = last_entity;
        last_entity = entity;
    }
    else first_entity = last_entity = entity;
    transform.position.x = position_x;
    transform.position.y = position_y;
    transform.position.z = position_z;
    transform.rotation.x = rotation_x;
    transform.rotation.y = rotation_y;
    transform.rotation.z = rotation_z;
    transform.scale = scale;
    entity->handle_event(entity, EVENT_SPAWN, &transform);
    return true;
}

void
despawn_entity(struct entity *entity)
{
    entity->handle_event(entity, EVENT_DESPAWN, NULL);
    if (entity == first_entity)
        first_entity = entity->next;
    else
        entity->previous->next = entity->next;
    if (entity == last_entity)
        last_entity = entity->previous;
    else
        entity->next->previous = entity->previous;
    free(entity);
}
