#version 330 core

flat in int faceID;
out vec4 FragColor;

void main()
{
	if (faceID % 3 == 0)
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    else if (faceID % 3 == 1)
        FragColor = vec4(0.6, 0.6, 0.6, 1.0);
    else
        FragColor = vec4(0.3, 0.3, 0.3, 1.0);
}