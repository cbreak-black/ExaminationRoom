uniform sampler2D tex;
uniform float side;

void main()
{
	// Assigns 0 or 1 to lines depending on if they are rendered
	float f = mod(floor(gl_FragCoord.y+side),2.0);
	if (f < 0.50)
		discard;

	gl_FragColor = gl_Color * texture2D(tex, gl_TexCoord[0].xy);
}
