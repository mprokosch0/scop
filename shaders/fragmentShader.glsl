#version 330 core

//flat in int faceID;
in vec2 TexCoords;
uniform sampler2D ourTexture;
out vec3 FragColor;

void main()
{
    FragColor = texture(ourTexture, TexCoords).rgb;
	//if (faceID % 3 == 0)
    //    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    //else if (faceID % 3 == 1)
    //    FragColor = vec4(0.6, 0.6, 0.6, 1.0);
    //else
    //    FragColor = vec4(0.3, 0.3, 0.3, 1.0);
}
