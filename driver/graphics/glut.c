#include "GL/freeglut.h"
#include "GL/gl.h"

#include "../../cgame.h"

extern void (*glSwapBuffers)(void);

void initialize_opengl(void);
void idle_proc(void);

static void display_graphics(void);
static void keyboard_input(unsigned char key, int x, int y);

void *
graphics_thread_proc(void *arg)
{
    int argc = 0;
    char *argv[] = { NULL };

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("CGame");
    glutIdleFunc(idle_proc);
    glutDisplayFunc(display_graphics);
    glutKeyboardFunc(keyboard_input);

    glSwapBuffers = glutSwapBuffers;
    initialize_opengl();

    glutMainLoop();

    exit_game();

    return NULL;
}

void
display_graphics(void)
{
    render_scene();
    glutPostRedisplay();
}

void
keyboard_input(unsigned char key, int x, int y)
{
    glutPostRedisplay();
}
