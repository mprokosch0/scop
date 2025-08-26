
#include "scop.h"

void check_nb_faces(t_faces *faces, char *str)
{
	unsigned int	i;
	unsigned int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (str[i] && str[i] != ' ')
		{
			count++;
			while (str[i] != ' ' && str[i])
				i++;
		}
	}
	count = (count - 2) * 3;
	faces->nb_faces += count;
}

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
		if (str && !strncmp(str, "v ", 2))
			data->obj->vertex->nb_vertex++;
		else if (str && !strncmp(str, "vn", 2))
			data->obj->vertex->nb_n++;
		else if (str && !strncmp(str, "vt", 2))
			data->obj->vertex->nb_uvp++;
		else if (str && !strncmp(str, "f ", 2))
			check_nb_faces(data->obj->faces, &str[1]);
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

void	divide_f(char *str, GLuint *nb, GLuint *vt, GLuint *vn)
{
	int i = 0;
	*nb = ft_atoi(str) - 1;
	while (str[i] && str[i] != '/')
		i++;
	if (!str[i++] || !str[i])
		return ;
	if (str[i] == '/' && str[++i])
	{
		*vn = ft_atoi(&str[i]) - 1;
		return ;
	}
	*vt = ft_atoi(&str[i]) - 1;
	while (str[i] && str[i] != '/')
		i++;
	if (!str[++i])
		return ;
	*vn = ft_atoi(&str[i]) - 1;
}

int	fill_face(t_faces *faces, t_vertex *v, char **res, int *index)
{
	int		i = 0;
	int		count = 0;

	while (res[i++])
		count++;
	i = 0;
	GLuint nb1 = 0;
	GLuint vt1 = 0;
	GLuint vn1 = 0;
	divide_f(res[i++], &nb1, &vt1, &vn1);
	for (i = 1; i < count - 1; i++)
	{
		GLuint nb = 0;
		GLuint vt = 0;
		GLuint vn = 0;
		faces->faces[(*index)] = nb1;
		if (v->n)
			faces->n[(*index)] = vn1;
		if (v->uvp)
			faces->uvp[(*index)] = vt1;
		(*index)++;
		divide_f(res[i], &nb, &vt, &vn);
		faces->faces[(*index)] = nb;
		if (v->n)
			faces->n[(*index)] = vn;
		if (v->uvp)
			faces->uvp[(*index)] = vt;
		(*index)++;
		divide_f(res[i + 1], &nb, &vt, &vn);
		faces->faces[(*index)] = nb;
		if (v->n)
			faces->n[(*index)] = vn;
		if (v->uvp)
			faces->uvp[(*index)] = vt;
		(*index)++;
	}
	return 1;
}


int	fill_info(t_data *data, char *fileName)
{
	int		fd;
	char	*str;
	char	**res;
	int		v = 0;
	int		f = 0;
	int		vt = 0;
	int		vn = 0;

	fd = open(fileName, O_RDONLY);
	if (fd == -1)
		return 0;
	str = ft_strdup("");
	while (str)
	{
		free(str);
		str = get_next_line(fd);
		if (!str)
			break ;
		if (str[0] == 'f' || str[0] == 'v')
		{
			res = ft_split(&str[1], ' ');
			if (!res)
				return (free(str), close(fd), get_next_line(-1), 0);
			if (!ft_strncmp(str, "v ", 2))
			{
				data->obj->vertex->co[v++] = atof(res[0]);
				data->obj->vertex->cx += data->obj->vertex->co[v - 1];
				data->obj->vertex->co[v++] = atof(res[1]);
				data->obj->vertex->cy += data->obj->vertex->co[v - 1];
				data->obj->vertex->co[v++] = atof(res[2]);
				data->obj->vertex->cz += data->obj->vertex->co[v - 1];
			}
			else if (!ft_strncmp(str, "vt", 2))
			{
				data->obj->vertex->uvp[vt++] = atof(res[0]);
				data->obj->vertex->uvp[vt++] = atof(res[1]);
			}
			else if (!ft_strncmp(str, "vn", 2))
			{
				data->obj->vertex->n[vn++] = atof(res[0]);
				data->obj->vertex->n[vn++] = atof(res[1]);
				data->obj->vertex->n[vn++] = atof(res[2]);
			}
			else if (str[0] == 'f')
				fill_face(data->obj->faces, data->obj->vertex, res, &f);
			free_str_tab(res);
		}
	}
	get_next_line(-1);
	data->obj->vertex->cx /= data->obj->vertex->nb_vertex;
	data->obj->vertex->cy /= data->obj->vertex->nb_vertex;
	data->obj->vertex->cz /= data->obj->vertex->nb_vertex;
	close(fd);
	return 1;
}

int	parsing(t_data *data, char *fileName)
{
	if (!get_info(data, fileName))
		return 0;
	data->obj->faces->faces = ft_calloc(data->obj->faces->nb_faces, sizeof(GLuint));
	if (!data->obj->faces->faces)
		return 0;
	data->obj->faces->uvp = ft_calloc(data->obj->faces->nb_faces, sizeof(GLuint));
	if (!data->obj->faces->uvp)
		return 0;
	data->obj->faces->n = ft_calloc(data->obj->faces->nb_faces, sizeof(GLuint));
	if (!data->obj->faces->n)
		return 0;
	data->obj->vertex->co = ft_calloc(data->obj->vertex->nb_vertex * 3, sizeof(float));
	if (!data->obj->vertex->co)
		return 0;
	if (data->obj->vertex->nb_uvp)
	{
		data->obj->vertex->uvp = ft_calloc(data->obj->vertex->nb_uvp * 2, sizeof(float));
		if (!data->obj->vertex->uvp)
			return 0;
	}
	if (data->obj->vertex->nb_n)
	{
		data->obj->vertex->n = ft_calloc(data->obj->vertex->nb_n * 3, sizeof(float));
		if (!data->obj->vertex->n)
			return 0;
	}
	if (!fill_info(data, fileName))
		return 0;
	return 1;
}
