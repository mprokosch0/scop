#ifndef STRUCTS_H
#define STRUCTS_H

#include "scop.h"

#define WIDTH 1920
#define HEIGHT 1080
#define PI 3.1415926535

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_l;
	int		endian;
	int		height;
	int		width;
}	t_img;

typedef struct t_mlx
{
    void    *mlx;
    void    *win;
    t_img   *img;
}t_mlx;

typedef struct cof
{
    float x;
    float y;
    float z;
}   t_cof;

typedef struct co
{
    int x;
    int y;
    int z;
}   t_co;

typedef struct faces
{
    int **faces;
    int *nb_points;
    int nb_faces;
}   t_faces;

typedef struct vertex
{
    t_cof   *co;
    int     nb_vertex;
}   t_vertex;

typedef struct obj
{
    t_vertex    *vertex;
    t_faces     *faces;
    float       z_buffer[HEIGHT][WIDTH];
}   t_obj;

typedef struct settings
{
    int bfc;
    int left_arr;
    int right_arr;
    int up_arr;
    int down_arr;
}   t_sett;

typedef struct data
{
    t_mlx   *mlx;
    t_obj   *obj;
    t_sett  *sett;
}   t_data;

#endif