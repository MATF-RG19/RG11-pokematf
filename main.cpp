#include <GL/glut.h>
#include <iostream>

void display();
void reshape(int, int);
void timer(int);

void init()
{
    glClearColor(0.0, 0.0, 0, 1.0);
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
    glutTimerFunc(0, timer, 0);
    init();


    glutMainLoop();
}

float x_position = 0;
int state = 1;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);  //cistim framebuffer
    glLoadIdentity();              //resetujem matrice transformacija

    glTranslatef(x_position,0,0);

    glBegin(GL_POLYGON);

    glColor3f(1, 0, 0);
    glVertex2f(-1, 1);

    glColor3f(1, 1, 0);
    glVertex2f(-1, -1);

    glColor3f(0, 1, 0);
    glVertex2f(1, -1);

    glColor3f(0, 0, 1);
    glVertex2f(1, 1);

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

void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000/60, timer, 0);

    switch(state)
    {
        case(1):
            if(x_position<9)
                x_position+=0.15;
            else
                state = -1;
            break;
        case(-1):
            if(x_position>-9)
                x_position-=0.15;
            else
                state = 1;
            break;
        
    }
}



