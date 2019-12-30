#include <GL/glut.h>
#include <iostream>
#include "pokematf.h"
#include <vector>

static float player_x = 0;
static float player_y = 0;
static float pokemon_x = 0;
static float pokemon_y = 0;
static float player_size = 1;
static float pokemon_size = 0.3;
static int state = 1;
static bool window_select = WINDOW_FIELD;
static int animation_parametar = 0;
static int animation_ongoing = 0;
static bool move_pokemon = true;
static int mouse_x;
static int mouse_y;

float matrix[16];
int window_width = 700;
int window_height = 700;
GLuint names[2];

//PUBLIC

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
        glDeleteTextures(2, names);
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
            /* Inicijalizujemo matricu rotacije. */
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
            reshape2(window_width, window_height); 
            glutTimerFunc(TIMER_INTERVAL, timer, TIMER_ID);
        }
    break;
    } 
    }

    if(window_select == WINDOW_FIELD)
    {
    switch (key) {
    case 'w':
    case 'W':
        if(player_y <= 10 )
        {
        player_y+=.1;
        glutPostRedisplay();
        }
        break;
    case 's':
    case 'S':
        if(player_y - 0.1 >= -10 + player_size)
        {
        player_y-=.1;
        glutPostRedisplay();
        }
        break;
    case 'a':
    case 'A':
        if(player_x >= -10)
        {
        player_x-=.1;
        glutPostRedisplay();
        }
        break;
    case 'd':
    case 'D':
        if(player_x+.1 <= 10 - player_size)
        {
        player_x+=.1;
        glutPostRedisplay();
        }
        break;
       
    }
    }
}

void mouse(int button, int state, int x, int y)
{
    /* Pamti se pozicija pokazivaca misa. */
    mouse_x = x;
    mouse_y = y;
}

void motion(int x, int y)
{
    /* Promene pozicije pokazivaca misa. */
    int delta_x, delta_y;

    /* Izracunavaju se promene pozicije pokazivaca misa. */
    delta_x = x - mouse_x;
    delta_y = y - mouse_y;

    /* Pamti se nova pozicija pokazivaca misa. */
    mouse_x = x;
    mouse_y = y;

    /* Izracunava se nova matrica rotacije. */
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glLoadIdentity();
        glRotatef(180 * (float) delta_x / window_width, 0, 1, 0);
        glRotatef(180 * (float) delta_y / window_height, 1, 0, 0);
        glMultMatrixf(matrix);

        glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();

    /* Forsira se ponovno iscrtavanje prozora. */
    glutPostRedisplay();
}

//PRIVATE


static void draw_axes(float len) {
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

static void display1()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 5,
              0, 0, 0,
              0, 1, 0);


    glPushMatrix();

    glDisable(GL_LIGHTING); 
    //draw_axes(50);    

    if( check_collision( player_x, player_y, player_size, player_size,
                        pokemon_x, pokemon_y, pokemon_size, pokemon_size))
        printf("touching!!!\n");

    draw_player();
    draw_wild_pokemon();

    draw_textures();

    glPopMatrix();


    glEnable(GL_LIGHTING);
    glutSwapBuffers();                  
}

static void display2()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(10, 10, 10,
              0, 0, 0,
              0, 1, 0);

    glMultMatrixf(matrix);

    glDisable(GL_LIGHTING); 
    draw_axes(50);
    glEnable(GL_LIGHTING);

    draw_pokeball();

    glutSwapBuffers();                    
}

static void draw_pokeball()
{
    glPushMatrix();

    glColor3f(1, 0, 0);
    glTranslatef(animation_parametar/100.0, animation_parametar/100.0, animation_parametar/100.0);
    glRotatef(60, 1, 1, 1);
    glutSolidSphere(1, 25, 25);

    glPopMatrix();

}

static void draw_player()
{
    glPushMatrix();

    glTranslatef(player_x,player_y,0);
    glScalef( player_size, player_size, 0);

    glDisable(GL_LIGHTING); 
    glBegin(GL_POLYGON);

    glColor3f(1, 0, 0);

    glVertex3f(0, 0, 10);
    glVertex3f(0, -1, 10);
    glVertex3f(1, -1, 10);
    glVertex3f(1, 0, 10);

    glEnd();

    glPopMatrix();
}

static void draw_wild_pokemon()
{
    glPushMatrix();
    
    if ( move_pokemon )
    {
        srand((unsigned)time(0));
        pokemon_x = rand()%9;
        pokemon_y = rand()%9;
        move_pokemon = false;
    }

    glTranslatef( pokemon_x, pokemon_y, 0 );
    glScalef( pokemon_size, pokemon_size, 0);

    glDisable(GL_LIGHTING); 

    glBegin(GL_POLYGON);

    glColor3f(0, 1, 0);

    glVertex3f(0, 0, 0.02);
    glVertex3f(0, -1, 0.02);
    glVertex3f(1, -1, 0.02);
    glVertex3f(1, 0, 0.02);

    glEnd();

    glPopMatrix();
}

static GLboolean check_collision(float obj_1_x, float obj_1_y, float obj_1_w, float obj_1_h, 
                        float obj_2_x, float obj_2_y, float obj_2_w, float obj_2_h)
{
    return obj_1_x < obj_2_x + obj_2_w &&
           obj_1_x + obj_1_w > obj_2_x &&
           obj_1_y < obj_2_y + obj_2_h &&
           obj_1_y + obj_1_h > obj_2_y;
}


static void reshape1(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-10, 10, -10, 10, -10, 10);
}

static void reshape2(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(30, (float) w/h, 1, 20);

    glMatrixMode(GL_MODELVIEW);
}

static void draw_textures()
{
    /* Crtaju se vrata kuce. */
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(0, 0, 0);

        glTexCoord2f(1, 0);
        glVertex3f(10, 0, 0);

        glTexCoord2f(1, 1);
        glVertex3f(10, 10, 0);

        glTexCoord2f(0, 1);
        glVertex3f(0, 10, 0);
    glEnd();
    
        /* Crta se zid kuce. */
    glBindTexture(GL_TEXTURE_2D, names[1]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(-10, -10, 0);

        glTexCoord2f(12, 0);
        glVertex3f(10, -10, 0);

        glTexCoord2f(12, 6);
        glVertex3f(10, 10, 0);

        glTexCoord2f(0, 6);
        glVertex3f(-10, 10, 0);
    glEnd();
    




    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);
}

