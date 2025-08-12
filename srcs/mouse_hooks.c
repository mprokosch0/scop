#include "scop.h"

int mouse_hook(int click, int x, int y, t_data *data)
{
	(void)x, (void)y, (void)data;
	if (click == 4)
		data->sett->camera -= 0.2;
	if (click == 5)
		data->sett->camera += 0.2;
	return 1;
}
