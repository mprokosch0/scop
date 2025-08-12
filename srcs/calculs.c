#include "scop.h"

float	miny(t_vertex *vertex)
{
	float	min = vertex->co[0].y;
	for (int i = 0; i < vertex->nb_vertex; i++)
		if (vertex->co[i].y < min)
			min = vertex->co[i].y;
	return min;
}

float	minx(t_vertex *vertex)
{
	float	min = vertex->co[0].x;
	for (int i = 0; i < vertex->nb_vertex; i++)
		if (vertex->co[i].x < min)
			min = vertex->co[i].x;
	return min;
}

float	maxy(t_vertex *vertex)
{
	float	max = vertex->co[0].y;
	for (int i = 0; i < vertex->nb_vertex; i++)
		if (vertex->co[i].y > max)
			max = vertex->co[i].y;
	return max;
}

float	maxx(t_vertex *vertex)
{
	float	max = vertex->co[0].x;
	for (int i = 0; i < vertex->nb_vertex; i++)
		if (vertex->co[i].x > max)
			max = vertex->co[i].x;
	return max;
}

void	center_vertex(t_vertex *vertex)
{
	float	midx = (maxx(vertex) + minx(vertex)) / 2;
	float	midy = (maxy(vertex) + miny(vertex)) / 2;

	for (int i = 0; i < vertex->nb_vertex; i++)
	{
		vertex->co[i].x -= midx;
		vertex->co[i].y -= midy;
	}
}

t_cof normale(t_cof *a, t_cof *b, t_cof *c)
{
	t_cof normale, ab, ac;
	float len;

	ab.x = b->x - a->x;
	ab.y = b->y - a->y;
	ab.z = b->z - a->z;

	ac.x = c->x - a->x;
	ac.y = c->y - a->y;
	ac.z = c->z - a->z;

	normale.x = ab.y * ac.z - ab.z * ac.y;
	normale.y = ab.z * ac.x - ab.x * ac.z;
	normale.z = ab.x * ac.y - ab.y * ac.x;

	len = sqrtf(normale.x * normale.x + normale.y * normale.y + normale.z * normale.z);
	if (len != 0.0f)
	{
		normale.x /= len;
		normale.y /= len;
		normale.z /= len;
	}
	return normale;
}

void project_vertex(t_cof *in, float *x, float *y, float *z)
{
    float fov = 90.0f;
    float aspect = (float)WIDTH / HEIGHT;
    float f = 1.0f / tanf((fov * 0.5f) * (PI / 180.0f));
	t_data *data;

	data = static_data(NULL);
    float nz = in->z + data->sett->camera;
    if (nz <= 0.1f)
        nz = 0.1f;

    float x_ndc = (f / aspect) * in->x / nz;
    float y_ndc = f * in->y / nz;

    *x = (int)((x_ndc + 1.0f) * 0.5f * WIDTH);
    *y = (int)((y_ndc + 1.0f) * 0.5f * HEIGHT);
	*z = nz;
}

void	do_rotations(t_data *data)
{
	if (data->sett->left_arr)
		rotate_y(data->obj->vertex, -0.02);
	if (data->sett->right_arr)
		rotate_y(data->obj->vertex, 0.02);
	if (data->sett->up_arr)
		rotate_x(data->obj->vertex, -0.02);
	if (data->sett->down_arr)
		rotate_x(data->obj->vertex, 0.02);
	if (data->sett->z)
		rotate_z(data->obj->vertex, -0.02);
	if (data->sett->x)
		rotate_z(data->obj->vertex, 0.02);
}

void	rotate_x(t_vertex *vertex, float angle)
{
	float tmp[2];

	for (int i = 0; i < vertex->nb_vertex; i++)
	{
		float y = vertex->co[i].y ;
		tmp[0] = y * cosf(angle) - vertex->co[i].z * sinf(angle);
		tmp[1] = y * sinf(angle) + vertex->co[i].z * cosf(angle);
		vertex->co[i].y = tmp[0] ;
		vertex->co[i].z = tmp[1];
	}
}

void	rotate_y(t_vertex *vertex, float angle)
{
	float tmp[2];

	for (int i = 0; i < vertex->nb_vertex; i++)
	{
		float x = vertex->co[i].x;
		tmp[0] = x * cosf(angle) - vertex->co[i].z * sinf(angle);
		tmp[1] = x * sinf(angle) + vertex->co[i].z * cosf(angle);
		vertex->co[i].x = tmp[0];
		vertex->co[i].z = tmp[1];
	}
}

void	rotate_z(t_vertex *vertex, float angle)
{
	float tmp[2];

	for (int i = 0; i < vertex->nb_vertex; i++)
	{
		tmp[0] = vertex->co[i].x * cosf(angle) - vertex->co[i].y * sinf(angle);
		tmp[1] = vertex->co[i].x * sinf(angle) + vertex->co[i].y * cosf(angle);
		vertex->co[i].x = tmp[0];
		vertex->co[i].y = tmp[1];
	}
}
