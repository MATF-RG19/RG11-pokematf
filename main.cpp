#include <GL/glut.h>
#include <iostream>

void display();


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);

    glutInitWindowPosition(200, 100);
    glutInitWindowSize(300, 300);
    glutCreateWindow("Window 1");

    glutDisplayFunc(display);


    glutMainLoop();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);  //cistim framebuffer
    glLoadIdentity();              //resetujem matrice transformacija





    glFlush();                     //crtam framebuffer
}




