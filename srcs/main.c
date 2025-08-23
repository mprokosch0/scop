
#include "scop.h"

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	*window = glfwCreateWindow(WIDTH, HEIGHT, str, NULL, NULL);
	if(*window == NULL)
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(*window);
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
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

int initiate_shaders(GLuint *shaderProgram, char **vertexSrc, char **fragmentSrc)
{
	oneLine(vertexSrc, "shaders/vertexShader.glsl");
	if (!(*vertexSrc))
		return (0);
	oneLine(fragmentSrc, "shaders/fragmentShader.glsl");
	if (!(*fragmentSrc))
	{
		free(*vertexSrc);
		return 0;
	}
	*shaderProgram = createShaderProgram(*vertexSrc, *fragmentSrc);
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
}

int main(int ac, char **av)
{
	t_data data;
	GLFWwindow* window = NULL;
	GLuint shaderProgram;
	char *vertexSrc = NULL;
	char *fragmentSrc = NULL;

	if (ac != 2)
        return 1;
	data.obj = &(t_obj){0, 0, 0, 0, 0, 0, 0, 0};
	data.obj->faces = &(t_faces){0, 0};
    data.obj->vertex = &(t_vertex){0, 0, 0, 0, 0};
	if (!parsing(&data, av[1]))
		return 2;
	if (!initiate_window(&window, av[1]))
		return 3;
	if (!initiate_shaders(&shaderProgram, &vertexSrc, &fragmentSrc))
		return 4;

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint vao, vbo, ebo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	// buffer des sommets
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.obj->vertex->nb_vertex * 3 * sizeof(float), data.obj->vertex->co, GL_STATIC_DRAW);

	// buffer des indices (faces)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.obj->faces->nb_faces * sizeof(GLuint), data.obj->faces->faces, GL_STATIC_DRAW);

	// layout: ici 3 floats par sommet
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 )
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glEnable(GL_DEPTH_TEST);
		create_and_bind_matrices(&data, window, shaderProgram);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, data.obj->faces->nb_faces, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return (0);
}