#version 130

/*"in layout(location=0) vec2 position;"*/
attribute vec3 vPosition;
attribute vec3 vertexColor;

uniform mat4 fullTransformMatrix;

out vec3 theColor;
void main()
{
	vec4 v = vec4(vPosition ,1.0 );
	gl_Position = fullTransformMatrix * v;

	theColor = vertexColor;
}
