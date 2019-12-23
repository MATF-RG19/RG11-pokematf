#ifndef POKEMATF_H
#define POKEMATF_H

#include <GL/glut.h>
#include <iostream>


#define WINDOW_FIELD         0  
#define WINDOW_POKEBALL      1

#define TIMER_ID             1
#define TIMER_INTERVAL       20

extern float x_position;
extern float y_position;
extern float player_size;
extern int state;
extern bool window_select;

void draw_axes(float len);

void display1();

void display2();

void display();

void reshape(int w, int h);

void reshape1(int w, int h);

void reshape2(int w, int h);

void timer(int);

void keyboard(unsigned char key, int x, int y);

#endif