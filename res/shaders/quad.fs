varying vec2 v_uv;
uniform float u_aspect_ratio;
uniform int ex;
uniform int prob;

void main()
{	
	if (ex == 1) {
		if (prob == 1){
			vec3 c1 = mix(vec3(0.0, 0.0, 1.0), vec3(1.0, 0.0, 0.0), v_uv.x);
			gl_FragColor = vec4(c1, 1.0);
		}
		if (prob == 2){
			vec3 c2 = vec3(distance(vec2(0.5*u_aspect_ratio,0.5*u_aspect_ratio), v_uv));
			gl_FragColor = vec4(c2, 1.0);
		}
		
	}
	

	
}
