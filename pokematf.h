#ifndef POKEMATF_H
#define POKEMATF_H

#include <GL/glut.h>
#include <iostream>


#define WINDOW_FIELD              0  
#define WINDOW_POKEBALL           1

#define TIMER_ID                  1
#define TIMER_INTERVAL            20
#define POKECENTAR_POSITION_X    -5
#define POKECENTAR_POSITION_Y    -5

/* Imena fajlova sa teksturama. */
#define FILENAME0 "floor.bmp"
#define FILENAME1 "grass.bmp"

/* Identifikatori tekstura. */
extern GLuint names[2];

extern int window_width;
extern int window_height;
extern float matrix[16];

//PRIVATE

static void draw_axes(float len);

static void display1();

static void display2();

static void reshape1(int w, int h);

static void reshape2(int w, int h);

static void draw_player();

static void draw_pokeball();

static void draw_wild_pokemon();

static GLboolean check_collision( float obj_1_x, float obj_1_y, float obj_1_w, float obj_1_h, 
                      float obj_2_x, float obj_2_y, float obj_2_w, float obj_2_h);

static void draw_textures();

//PUBLIC

void display();

void reshape(int w, int h);

void timer(int);

void keyboard(unsigned char key, int x, int y);

void mouse(int button, int state, int x, int y);

void motion(int x, int y);


#endif