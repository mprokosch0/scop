
#include "scop.h"

int	get_info(t_data *data, char *fileName)
{
	int		fd;
	char	*str;

	fd = open(fileName, O_RDONLY);
	if (fd == -1)
		return 0;
	str = ft_strdup("");
	while (str)
	{
		free(str);
		str = get_next_line(fd);
		if (str && str[0] == 'v')
			data->obj->vertex->nb_vertex++;
		else if (str && str[0] == 'f')
			data->obj->faces->nb_faces++;
	}
	close(fd);
	return 1;
}

void	free_str_tab(char **str)
{
	for (int i = 0; str[i]; i++)
	{
		if (str[i])
			free(str[i]);
	}
	if (str)
			free(str);
}

int	fill_face(t_faces *faces, char *s, int index)
{
	int	i = 0;
	char	**res;

	while (s[i])
	{
		while (s[i] == ' ')
			i++;
		if (s[i] != ' ' && s[i])
		{
			faces->nb_points[index]++;
			while (s[i] != ' ' && s[i])
				i++;
		}
	}
	faces->faces[index] = calloc(faces->nb_points[index], sizeof(int));
	if (!faces->faces[index])
		return (0);
	i = 0;
	res = ft_split(s, ' ');
	while (i < faces->nb_points[index])
	{
		faces->faces[index][i] = ft_atoi(res[i]) - 1;
		i++;
	}
	free_str_tab(res);
	return 1;
}


int	fill_info(t_data *data, char *fileName)
{
	int		fd;
	char	*str;
	char	**res;
	int		v = 0;
	int		f = 0;

	fd = open(fileName, O_RDONLY);
	if (fd == -1)
		return 0;
	str = ft_strdup("");
	while (str)
	{
		free(str);
		str = get_next_line(fd);
		if (str && str[0] == 'v')
		{
			res = ft_split(&str[1], ' ');
			data->obj->vertex->co[v].x = atof(res[0]);
			data->obj->vertex->co[v].y = atof(res[1]);
			data->obj->vertex->co[v].z = atof(res[2]);
			v++;
			free_str_tab(res);
		}
		else if (str && str[0] == 'f')
			fill_face(data->obj->faces, &str[1], f++);
	}
	close(fd);
	return 1;
}

int	parsing(t_data *data, char *fileName)
{
	if (!get_info(data, fileName))
		return 0;
	data->obj->faces->faces = calloc(data->obj->faces->nb_faces, sizeof(int *));
	if (!data->obj->faces->faces)
		return 0;
	data->obj->faces->nb_points = calloc(data->obj->faces->nb_faces, sizeof(int));
	if (!data->obj->faces->nb_points)
		return 0;
	data->obj->vertex->co = calloc(data->obj->vertex->nb_vertex, sizeof(t_cof));
	if (!data->obj->vertex->co)
		return 0;
	if (!fill_info(data, fileName))
		return 0;
	return 1;
}
