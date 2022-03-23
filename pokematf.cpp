#include <GL/glut.h>
#include <iostream>
#include "pokematf.h"
#include <unordered_set>
#include <string>
#include <deque>

//STRUCTURES

//! Struct used for storing coordinates, width and height of objects in the project.
typedef struct Position_info{
    float x, y, width, height;
} Position_info;

//! Struct used for storing info about every Pokemon.
using Pokemon_info = struct Pokemon_info{
    int health, attack_min, attack_max;
};

//GLOBAL VARIABLES

//! Initial window width
int window_width = 700;
//! Initial window height   
int window_height = 700;

//! Determines which window should be drawn in window_select variable.
#define WINDOW_FIELD              0
//! Determines which window should be drawn in window_select variable. 
#define WINDOW_POKEDEX            1
//! Determines which window should be drawn in window_select variable.
#define WINDOW_BATTLE             2
//PRIVATE VARIABLES

//! State represents the phase of the battle.
static int state = 1;
//! Determines which window will be displayed in display func
static int window_select = WINDOW_FIELD;
//! Animation parametar that changes in timer func.
static int animation_parametar = 0;
//! Generate new position for the red dot( wild Pokemon ).
static bool move_pokemon = true;
//! Wild Pokemon Position_info
static Position_info wild_pokemon_info = { 0, 0, 0.3, 0.3};
//! Players Position_info
static Position_info player_info = { 0, 0, 1, 2};
//! Pokecenter Position_info
static Position_info pokecenter_info = { -7, 8, 3.5, 3.5};
//! Info about every loaded Pokemon
static Pokemon_info poke_info[50];
//! Random value between min and max attack of the currently selected Pokemon.
static int favorite_current_attack;
//! Random value between min and max attack of the wild Pokemon player encountered.
static int wild_current_attack;
//! Info about the wild Pokemon player encountered.
static Pokemon_info wild_pokemon_stats;
//! Used for displaying a messsage for a brief time.
static bool write_message = false;
//! Message to be sent to text_log and displayed.
static const char* message;
//! Timer for displaying a message for a brief time. 
static int message_time = 100;
//! Determines which Pokemon will be drawn.
static int show_pokemon = 0;
//! Set containing all owned Pokemon.
static std::unordered_set<int> owned_pokemons = {};
//! Buffer used for storing message to be sent to text_log for displaying.
static char buffer[50];
//! Textures for player and Professor Oak.
static GLuint player_sprites[2];
//! Pokecenter texture.
static GLuint pokecenter_sprite;
//! All loaded Pokemon textures. 
static GLuint pokemon_sprites[50];
//! All loaded Pokemon silhouettes.
static GLuint pokemon_sprites_black[50];
//! Backgrounds textures.
static GLuint background_textures[10];
//! Arrow textures
static GLuint arrow_sprite;
//! Used for displayinh battle.
static int battle_drawing = 0;
//! Determines which wild Pokemon the player is fighting.
static int show_wild_pokemon = 0;
//! Stores the value of player currently selected Pokemon.
static int favorite_pokemon = -1;
//! Determines which turn it is in battle.
static bool turn = true; 
//! Used for displaying attack animation.
static bool hit = false;
//! Timer for displaying attack animation.
static int hit_time = 50;
//! Used for displaying running animation.
static int running = 0;
//! Timer for displaying running animation.
static int running_time = 0;
//! Used for catching running animation.
static int catching = 0;
//! Timer for catching running animation.
static int catching_time = 0;
//! Number of potion charges left.
static int potion_charges = 3;
//! Determines phase of the battle.
static int battle_state = -1;
//! Log storing last 8 messages and events that happened in battle.
static std::deque<std::string> battle_log;
//! Tmp var.
static int tmp;
//! Determines if player chose a starter Pokemon.
static bool choose_starter = true;


//PRIVATE FUNCTION DECLARATION

//! Additional for drawing axes, not used in final project.
static void draw_axes(float len);

//! Function used to display text on the screen, x and y represent coordinates of the text and s is the string that will be displayed.
static void text_log( float x, float y, const char *s);

//! Additional reshape function called by reshape(...).
static void reshape1(int w, int h);

//! Function initializes random pokemon min and max attack in predetermined range.
static void init_pokemon_stats();

//! First display mode where player moves around looking for wild pokemons.
static void display_field();

//! Second display mode - Pokedex
static void display_pokemons();

//! Third display mode showing the battle between pokemons.
static void display_battle();

//! Draws background texture on the first display.
static void draw_floor();

//! Draws player on the first display using provided texture and position determined by player movement.
static void draw_player();

//! Draws Proffesor Oak on the first display using provided texture.
static void draw_oak();

//! Draws grass on the first display using provided texture.
static void draw_grass();

//! Draws Pokecenter on the first display using provided texture.
static void draw_pokecenter();

//! Draws the red dot represening wild pokemon in the grass. Random position after every battle.
static void draw_wild_pokemon();

//! Checks collision between two objects using their coordinates, width and height. Returns bool value that determines if collision occurred.
static auto check_collision(float x1, float y1, float w1, float h1, 
                        float x2, float y2, float w2, float h2) -> bool;

//! Checks if collision would occuring when player moved in any direction by calling check_collision func.
static auto check_proximity(float x1, float y1, float w1, float h1, 
                        float x2, float y2, float w2, float h2) -> bool;


//! Init function for displaying Pokedex. Triggered by a keyboard action.
static void open_pokedex();

//! Draws background texture on the second display.
static void draw_pokedex_background();

//! Draws Pokemon that was previously selected with show_wild_pokemon variable. Function has different ways of drawing a Pokemon depending on the display mode.
static void draw_pokemons();

//! Draws Pokemon stats. Function has different ways of drawing a Pokemon depending on the display mode.
static void draw_stats();

//! Draws arrows
static void draw_arrows();

//! Init function for displaying battle.
static void init_battle();

//! Draws background texture on the third display.
static void draw_forest_background();

//! Light attack function that starts an attack animation and reduces opponents health.
static void light_attack();

//! Try to catch Pokemon with predetermined probability of success.
static void catch_pokemon();

//! Adds event that occurred in battle to the battle log.
static void add_to_battle_log( std::string s );

//! Displays all the events in the battle log on the screen.
static void draw_battle_log();

//! Heal pokemon. Function has two ways of working depending from which display mode it was called.
static void heal_pokemon();

//PRIVATE FUNCTION DEFINITION

static void init_pokemon_stats()
{
    srand(time(nullptr));
    for(auto & i : poke_info)
    {
        i.health = 100;
        i.attack_min = rand()%10+1;
        i.attack_max = rand()%10+11;
    }

    poke_info[15].attack_min = rand()%10+20;
    poke_info[15].attack_max = rand()%10+30;
    poke_info[14].attack_min = rand()%10+20;
    poke_info[14].attack_max = rand()%10+30;
    poke_info[13].attack_min = rand()%10+20;
    poke_info[13].attack_max = rand()%10+30;
}

static void draw_battle_log()
{
    int i = 9;
    for( auto s : battle_log )
    {
        text_log(-9, i, s.c_str());
        i--;
    }
}

static void add_to_battle_log( std::string s )
{
    battle_log.push_front( s );

    if(battle_log.size() == 10)
    {
        battle_log.pop_back();
    }
}

static void heal_pokemon()
{
    if ( potion_charges <= 0 )
    {
        add_to_battle_log("You are out of potions" );
    }
    else
    {
        if ( poke_info[ show_pokemon ].health <=0 )
        {
            add_to_battle_log("Pokemon is too tired to get up, heal him at Pokecenter" );
            return;
        }
        potion_charges--;
        poke_info[ show_pokemon ].health += 20;
        if ( poke_info[ show_pokemon ].health > 100 )
            poke_info[ show_pokemon ].health = 100;
        add_to_battle_log("+20hp ~ effect of healing potion" );
        turn = false;
        if ( battle_state == 0 )
        {
            catching = 1;
            catching_time = 0;
        }
        glutPostRedisplay();
    }
    
}

static void light_attack()
{
    if(turn)
    {
        srand(time(nullptr));
        favorite_current_attack = rand() % (poke_info[ favorite_pokemon ].attack_max + 1 - poke_info[ favorite_pokemon ].attack_min) + poke_info[ favorite_pokemon ].attack_min;
        add_to_battle_log("Light attack " + std::to_string( favorite_current_attack ) );
        turn = false;                 
        wild_pokemon_stats.health -= favorite_current_attack;
        hit = true;
        hit_time = 50;
        if(wild_pokemon_stats.health <= 0 )
        {
            battle_state = 1;
            running = 2;
            running_time = 0;
            add_to_battle_log("You won the battle, wild Pokemon is fleeing");
        }
        glutPostRedisplay();
    }
    else
    {
        srand(time(nullptr));
        wild_current_attack = rand() % (wild_pokemon_stats.attack_max + 1 - wild_pokemon_stats.attack_min) + wild_pokemon_stats.attack_min;
        add_to_battle_log("- " + std::to_string( wild_current_attack ) + "hp ~ wild pokemon light attack" );
        turn = true;                
        poke_info[ favorite_pokemon ].health -= wild_current_attack;
        hit = true;
        hit_time = 50;
        if ( poke_info[ favorite_pokemon ].health <=0 )
        {
            poke_info[ favorite_pokemon ].health = 0;
            battle_state = 2;
            running = 1;
            running_time = 0;            
            add_to_battle_log("Your Pokemon is too tired to continue");
        }
        glutPostRedisplay();
    }

}

static void init_battle()
{
    if ( !choose_starter )
    {
    battle_log.clear();
    battle_state = 0;
    if ( poke_info[ favorite_pokemon ].health <= 0 )
        battle_state = 2;
    turn = true;
    window_select = WINDOW_BATTLE;
    move_pokemon = true;
    srand(time(nullptr));
    show_wild_pokemon = rand()%16;
    wild_pokemon_stats.health = 100;
    wild_pokemon_stats.attack_min = poke_info[show_wild_pokemon].attack_min;
    wild_pokemon_stats.attack_max = poke_info[show_wild_pokemon].attack_max;
    }
}


static void draw_axes(float len) {

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
}

static void display_field()
{
    glEnable(GL_TEXTURE_2D);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 5,
              0, 0, 0,
              0, 1, 0);

    
    glPushMatrix();

    // glMultMatrixf(matrix);

    glDisable(GL_TEXTURE_2D);
    // draw_axes(10);  
    
    if( !choose_starter )
    {
    if(write_message)
    {
        text_log(-8, 8.3, message);
    }
    else
    {
        text_log(-8, 8.8, "Heal pokemons or restore \npotion charges at Pokecenter ( H )");
    }
    }
    else
    {
        text_log(-8, 8.8, " Open Pokedex to choose\n your starter Pokemon ");
    }
    
    
    text_log(-9.5, -9.5, "Pokedex ( P )");

    draw_wild_pokemon();

    glEnable(GL_TEXTURE_2D);

    draw_floor();

    draw_grass();

    draw_oak();

    draw_player();

    draw_pokecenter();
    
    glPopMatrix();
    glutSwapBuffers();                  
}

static void draw_oak()
{
    glPushMatrix();

    glTranslatef(-8.5, 7, 0);
    glScalef( player_info.width, player_info.height, 1);

    glBindTexture(GL_TEXTURE_2D, player_sprites[1]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 1);
        glVertex3f(0, 0, 0.3);

        glTexCoord2f(0 , 0);
        glVertex3f(0, -1, 0.3);

        glTexCoord2f(1, 0);
        glVertex3f(1, -1, 0.3);

        glTexCoord2f(1, 1);
        glVertex3f(1, 0, 0.3);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
}

static void draw_wild_pokemon()
{
    glPushMatrix();
    
    if(move_pokemon)
    {
        srand(time(nullptr));
        move_pokemon = false;
        wild_pokemon_info.x = rand()%10;
        wild_pokemon_info.y = rand()%10;
    }

    glTranslatef( wild_pokemon_info.x, wild_pokemon_info.y, 0);
    glScalef( wild_pokemon_info.width, wild_pokemon_info.height, 1);
    
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, 10);
        glVertex3f(0, -1, 10);
        glVertex3f(1, -1, 10);
        glVertex3f(1, 0, 10);
    glEnd();
    glPopMatrix();
}

static void display_pokemons()
{
    glEnable(GL_TEXTURE_2D);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 5,
              0, 0, 0,
              0, 1, 0);

    
    glPushMatrix();

    glDisable(GL_TEXTURE_2D);
    // draw_axes(10);

    text_log(7.4, -9, "Back ( K )" );

    if ( battle_state == -1 && owned_pokemons.count(show_pokemon))
    {   
        sprintf(buffer, "Potion charges : %d ( Y )", potion_charges);
        text_log(4.3, -8, buffer);
    }

    if( (owned_pokemons.count(show_pokemon) && 
         show_pokemon!=favorite_pokemon)    ||
         ( ( show_pokemon == 0 || show_pokemon == 1 || show_pokemon == 2) &&
             choose_starter ) )
    {
        text_log( - 9, -8, "Make favorite ( F )" );
    }

    if( show_pokemon == favorite_pokemon )
    {
        text_log( -9, -8, "FAVORITE");
    }  

    if( owned_pokemons.count( show_pokemon ) )
    {
        text_log( -9, 8, "OWNED");

        battle_drawing = 0;
        draw_stats();
    }
    else
    {
        text_log( -9, 8, "NOT OWNED");
    }
    
    glEnable(GL_TEXTURE_2D);

    draw_pokedex_background();

    draw_arrows();

    draw_pokemons();

    glPopMatrix();
    glutSwapBuffers(); 
}

static void display_battle()
{
    glEnable(GL_TEXTURE_2D);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 5,
              0, 0, 0,
              0, 1, 0);

    
    glPushMatrix();

    glDisable(GL_TEXTURE_2D);
    // draw_axes(10);

    draw_battle_log();

    text_log(6, -9, "Exit battle ( R )" );
    if ( turn && battle_state == 0 )
    {
        sprintf(buffer, "Potion charges : %d ( Y )", potion_charges);
        text_log(4.3, -8, buffer);
        text_log(6, -7, "Light attack ( J )" );
        text_log(4.8, -5, " Catch pokemon ( H )");
    }

    if ( turn )
    {
        text_log(4.5, -6, "Change pokemon ( P )" );
    }
    
    glEnable(GL_TEXTURE_2D);

    draw_forest_background();


    battle_drawing = 1;
    draw_pokemons();
    if ( poke_info[ favorite_pokemon ].health > 0 )
    {
        draw_stats();
    }

    battle_drawing = 2;
    show_pokemon = show_wild_pokemon;
    draw_pokemons();
    if ( wild_pokemon_stats.health > 0 )
    {
    draw_stats();
    }

    battle_drawing = 0;

    glPopMatrix();
    glutSwapBuffers(); 
}

static void catch_pokemon()
{
    srand(time(nullptr));

    if ( wild_pokemon_stats.health > 0  &&
         wild_pokemon_stats.health <=30 &&      
         poke_info[ favorite_pokemon ].health > 0 &&
         rand()%2 == 0)
    {
        
        
        if( owned_pokemons.count( show_wild_pokemon ) )
        {
            add_to_battle_log("You already have this pokemon, sending it to Professor Oak..." );
        }
        else
        {
            add_to_battle_log("Pokemon caught!!!" );
            owned_pokemons.insert(show_wild_pokemon);
        }
        catching = 2;
        catching_time = 0;
        battle_state = 3;
        wild_pokemon_stats.health = 0;
    }
    else
    {
        add_to_battle_log("Pokemon resisted catching." );
    }
    

    turn = false;
    if( battle_state == 0 )
    {
        hit = true;
        hit_time = 10;
        glutPostRedisplay();
        //light_attack();
    }
}

static void draw_forest_background()
{
    glBindTexture(GL_TEXTURE_2D, background_textures[3]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(-10, -10, 0);

        glTexCoord2f(1, 0);
        glVertex3f(10, -10, 0);

        glTexCoord2f(1, 1);
        glVertex3f(10, 10, 0);

        glTexCoord2f(0, 1);
        glVertex3f(-10, 10, 0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}

static void draw_stats()
{
    glPushMatrix();

    if(battle_drawing == 0)
    {
        glTranslatef( -3, -4, 0);
        glScalef( 6 * poke_info[ show_pokemon ].health/100.0, 0.7, 1);
        sprintf (buffer, "Attack : %d ~ %d", poke_info[ show_pokemon ].attack_min, poke_info[ show_pokemon ].attack_max);
        text_log(0, -1.8, buffer);
        sprintf (buffer, "Health : %d", poke_info[ show_pokemon ].health);

    }
    if(battle_drawing == 1)
    {
        glTranslatef( -8, -1, 4);
        glScalef( 4 * poke_info[ favorite_pokemon ].health/100.0, 0.7, 1);
        sprintf (buffer, "Attack : %d ~ %d", poke_info[ favorite_pokemon ].attack_min, poke_info[ favorite_pokemon ].attack_max);
        text_log(0, -1.8, buffer);
        sprintf (buffer, "Health : %d", poke_info[ favorite_pokemon ].health);

    }
    if(battle_drawing == 2)
    {
        glTranslatef( 3, 1.5, 4);
        glScalef( 4 * wild_pokemon_stats.health/100.0, 0.7 , 1);
        sprintf (buffer, "Attack : %d ~ %d", wild_pokemon_stats.attack_min, wild_pokemon_stats.attack_max);
        text_log(0, -1.8, buffer);
        sprintf (buffer, "Health : %d", wild_pokemon_stats.health);
    }

    
    text_log(0, 0.2, buffer);
    
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, 4);
        glVertex3f(0, -1, 4);
        glVertex3f(1, -1, 4);
        glVertex3f(1, 0, 4);
    glEnd();
    glPopMatrix();
}

static void draw_pokecenter()
{
    glPushMatrix();

    glTranslatef( pokecenter_info.x, pokecenter_info.y, 0 );
    glScalef( pokecenter_info.width, pokecenter_info.height, 1); 

    glBindTexture(GL_TEXTURE_2D, pokecenter_sprite);
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

static void draw_arrows()
{
    if(show_pokemon < 15 )
    {

    text_log(7.8, -0.15, "D");

    glPushMatrix();

    glTranslatef( 6, 0.75, 0 );
    glScalef( 3, 1.5, 1);

    glBindTexture(GL_TEXTURE_2D, arrow_sprite);
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

    if(show_pokemon > 0 )
    {

    text_log(-8., -0.15, "A");

    glPushMatrix();

    glRotatef(180, 0, 0, 1);
    glTranslatef( 6, 0.75, 0 );
    glScalef( 3, 1.5, 1); 
    
    glBindTexture(GL_TEXTURE_2D, arrow_sprite);
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
}


static void text_log( float x, float y, const char *s)
{
    glDisable(GL_TEXTURE_2D);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glRasterPos3f(x, y, 10);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, reinterpret_cast<const unsigned char *>( s ) ); 

    glEnable(GL_TEXTURE_2D);
}

static void draw_player()
{
    glPushMatrix();

    glTranslatef(player_info.x,player_info.y,0);
    glScalef( player_info.width, player_info.height, 1);

    glBindTexture(GL_TEXTURE_2D, player_sprites[0]);
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

static void draw_pokemons()
{
    glPushMatrix();
    
    if(battle_drawing == 0)
    {
        glTranslatef( -5, 7, 4);
        glScalef( 10, 10, 1);
        if( owned_pokemons.count( show_pokemon ) ||
            show_pokemon == 0 || 
            show_pokemon == 1 || 
            show_pokemon == 2)
        {
            glBindTexture(GL_TEXTURE_2D, pokemon_sprites[ show_pokemon ]);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, pokemon_sprites_black[ show_pokemon ]);
        }
    }
    if(battle_drawing == 1)
    {
        if ( catching == 1 )
        {
            glTranslatef ( 0, catching_time%20/20.0, 0 );
        }
        else if ( running == 1 )
        {
            glTranslatef(-running_time/5.0, 0, 0);
        }
        else if ( battle_state == 2 )
        {
            glTranslatef(-60/5.0, 0, 0);
        }
        else if( hit && turn)
        {
            glTranslatef((rand()%51-25)/100.0, (rand()%51-25)/100.0, 0);
        }
        glTranslatef( -8, -2.5, 4);
        glScalef( 7, 7, 1);
        glBindTexture(GL_TEXTURE_2D, pokemon_sprites[ favorite_pokemon ]);    
    }
    if(battle_drawing == 2)
    {
        if ( catching == 2 )
        {
            glTranslatef ( 0, catching_time%20/20.0, 0 );
        }
        else if ( running == 2 )
        {
            glTranslatef(running_time/8.0, 0, 0);
        }
        else if ( battle_state == 1 )
        {
            glTranslatef(60/8.0, 0, 0);
        }
        else if ( hit && !turn ) 
        {
            glTranslatef((rand()%51-25)/100.0, (rand()%51-25)/100.0, 0);
        }
        glTranslatef( 4 + 2.5, 0, 4);
        glRotatef( 180, 0, 1, 0);
        glScalef( 2.5, 2.5, 1);
        glBindTexture(GL_TEXTURE_2D, pokemon_sprites[ show_wild_pokemon ]);
    }

    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 1);
        glVertex3f(0, 0, 0);

        glTexCoord2f(0 , 0);
        glVertex3f(0, -1, 0);

        glTexCoord2f(1, 0);
        glVertex3f(1, -1, 0);

        glTexCoord2f(1, 1);
        glVertex3f(1, 0, 0);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
}

static auto check_collision(float x1, float y1, float w1, float h1, 
                        float x2, float y2, float w2, float h2) -> bool
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

static auto check_proximity(float x1, float y1, float w1, float h1, 
                        float x2, float y2, float w2, float h2) -> bool
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
}

static void draw_pokedex_background()
{
    /* Crta se pod*/
    glBindTexture(GL_TEXTURE_2D, background_textures[2]);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(-10, -10, 0);

        glTexCoord2f(1, 0);
        glVertex3f(10, -10, 0);

        glTexCoord2f(1, 1);
        glVertex3f(10, 10, 0);

        glTexCoord2f(0, 1);
        glVertex3f(-10, 10, 0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}  

static void draw_grass()
{
    /* Crtaju se trava */
    glBindTexture(GL_TEXTURE_2D, background_textures[1]);
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
    glBindTexture(GL_TEXTURE_2D, background_textures[0]);
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

static void open_pokedex()
{
    show_pokemon = 0;
    window_select = WINDOW_POKEDEX;
    glutPostRedisplay();
}

//PUBLIC

void display()
{
    if(window_select == WINDOW_FIELD)
    {
        display_field();
    }
    else if(window_select == WINDOW_POKEDEX)
    {
        display_pokemons();
    }
    else if(window_select == WINDOW_BATTLE)
    {
        display_battle();
    }

}

void reshape(int w, int h)
{
    window_width = w;
    window_height = h;

    reshape1(w, h);
}

void timer(int timer_id)
{
    if (timer_id == TIMER_ID) 
    {
        if(animation_parametar >= 100)
            animation_parametar = 0;
        animation_parametar += 1;

        if ( write_message )
        {
            message_time -= 1;
            if ( message_time <=0 )
            {
                write_message = false;
                message_time = 100;
            }
        }

        if ( hit )
        {
            hit_time -= 1;
            if ( hit_time <=0 )
            {
                hit = false;
                hit_time = 50;

                if(!turn && battle_state==0 )
                    light_attack();
            }
        }

        if ( running )
        {
            running_time += 1;
            if ( running_time >= 60 )
            {
                running = 0;
                running_time = 0;
            }       
        }
        if ( catching )
        {
            catching_time += 1;
            if ( catching_time >= 60 )
            {
                tmp = catching;                
                catching = 0;
                catching_time = 0;
                if( tmp == 1 )
                    light_attack();
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
            glDeleteTextures(2, background_textures);
            glDeleteTextures(1, &pokecenter_sprite);
            glDeleteTextures(1, player_sprites);
            exit(0);
        break;
        }
    }

    if(window_select == WINDOW_POKEDEX)
    {
        switch(key)
        {
        case 'a':
        case 'A':
            if(show_pokemon <= 0 )
                break;
            show_pokemon --;
            glutPostRedisplay();
            break;
        case 'd':
        case 'D':
            if(show_pokemon >= 15 )
                break;
            show_pokemon ++;
            glutPostRedisplay();
            break;
        case 'f':
        case 'F':
            if(owned_pokemons.count(show_pokemon))
            {
                favorite_pokemon = show_pokemon;
                glutPostRedisplay();   
            }
            else if ( choose_starter )
            {
                favorite_pokemon = show_pokemon;
                owned_pokemons.insert( favorite_pokemon );
                choose_starter = false;
                glutPostRedisplay();
            }
            
            break;
        case 'y':
        case 'Y':
            if ( battle_state == -1 && owned_pokemons.count(show_pokemon))
                heal_pokemon();
            break;
        case 'k':
        case 'K':
            if ( battle_state == 0 || battle_state == 2 )
            {
                window_select = WINDOW_BATTLE;
                turn = false;
                battle_state = 0;
                light_attack();
            }
            else
            {
                window_select = WINDOW_FIELD;
            }
            glutPostRedisplay();
        break;
        }
    }
    

    if(window_select == WINDOW_BATTLE)
    {
        switch(key)
        {
        case 'j':
        case 'J':
            if ( turn && battle_state == 0 )
                light_attack();
            break;
        case 'h':
        case 'H':
            if ( turn && battle_state == 0 )
                catch_pokemon();
            break;
        case 'y':
        case 'Y':
            if ( turn && battle_state == 0 )
            {
                show_pokemon = favorite_pokemon;
                heal_pokemon();
            }
            break;
        case 'p':
        case 'P':
            if ( turn || battle_state == 2 )
            {
                open_pokedex();
            }
        break;
        case 'r':
        case 'R':
            window_select = WINDOW_FIELD;
            battle_state = -1;
            glutPostRedisplay();
        break;
        }
    }    

    if(window_select == WINDOW_FIELD)
    {
    switch (key) {
    case 'p':
    case 'P':
    {
        open_pokedex();
    break;
    }
    case 'w':
    case 'W':
        if(player_info.y <= 10 )
        {
            if(!check_collision( player_info.x, player_info.y + .1, player_info.width, player_info.height,
                         pokecenter_info.x, pokecenter_info.y, pokecenter_info.width, pokecenter_info.height))
            {
                if(check_collision( player_info.x, player_info.y + .1, player_info.width, player_info.height,
                         wild_pokemon_info.x, wild_pokemon_info.y, wild_pokemon_info.width, wild_pokemon_info.height))
                {   
                    init_battle();          
                }

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
                if(check_collision( player_info.x, player_info.y - .1, player_info.width, player_info.height,
                         wild_pokemon_info.x, wild_pokemon_info.y, wild_pokemon_info.width, wild_pokemon_info.height))
                {             
                    init_battle();
                }

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
                if(check_collision( player_info.x - .1, player_info.y, player_info.width, player_info.height,
                         wild_pokemon_info.x, wild_pokemon_info.y, wild_pokemon_info.width, wild_pokemon_info.height))
                {             
                    init_battle();
                }

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
                if(check_collision( player_info.x + .1, player_info.y, player_info.width, player_info.height,
                         wild_pokemon_info.x, wild_pokemon_info.y, wild_pokemon_info.width, wild_pokemon_info.height))
                {             
                    init_battle();
                }

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
                for( auto elem : owned_pokemons)
                    poke_info[elem].health = 100;
                potion_charges = 3;
                glutPostRedisplay();
            }
        break;
    }
       
    }
    }
}

void texture_init()
{
        Image * image;

    /*
     * Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz
     * fajla.
     */
    image = image_init(0, 0);

    glGenTextures(4, background_textures);

    /* Kreira se prva tekstura. */

    image_read(image, "./resources/grass.bmp");

    glBindTexture(GL_TEXTURE_2D, background_textures[1]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    /* Kreira se druga tekstura. */
    image_read(image, "./resources/floor.bmp");

    glBindTexture(GL_TEXTURE_2D, background_textures[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);


    /* Kreira se treca tekstura. */
    image_read(image, "./resources/background.bmp");

    glBindTexture(GL_TEXTURE_2D, background_textures[2]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    
    image_read(image, "./resources/forest.bmp");

    glBindTexture(GL_TEXTURE_2D, background_textures[3]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);


    glGenTextures(1, &pokecenter_sprite);

    /* Kreira se pokecentar. */
    image_read(image, "./resources/pokecenter.bmp");

    glBindTexture(GL_TEXTURE_2D, pokecenter_sprite);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    
    glGenTextures(2, player_sprites);
    image_read(image, "./resources/character.bmp");

    glBindTexture(GL_TEXTURE_2D, player_sprites[0]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    
    image_read(image, "./resources/oak.bmp");    
    glBindTexture(GL_TEXTURE_2D, player_sprites[1]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);


    glGenTextures(1, &arrow_sprite);
    image_read(image, "./resources/arrow.bmp");

    glBindTexture(GL_TEXTURE_2D, arrow_sprite);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    


////////////////////////////////////////// POKEMON LOADING

    glGenTextures(16, pokemon_sprites);
    glGenTextures(16, pokemon_sprites_black);

    image_read(image, "./resources/pokemon_sprites/bulbasaur.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[0]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    

    
    image_read(image, "./resources/pokemon_sprites/squirtle.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[1]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    
    image_read(image, "./resources/pokemon_sprites/charmander.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[2]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    

    image_read(image, "./resources/pokemon_sprites/pikachu.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[3]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    
    image_black( image );
    glBindTexture(GL_TEXTURE_2D, pokemon_sprites_black[3]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);



    image_read(image, "./resources/pokemon_sprites/abra.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[4]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    image_black( image );
    glBindTexture(GL_TEXTURE_2D, pokemon_sprites_black[4]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);        

    image_read(image, "./resources/pokemon_sprites/articuno.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[13]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    image_black( image );
    glBindTexture(GL_TEXTURE_2D, pokemon_sprites_black[13]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    image_read(image, "./resources/pokemon_sprites/eevee.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[6]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    image_black( image );
    glBindTexture(GL_TEXTURE_2D, pokemon_sprites_black[6]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
                

    image_read(image, "./resources/pokemon_sprites/growlithe.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[7]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    image_black( image );
    glBindTexture(GL_TEXTURE_2D, pokemon_sprites_black[7]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);


    image_read(image, "./resources/pokemon_sprites/lapras.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[8]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    image_black( image );
    glBindTexture(GL_TEXTURE_2D, pokemon_sprites_black[8]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);


    image_read(image, "./resources/pokemon_sprites/snorlax.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[9]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    image_black( image );
    glBindTexture(GL_TEXTURE_2D, pokemon_sprites_black[9]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);


    image_read(image, "./resources/pokemon_sprites/zapdos.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[14]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    image_black( image );
    glBindTexture(GL_TEXTURE_2D, pokemon_sprites_black[14]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);


    image_read(image, "./resources/pokemon_sprites/moltres.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[15]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    image_black( image );
    glBindTexture(GL_TEXTURE_2D, pokemon_sprites_black[15]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);


    image_read(image, "./resources/pokemon_sprites/cubone.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[12]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    image_black( image );
    glBindTexture(GL_TEXTURE_2D, pokemon_sprites_black[12]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);


    image_read(image, "./resources/pokemon_sprites/pidgey.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[5]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    image_black( image );
    glBindTexture(GL_TEXTURE_2D, pokemon_sprites_black[5]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    image_read(image, "./resources/pokemon_sprites/vulpix.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[10]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    image_black( image );
    glBindTexture(GL_TEXTURE_2D, pokemon_sprites_black[10]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    image_read(image, "./resources/pokemon_sprites/onix.bmp");

    glBindTexture(GL_TEXTURE_2D, pokemon_sprites[11]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    image_black( image );
    glBindTexture(GL_TEXTURE_2D, pokemon_sprites_black[11]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);


    init_pokemon_stats();

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);
}

