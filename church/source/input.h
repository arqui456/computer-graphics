#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <glut.h>

extern double eye[3];
extern double eye_ed[3];
extern int RunMode;

extern float AnimationAngle;
extern float AnimateStep;
extern double viewAngle_horizontal;
extern double viewAngle_vertical;
extern double eye_distance;

#define MOVEMENT_COEFFICIENT 0.05

extern int window_h;
extern int window_w;

void myKeyboardFunc( unsigned char key, int x, int y );

void mySpecialKeyFunc( int key, int x, int y );

void mouseMovement(int _x, int _y);

void locateCamera();
