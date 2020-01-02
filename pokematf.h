#ifndef POKEMATF_H
#define POKEMATF_H

#include <GL/glut.h>
#include <iostream>
#include <GL/freeglut.h>

#define TIMER_ID                  1
#define TIMER_INTERVAL            20

/* Identifikatori tekstura. */
extern GLuint player_sprites[3];
extern GLuint pokecenter_sprite;
extern GLuint pokemon_sprites[50];
extern GLuint background_textures[3];

extern int window_width;
extern int window_height;
extern float matrix[16];


//PUBLIC FUNCTIONS

void display();

void reshape(int w, int h);

void timer(int);

void keyboard(unsigned char key, int x, int y);

void mouse(int button, int state, int x, int y);

void motion(int x, int y);


#endif