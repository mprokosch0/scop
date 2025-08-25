#ifndef SCOP_H
# define SCOP_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "libft.h"
#include <limits.h>
#include "../glew-2.2.0/include/GL/glew.h"
#include <GLFW/glfw3.h>
#include "structs.h"

int		parsing(t_data *data, char *fileName);

void	rotate_y(float *mat, float *angle, GLFWwindow *window);
void	rotate_x(float *mat, float *angle, GLFWwindow *window);
void	rotate_z(float *mat, float *angle, GLFWwindow *window);
void	center_obj(t_vertex *v, float *mat);
void	translate_obj(t_obj *b, float *mat, GLFWwindow *window);
void	project_points(float *mat, GLFWwindow *window);
void	lookAt(float *mat, float eyeX, float eyeY, float eyeZ,
                        float centerX, float centerY, float centerZ,
                        float upX, float upY, float upZ);

#endif