#version 330

/*"in layout(location=0) vec2 position;"*/
attribute vec2 position;
attribute float offset;

/*attribute vec3 vPosition;*/
/*attribute vec3 vertexColor;*/
/*uniform mat4 fullTransformMatrix;*/

out vec3 theColor;
void main()
{
	/*vec4 v = vec4(vPosition ,1.0 );*/
	/*gl_Position = fullTransformMatrix * v;*/

	gl_Position = vec4(position.x+offset, position.y, 0, 1);

	/*theColor = vertexColor;*/
	theColor = vec3(1,0,0);
}
