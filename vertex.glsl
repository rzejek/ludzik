#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Kolor;
layout (location = 2) in vec3 Normalna;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

uniform vec3 LightPozycja;

out vec3 KolorOut;

float ConstantAttenuation = .4;
float LinearAttenuation = 1.2;
float QuadraticAttenuation = 20.001;

void main()
{
	mat4 mvp = Projection*View*Model;
	
    gl_Position = mvp*vec4(Position, 1.0);

	vec3 TestPos = Position;
	TestPos = vec3(View*Model*vec4(Position, 1.0));
	vec3 LightPos = LightPozycja;
	LightPos = vec3(View*vec4(LightPozycja, 1.0));
	
	vec3 s = normalize(LightPos - TestPos);
	
	vec3 NormalOut;
	NormalOut = normalize( transpose(inverse(mat3(View*Model)) ) * Normalna );

    KolorOut = Kolor * max(dot(s, NormalOut), 0.0);
}

