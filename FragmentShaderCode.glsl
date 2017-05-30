#version 330

out vec4 daColor;

in vec3 theColor;

void main()
{
/*"	daColor = vec4(0.0, 1.0 ,0.0 ,1.0);"*/
	daColor = vec4(theColor,1.0);
/*"	gl_FragColor = vec4(theColor ,1.0);"*/
}
