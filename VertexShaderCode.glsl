#version 130

/*"in layout(location=0) vec2 position;"*/
attribute vec3 vPosition;
attribute vec3 vertexColor;

uniform mat4 modelTransformMatrix;
uniform mat4 projectionMatrix;

out vec3 theColor;
void main()
{
	vec4 v = vec4(vPosition ,1.0 );
	vec4 newPosition = modelTransformMatrix * v;
	vec4 projectedPosition = projectionMatrix * newPosition;

	gl_Position = projectedPosition;
	theColor = vertexColor;
}
