#include "scop.h"

int release_key(int key, t_data *data)
{
  if (key == 65361)
   data->sett->left_arr = 0;
  if (key == 65362)
    data->sett->up_arr = 0;
  if (key == 65363)
    data->sett->right_arr = 0;
  if (key == 65364)
    data->sett->down_arr = 0;
  return 1;
}

int deal_key(int key, t_data *data)
{
  if (key == 65307)
		quit_prog(data);
  if (key == 65361)
    data->sett->left_arr = 1;
  if (key == 65362)
    data->sett->up_arr = 1;
  if (key == 65363)
    data->sett->right_arr = 1;
  if (key == 65364)
    data->sett->down_arr = 1;
  if (key == 'b')
  {
    if (data->sett->bfc)
      data->sett->bfc = 0;
    else
      data->sett->bfc = 1;
  }
  return 1;
}
