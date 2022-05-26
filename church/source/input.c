#include "input.h"
#include "planets.h"
#include <windows.h>

float AnimateStep;

void myKeyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 'r':
            RunMode = !RunMode;
            if (RunMode == 1) glutPostRedisplay();
            break;
        case 't':
            RunMode = 1;
            glutPostRedisplay();
            RunMode = 0;
            break;
        case '1':
            if(enableAxis() == 0) disableAxis();
            glutPostRedisplay();
            break;
        case '2':
            if(enableOrbit() == 0) disableOrbit();
            glutPostRedisplay();
            break;
        case '3':
            if(enableTardis() == 0) disableTardis();
            glutPostRedisplay();
            break;
        case 'w':
            eye_distance -= MOVEMENT_COEFFICIENT * 5;
            glutPostRedisplay();
            break;
        case 's':
            eye_distance += MOVEMENT_COEFFICIENT * 5;
            glutPostRedisplay();
            break;
        case 'a':
            viewAngle_horizontal += MOVEMENT_COEFFICIENT;
            glutPostRedisplay();
            break;
        case 'd':
            viewAngle_horizontal -= MOVEMENT_COEFFICIENT;
            glutPostRedisplay();
            break;
        case 'c':
            viewAngle_vertical -= MOVEMENT_COEFFICIENT;
            glutPostRedisplay();
            break;
        case 'v':
            viewAngle_vertical += MOVEMENT_COEFFICIENT;
            glutPostRedisplay();
            break;
        case 27:
            exit(1);
    }
    locateCamera();
}

void locateCamera() {
        eye[2] = -sin(viewAngle_vertical) * eye_distance;
        eye[1] = cos(viewAngle_horizontal) * (eye_distance - (1 - cos(viewAngle_vertical))* eye_distance);
        eye[0] = sin(viewAngle_horizontal) * (eye_distance - (1 - cos(viewAngle_vertical))* eye_distance);
}

void mySpecialKeyFunc(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            if (AnimateStep < 1.0e3) AnimateStep *= sqrt(2.0);
            break;
        case GLUT_KEY_DOWN:
            if (AnimateStep > 1.0e-6) AnimateStep /= sqrt(2.0);
            break;
    }
}