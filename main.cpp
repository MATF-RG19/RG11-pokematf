#include <GL/glut.h>
#include <iostream>

//MACROS
#define WINDOW_FIELD 0
#define WINDOW_POKEBALL 1



//FUNCTION PROTOTYPES
void display();
void reshape(int, int);
void timer(int);
void keyboard(unsigned char key, int x, int y);
void display1();
void display2();

//GLOBAL VARIABLES
float x_position;
float y_position;
float player_size;
int state;
bool window_select;


void init()
{
    glClearColor(0.3, 0.7, 0.3, 1.0);
    glShadeModel (GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float light_position[] = {-1, 3, 2, 1};
    float light_ambient[] = {.3f, .3f, .3f, 1};
    float light_diffuse[] = {.7f, .7f, .7f, 1};
    float light_specular[] = {.7f, .7f, .7f, 1};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    glEnable(GL_COLOR_MATERIAL);

    x_position = 0;
    y_position = 0;
    player_size = 2;
    state = 1;
    window_select = WINDOW_FIELD;

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

void draw_axes(float len) {
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(len,0,0);

        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,len,0);

        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,len);
    glEnd();

    glEnable(GL_LIGHTING);
}

void display1()
{
    gluLookAt(0, 0, 1000, 0, 0, 0, 0, 1, 0);

    glDisable(GL_LIGHTING);

    glClear(GL_COLOR_BUFFER_BIT);  
    glLoadIdentity();              

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



    glutSwapBuffers();                  
}

void display2()
{
    gluLookAt(4, 4, 4,
              0, 0, 0,
              0, 1, 0);

    glEnable(GL_LIGHTING);
    glClear(GL_COLOR_BUFFER_BIT);  
    glLoadIdentity();

    draw_axes(5);

    glColor3f(1, 0, 0);
    glRotatef(60, 1, 1, 1);
    glutSolidSphere(2, 25, 25);

    glutSwapBuffers();                    
}


void display()
{
    if(window_select == WINDOW_FIELD)
    {
        display1();
    }
    else if(window_select == WINDOW_POKEBALL)
    {
        display2();
    }
}

void reshape(int w, int h)
{

    if(window_select == WINDOW_FIELD)
    {
        reshape1(w, h);
    }
    else if(window_select == WINDOW_POKEBALL)
    {
        reshape2(w, h);
    }

}

void reshape1(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-10, 10, -10, 10, -10, 10);

    glMatrixMode(GL_MODELVIEW);
}

void reshape2(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(30, (float) w/h, 1, 20);

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
    switch(key)
    {
    case 27:
    {
        /* Zavrsava se program. */
        exit(0);
    break;
    }
    case 'k':
    case 'K':
    {
        window_select = WINDOW_FIELD;
    break;
    }
    case 'l':
    case 'L':
    {
        window_select = WINDOW_POKEBALL;
    break;
    } 
    }

    //move player only if in field window
    if(window_select == WINDOW_FIELD)
    {
    switch (key) {
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
}



