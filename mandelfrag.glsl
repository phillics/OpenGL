#version 400

out vec4 frag_color;

void main() {
	
	vec2 z, c;
	float scale = .001;
	int itr = 100;

	c.x = (gl_FragCoord.x - 2.2 * 960) * scale;
	c.y = (gl_FragCoord.y - 540) * scale;
	z = c;

	float x, y;
	int i;
	for(i = 0; i < itr; i++){
		x = (z.x * z.x - z.y * z.y) + c.x;
		y = (z.y * z.x + z.x * z.y) + c.y;

		if((x * x + y * y) > 4.0) break;
		z.x = x;
		z.y = y;
	}

	if(i >= 50){i = 100-i;}

	frag_color = vec4(0.0, i / 50.0, 0.0, 1.0);
}