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
		
		if (prob == 3){
			vec3 c3 = vec3(0.0, 0.0, 1.0)*step(0.8,(cos(v_uv.y/0.01)))*u_aspect_ratio + vec3(1.0, 0.0, 0.0)*step(0.8,(sin(v_uv.x/0.01)*u_aspect_ratio));
			gl_FragColor = vec4(c3, 1.0);

		}

		if (prob == 4){
			//vec3 c4 = vec3(0.0, 0.0, 1.0)*floor(v_uv.x/4) + vec3(1.0, 0.0, 0.0)*floor(v_uv.y/4);
			//gl_FragColor = vec4(c4, 1.0);
			float pixelSize = 20.0;
    
			// Redondea las coordenadas de textura
			vec2 roundedUV = vec2(floor(v_uv.x * pixelSize) / pixelSize, floor(v_uv.y * pixelSize) / pixelSize);
			
			// Calcula el color pixelado
			vec3 c4 = vec3(1.0, 0.0, 0.0) * roundedUV.x + vec3(0.0, 1.0, 0.0) * roundedUV.y;
			
			gl_FragColor = vec4(c4, 1.0);

		}

	}
	

	
}
