
#include "scop.h"

void	clear_img(t_img *img)
{
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++)
			my_mlx_pixel_put(img, i, j, 0);
}

void	new_img(t_data *data, t_img *img)
{
	img->img = mlx_new_image(data->mlx->mlx, img->width, img->height);
	if (!img->img)
		return ;
	img->addr = mlx_get_data_addr(img->img, &img->bpp,
			&img->line_l, &img->endian);
}

int    quit_prog(t_data *data)
{
    mlx_destroy_image(data->mlx->mlx, data->mlx->img->img);
    mlx_destroy_window(data->mlx->mlx, data->mlx->win);
    mlx_destroy_display(data->mlx->mlx);
    free(data->mlx->mlx);
	if (data->obj->faces->faces)
		ft_freee((void **)data->obj->faces->faces, data->obj->faces->nb_faces);
	if (data->obj->faces->nb_points)
		free(data->obj->faces->nb_points);
	if (data->obj->vertex->co)
		free(data->obj->vertex->co);
    exit(0);
}

void	my_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	{
		dst = data->addr + (y * data->line_l
				+ x * (data->bpp / 8));
		*(unsigned int *)dst = color;
	}
}
