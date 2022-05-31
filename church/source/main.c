#include <stdio.h>
#include <stdlib.h>
#include "church.h"
#include "input.h"

void DrawScene(void);

int RunMode = 1;
double viewAngle_horizontal = 0.3;
double viewAngle_vertical = -0.39;
double eye_distance = 30;
double eye[3]={0,0,0};
int window_w = 640;
int window_h = 480;
float AnimationAngle = 0.0f;
float AnimationStep = 0.0001f;
const double Xmin = -10.0, Xmax = 10.0;
const double Ymin = -10.0, Ymax = 10.0;
const GLfloat light_pos[] = {0.0, 0.0, 0.0};
const GLfloat light_ambient[] = {0.3, 0.3, 0.3};
const GLfloat light_0_col[] = {1.0, 1.0, 1.0};


void drawScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    if (RunMode == 1) {
        AnimationAngle += AnimateStep;
        if(AnimationAngle > 360.0) AnimationAngle -= 360.0*floor(AnimationAngle/360.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    menu();
    gluLookAt(eye[0], eye[1], eye[2], 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    drawStars(AnimationAngle);
    drawBench(AnimationAngle);
    drawFloor(AnimationAngle);
    //drawAngel(AnimationAngle);
    glFlush();
    glutSwapBuffers();
    if(RunMode == 1) glutPostRedisplay();
}

void initRendering() {
    enableOrbit();
    enableAxis();
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_EMISSION, light_0_col);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, light_ambient);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
    locateCamera();
    loadTexture();
}

void resizeWindow(int w, int h) {
    glViewport(0, 0, w, h);
    window_h = h;
    window_w = w;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, w/h, 1, 100);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(10, 60);
    glutInitWindowSize(window_w, window_h);
    glutCreateWindow("Church");
    initRendering();
    glutKeyboardFunc(myKeyboardFunc);
    glutSpecialFunc(mySpecialKeyFunc);
    glutReshapeFunc(resizeWindow);
    glutDisplayFunc(drawScene);
    glutMainLoop();
    return 0;
}