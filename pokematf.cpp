#include <GL/glut.h>
#include <iostream>
#include "pokematf.h"
#include <vector>

float x_position = 0;
float y_position = 0;
float player_size = 2;
int state = 1;
bool window_select = WINDOW_FIELD;
int window_width = 700;
int window_height = 700;

int animation_parametar = 0;
int animation_ongoing = 0;

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 5,
             0, 0, 0,
              0, 1, 0);

    glDisable(GL_LIGHTING); 
    draw_axes(50);    

    if( CheckCollision( x_position, y_position, player_size, player_size,
                        POKECENTAR_POSITION_X, POKECENTAR_POSITION_Y, player_size, player_size))
        printf("touching!!!\n");

    draw_player();
    draw_pokecentar();


    glEnable(GL_LIGHTING);
    glutSwapBuffers();                  
}

void display2()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(10, 10, 10,
              0, 0, 0,
              0, 1, 0);

    glDisable(GL_LIGHTING); 
    draw_axes(50);
    glEnable(GL_LIGHTING);

    draw_pokeball();

    glutSwapBuffers();                    
}

void draw_pokeball()
{
    glPushMatrix();

    glColor3f(1, 0, 0);
    glTranslatef(animation_parametar/100.0, animation_parametar/100.0, animation_parametar/100.0);
    glRotatef(60, 1, 1, 1);
    glutSolidSphere(1, 25, 25);

    glPopMatrix();

}

void draw_player()
{
    glPushMatrix();

    glTranslatef(x_position,y_position,0);

    glDisable(GL_LIGHTING); 
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

    glPopMatrix();
}

void draw_pokecentar()
{
    glPushMatrix();

    glTranslatef( POKECENTAR_POSITION_X, POKECENTAR_POSITION_Y, 0 );

    glDisable(GL_LIGHTING); 

    glBegin(GL_POLYGON);

    glColor3f(1, 1, 1);

    glVertex2f(0, 0);
    glVertex2f(player_size, 0);
    glVertex2f(player_size, player_size);
    glVertex2f(0, player_size);

    glEnd();

    glPopMatrix();
}

GLboolean CheckCollision(int obj_1_x, int obj_1_y, int obj_1_w, int obj_1_h, 
                        int obj_2_x, int obj_2_y, int obj_2_w, int obj_2_h) // AABB - AABB collision
{
    // Collision x-axis?
    bool collisionX = obj_1_x + obj_1_w >= obj_2_x &&
        obj_2_x + obj_2_w >= obj_1_x;
    // Collision y-axis?
    bool collisionY = obj_1_y + obj_1_h >= obj_2_y &&
        obj_2_y + obj_2_h >= obj_1_y;
    // Collision only if on both axes
    return collisionX && collisionY;
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
    window_width = w;
    window_height = h;

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
}

void reshape2(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(30, (float) w/h, 1, 20);

    glMatrixMode(GL_MODELVIEW);
}


void timer(int timer_id)
{
    if (timer_id == TIMER_ID) 
    {
        if(animation_parametar >= 100)
            animation_parametar = 0;
        animation_parametar += 1;
    }

    glutPostRedisplay();

    if(animation_ongoing)
        glutTimerFunc(TIMER_INTERVAL, timer, TIMER_ID);
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
        animation_ongoing = 0;
        window_select = WINDOW_FIELD;
        reshape1(window_width, window_height);
        glutPostRedisplay();
    break;
    }
    case 'l':
    case 'L':
    {
        if(!animation_ongoing)
        {
            animation_ongoing = 1;
            window_select = WINDOW_POKEBALL;
            reshape2(window_width, window_height); 
            glutTimerFunc(TIMER_INTERVAL, timer, TIMER_ID);
        }
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
