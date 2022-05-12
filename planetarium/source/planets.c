#include <math.h>
#include "planets.h"
#include <SDL2/SDL.H>
#include <SDL2/SDL_image.h>
#include <windows.h>
#include <stdio.h>


#define GL_BGRA 0x80E1
#define PI 3.14159265358979323846
int AXIS = 0;
int ORBIT = 0;
int TARDIS = 0;
SDL_Surface* sdlimage;
static GLuint texName1;
static GLuint texName2;
static GLuint texName3;
static GLuint texName4;
static GLuint texName5;

void drawAxis(double radius, GLfloat color[3]);

void drawPlanet(double main_animation_angle, double radius, double pos_x, double pos_y, double pos_z,
                double year_length, double day_length,
                double orbital_angle, double orb_x, double orb_y, double orb_z,
                double axis_angle, double axis_x, double axis_y, double axis_z,
                GLuint text_name){

    glPushMatrix();
        GLfloat col[] = {0.5, 0.5, 0.5};
        glRotatef(orbital_angle, orb_x, orb_y, orb_z);
        if (ORBIT) drawOrbit(sqrt(pos_x * pos_x + pos_y * pos_y + pos_z * pos_z), col);
        if (year_length) glRotatef(main_animation_angle * (365./year_length), 0, 0, 1);
        glPushMatrix();
            glTranslatef(pos_x, pos_y, pos_z);
            glRotatef(axis_angle, axis_x, axis_y, axis_z);
            glRotatef(main_animation_angle * 365 * (24./day_length), 0, 0, 1);
            GLUquadric *qobj = gluNewQuadric();
            gluQuadricTexture(qobj, GL_TRUE);
            glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, text_name);
                gluSphere(qobj, radius, 50, 50);
                gluDeleteQuadric(qobj);
            glDisable(GL_TEXTURE_2D);
            if (AXIS) drawAxis(radius, col);
        glPopMatrix();
    glPopMatrix();
}

void drawSun(double angle){
    glPushAttrib(GL_LIGHTING_BIT);
        GLfloat material_col[] = {1.0, 1.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, material_col);
        drawPlanet(1000 * angle, 2.0, 0.0, 0.0, 0.0,
                                 0.0, 25 * 24,
                                 0.0, 0.0, 0.0, 0.0,
                                 0.0, 0.0, 0.0, 0.0,
                                 texName1);
    glPopAttrib();
}

void drawEarth(double angle) {
    drawPlanet( 1000 * angle, 1.5, 7.0, 0.0, 0.0,
                              365.0, 24.0,
                              10, 1.0, 0.0, 0.0,
                              35.0, 0.0, 1.0, 0.0,
                              texName2);
}

void drawMars(double angle) {
    drawPlanet( 1000 * angle, 0.9, 14.0, 0.0, 0.0,
                              687, 24.0,
                              6.0, 0.0, 1.0, 1.0,
                              10.0, 0.0, 1.0, 0.0,
                              texName3);
}

void createTexture(GLuint* text_name, char* file_path){
    printf("Sem segf ate aqui");
    sdlimage = IMG_Load(file_path);
    glGenTextures(1, text_name);
    glBindTexture(GL_TEXTURE_2D, *text_name);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sdlimage->w, sdlimage->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, sdlimage->pixels);
}

void loadTexture(){
    printf("Sem segf ate aqui");
    createTexture(&texName1, "./texture/sun.png");
    createTexture(&texName2, "./texture/earth.png");
    createTexture(&texName3, "./texture/mars.png");
    createTexture(&texName4, "./texture/stars.png");
    createTexture(&texName5, "./texture/Tardis.png");
}

int enableOrbit(){
    if(ORBIT) {
        ORBIT = 1;
        return 0;  
    }
    ORBIT = 1;
    return 1;
}

void disableOrbit(){ORBIT=0;}

int enableAxis() {
    if(AXIS) {
        AXIS = 1;
        return 0;
    }
    AXIS = 1;
    return 1;
}

void disableAxis(){AXIS=0;}

void drawCarth();

int enableTardis() {
    if(TARDIS) {
        TARDIS = 1;
        return 0;
    }
    TARDIS = 1;
    return 1;
}

void disableTardis(){TARDIS=0;}

void drawStars(double angle) {
    int lastState = enableAxis();
    if(lastState == 0) disableAxis();
    disableAxis();
    glPushAttrib(GL_LIGHTING_BIT);
        GLfloat material_col[] = {1.0, 1.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, material_col);
        drawPlanet( 1000 * angle, 50, 0.0, 0.0, 0.0, 
                                   0, 1000000.0,
                                   0.0, 0.0, 0.0, 0.0,
                                   0.0, 0.0, 0.0, 0.0,
                                   texName4);
    glPopAttrib();
    if(lastState == 0) enableAxis();
}

void drawOrbit(double radius, GLfloat color[3]){
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_LINES);
        for (double i = 0; i < 2 * PI; i+= PI / 100) glVertex3f(cos(i) * radius, sin(i) * radius, 0.0);
    glEnd();
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void drawAxis(double radius, GLfloat color[3]) {
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, -2 * radius);
        glVertex3f(0.0, 0.0, +2 * radius);
    glEnd();
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void drawString(void *font, char *s, float x, float y, float z){
    glRasterPos3f(x, y, z);
    glColor3f(1, 1, 1);
    for(unsigned int i; i < strlen(s); i++) glutBitmapCharacter(font, s[i]);
}

void menu(){
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0, 0.8, 0.5);
    drawString(GLUT_BITMAP_HELVETICA_12, "Show Axis: 1       Show Orbit: 2         Enable Tardis: 3", -5, 5.3, -10);
    glColor3f(1.0, 1.0, 1.0);
    drawString(GLUT_BITMAP_HELVETICA_12, "Zoom: W/S          Horizontal: A/D       Vertical: C/V", -5, 4.9, -10);
    drawString(GLUT_BITMAP_HELVETICA_12, "Speed: Arrow Up/down", -5, 4.5, -10);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

void WHO(double angle){
    if(TARDIS){
        glPushAttrib(GL_LIGHTING_BIT);
            GLfloat material_col[] = {1.0, 1.0, 1.0, 0.0};
            glMaterialfv(GL_FRONT, GL_EMISSION, material_col);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texName5);
            glPushMatrix();
                glRotatef(10 * angle * 0.9, 0, 0, 1);
                glRotatef(10 * angle * 0.8, 0, 1, 0);
                glRotatef(10 * angle * 0.5, 1, 0, 0);
                glTranslatef(15,15,0);
                glRotatef(135, 0, 0, 1);
                glRotatef(-90, 1, 0, 0);
                glBegin(GL_QUADS);
                    glTexCoord2f(0.0, 0.0);
                    glVertex3f(-2.0, -1.0, 0.0);
                    glTexCoord2f(0.0, 1.0);
                    glVertex3f(-2.0, 1.0, 0.0);
                    glTexCoord2f(1.0, 1.0);
                    glVertex3f(0.0, 1.0, 0.0);
                    glTexCoord2f(1.0, 0.0);
                    glVertex3f(0.0, -1.0, 0.0);
                glEnd();
            glPopMatrix();
            glDisable(GL_TEXTURE_2D);
        glPopAttrib();
    }
}