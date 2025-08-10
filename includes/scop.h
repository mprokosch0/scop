
#ifndef SCOP_H
#define SCOP_H

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "structs.h"
#include <unistd.h>
#include "libft.h"
#include <limits.h>
#include "mlx.h"

#define Z_NEAR 0.1f
#define Z_FAR 100.0f

void	new_img(t_data *data, t_img *img);
int     quit_prog(t_data *data);
void	my_mlx_pixel_put(t_img *data, int x, int y, int color);
int     deal_key(int key, t_data *data);
void	clear_img(t_img *img);
t_data	*static_data(t_data *data);

int		parsing(t_data *data, char *fileName);

t_cof	normale(t_cof *a, t_cof *b, t_cof *c);
void	center_vertex(t_vertex *vertex);
void	project_vertex(t_cof *in, float *x, float *y, float *nz);
void	rotate_x(t_vertex *vertex, float angle);
void	rotate_y(t_vertex *vertex, float angle);
void	rotate_z(t_vertex *vertex);

#endif