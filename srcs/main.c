
#include "../includes/scop.h"

GLuint compileShader(const char* source, GLenum type)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    }
    return shader;
}

GLuint createShaderProgram(const char* vertexSrc, const char* fragmentSrc)
{
    GLuint vertex = compileShader(vertexSrc, GL_VERTEX_SHADER);
    GLuint fragment = compileShader(fragmentSrc, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::PROGRAM::LINK_FAILED\n%s\n", infoLog);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}

int	initiate_window(GLFWwindow **window, char *str)
{
	if(!glfwInit())
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return 0;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	*window = glfwCreateWindow(WIDTH, HEIGHT, str, NULL, NULL);
	if(*window == NULL)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		getchar();
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(*window);
	if (!loadGLFunctions())
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		getchar();
		glfwTerminate();
		return 0;
	}
	return (1);
}

static void oneLine(char **dest, char *file)
{
	int		fd;
	char	*str;
	int		old_size = 0;
	int		len;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return ;
	str = get_next_line(fd);
	while (str)
	{
		len = ft_strlen(str);
		*dest = ft_realloc(*dest, len + old_size + 1, old_size);
		if (!(*dest))
		{
			free(str);
			close(fd);
			return ;
		}
		ft_memcpy(&(*dest)[old_size], str, len);
		old_size += len;
		(*dest)[old_size] = '\0';
		free(str);
		str = get_next_line(fd);
	}
	close(fd);
}

int initiate_shaders(GLuint *shaderProgram)
{
	char *vertexSrc = NULL;
	char *fragmentSrc = NULL;
	oneLine(&vertexSrc, "shaders/vertexShader.glsl");
	if (!(vertexSrc))
		return (0);
	oneLine(&fragmentSrc, "shaders/fragmentShader.glsl");
	if (!(fragmentSrc))
	{
		free(vertexSrc);
		return 0;
	}
	*shaderProgram = createShaderProgram(vertexSrc, fragmentSrc);
	free(vertexSrc);
	free(fragmentSrc);
	return 1;
}

void	create_and_bind_matrices(t_data *data, GLFWwindow *window, GLuint shaderProgram)
{
	t_vertex *v = data->obj->vertex;
	float rotX[16];
	float rotY[16];
	float rotZ[16];
	float translate[16];
	float center[16];
	float projection[16];
	float camera[16];
	center_obj(data->obj->vertex, center);
	rotate_y(rotY, &data->obj->angleY, window);
	rotate_x(rotX, &data->obj->angleX, window);
	rotate_z(rotZ, &data->obj->angleZ, window);
	translate_obj(data->obj, translate, window);
	project_points(projection, window);
	lookAt(camera, 0.0f, 0.0f, 15.0f,
					-v->cx, -v->cy, -v->cz,
					0.0f, 1.0f, 0.0f);
	GLuint modelLoc = glGetUniformLocation(shaderProgram, "rotX");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, rotX);
	modelLoc = glGetUniformLocation(shaderProgram, "rotY");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, rotY);
	modelLoc = glGetUniformLocation(shaderProgram, "rotZ");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, rotZ);
	modelLoc = glGetUniformLocation(shaderProgram, "center");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, center);
	modelLoc = glGetUniformLocation(shaderProgram, "translate");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, translate);
	modelLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, projection);
	modelLoc = glGetUniformLocation(shaderProgram, "camera");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, camera);
	modelLoc = glGetUniformLocation(shaderProgram, "which_uv");
	glUniform1i(modelLoc, data->obj->which_uv);
	modelLoc = glGetUniformLocation(shaderProgram, "tex");
	glUniform1i(modelLoc, data->obj->tex);
}



float	*generateUVp(float *vertices, int nb_vertices)
{
    float *uvs = ft_calloc(sizeof(float), nb_vertices * 2);
    if (!uvs)
		return NULL;
    float minY = vertices[1], maxY = vertices[1];
	float minZ = vertices[2], maxZ = vertices[2];

    for (int i = 3; i < nb_vertices * 3; i += 3)
	{
        if (vertices[i + 1] < minY)
			minY = vertices[i];
        if (vertices[i + 1] > maxY)
			maxY = vertices[i];
		if (vertices[i + 2] < minZ)
			minZ = vertices[i];
        if (vertices[i + 2] > maxZ)
			maxZ = vertices[i];
    }

	for (int v = 0; v < nb_vertices; v++)
	{
		float y = vertices[v * 3 + 1];
		float z = vertices[v * 3 + 2];
		float rangeY = maxY - minY;
		float rangeZ = maxZ - minZ;
		float maxRange = fmax(rangeY, rangeZ);
		float u = ((y - minY) / maxRange) * 0.5;
		float vcoord = ((z - minZ) / maxRange) * 0.5;

		uvs[v * 2 + 1] = u;
		uvs[v * 2 + 0] = vcoord;
    }

    return uvs;
}

float	*generateUVs(float *vertices, int nb_vertices)
{
    float *uvs = ft_calloc(sizeof(float), nb_vertices * 2);
    if (!uvs)
		return NULL;

	float minY = vertices[1], maxY = vertices[1];

    for (int i = 3; i < nb_vertices * 3; i += 3)
	{
        if (vertices[i + 1] < minY)
			minY = vertices[i];
        if (vertices[i + 1] > maxY)
			maxY = vertices[i];
    }

	for (int v = 0; v < nb_vertices; v++)
	{
		float x = vertices[v * 3];
		float y = vertices[v * 3 + 1];
		float z = vertices[v * 3 + 2];
		float ny = 2.0 * (y - minY) / (maxY - minY) - 1.5;
		uvs[v * 2 + 0] = (0.5 + atan2(z, x) / (2 * PI));
		uvs[v * 2 + 1] = (0.5 + asin(ny) / PI);
    }

    return uvs;
}

void	init_buffers(t_data *data, GLuint *vao, GLuint *vbo, GLuint *vboUVp, GLuint *vboUVs, GLuint *ebo)
{
	if (!data->obj->vertex->uvp)
		data->obj->vertex->uvp = generateUVp(data->obj->vertex->co, data->obj->vertex->nb_vertex);
	if (!data->obj->vertex->uvp)
		return ; // à protéger
	data->obj->vertex->uvs = generateUVs(data->obj->vertex->co, data->obj->vertex->nb_vertex);
	if (!data->obj->vertex->uvs)
		return ; // à protéger
	glGenVertexArrays(1, vao);
	glGenBuffers(1, vbo);
	glGenBuffers(1, vboUVp);
	glGenBuffers(1, vboUVs);
	glGenBuffers(1, ebo);

	glBindVertexArray(*vao);

	// buffer des sommets
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, data->obj->vertex->nb_vertex * 3 * sizeof(float), data->obj->vertex->co, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, *vboUVp);
	glBufferData(GL_ARRAY_BUFFER, data->obj->vertex->nb_vertex * 2 * sizeof(float), data->obj->vertex->uvp, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, *vboUVs);
	glBufferData(GL_ARRAY_BUFFER, data->obj->vertex->nb_vertex * 2 * sizeof(float), data->obj->vertex->uvs, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	//buffer des indices (faces)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->obj->faces->nb_faces * sizeof(GLuint), data->obj->faces->faces, GL_STATIC_DRAW);

	glBindVertexArray(0);
	free(data->obj->vertex->uvp);
	free(data->obj->vertex->uvs);
}

void	free_data(t_data *data)
{
	t_vertex *v = data->obj->vertex;
	t_faces *f = data->obj->faces;
	if (v->co)
		free(v->co);
	if (v->n)
		free(v->n);
	if (v->uvp)
		free(v->uvp);
	if (v->uvs)
		free(v->uvs);
	if (f->faces)
		free(f->faces);
	if (f->n)
		free(f->n);
	if (f->uvp)
		free(f->uvp);
}

int	parse_and_init(t_data *data, int ac, char **av, GLFWwindow **window, GLuint *shaderProgram)
{
	if (ac != 2)
        return 1;
	if (!parsing(data, av[1]))
	{
		free_data(data);
		return 2;
	}
	if (!initiate_window(window, av[1]))
	{
		free_data(data);
		return 3;
	}
	if (!initiate_shaders(shaderProgram))
	{
		free_data(data);
		glfwDestroyWindow(*window);
		glfwTerminate();
		return 4;
	}
	
	return 0;
}

void	fill_text(unsigned char **text, unsigned int *width, unsigned int *height)
{
	unsigned int imageSize; 

	int fd = open("cats.bmp", O_RDONLY);
    if (fd == -1)
        return;

    unsigned char header[54];
    if (read(fd, header, 54) != 54)
    {
        close(fd);
        return;
    }

    *width  = *(int*)&header[0x12];
    *height = *(int*)&header[0x16];

    unsigned int offset = *(int*)&header[0x0A];
    imageSize = *width * *height * 3;

    *text = ft_calloc(imageSize, 1);
    if (!*text)
    {
        close(fd);
        return;
    }

    lseek(fd, offset, SEEK_SET);

    if (read(fd, *text, imageSize) != (ssize_t)imageSize)
    {
        free(*text);
        *text = NULL;
    }

    close(fd);
}

GLuint load_text()
{
	unsigned char	*texture = NULL;
	unsigned int	width, height;
	GLuint			textureID;

	fill_text(&texture, &width, &height);
	if (!texture)
		return 0;
	glGenTextures(1, &textureID); 
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, texture);
	glGenerateMipmap(GL_TEXTURE_2D); 
	free(texture);
	return textureID;
}

void	manage_keys(t_data *data, GLFWwindow *window)
{
	static int pressed = 0, pressed2 = 0, uv = 0, tex = 0;
	static int pressed3 = 0, tri = 0;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
			pressed = 1;
	else
		pressed = uv = 0;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
			pressed2 = 1;
	else
		pressed2 = tex = 0;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			pressed3 = 1;
	else
		pressed3 = tri = 0;
	if (pressed && !uv)
	{
		if (data->obj->which_uv)
			data->obj->which_uv = 0;
		else
			data->obj->which_uv = 1;
		uv = 1;
	}
	if (pressed2 && !tex)
	{
		if (data->obj->tex)
			data->obj->tex = 0;
		else
			data->obj->tex = 1;
		tex = 1;
	}
	if (pressed3 && !tri)
	{
		if (data->obj->tri)
			data->obj->tri = 0;
		else
			data->obj->tri = 1;
		tri = 1;
	}
}

int main(int ac, char **av)
{
	t_data data;
	GLFWwindow* window = NULL;
	GLuint shaderProgram = 0;
	GLuint vao, vbo, vboUVp, vboUVs, ebo;
	GLuint textureID;

	data.obj = &(t_obj){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	data.obj->faces = &(t_faces){0, 0, 0, 0};
    data.obj->vertex = &(t_vertex){0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int a = parse_and_init(&data, ac, av, &window, &shaderProgram);
	if (a)
		return (a);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	textureID= load_text();
	init_buffers(&data, &vao, &vbo, &vboUVp, &vboUVs, &ebo);
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 )
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram);
		manage_keys(&data, window);
		create_and_bind_matrices(&data, window, shaderProgram);
		glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, textureID);
    	glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);
		glBindVertexArray(vao);
		if (!data.obj->tri)
			glDrawElements(GL_TRIANGLES, data.obj->faces->nb_faces, GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_POINTS, 0, data.obj->faces->nb_faces);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	free(data.obj->vertex->co);
	free(data.obj->faces->faces);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(shaderProgram);
	glfwDestroyWindow(window);
	glfwTerminate();
	return (0);
}
