#version 330 core
layout(location=0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec2 aTexSCoord;

uniform mat4 center;
uniform mat4 rotX;
uniform mat4 rotY;
uniform mat4 rotZ;
uniform mat4 projection;
uniform mat4 camera;
uniform mat4 translate;
uniform int which_uv;

flat out int faceID;
out vec2 TexCoords;

void main()
{
	
	mat4 model = translate * rotX * rotY * rotZ * center;
	gl_Position = projection * camera * model * vec4(aPos, 1.0);

	if (which_uv == 0)
		TexCoords = aTexCoord;
	else
		TexCoords = aTexSCoord;
	faceID = gl_VertexID / 3;
}