#ifndef STRUCTS_H
#define STRUCTS_H

#include "scop.h"

#define WIDTH 1920
#define HEIGHT 1080
#define PI 3.1415926535


typedef struct co
{
    int x;
    int y;
    int z;
}   t_co;

typedef struct faces
{
    GLuint *faces;
    GLuint *uvp;
    GLuint *n;
    int nb_faces;
}   t_faces;

typedef struct vertex
{
    float   *co;
    float   *uvp;
    float   *uvs;
    float   *n;
    float   cx;
    float   cy;
    float   cz;
    int     nb_vertex;
    int     nb_uvp;
    int     nb_n;
}   t_vertex;

typedef struct obj
{
    t_vertex    *vertex;
    t_faces     *faces;
    float       angleX;
    float       angleY;
    float       angleZ;
    float       tx;
    float       ty;
    float       tz;
    int         which_uv;
    int         tri;
    int         tex;
}   t_obj;

typedef struct data
{
    t_obj   *obj;
    int     nb_obj;
}   t_data;

#endif