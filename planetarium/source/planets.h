#include <GL/gl.h>
#include <glut.h>

void loadTexture();
void drawSun(double angle);
void drawEarth(double angle);
void drawMars(double angle);
void drawStars(double radius);
void WHO(double angle);
void drawOrbit(double radius, GLfloat color[3]);

int enableOrbit();
void disableOrbit();
int enableAxis();
void disableAxis();
int enableTardis();
void disableTardis();
void menu();
