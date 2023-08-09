#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "../../cgame.h"

struct object
{
    int mesh_count;
    struct mesh *mesh;
};

bool graphics_initialized = false;

void (*glSwapBuffers)(void);

static struct
{
    bool use_vertex_array_pointers   : 1,
         use_texture_objects         : 1,
         use_polygon_offsets         : 1,
         use_texcoord_edge_clamping  : 1,
         use_multitexturing          : 1,
         use_mirrored_texture_repeat : 1,
         use_depth_texture_shadows   : 1,
         use_buffer_objects          : 1,
         use_shader_objects          : 1,
         use_framebuffer_objects     : 1,
         use_vertex_array_objects    : 1,
         use_transform_feedback      : 1,
         use_uniform_buffer_objects  : 1;
    short glsl_version_major,
          glsl_version_minor;
} features = {0};

struct texture texture =
{
    .width  = 256,
    .height = 256,
};

struct material material =
{
    .color = RGBA32(1.0, 1.0, 1.0, 1.0),
    .main_texture = &texture,
};

size_t image_size = 256 * 256 * 3;
void *image_data;

void
initialize_opengl(void)
{
    int gl_version_major = 1,
        gl_version_minor = 0;
    GLuint texture_id;

    if (gl_version_major > 1 || gl_version_minor > 0)
    {
        features.use_vertex_array_pointers   = true;
        features.use_texture_objects         = true;
        features.use_polygon_offsets         = true;
    }
    if (gl_version_major > 1 || gl_version_minor > 4)
    {
        features.use_texcoord_edge_clamping  = true;
        features.use_multitexturing          = true;
        features.use_mirrored_texture_repeat = true;
        features.use_depth_texture_shadows   = true;
        features.use_buffer_objects          = true;
    }
    if (gl_version_major > 1)
    {
        features.use_shader_objects          = true;
        features.glsl_version_major          = 1;
        features.glsl_version_minor          = 10;
    }
    if (gl_version_major == 2 && gl_version_minor > 0)
    {
        features.glsl_version_minor          = 20;
    }
    if (gl_version_major > 2)
    {
        features.use_framebuffer_objects     = true;
        features.use_vertex_array_objects    = true;
        features.use_transform_feedback      = true;
        features.glsl_version_minor          = 30;
    }
    if ((gl_version_major == 3 && gl_version_minor > 0) ||
        gl_version_major > 3)
    {
        features.use_uniform_buffer_objects  = true;
    }

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    image_data = malloc(image_size);
    if (image_data == NULL)
        error("failed to malloc memory for texture image data.\n");

    FILE *f = fopen("crate.rgb", "rb");
    if (f == NULL)
        error("failed to open 'crate.rgb'.\n");
    if (fread(image_data, 1, image_size, f) < image_size)
        error("texture image data is shorter than expected.");
    fclose(f);

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8,
                 texture.width, texture.height,
                 0, GL_RGB, GL_UNSIGNED_BYTE,
                 image_data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    texture.id = texture_id;
    free(image_data);

    graphics_initialized = true;
}

void
swap_buffers(void)
{
    /**
     * TODO:
     *     The glFlush should be done in a separate function that
     *     posts a message to a message queue so that the update
     *     thread can wait for rendering to finish before starting
     *     the next frame.
     **/
    glFlush();
    glSwapBuffers();
}

void
setup_camera(struct camera *camera)
{
    unsigned char r, g, b, a;
    unpack_RGBA32(camera->clear_color, &r, &g, &b, &a);
    glClearColor(1.0 / 255 * r,
                 1.0 / 255 * g,
                 1.0 / 255 * b,
                 1.0 / 255 * a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect_ratio = (camera->ortho.right - camera->ortho.left)
                       / (camera->ortho.bottom - camera->ortho.top);
    gluPerspective(camera->fov, aspect_ratio,
                   camera->ortho.near, camera->ortho.far);

/*
    glOrtho(camera->ortho.left,
              camera->ortho.right,
              camera->ortho.bottom,
              camera->ortho.top,
              camera->ortho.near,
              camera->ortho.far);
*/

//    glDepthRange(camera->ortho.near, camera->ortho.far);
}

void
render_mesh(struct mesh *mesh, struct transform *transform,
            int material_count, struct material *materials)
{
    if (features.use_vertex_array_pointers)
    {
    }
    else
    {
        struct triangle t;
        struct vertex v1, v2, v3;

        if (materials != NULL)
        {
            struct material *material = materials + 0;
            unsigned char r, g, b, a;

            if (material->main_texture != NULL)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, material->main_texture->id);
                glEnable(GL_TEXTURE_2D);

                glMatrixMode(GL_TEXTURE);
                glLoadIdentity();
                glScalef(1.0 / (material->main_texture->width - 1),
                         1.0 / (material->main_texture->height - 1), 1.0);
            }
            else
            {
                glDisable(GL_TEXTURE_2D);
            }

            unpack_RGBA32(material->color, &r, &g, &b, &a);
            glColor4ub(r, g, b, a);

            if (a != 255)
            {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glEnable(GL_BLEND);
            }
        }
        else
        {
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
            glColor4ub(255, 255, 255, 255);
        }

        //glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glShadeModel(GL_SMOOTH);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslatef(transform->position.x,
                     transform->position.y,
                     -transform->position.z);

        glScalef(transform->scale,
                 transform->scale,
                 transform->scale);

        glRotatef(transform->rotation.z, 0.0, 0.0, 1.0);
        glRotatef(transform->rotation.y, 0.0, 1.0, 0.0);
        glRotatef(transform->rotation.x, 1.0, 0.0, 0.0);

        // TODO: build and use display lists.
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < mesh->triangle_count; i++)
        {
            t = mesh->triangle[i];
            v1 = mesh->vertex[t.v1],
            v2 = mesh->vertex[t.v2],
            v3 = mesh->vertex[t.v3];
            glTexCoord2s(v1.s, v1.t);
            glVertex3s(v1.x, v1.y, v1.z);
            glTexCoord2s(v2.s, v2.t);
            glVertex3s(v2.x, v2.y, v2.z);
            glTexCoord2s(v3.s, v3.t);
            glVertex3s(v3.x, v3.y, v3.z);
        }
        glEnd();
    }
}

struct mesh *
get_cube(int x, int y, int z, int w, int h, int d)
{
    static struct triangle triangle[12];
    static struct vertex vertex[4 * 6];
    static struct mesh mesh;

    w /= 2;
    h /= 2;
    d /= 2;

    // front face
    {
        // bottom left
        vertex[0].x = x - w;
        vertex[0].y = y - h;
        vertex[0].z = z - d;
        vertex[0].s = 0;
        vertex[0].t = 0;
        // top left
        vertex[1].x = x - w;
        vertex[1].y = y + h;
        vertex[1].z = z - d;
        vertex[1].s = 0;
        vertex[1].t = texture.height - 1;
        // top right
        vertex[2].x = x + w;
        vertex[2].y = y + h;
        vertex[2].z = z - d;
        vertex[2].s = texture.width - 1;
        vertex[2].t = texture.height - 1;
        // bottom right
        vertex[3].x = x + w;
        vertex[3].y = y - h;
        vertex[3].z = z - d;
        vertex[3].s = texture.width - 1;
        vertex[3].t = 0;
    }
    // back face
    {
        // bottom left
        vertex[4].x = x + w;
        vertex[4].y = y - h;
        vertex[4].z = z + d;
        vertex[4].s = 0;
        vertex[4].t = 0;
        // top left
        vertex[5].x = x + w;
        vertex[5].y = y + h;
        vertex[5].z = z + d;
        vertex[5].s = 0;
        vertex[5].t = texture.height - 1;
        // top right
        vertex[6].x = x - w;
        vertex[6].y = y + h;
        vertex[6].z = z + d;
        vertex[6].s = texture.width - 1;
        vertex[6].t = texture.height - 1;
        // bottom right
        vertex[7].x = x - w;
        vertex[7].y = y - h;
        vertex[7].z = z + d;
        vertex[7].s = texture.width - 1;
        vertex[7].t = 0;
    }
    // left face
    {
        // bottom left
        vertex[8].x = x - w;
        vertex[8].y = y - h;
        vertex[8].z = z + d;
        vertex[8].s = 0;
        vertex[8].t = 0;
        // top left
        vertex[9].x = x - w;
        vertex[9].y = y + h;
        vertex[9].z = z + d;
        vertex[9].s = 0;
        vertex[9].t = texture.height - 1;
        // top right
        vertex[10].x = x - w;
        vertex[10].y = y + h;
        vertex[10].z = z - d;
        vertex[10].s = texture.width - 1;
        vertex[10].t = texture.height - 1;
        // bottom right
        vertex[11].x = x - w;
        vertex[11].y = y - h;
        vertex[11].z = z - d;
        vertex[11].s = texture.width - 1;
        vertex[11].t = 0;
    }
    // right face
    {
        // bottom left
        vertex[12].x = x + w;
        vertex[12].y = y - h;
        vertex[12].z = z - d;
        vertex[12].s = 0;
        vertex[12].t = 0;
        // top left
        vertex[13].x = x + w;
        vertex[13].y = y + h;
        vertex[13].z = z - d;
        vertex[13].s = 0;
        vertex[13].t = texture.height - 1;
        // top right
        vertex[14].x = x + w;
        vertex[14].y = y + h;
        vertex[14].z = z + d;
        vertex[14].s = texture.width - 1;
        vertex[14].t = texture.height - 1;
        // bottom right
        vertex[15].x = x + w;
        vertex[15].y = y - h;
        vertex[15].z = z + d;
        vertex[15].s = texture.width - 1;
        vertex[15].t = 0;
    }
    // top face
    {
        // bottom left
        vertex[16].x = x - w;
        vertex[16].y = y + h;
        vertex[16].z = z - d;
        vertex[16].s = 0;
        vertex[16].t = 0;
        // top left
        vertex[17].x = x - w;
        vertex[17].y = y + h;
        vertex[17].z = z + d;
        vertex[17].s = 0;
        vertex[17].t = texture.height - 1;
        // top right
        vertex[18].x = x + w;
        vertex[18].y = y + h;
        vertex[18].z = z + d;
        vertex[18].s = texture.width - 1;
        vertex[18].t = texture.height - 1;
        // bottom right
        vertex[19].x = x + w;
        vertex[19].y = y + h;
        vertex[19].z = z - d;
        vertex[19].s = texture.width - 1;
        vertex[19].t = 0;
    }
    // bottom face
    {
        // bottom left
        vertex[20].x = x - w;
        vertex[20].y = y - h;
        vertex[20].z = z + d;
        vertex[20].s = 0;
        vertex[20].t = 0;
        // top left
        vertex[21].x = x - w;
        vertex[21].y = y - h;
        vertex[21].z = z - d;
        vertex[21].s = 0;
        vertex[21].t = texture.height - 1;
        // top right
        vertex[22].x = x + w;
        vertex[22].y = y - h;
        vertex[22].z = z - d;
        vertex[22].s = texture.width - 1;
        vertex[22].t = texture.height - 1;
        // bottom right
        vertex[23].x = x + w;
        vertex[23].y = y - h;
        vertex[23].z = z + d;
        vertex[23].s = texture.width - 1;
        vertex[23].t = 0;
    }

    // front face
    triangle[0].v1 = 0;
    triangle[0].v2 = 1;
    triangle[0].v3 = 2;
    triangle[1].v1 = 2;
    triangle[1].v2 = 3;
    triangle[1].v3 = 0;
    // back face
    triangle[2].v1 = 4;
    triangle[2].v2 = 5;
    triangle[2].v3 = 6;
    triangle[3].v1 = 6;
    triangle[3].v2 = 7;
    triangle[3].v3 = 4;
    // left face
    triangle[4].v1 = 8;
    triangle[4].v2 = 9;
    triangle[4].v3 = 10;
    triangle[5].v1 = 10;
    triangle[5].v2 = 11;
    triangle[5].v3 = 8;
    // right face
    triangle[6].v1 = 12;
    triangle[6].v2 = 13;
    triangle[6].v3 = 14;
    triangle[7].v1 = 14;
    triangle[7].v2 = 15;
    triangle[7].v3 = 12;
    // top face
    triangle[8].v1 = 16;
    triangle[8].v2 = 17;
    triangle[8].v3 = 18;
    triangle[9].v1 = 18;
    triangle[9].v2 = 19;
    triangle[9].v3 = 16;
    // bottom face
    triangle[10].v1 = 20;
    triangle[10].v2 = 21;
    triangle[10].v3 = 22;
    triangle[11].v1 = 22;
    triangle[11].v2 = 23;
    triangle[11].v3 = 20;

    mesh.triangle_count = 12;
    mesh.vertex_count = 8;
    mesh.triangle = triangle;
    mesh.vertex = vertex;

    return &mesh;
}
