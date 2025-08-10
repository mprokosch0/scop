#include "scop.h"

int deal_key(int key, t_data *data)
{
  if (key == 65307)
		quit_prog(data);
  if (key == 65361)
    rotate_y(data->obj->vertex, -0.02);
  if (key == 65362)
    rotate_x(data->obj->vertex, -0.02);
  if (key == 65363)
    rotate_y(data->obj->vertex, 0.02);
  if (key == 65364)
    rotate_x(data->obj->vertex, 0.02);
  if (key == 'b')
  {
    if (data->sett->bfc)
      data->sett->bfc = 0;
    else
      data->sett->bfc = 1;
  }
  return 1;
}
