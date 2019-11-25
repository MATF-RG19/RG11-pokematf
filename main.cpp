#include <GL/glut.h>
#include <iostream>

void display();
void reshape(int, int);

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
    glutReshapeFunc(reshape);
    init();


    glutMainLoop();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);  //cistim framebuffer
    glLoadIdentity();              //resetujem matrice transformacija



    glPointSize(10.0);

    glBegin(GL_POLYGON);

    glVertex2f(0, 5);
    glVertex2f(-4, -3);
    glVertex2f(4, -3);
    glVertex2f(5, -2);

    glEnd();






    glFlush();                     //crtam framebuffer
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-10,10,-10,10);

    glMatrixMode(GL_MODELVIEW);

}



