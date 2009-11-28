uniform sampler2D tex;
uniform float side;

void main()
{
	vec4 c = texture2D(tex, gl_TexCoord[0].xy);
	gl_FragColor = vec4(c.r*(1.0-side), c.g*(side), c.b*0.5, 0.5);
}
