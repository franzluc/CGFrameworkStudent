varying vec2 v_uv;

uniform float u_aspect_ratio;
uniform int ex;
uniform int prob;
uniform float u_heigth;
uniform float u_width;
uniform sampler2D u_texture;

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
			float p = 25.0;
    		
			vec2 rd = vec2((floor(v_uv.x * p)/p), (floor(v_uv.y * p)/p));
			vec3 c4 = vec3(1.0, 0.0, 0.0) * rd.x *  u_aspect_ratio + vec3(0.0, 1.0, 0.0) * rd.y * u_aspect_ratio;
			
			gl_FragColor = vec4(c4, 1.0);

		}

	}

	if (ex == 2){
		if (prob == 0){
			
			vec4 t0 = texture2D(u_texture,  v_uv);
			gl_FragColor = t0;

		}

		if (prob == 1){

			vec3 t1 = texture2D(u_texture, v_uv).xyz;
			gl_FragColor = vec4((t1.x + t1.y + t1.z)/3.0, (t1.x + t1.y + t1.z)/3.0, (t1.x + t1.y + t1.z)/3.0, 1.0); 
		}

		if (prob == 2){

			vec3 t1 = texture2D(u_texture, v_uv).xyz;
			gl_FragColor = vec4(1.0 - t1.x, 1.0 - t1.y, 1.0 - t1.z, 1.0); 
		}

		if (prob == 3){

			vec3 t1 = texture2D(u_texture, v_uv).xyz * vec3(1.0, 1.0, 0.0);
			gl_FragColor = vec4(t1.x, t1.y, t1.z, 1.0); 
		}

		if (prob == 4){

			vec3 t1 = texture2D(u_texture, v_uv).xyz;
			float sat = step(0.6, (t1.x + t1.y + t1.z)/3.0);

			gl_FragColor = vec4(sat, sat, sat, 1.0);  
		}

		if (prob == 5){
			
			vec3 ct = vec3(distance(vec2(0.5*u_aspect_ratio,0.5*u_aspect_ratio), v_uv));

			vec3 t1 = texture2D(u_texture,  v_uv).xyz * vec3(1.0 - ct.x, 1.0 - ct.y, 1.0 - ct.z);
			gl_FragColor = vec4(t1, 1.0);

		}

		if (prob == 6){
			
			vec2 px = vec2(1.0/u_width, 1.0/u_heigth);

			vec4 tf = (texture2D(u_texture, v_uv - 2.0*px) +
					   texture2D(u_texture, v_uv + 2.0*px) + 
			           texture2D(u_texture, v_uv - vec2(2.0*px.x, 0.0)) + 
					   texture2D(u_texture, v_uv + vec2(2.0*px.x, 0.0)) +
					   texture2D(u_texture, v_uv + vec2(0.0, 2.0*px.y)) +
					   texture2D(u_texture, v_uv - vec2(0.0, 2.0*px.y)) +
					   texture2D(u_texture, v_uv + vec2(2.0*px.x, - 2.0*px.y)) +
					   texture2D(u_texture, v_uv - vec2(2.0*px.x, - 2.0*px.y))) / 8.0;
					   

			gl_FragColor = tf;
					  


		}
		 

		









	}
	

	
}
