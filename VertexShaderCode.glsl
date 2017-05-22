#version 130

/*"in layout(location=0) vec2 position;"*/
attribute vec3 vPosition;
attribute vec3 vertexColor;
uniform vec3 dominatingColor;
uniform float yFlip;

out vec3 theColor;
void main()
{
	gl_Position = vec4(vPosition ,1.0 );
	gl_Position.y = gl_Position.y * yFlip;
/*"gl_Position = vPosition;"*/
	theColor = dominatingColor;
}
