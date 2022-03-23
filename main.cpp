#include <GL/glut.h>
#include <iostream>
#include "pokematf.h"

//! Initializes necessary OpenGL and glut settings for project.
void init()
{
    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(1, 1, 1, 0);
    glShadeModel (GL_SMOOTH);

    glEnable(GL_COLOR_MATERIAL);


    /* Ukljucuje se testiranje z-koordinate piksela. */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 

    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendEquation(GL_FUNC_ADD);

    /* Ukljucuju se teksture. */
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);


    texture_init();
}

//! Main function.
auto main(int argc, char **argv) -> int 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowPosition(200, 100);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("Pokematf");

    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(TIMER_INTERVAL, timer, TIMER_ID);
    init();

    glutMainLoop();
}
