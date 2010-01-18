uniform sampler2D tex;
uniform float side;

// Factor that determines how much of the other
// colors is mixed into the primary channel of that
// side. This is the same lambda as in the mayan paper.
uniform float lambda;

void main()
{
	float facR = 1.0-side;
	float facG = side;

	vec4 c = texture2D(tex, gl_TexCoord[0].xy);
	gl_FragColor = vec4(
		facR*mix(c.r, (c.g+c.b)*0.5, lambda), // Red
		facG*mix(c.g, (c.r+c.b)*0.5, lambda), // Green
		c.b*0.5, // Blue
		0.5); // Alpha
}
