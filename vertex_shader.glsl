#version 400

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 texCoord;


out vec2 textureCoord;
out vec3 norm;
out vec3 fragPos;

uniform mat4 MVP;
uniform mat4 model;
uniform float timer;
uniform float amplitude;
uniform vec2 direction;
uniform float frequency;
uniform float phase;

vec3 newpos;
vec2 circular;
vec2 asd;

void main()
{	
	textureCoord = texCoord;
	newpos=pos;
	asd=vec2(newpos.x,newpos.y)-vec2(1,1);
	circular=(vec2(newpos.x,newpos.y)-vec2(1,1))/mod(asd.x,asd.y);
	//newpos.y=2*1.15*(((sin(2*dot(newpos.x,newpos.z)*2*3.14/1+timer*1.8*2*3.14/1)+1)/2)*((sin(2*dot(newpos.x,newpos.z)*2*3.14*1+timer*2*3.14*1.5)+1)/2));
	newpos.y=2*amplitude*(((sin(dot(circular,vec2(newpos.x,newpos.z))*frequency+timer*phase)+1)/2)*((sin(dot(circular,vec2(newpos.x,newpos.z))*frequency+timer*phase)+1)/2));

	fragPos = vec3(model * vec4(pos, 1.0f));
	//norm = normals;
	norm = mat3(transpose(inverse(model)))*normals;
	gl_Position = MVP * vec4(newpos, 1.0f);
}