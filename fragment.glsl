#version 330

out vec4 FragColor;

in vec3 KolorOut;

void main()
{
	FragColor = vec4(KolorOut, 1.0); 
//	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}

