varying vec2 v_uv;

uniform float u_aspect_ratio;
uniform int ex;
uniform int prob;
uniform float u_heigth;
uniform float u_width;
uniform float time;
uniform sampler2D u_texture;


void main()
{	
	if (ex == 1) {
		if (prob == 1){ // Mezclamos los colores en funcion del eje x
			vec3 c1 = mix(vec3(0.0, 0.0, 1.0), vec3(1.0, 0.0, 0.0), v_uv.x);
			gl_FragColor = vec4(c1, 1.0);
		}
		if (prob == 2){ // Atribuimos un color en funcion de la distancia con el punto (0.5, 0.5)
			vec3 c2 = vec3(distance(vec2(0.5*u_aspect_ratio, 0.5), v_uv));
			gl_FragColor = vec4(c2, 1.0);
		}
		
		if (prob == 3){ // Utilizamos la función step para pintar solo los valores para los cuales el coseno y seno es mayor que 0.8 
			vec3 c3 = vec3(1.0, 0.0, 0.0)*step(0.8,(cos(v_uv.x/0.01)))*u_aspect_ratio + vec3(0.0, 0.0, 1.0)*step(0.8,(sin(v_uv.y/0.01)));
			gl_FragColor = vec4(c3, 1.0);

		}

		if (prob == 4){
			float p = 20.0; 
    		
			vec2 rd = vec2((floor(v_uv.x * p)/p), (floor(v_uv.y * p)/p)); // Redondeamos todos los pixeles que se encuentran entre la normalización en funcion de p

			vec3 c4 = vec3(1.0, 0.0, 0.0) * rd.x *  u_aspect_ratio + vec3(0.0, 1.0, 0.0) * rd.y; // Multiplicamos este redondeo por nuestros colores
			
			gl_FragColor = vec4(c4, 1.0);

		}

		if (prob == 5){
			float p = 20.0;
    		
			vec2 rd = vec2((floor(v_uv.x*u_aspect_ratio * p)), (floor(v_uv.y * p))); // Realizamos la misma operación que antes sin normalizar

			float blanco = mod((floor(rd.x+rd.y)), 2.0); // El modulo será 0 o 1, lo cual si se lo restamos a 0 o 1 nos dará blanco o negro 
			vec3 c5 = vec3(1.0 - blanco, 1.0 - blanco, 1.0-blanco);
			
			gl_FragColor = vec4(c5, 1.0);


		}

		if (prob == 6){
			
			float y = sin(3.14*v_uv.x*u_aspect_ratio*2.0)*0.3 + 0.5; // Establecemos la función seno que nos delimitará la división y ajustamos parámetros para
																	 //	que se parezca lo máximo posible a la imagen dada.

			vec3 c6 = vec3(mix(vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 0.0), v_uv.y))*step(y, v_uv.y) + vec3(mix(vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), v_uv.y))*floor(cos(step(y, v_uv.y)));
			// A través de la función step, los valores mayores a la función serán un degradado, y los menores otro.

			gl_FragColor = vec4(c6, 1.0);
		}

	}

	if (ex == 2){
		if (prob == 0){
			
			vec4 t0 = texture2D(u_texture,  v_uv); // Caso inicial donde cargamos la textura
			gl_FragColor = t0;

		}

		if (prob == 1){

			vec3 t1 = texture2D(u_texture, v_uv).xyz; // Realizamos la media entre los colores para obtener blanco y negro
			gl_FragColor = vec4((t1.x + t1.y + t1.z)/3.0, (t1.x + t1.y + t1.z)/3.0, (t1.x + t1.y + t1.z)/3.0, 1.0); 
		}

		if (prob == 2){

			vec3 t1 = texture2D(u_texture, v_uv).xyz; // Restamos el valor a 1 para obtener el negativo
			gl_FragColor = vec4(1.0 - t1.x, 1.0 - t1.y, 1.0 - t1.z, 1.0); 
		}

		if (prob == 3){

			vec3 t1 = texture2D(u_texture, v_uv).xyz * vec3(1.0, 1.0, 0.0); // Multiplicamos nuestro valor por el valor del amarillo para tener un filtro amarillo
			gl_FragColor = vec4(t1.x, t1.y, t1.z, 1.0); 
		}

		if (prob == 4){

			vec3 t1 = texture2D(u_texture, v_uv).xyz; // Utilizamos la función step para que no haya escala de grises
			float sat = step(0.6, (t1.x + t1.y + t1.z)/3.0);

			gl_FragColor = vec4(sat, sat, sat, 1.0);  
		}

		if (prob == 5){
			
			vec3 ct = vec3(distance(vec2(0.5*u_aspect_ratio,0.5), v_uv)); // Reutilizamos el código del 1.b para este apartado, sólo que invirtiendo el efecto

			vec3 t1 = texture2D(u_texture,  v_uv).xyz * vec3(1.0 - ct.x, 1.0 - ct.y, 1.0 - ct.z);
			gl_FragColor = vec4(t1, 1.0);

		}

		if (prob == 6){ // Calculamos la cantidad de pixeles horizontal y vertical

			
			float ph = 3.0/u_width; //Multiplicamos por 3 para que se vea mas borroso.
			float pv = 3.0/u_heigth;

			// Calculamos los colores de los pixeles vecinos y calculamos la media 
			vec4 tf = (texture2D(u_texture, v_uv - vec2(ph, pv)) +
					   texture2D(u_texture, v_uv + vec2(ph, pv)) + 
			           texture2D(u_texture, v_uv - vec2(ph, 0.0)) + 
					   texture2D(u_texture, v_uv + vec2(ph, 0.0)) +
					   texture2D(u_texture, v_uv + vec2(0.0, pv)) +
					   texture2D(u_texture, v_uv - vec2(0.0, pv)) +
					   texture2D(u_texture, v_uv + vec2(ph, - pv)) +
					   texture2D(u_texture, v_uv - vec2(ph, - pv))) / 8.0;
					   

			gl_FragColor = tf;
		
		}

	}	

	if ( ex == 3){
		if (prob == 1){
			// A través de una función seno en función del tiempo, se va poniendo cada vez más borrosa, para despues volver al estado original 
			float ph = sin(time)*200.0/u_width;
			float pv = sin(time)*200.0/u_heigth;

			vec4 tf = (texture2D(u_texture, v_uv - vec2(ph, pv)) +
					   texture2D(u_texture, v_uv + vec2(ph, pv)) + 
			           texture2D(u_texture, v_uv - vec2(ph, 0.0)) + 
					   texture2D(u_texture, v_uv + vec2(ph, 0.0)) +
					   texture2D(u_texture, v_uv + vec2(0.0, pv)) +
					   texture2D(u_texture, v_uv - vec2(0.0, pv)) +
					   texture2D(u_texture, v_uv + vec2(ph, - pv)) +
					   texture2D(u_texture, v_uv - vec2(ph, - pv))) / 8.0;
					   

			gl_FragColor = tf;


		}

		if (prob == 2){
			float p = sin(time)*250.0; // Volvemos a usar la función seno, esta vez para realizar una pixelación igual que en el apartado 1d 
    		
			vec2 rd = vec2((floor(v_uv.x * p)/p), (floor(v_uv.y * p)/p));
			
			vec3 c5 = texture2D(u_texture, rd).xyz;

			

			
			gl_FragColor = vec4(c5, 1.0);

		}

		if (prob == 3){
			vec2 m = vec2(v_uv.x, cos(3.0*time)*v_uv.y); // Deformamos la imagen en el momento de atribuir colores a las coordenadas. Se modifican con una funcion coseno.

			vec3 color = texture2D(u_texture, m).xyz;

			gl_FragColor = vec4(color, 1.0);
		}


	}	
		 

		









	
	

	
}
