#include <windows.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <GL/gl.h>
#include <glut.h>

#define MAX_MEM 10000

typedef struct {
    double x, y, z;
} ObjPoint;

typedef struct {
    size_t vertices[3]; 
    size_t normals[3];
} ObjIndex;

typedef struct {
    ObjPoint vertices[MAX_MEM];
    ObjPoint normals[MAX_MEM];
    ObjIndex triangles[MAX_MEM];
    size_t num_triangles;
    size_t num_vertices;
    size_t num_normals;
} Object;

typedef struct {
  GLuint vb;
  int numTriangles;
} DrawObject;

void loadTexture();
void drawFloor(float angle);
void drawAngel(double angle);
void drawBench(double angle);
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
