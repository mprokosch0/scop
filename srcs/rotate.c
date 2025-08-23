
#include "scop.h"

void	lookAt(float *mat, float eyeX, float eyeY, float eyeZ,
                        float centerX, float centerY, float centerZ,
                        float upX, float upY, float upZ)
{
    float f[3] = {
        centerX - eyeX,
        centerY - eyeY,
        centerZ - eyeZ
    };
    float f_len = sqrtf(f[0]*f[0] + f[1]*f[1] + f[2]*f[2]);
    f[0] /= f_len; f[1] /= f_len; f[2] /= f_len;

    float up[3] = {upX, upY, upZ};
    float up_len = sqrtf(up[0]*up[0] + up[1]*up[1] + up[2]*up[2]);
    up[0] /= up_len; up[1] /= up_len; up[2] /= up_len;

    float s[3] = {
        f[1]*up[2] - f[2]*up[1],
        f[2]*up[0] - f[0]*up[2],
        f[0]*up[1] - f[1]*up[0]
    };
    float s_len = sqrtf(s[0]*s[0] + s[1]*s[1] + s[2]*s[2]);
    s[0] /= s_len; s[1] /= s_len; s[2] /= s_len;

    float u[3] = {
        s[1]*f[2] - s[2]*f[1],
        s[2]*f[0] - s[0]*f[2],
        s[0]*f[1] - s[1]*f[0]
    };

    mat[0] = s[0]; mat[1] = u[0]; mat[2] = -f[0]; mat[3] = 0;
    mat[4] = s[1]; mat[5] = u[1]; mat[6] = -f[1]; mat[7] = 0;
    mat[8] = s[2]; mat[9] = u[2]; mat[10] = -f[2]; mat[11] = 0;
    mat[12] = -(s[0]*eyeX + s[1]*eyeY + s[2]*eyeZ);
    mat[13] = -(u[0]*eyeX + u[1]*eyeY + u[2]*eyeZ);
    mat[14] =  (f[0]*eyeX + f[1]*eyeY + f[2]*eyeZ);
    mat[15] = 1;
}

void center_obj(t_vertex *v, float *mat)
{
    mat[0] = 1; mat[1] = 0; mat[2] = 0; mat[3] = 0;
    mat[4] = 0; mat[5] = 1; mat[6] = 0; mat[7] = 0;
    mat[8] = 0; mat[9] = 0; mat[10] = 1; mat[11] = 0;
    mat[12] = -v->cx;
    mat[13] = -v->cy;
    mat[14] = -v->cz;
    mat[15] = 1;
}

void translate_obj(t_obj *b, float *mat, GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		b->ty += 0.02;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		b->ty -= 0.02;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		b->tx -= 0.02;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		b->tx += 0.02;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		b->tz -= 0.05;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		b->tz += 0.05;
    mat[0] = 1; mat[1] = 0; mat[2] = 0; mat[3] = 0;
    mat[4] = 0; mat[5] = 1; mat[6] = 0; mat[7] = 0;
    mat[8] = 0; mat[9] = 0; mat[10] = 1; mat[11] = 0;
    mat[12] = b->tx;
    mat[13] = b->ty;
    mat[14] = b->tz;
    mat[15] = 1;
}

void	project_points(float *mat, GLFWwindow *window)
{
	float	ratio = (float)WIDTH / (float)HEIGHT;
	float	fov = 45.0f * (PI / 180);
	float	near = 0.1f;
	float	far = 100.0f;
	float t = tanf(fov / 2.0f);

	(void)window;
	mat[0]  = 1 / (ratio * t); mat[1]  = 0;					 mat[2]  = 0; 							   mat[3]  = 0;
	mat[4]  = 0;    		   mat[5]  = 1 / t;				 mat[6]  = 0;							   mat[7]  = 0;
	mat[8]  = 0;			   mat[9]  = 0;					 mat[10] = -(far + near) / (far - near);   mat[11] = -1;
	mat[12] = 0;    		   mat[13] = 0; 		    	 mat[14] = -2 * far * near / (far - near); mat[15] = 0;
}

void	rotate_y(float *mat, float *angle, GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		*angle -= 0.02;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		*angle += 0.02;
	float cosA = cosf(*angle);
	float sinA = sinf(*angle);
	mat[0]  = cosA; mat[1]  = 0; mat[2]  = -sinA; mat[3]  = 0;
	mat[4]  = 0;    mat[5]  = 1; mat[6]  = 0;     mat[7]  = 0;
	mat[8]  = sinA; mat[9]  = 0; mat[10] = cosA;  mat[11] = 0;
	mat[12] = 0;    mat[13] = 0; mat[14] = 0;     mat[15] = 1;
}

void	rotate_x(float *mat, float *angle, GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		*angle += 0.02;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		*angle -= 0.02;
	float cosA = cosf(*angle);
	float sinA = sinf(*angle);
	mat[0]  = 1; mat[1]  = 0;    mat[2]  = 0;	  mat[3]  = 0;
	mat[4]  = 0; mat[5]  = cosA; mat[6]  = -sinA; mat[7]  = 0;
	mat[8]  = 0; mat[9]  = sinA; mat[10] = cosA;  mat[11] = 0;
	mat[12] = 0; mat[13] = 0;	 mat[14] = 0;     mat[15] = 1;
}

void	rotate_z(float *mat, float *angle, GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		*angle += 0.02;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		*angle -= 0.02;
	float cosA = cosf(*angle);
	float sinA = sinf(*angle);
	mat[0]  = cosA; mat[1]  = -sinA; mat[2]  = 0; mat[3]  = 0;
	mat[4]  = sinA; mat[5]  = cosA;  mat[6]  = 0; mat[7]  = 0;
	mat[8]  = 0;	mat[9]  = 0;	 mat[10] = 1;  mat[11] = 0;
	mat[12] = 0;	mat[13] = 0;	 mat[14] = 0;     mat[15] = 1;
}
