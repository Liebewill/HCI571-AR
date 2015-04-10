uniform float colorvalue;

void main()
{
	// gl_FragColor is the output variable
	// gl_Color the input that comes from the vertex shader program. 
	gl_FragColor = gl_Color;
	
	// This line replaces the output color red with the uniform variable colorvalue;
	gl_FragColor.r = colorvalue/100.0;
}