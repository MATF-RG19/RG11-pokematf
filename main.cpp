#include <GL/glut.h>
#include <iostream>

void display();

void init()
{
    glClearColor(0.0, 0.0, 1.0, 1.0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);

    glutInitWindowPosition(200, 100);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Window 1");

    glutDisplayFunc(display);
    init();


    glutMainLoop();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);  //cistim framebuffer
    glLoadIdentity();              //resetujem matrice transformacija





    glFlush();                     //crtam framebuffer
}




