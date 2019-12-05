#include <GL/glut.h>
#include <iostream>

void display();
void reshape(int, int);
void timer(int);
void keyboard(unsigned char key, int x, int y);

void init()
{
    glClearColor(0.3, 0.7, 0.3, 1.0);
    glShadeModel (GL_SMOOTH);
}

int main(int argc, char **argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowPosition(200, 100);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Window 1");

    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    init();


    glutMainLoop();
}

float x_position = 0, y_position = 0;
float player_size = 2;
int state = 1;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);  //cistim framebuffer
    glLoadIdentity();              //resetujem matrice transformacija

    glTranslatef(x_position,y_position,0);

    glBegin(GL_POLYGON);

    glColor3f(1, 0, 0);
    glVertex2f(0, 0);

    glColor3f(1, 1, 0);
    glVertex2f(player_size, 0);

    glColor3f(0, 1, 0);
    glVertex2f(player_size, player_size);

    glColor3f(0, 0, 1);
    glVertex2f(0, player_size);

    glEnd();



    glutSwapBuffers();                    //crtam framebuffer
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-10,10,-10,10, -1, 1);

    glMatrixMode(GL_MODELVIEW);

}

void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000/60, timer, 0);

    /*switch(state)
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
        
    }*/
}


void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        /* Zavrsava se program. */
        exit(0);
        break;
    case 'w':
    case 'W':
        if(y_position+.1 <= 10 - player_size)
        {
        y_position+=.1;
        glutPostRedisplay();
        }
        break;
    case 's':
    case 'S':
        if(y_position >= -10)
        {
        y_position-=.1;
        glutPostRedisplay();
        }
        break;
    case 'a':
    case 'A':
        if(x_position >= -10)
        {
        x_position-=.1;
        glutPostRedisplay();
        }
        break;
    case 'd':
    case 'D':
        if(x_position+.1 <= 10 - player_size)
        {
        x_position+=.1;
        glutPostRedisplay();
        }
        break;
    }
}



