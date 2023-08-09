#ifndef CGAME_H
#define CGAME_H

#include <stdbool.h>

#define CENTIMETERS_TO_METERS  (1.0/100)
#define MILLIMETERS_TO_METERS  (1.0/1000)

#define RGBA32(r, g, b, a) RGBA8888(r, g, b, a)
#define unpack_RGBA32(c, r, g, b, a) unpack_RGBA8888(c, r, g, b, a)

#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)

#define RGBA8888(r, g, b, a) \
    (((unsigned long)((r) * 255) << 24) | \
     ((unsigned long)((g) * 255) << 16) | \
     ((unsigned long)((b) * 255) <<  8) | \
     ((unsigned long)((a) * 255)      ))

#define unpack_RGBA8888(c, r, g, b, a) do \
{ \
    *r = (c) >> 24; \
    *g = (c) >> 16 & 0xFF; \
    *b = (c) >>  8 & 0xFF; \
    *a = (c)       & 0xFF; \
} while (0)

#else /* __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ */

#define RGBA8888(r, g, b, a) \
    (((unsigned long)((r) * 255)      ) | \
     ((unsigned long)((g) * 255) <<  8) | \
     ((unsigned long)((b) * 255) << 16) | \
     ((unsigned long)((a) * 255) << 24))

#define unpack_RGBA8888(c, r, g, b, a) do \
{ \
    *r = (c)       & 0xFF; \
    *g = (c) >>  8 & 0xFF; \
    *b = (c) >> 16 & 0xFF; \
    *a = (c) >> 24; \
} while (0)

#endif /* __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ */

#define ENTITY_HEADER \
    unsigned short entity_type; \
    unsigned char tag; \
    unsigned char layers; \
    struct entity *previous, *next; \
    void (*handle_event)(struct entity *entity, int event, void *message);

struct entity_type
{
    unsigned short entity_size;
    unsigned char tag;
    unsigned char layers;
    void (*handle_event)(/* struct entity *entity, */
                         /* int event, void *message */);
};

struct entity
{
    ENTITY_HEADER
    // ...
};

struct scene
{
    void (*on_load_scene)(void);
};

struct vector3
{
    float x, y, z;
};

struct camera
{
    unsigned short layer_mask;
    unsigned long clear_color;
    float fov;
    struct
    {
        float left,
              right,
              top,
              bottom,
              near,
              far;
    } ortho;
};

struct transform
{
    struct vector3 position,
                   rotation;
    float scale;
};

struct texture
{
    int id;
    unsigned short width,
                   height;
};

struct material
{
    unsigned long color;
    struct texture *main_texture;
};

// Submeshes can each use a different material.
struct submesh
{
    unsigned short triangle_start,
                   triangle_count;
};

struct mesh
{
    unsigned short submesh_count,
                   triangle_count,
                   vertex_count;
    struct submesh *mesh;
    struct triangle *triangle;
    struct vertex *vertex;
};

struct triangle
{
    unsigned short v1, v2, v3;
};

struct vertex
{
    short x, y, z, w,
          s, t;
    unsigned char r, g, b, a;
};

enum
{
    EVENT_SPAWN,
    EVENT_DESPAWN,
    EVENT_UPDATE,
    EVENT_RENDER,
};

void debug_log(const char *log, ...);
void warning(const char *warn, ...);
void error(const char *err, ...);
void fatal_error(const char *err, ...);

void exit_game(void);

float getdt(void);

bool spawn_entity(int entity_type,
                  float position_x, float position_y, float position_z,
                  float rotation_x, float rotation_y, float rotation_z,
                  float scale);
void despawn_entity(struct entity *entity);

bool load_scene(int scene);
void unload_scene(void);
void update_scene(void);
void render_scene(void);

void register_camera(struct camera *camera);
void unregister_camera(struct camera *camera);
void setup_camera(struct camera *camera);

void render_mesh(struct mesh *mesh, struct transform *transform,
                 int material_count, struct material *materials);

void swap_buffers(void);

#endif // ndef CGAME_H
