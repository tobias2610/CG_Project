#version 120

// vertex attributes
attribute vec3 position;
attribute vec3 normal;
attribute vec2 texcoord;

// matrix
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

varying vec3 norm;
varying vec2 tc;
varying vec4 epos;

void main()
{
	vec4 wpos = modelMatrix * vec4(position, 1.0);
	epos = viewMatrix * wpos;
	gl_Position = projectionMatrix * epos;

	// pass normal to fragment shader
    norm = normalize(mat3(viewMatrix*modelMatrix)*normal);
	tc = texcoord;
}
