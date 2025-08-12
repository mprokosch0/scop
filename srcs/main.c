
#include "scop.h"

void    reset_z_buffer(t_obj *obj)
{
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            obj->z_buffer[i][j] = INFINITY;
}

float interpolate_z_triangle(t_cof *v0, t_cof *v1, t_cof *v2, float x, float y)
{
    float x1 = v0->x, y1 = v0->y, z1 = v0->z;
    float x2 = v1->x, y2 = v1->y, z2 = v1->z;
    float x3 = v2->x, y3 = v2->y, z3 = v2->z;

    float area = ((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1));
    if (area == 0)
        return -1;
    float w1 = ((x2 - x) * (y3 - y) - (x3 - x) * (y2 - y)) / area;
    float w2 = ((x3 - x) * (y1 - y) - (x1 - x) * (y3 - y)) / area;
    float w3 = ((x1 - x) * (y2 - y) - (x2 - x) * (y1 - y)) / area;
    if (w1 < 0 || w2 < 0 || w3 < 0)
        return -1;
    float sum = w1 + w2 + w3;
    w1 /= sum;
    w2 /= sum;
    w3 /= sum;
    return w1 * z1 + w2 * z2 + w3 * z3;
}


float z_interpolate(t_data *data, int x, int y, int index)
{
    t_vertex *vertex = data->obj->vertex;
    t_faces *faces = data->obj->faces;
    float close = INFINITY;
    int *face = faces->faces[index];
    int n = faces->nb_points[index];

    for (int i = 1; i < n - 1; i++)
    {
        t_cof v0 = {0}, v1 = {0}, v2 = {0};
        project_vertex(&vertex->co[face[0]], &v0.x, &v0.y, &v0.z);
        project_vertex(&vertex->co[face[i]], &v1.x, &v1.y, &v1.z);
        project_vertex(&vertex->co[face[i + 1]], &v2.x, &v2.y, &v2.z);
        float z = interpolate_z_triangle(&v0, &v1, &v2, x, y);
        if (z != -1 && z < close)
            close = z;
    }
    return close;
}

void    fill_min_max(t_cof *min, t_cof *max, t_data *data, int index)
{
    t_vertex *vertex = data->obj->vertex;
    t_faces *faces = data->obj->faces;

    for (int j = 0; j < faces->nb_points[index]; j++)
    {
        float x, y, z;
        project_vertex(&vertex->co[faces->faces[index][j]], &x, &y, &z);
        if (x < min->x)
            min->x = x;
        if (x > max->x)
            max->x = x;
        if (y < min->y)
            min->y = y;
        if (y > max->y)
            max->y = y;
    }
}

int winding_number(t_data *data, int x, int y, int index)
{
    t_vertex *vertex = data->obj->vertex;
    t_faces *faces = data->obj->faces;
    int n = faces->nb_points[index];
    int wn = 0;

    for (int i = 0; i < faces->nb_points[index]; i++)
    {
        t_cof A, B;
        float z;
        project_vertex(&vertex->co[faces->faces[index][i]], &A.x, &A.y, &z);
        project_vertex(&vertex->co[faces->faces[index][(i + 1) % n]], &B.x, &B.y, &z);
        if (A.y <= y)
        {
            if (B.y > y)
            {
                double x_inter = A.x + (y - A.y) * (B.x - A.x) / (B.y - A.y);
                if (x_inter > x)
                    wn += 1;
            }
        }
        else
        {
            if (B.y <= y)
            {
                double x_inter = A.x + (y - A.y) * (B.x - A.x) / (B.y - A.y);
                if (x_inter > x)
                    wn -= 1;
            }
        }
    }
    return wn;
}


int is_in_face(t_data *data, int x, int y, int index)
{
    t_vertex *vertex = data->obj->vertex;
    t_faces *faces = data->obj->faces;
    int     inside = 0;
    int     n = faces->nb_points[index];

    for (int i = 0, j = n - 1; i < n; j = i++)
    {
        float xi, yi, xj, yj;
        float z;
        project_vertex(&vertex->co[faces->faces[index][i]], &xi, &yi, &z);
        project_vertex(&vertex->co[faces->faces[index][j]], &xj, &yj, &z);
        if (((yi > y) != (yj > y)) &&
            (x < (xj - xi) * (y - yi) / (yj - yi) + xi))
            inside = !inside;
    }
    return inside;
}

int is_in_sight(t_data *data, int index)
{
    t_vertex    *vertex = data->obj->vertex;
    t_faces     *faces = data->obj->faces;
    t_cof       norm;

    norm = normale(&vertex->co[faces->faces[index][0]],
        &vertex->co[faces->faces[index][1]],
        &vertex->co[faces->faces[index][2]]);
    t_cof view_dir = {vertex->co[faces->faces[index][0]].x,
        vertex->co[faces->faces[index][0]].y,
        vertex->co[faces->faces[index][0]].z + data->sett->camera};
    if (norm.x * view_dir.x + norm.y * view_dir.y + norm.z * view_dir.z < 0.0f)
        return 1;
    return 0;
}

void    do_calc(t_data *data, int x, int y, int i)
{
    if (winding_number(data, x, y, i))
    {
        float z = z_interpolate(data, x, y, i);
        if (x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT
            && z < data->obj->z_buffer[y][x])
        {
            data->obj->z_buffer[y][x] = z;
            if (i % 3 == 1)
                my_mlx_pixel_put(data->mlx->img, x, y, 0xffffff);
            else if (i % 3 == 2)
                my_mlx_pixel_put(data->mlx->img, x, y, 0xc0c0c0);
            else
                my_mlx_pixel_put(data->mlx->img, x, y, 0x808080);
        }
    }
}

int display_loop(t_data *data)
{
    t_faces *faces = data->obj->faces;

    clear_img(data->mlx->img);
    reset_z_buffer(data->obj);
    do_rotations(data);
    for (int i = 0; i < faces->nb_faces; i++)
    {
        t_cof min = {(float)INT_MAX, (float)INT_MAX, 0};
        t_cof max = {(float)INT_MIN, (float)INT_MIN, 0};
        fill_min_max(&min, &max, data, i);
        for (int x = (int)min.x; x <= (int)max.x; x++)
        {
            for (int y = (int)min.y; y <= (int)max.y; y++)
            {
                if (data->sett->bfc && is_in_sight(data, i))
                    do_calc(data, x, y, i);
                else if (!data->sett->bfc)
                    do_calc(data, x, y, i);
            }
        }
    }
    mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->mlx->img->img, 0, 0);
    return 1;
}

t_data *static_data(t_data *data)
{
    static t_data *res = NULL;

    if (data)
        res = data;
    return res;
}

int main(int ac, char **av)
{
    t_data data;

    if (ac != 2)
        return 1;
    data.mlx = &(t_mlx){0, 0, 0};
    data.mlx->img = &(t_img){0, 0, 0, 0, 0, 1080, 1920};
    data.obj = &(t_obj){0, 0, {{INFINITY}}};
    data.obj->faces = &(t_faces){0, 0, 0};
    data.sett = &(t_sett){1, 0, 0, 0, 0, 0, 0, 5.0f};
    data.obj->vertex = &(t_vertex){0, 0};
    if (!parsing(&data, av[1]))
        return 2 ;
    center_vertex(data.obj->vertex);
    data.mlx->mlx = mlx_init();
    data.mlx->win = mlx_new_window(data.mlx->mlx, 1920, 1080, "scop");
    static_data(&data);
    new_img(&data, data.mlx->img);
    rotate_x(data.obj->vertex, PI);
    mlx_put_image_to_window(data.mlx->mlx, data.mlx->win, data.mlx->img->img, 0, 0);
    mlx_hook(data.mlx->win, 17, 0, quit_prog, &data);
    mlx_hook(data.mlx->win, 2, (1L << 0), deal_key, &data);
    mlx_key_hook(data.mlx->win, release_key, &data);
    mlx_mouse_hook(data.mlx->win, mouse_hook, &data);
    mlx_loop_hook(data.mlx->mlx, display_loop, &data);
    mlx_loop(data.mlx->mlx);
}
