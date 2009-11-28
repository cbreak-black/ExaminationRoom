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
	float mixFactor = (1.0-lambda)*0.5;

	vec4 c = texture2D(tex, gl_TexCoord[0].xy);
	gl_FragColor = vec4(
		facR*(c.r*lambda + (c.g+c.b)*mixFactor), // Red
		facG*(c.g*lambda + (c.r+c.b)*mixFactor), // Green
		c.b*0.5, // Blue
		0.5); // Alpha
}
