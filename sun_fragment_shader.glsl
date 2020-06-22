#version 400

out vec4 fragColor;

uniform sampler2D texture1;
uniform vec4 lightColor;

void main()
{
    //fragColor = lightColor;
	fragColor = vec4(0.9f, 0.9f, 0.0f,1.0f);
}