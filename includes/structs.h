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
    int nb_faces;
}   t_faces;

typedef struct vertex
{
    float   *co;
    float   cx;
    float   cy;
    float   cz;
    int     nb_vertex;
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
}   t_obj;

typedef struct data
{
    t_obj   *obj;
}   t_data;

#endif