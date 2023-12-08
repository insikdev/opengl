#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec2 texCoord;
out vec3 posWorld;

void main() {
	gl_Position = projection * view * world * vec4(aPos, 1.0);

	normal = (transpose(inverse(world)) * vec4(aNormal, 0.0)).xyz;
	texCoord = aTexCoord;
	posWorld = (world * vec4(aPos, 1.0)).xyz;
}