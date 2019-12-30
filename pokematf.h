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


//PUBLIC

void display();

void reshape(int w, int h);

void timer(int);

void keyboard(unsigned char key, int x, int y);

void mouse(int button, int state, int x, int y);

void motion(int x, int y);


#endif