#include <GL/glut.h>
#include <iostream>
#include "pokematf.h"
#include <vector>

//STRUCTURES

typedef struct Position_info{
    float x, y, width, height;
} Position_info;

//GLOBAL VARIABLES

float matrix[16];
int window_width = 700;   
int window_height = 700;
GLuint names[4];

//PRIVATE VARIABLES

static int state = 1;
static bool window_select = WINDOW_FIELD;
static int animation_parametar = 0;
static bool move_pokemon = true;
static int mouse_x;
static int mouse_y;
static int choose_pokemon = 1;
static Position_info pokemon_position_info = { 0, 0, 0.3, 0.3};
static Position_info player_info = { 0, 0, 1, 2};
static Position_info pokecenter_info = { -7, 8, 3.5, 3.5};
static bool write_message = false;
static const char* message;
static int message_time = 100;

//PRIVATE FUNCTION DECLARATION

static void draw_axes(float len);

static void display1();

static void display2();

static void reshape1(int w, int h);

static void reshape2(int w, int h);

static void draw_player();

static void draw_pikachu();

static void draw_bulbasaur();

static void draw_squirtle();

static void draw_charmander();

static void draw_wild_pokemon();

static bool check_collision(float x1, float y1, float w1, float h1, 
                        float x2, float y2, float w2, float h2);

static void draw_grass();

static void draw_floor();

static void pick_pokemon( int id );

static void text_log( float x, float y, const char *s);

static void draw_pokecenter();

static bool check_proximity(float x1, float y1, float w1, float h1, 
                        float x2, float y2, float w2, float h2);

//PRIVATE FUNCTION DEFINITION


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

    glPointSize(10);
    glBegin(GL_POINTS);
        glColor3f(0,0,0);
        for(int i = -10; i <=10; i++)
            glVertex3i(i, 0, 10);
        for(int i = -10; i <=10; i++)
            glVertex3i(0, i, 10);
    glEnd();

    glEnable(GL_LIGHTING);
}

static void display1()
{
    glEnable(GL_TEXTURE_2D);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 5,
              0, 0, 0,
              0, 1, 0);

    
    glPushMatrix();

    glMultMatrixf(matrix);

    glDisable(GL_LIGHTING);

    glDisable(GL_TEXTURE_2D);
    draw_axes(10);  
    glEnable(GL_TEXTURE_2D);

    if(write_message)
        text_log(-8, 8.3, message);

    
    draw_floor();

    draw_grass();

    draw_player();

    draw_pokecenter();

    

    
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glutSwapBuffers();                  
}

static void draw_pokecenter()
{
    glPushMatrix();

    glTranslatef( pokecenter_info.x, pokecenter_info.y, 0 );
    glScalef( pokecenter_info.width, pokecenter_info.height, 1); 

    glBindTexture(GL_TEXTURE_2D, names[2]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 1);
        glVertex3f(0, 0, 4);

        glTexCoord2f(0  , 0);
        glVertex3f(0, -1, 4);

        glTexCoord2f(1, 0);
        glVertex3f(1, -1, 4);

        glTexCoord2f(1, 1);
        glVertex3f(1, 0, 4);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
}

static void text_log( float x, float y, const char *s)
{
    glDisable(GL_TEXTURE_2D);

    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    glRasterPos3f(x, y, 10);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, reinterpret_cast<const unsigned char *>( s ) ); 

    glEnable(GL_TEXTURE_2D);
}

static void display2()
{
    glDisable(GL_TEXTURE_2D);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(10, 10, 10,
              0, 0, 0,
              0, 1, 0);

    glPushMatrix();

    glMultMatrixf(matrix);

    glDisable(GL_LIGHTING); 
    draw_axes(50);
    glEnable(GL_LIGHTING);

    switch( choose_pokemon )
    {
        case 1:
            draw_pikachu();
            break;
        case 2:
            draw_squirtle();
            break;
        case 3:
            draw_bulbasaur();
            break;
        case 4:
            draw_charmander();
            break;
        default:
            break;
    }

    glPopMatrix();

    glutSwapBuffers();                    
}

static void draw_pikachu()
{
    glPushMatrix();

    glColor3f(1, 1, 0);
    glTranslatef(animation_parametar/100.0, animation_parametar/100.0, animation_parametar/100.0);
    glRotatef(60, 1, 1, 1);
    glutSolidSphere(1, 25, 25);

    glPopMatrix();

}

static void draw_bulbasaur()
{
    glPushMatrix();

    glColor3f(0, 1, 0);
    glTranslatef(animation_parametar/100.0, animation_parametar/100.0, animation_parametar/100.0);
    glRotatef(60, 1, 1, 1);
    glutSolidSphere(1, 25, 25);

    glPopMatrix();

}

static void draw_squirtle()
{
    glPushMatrix();

    glColor3f(0, 0, 1);
    glTranslatef(animation_parametar/100.0, animation_parametar/100.0, animation_parametar/100.0);
    glRotatef(60, 1, 1, 1);
    glutSolidSphere(1, 25, 25);

    glPopMatrix();

}

static void draw_charmander()
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

    glTranslatef(player_info.x,player_info.y,0);
    glScalef( player_info.width, player_info.height, 1);

    glDisable(GL_LIGHTING); 

    glBindTexture(GL_TEXTURE_2D, names[3]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 1);
        glVertex3f(0, 0, 4);

        glTexCoord2f(0 , 0);
        glVertex3f(0, -1, 4);

        glTexCoord2f(1, 0);
        glVertex3f(1, -1, 4);

        glTexCoord2f(1, 1);
        glVertex3f(1, 0, 4);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
}

static void draw_wild_pokemon()
{
    glPushMatrix();
    
    if ( move_pokemon )
    {
        srand((unsigned)time(0));
        pokemon_position_info.x = rand()%9;
        pokemon_position_info.y = rand()%9;
        move_pokemon = false;
    }

    // glTranslatef( pokemon_position_info.x, pokemon_position_info.y, 0 );
    // glScalef( pokemon_position_info.width, pokemon_position_info.width, 0);
    glScalef( 5, 5, 1); 

    glBindTexture(GL_TEXTURE_2D, names[2]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 1);
        glVertex3f(0, 0, 4);

        glTexCoord2f(0  , 0);
        glVertex3f(0, -1, 4);

        glTexCoord2f(1, 0);
        glVertex3f(1, -1, 4);

        glTexCoord2f(1, 1);
        glVertex3f(1, 0, 4);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
}

static bool check_collision(float x1, float y1, float w1, float h1, 
                        float x2, float y2, float w2, float h2)
{
    if (x1 + w1 >= x2 &&
       x1 <= x2 + w2 &&
       y1 - h1 <= y2 &&
       y1 >= y2 - h2 )
    {
    return true;
    }
    else
    {
        return false;
    }

}

static bool check_proximity(float x1, float y1, float w1, float h1, 
                        float x2, float y2, float w2, float h2)
{
    if ( check_collision( x1 + .1, y1, w1, h1, 
                        x2, y2, w2, h2 ) )
        return true;
    
    if ( check_collision( x1 - .1, y1, w1, h1, 
                        x2, y2, w2, h2 ) )
        return true;

    if ( check_collision( x1 , y1 + .1, w1, h1, 
                        x2, y2, w2, h2 ) )
        return true;

    if ( check_collision( x1 , y1 - .1, w1, h1, 
                        x2, y2, w2, h2 ) )
        return true;

    
    return false;
}


static void reshape1(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 10, -10, 10, -10, 10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}

static void reshape2(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, (float) w/h, 1, 20);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}

static void draw_grass()
{
    /* Crtaju se trava */
    glBindTexture(GL_TEXTURE_2D, names[0]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(0, 0, 3);

        glTexCoord2f(10, 0);
        glVertex3f(10, 0, 3);

        glTexCoord2f(10, 10);
        glVertex3f(10, 10, 3);

        glTexCoord2f(0, 10);
        glVertex3f(0, 10, 3);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}
static void draw_floor()
{
    /* Crta se pod*/
    glBindTexture(GL_TEXTURE_2D, names[1]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(-10, -10, 0);

        glTexCoord2f(6, 0);
        glVertex3f(10, -10, 0);

        glTexCoord2f(6, 6);
        glVertex3f(10, 10, 0);

        glTexCoord2f(0, 6);
        glVertex3f(-10, 10, 0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}  

static void pick_pokemon( int id )
{
    choose_pokemon = id;
    window_select = WINDOW_POKEBALL;
    reshape2(window_width, window_height);
    glutPostRedisplay();
}

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

        if(write_message)
        {
            message_time -= 1;
            if( message_time <=0 )
            {
                write_message = false;
                message_time = 100;
            }
        }
    }

    glutPostRedisplay();

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
        window_select = WINDOW_FIELD;
        reshape1(window_width, window_height);
        glutPostRedisplay();
    break;
    }
    case 'r':
    case 'R':
    {
        glLoadIdentity();
        glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
        glutPostRedisplay();
    break;
    }
    case '1':
    {
        pick_pokemon( 1 );
    break;
    }
    case '2':
    {
        pick_pokemon( 2 );
    break;
    }
    case '3':
    {
        pick_pokemon( 3 );
    break;
    }
    case '4':
    {
        pick_pokemon( 4 );
    break;
    }
    }

    if(window_select == WINDOW_FIELD)
    {
    switch (key) {
    case 'w':
    case 'W':
        if(player_info.y <= 10 )
        {
            if(!check_collision( player_info.x, player_info.y + .1, player_info.width, player_info.height,
                         pokecenter_info.x, pokecenter_info.y, pokecenter_info.width, pokecenter_info.height))
            {             
                player_info.y+=.1;
                glutPostRedisplay();
            }
        }
        break;
    case 's':
    case 'S':
        if(player_info.y - 0.1 >= -10 + player_info.height)
        {
            if(!check_collision( player_info.x, player_info.y - .1, player_info.width, player_info.height,
                         pokecenter_info.x, pokecenter_info.y, pokecenter_info.width, pokecenter_info.height))
            {
                player_info.y-=.1;
                glutPostRedisplay();
            }
        }
        break;
    case 'a':
    case 'A':
        if(player_info.x >= -10)
        {
            if(!check_collision( player_info.x - .1, player_info.y, player_info.width, player_info.height,
                         pokecenter_info.x, pokecenter_info.y, pokecenter_info.width, pokecenter_info.height))
            {
                player_info.x-=.1;
                glutPostRedisplay();
            }
        }
        break;
    case 'd':
    case 'D':
        if(player_info.x+.1 <= 10 - player_info.width)
        {
            if(!check_collision( player_info.x + .1, player_info.y, player_info.width, player_info.height,
                         pokecenter_info.x, pokecenter_info.y, pokecenter_info.width, pokecenter_info.height))
            {
                player_info.x+=.1;
                glutPostRedisplay();
            }
        }
        break;
    case 'h':
    case 'H':
    {
        if( check_proximity( player_info.x, player_info.y, player_info.width, player_info.height,
                         pokecenter_info.x, pokecenter_info.y, pokecenter_info.width, pokecenter_info.height))
            {
                write_message = true;
                message = "All pokemons healed";
                glutPostRedisplay();
            }
        break;
    }
       
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



