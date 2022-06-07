#include <memory.h>
#include <stdbool.h>
#include <string.h>
#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include "tinyobj_loader_c.h"
#include "church.h"
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>

#define GL_BGRA 0x80E1
#define PI 3.14159265358979323846
#define DEBUG if(0)
int AXIS = 0;
int ORBIT = 0;
int TARDIS = 0;
static GLuint texName1;
static GLuint texName2;
static GLuint texName3;
static GLuint texName4;
static GLuint texName5;
//static GLuint texName6;

static bool use_colors = false;
static bool draw_wireframe = true;
static const size_t OBJ_SIZE = sizeof(float) * 3 // pos
	+ sizeof(float) * 3 // normal
	+ sizeof(float) * 3 // color (based on normal)
	+ sizeof(float) * 3; // color from material file.

static DrawObject gDrawObject;

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

    ILuint ImgId = 0;
    ilGenImages(1, &ImgId);
    ilBindImage(ImgId);
    ilLoadImage(file_path);
    BYTE* data = ilGetData();
    if(!data) return;

    glGenTextures(1, text_name);
    glBindTexture(GL_TEXTURE_2D, *text_name);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void loadTexture(){
    ilInit();
    iluInit();
    ilEnable (IL_CONV_PAL);
    ilutEnable (ILUT_OPENGL_CONV);
    ilutRenderer (ILUT_OPENGL);
    createTexture(&texName1, "./texture/sun.png");
    createTexture(&texName2, "./texture/earth.png");
    createTexture(&texName3, "./texture/mars.png");
    createTexture(&texName4, "./texture/stars.png");
    createTexture(&texName5, "./texture/checkered-pattern.png");
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

void drawObj(){

}

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

void drawInteger(void *font, int num, float x, float y, float z){
    glRasterPos3f(x, y, z);
    glColor3f(1, 1, 1);
    char temp[10];
    sprintf(temp, "%d", num);
    for(unsigned int i; i < strlen(temp); i++) glutBitmapCharacter(font, temp[i]);
}

void drawFloat(void *font, float num, float x, float y, float z){
    glRasterPos3f(x, y, z);
    glColor3f(1, 1, 1);
    char temp[20];
    sprintf(temp, "%f", num);
    for(unsigned int i; i < strlen(temp); i++) glutBitmapCharacter(font, temp[i]);
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

static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]) {
    float v10[3];
    float v20[3];
    float len2;
    v10[0] = v1[0] - v0[0];
    v10[1] = v1[1] - v0[1];
    v10[2] = v1[2] - v0[2];
    v20[0] = v2[0] - v0[0];
    v20[1] = v2[1] - v0[1];
    v20[2] = v2[2] - v0[2];
    N[0] = v20[1] * v10[2] - v20[2] * v10[1];
    N[1] = v20[2] * v10[0] - v20[0] * v10[2];
    N[2] = v20[0] * v10[1] - v20[1] * v10[0];
    len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
    if (len2 > 0.0f) {
        float len = (float)sqrt((double)len2);
        N[0] /= len;
        N[1] /= len;
    }
}

static char* mmap_file(size_t* len, const char* filename) {
    HANDLE file =
        CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (file == INVALID_HANDLE_VALUE) return NULL;
    HANDLE fileMapping = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);
    assert(fileMapping != INVALID_HANDLE_VALUE);
    LPVOID fileMapView = MapViewOfFile(fileMapping, FILE_MAP_READ, 0, 0, 0);
    char* fileMapViewChar = (char*)fileMapView;
    assert(fileMapView != NULL);
    DWORD file_size = GetFileSize(file, NULL);
    (*len) = (size_t)file_size;
    return fileMapViewChar;
}

static void get_file_data(void* ctx, const char* filename, const int is_mtl,
                          const char* obj_filename, char** data, size_t* len) {
    (void)ctx;

    if(!filename) {
        fprintf(stderr, "null filename\n");
        (*data) = NULL;
        (*len) = 0;
        return;
    }
    
    size_t data_len = 0;
    *data = mmap_file(&data_len, filename);
    (*len) = data_len;
}

int LoadObject(const char* filename, Object* instanceOfObject) {
    tinyobj_attrib_t attrib;
    tinyobj_shape_t* shapes = NULL;
    size_t num_shapes;
    tinyobj_material_t* materials = NULL;
    size_t num_materials;
    unsigned int flags = TINYOBJ_FLAG_TRIANGULATE;
    int ret = tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials,
                                &num_materials, filename, get_file_data, NULL, flags);
    if (ret != TINYOBJ_SUCCESS) return 1;
    
    size_t index = 0;

    for(size_t vec_start = 0; vec_start < attrib.num_face_num_verts; vec_start +=3) {
        instanceOfObject->vertices[index].x  = attrib.vertices[vec_start];
        instanceOfObject->vertices[index].y = attrib.vertices[vec_start + 1];
        instanceOfObject->vertices[index++].z = attrib.vertices[vec_start + 2];
    }
    instanceOfObject->num_vertices = index;
    index = 0;

    for(size_t norm_start = 0; norm_start < attrib.num_normals; norm_start +=3) {
        instanceOfObject->normals[index].x = attrib.normals[norm_start];
        instanceOfObject->normals[index].y = attrib.normals[norm_start + 1];
        instanceOfObject->normals[index++].z = attrib.normals[norm_start + 2];
    }
    instanceOfObject->num_normals = index;
    int size = sizeof(attrib.material_ids)/sizeof(attrib.material_ids[0]); 
    DEBUG {
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        drawInteger(GLUT_BITMAP_HELVETICA_12, attrib.num_vertices, 0, 0, 0);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
    }   
    for (index = 0; index < attrib.num_face_num_verts; ++index) {
        // offset by 3 because values are grouped as vertex/normal/texture
        instanceOfObject->triangles[index].vertices[0] = attrib.faces[3 * index].v_idx;
        instanceOfObject->triangles[index].vertices[1] = attrib.faces[3 * index + 1].v_idx; 
        instanceOfObject->triangles[index].vertices[2] = attrib.faces[3 * index + 2].v_idx;
        instanceOfObject->triangles[index].normals[0] = attrib.faces[3 * index].vn_idx;
        instanceOfObject->triangles[index].normals[1] = attrib.faces[3 * index + 1].vn_idx;
        instanceOfObject->triangles[index].normals[2] = attrib.faces[3 * index + 2].vn_idx;
    }
    instanceOfObject->num_triangles = attrib.num_face_num_verts;
    tinyobj_attrib_free(&attrib);
    tinyobj_shapes_free(shapes, num_shapes);
    tinyobj_materials_free(materials, num_materials);
    return 0;
}

void drawFloor(float angle){
    glPushAttrib(GL_LIGHTING_BIT);
    glRotatef(angle, 0, 0, 0);
    glTranslatef(-20,-100,-2);
    glColor3f(1, 1, 1);
    glScalef(6, 20, 1);
    GLfloat material_col[] = {0.5, 0.1, 0.1, 0.1};
    glMaterialfv(GL_FRONT, GL_EMISSION, material_col);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texName5); 
    glPushMatrix();
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0,0.0);
        glVertex3f(2.0, 4.0, 0.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(8.0, 4.0, 0.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(8.0, 6.0, 0.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(2.0, 6.0, 0.0);
    glEnd();
    glFlush();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glPopAttrib();
}

void Drawer(Object obj, float angle, float rX, float rY, float rZ,
            float x, float y, float z, float r, float g, float b,
            int butcher, int tunner, float sX, float sY, float sZ){

    DEBUG {
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        drawInteger(GLUT_BITMAP_HELVETICA_12, obj.num_normals, 0, 0, 0);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
    }

    glPushMatrix();
        glPushAttrib(GL_LIGHTING_BIT);
        glRotatef(angle, rX, rY, rZ);
        glTranslatef(x,y,z);
        glColor3f(r, g, b);
        glScalef(sX, sY, sZ);
        glPushMatrix();
            glBegin(GL_TRIANGLES);
            for (size_t index = 0; index < obj.num_triangles /butcher + tunner; index++) {
                glNormal3f(obj.normals[obj.triangles[index].normals[0]].x, 
                        obj.normals[obj.triangles[index].normals[0]].y,
                        obj.normals[obj.triangles[index].normals[0]].z);
                glVertex3f(obj.vertices[obj.triangles[index].vertices[0]].x,
                        obj.vertices[obj.triangles[index].vertices[0]].y,
                        obj.vertices[obj.triangles[index].vertices[0]].z);
                glNormal3f(obj.normals[obj.triangles[index].normals[1]].x, 
                        obj.normals[obj.triangles[index].normals[1]].y,
                        obj.normals[obj.triangles[index].normals[1]].z);
                glVertex3f(obj.vertices[obj.triangles[index].vertices[1]].x,
                        obj.vertices[obj.triangles[index].vertices[1]].y,
                        obj.vertices[obj.triangles[index].vertices[1]].z);
                glNormal3f(obj.normals[obj.triangles[index].normals[2]].x, 
                        obj.normals[obj.triangles[index].normals[2]].y,
                        obj.normals[obj.triangles[index].normals[2]].z);
                glVertex3f(obj.vertices[obj.triangles[index].vertices[2]].x,
                        obj.vertices[obj.triangles[index].vertices[2]].y,
                        obj.vertices[obj.triangles[index].vertices[2]].z);
            }
            glEnd();
        glPopMatrix();
        glPopAttrib();
    glPopMatrix();
}

void drawAngel(double angle) {
    Object obj;
    int err = LoadObject("./objs/church2.obj", &obj);
    if (err) return;
    //glLoadIdentity();
    Drawer(obj, 90, 1, 0, 0, 0, 0, -5, 1, 1, 0.5, 1, 0, 1, 1, 1);
}

void drawBench(double angle) {
    Object obj;
    int err = LoadObject("./objs/bench.obj", &obj);
    if (err) return;
    
    Drawer(obj, 90, 1, 0, 0, 0, 0, 0, 0.44, 0.33, 0.23, 1, 0, 1, 1, 1);
    Drawer(obj, 90, 1, 0, 0, 5, 0, 0, 0.44, 0.33, 0.23, 1, 0, 1, 1, 1);
    Drawer(obj, 90, 1, 0, 0, 10, 0, 0, 0.44, 0.33, 0.23, 3, 10, 1, 1, 1);

    Drawer(obj, 90, 1, 0, 0, 15, 0, 0, 0.44, 0.33, 0.23, 1, 0, 1, 1, 1);
    Drawer(obj, 90, 1, 0, 0, 20, 0, 0, 0.44, 0.33, 0.23, 1, 0, 1, 1, 1);
    Drawer(obj, 90, 1, 0, 0, 25, 0, 0, 0.44, 0.33, 0.23, 3, 10, 1, 1, 1);

    Drawer(obj, 90, 1, 0, 0, 0, 0, 5, 0.44, 0.33, 0.23, 1, 0, 1, 1, 1);
    Drawer(obj, 90, 1, 0, 0, 5, 0, 5, 0.44, 0.33, 0.23, 1, 0, 1, 1, 1);
    Drawer(obj, 90, 1, 0, 0, 10, 0, 5, 0.44, 0.33, 0.23, 3, 10, 1, 1, 1);

    Drawer(obj, 90, 1, 0, 0, 15, 0, 5, 0.44, 0.33, 0.23, 1, 0, 1, 1, 1);
    Drawer(obj, 90, 1, 0, 0, 20, 0, 5, 0.44, 0.33, 0.23, 1, 0, 1, 1, 1);
    Drawer(obj, 90, 1, 0, 0, 25, 0, 5, 0.44, 0.33, 0.23, 3, 10, 1, 1, 1);

    Drawer(obj, 90, 1, 0, 0, 0, 0, 10, 0.44, 0.33, 0.23, 1, 0, 1, 1, 1);
    Drawer(obj, 90, 1, 0, 0, 5, 0, 10, 0.44, 0.33, 0.23, 1, 0, 1, 1, 1);
    Drawer(obj, 90, 1, 0, 0, 10, 0, 10, 0.44, 0.33, 0.23, 3, 10, 1, 1, 1);

    Drawer(obj, 90, 1, 0, 0, 15, 0, 10, 0.44, 0.33, 0.23, 1, 0, 1, 1, 1);
    Drawer(obj, 90, 1, 0, 0, 20, 0, 10, 0.44, 0.33, 0.23, 1, 0, 1, 1, 1);
    Drawer(obj, 90, 1, 0, 0, 25, 0, 10, 0.44, 0.33, 0.23, 3, 10, 1, 1, 1);
}

void Draw(const DrawObject* draw_object) {
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    glColor3f(1.0f, 1.0f, 1.0f);
    if (draw_object->vb >= 1) {
        //glBindBuffer(GL_ARRAY_BUFFER, draw_object->vb);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, OBJ_SIZE, (const void*)0);
        glNormalPointer(GL_FLOAT, OBJ_SIZE, (const void*)(sizeof(float) * 3));
        if (use_colors) {
            glColorPointer(3, GL_FLOAT, OBJ_SIZE, (const void*)(sizeof(float) * 9));
        } else {
            glColorPointer(3, GL_FLOAT, OBJ_SIZE, (const void*)(sizeof(float) * 6));
        }
        glDrawArrays(GL_TRIANGLES, 0, 3 * draw_object->numTriangles);
    }

    /* draw wireframe */
    glDisable(GL_POLYGON_OFFSET_FILL);
    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_LINE);
    glColor3f(0.0f, 0.0f, 0.4f);

    if (draw_object->vb >= 1 && draw_wireframe) {
        //glBindBuffer(GL_ARRAY_BUFFER, draw_object->vb);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, OBJ_SIZE, (const void*)0);
        glNormalPointer(GL_FLOAT, OBJ_SIZE, (const void*)(sizeof(float) * 3));

        glDrawArrays(GL_TRIANGLES, 0, 3 * draw_object->numTriangles);
    }
}