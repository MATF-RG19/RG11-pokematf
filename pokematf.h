#ifndef POKEMATF_H
#define POKEMATF_H

#include <GL/glut.h>
#include <iostream>
#include <GL/freeglut.h>
#include "image.h"

#define TIMER_ID                  1
#define TIMER_INTERVAL            20

/* Identifikatori tekstura. */

extern int window_width;
extern int window_height;

//PUBLIC FUNCTIONS

//! Display function.
void display();

//! Reshape function.
void reshape(int w, int h);

//! Timer function.
void timer(int);

//! Called when event on keyboard occured and takes necessary steps.
void keyboard(unsigned char key, int x, int y);

//! Loads textures and sets corresponding in various structures.
void texture_init();

#endif