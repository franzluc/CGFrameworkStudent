// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec2 v_uv;
varying vec3 v_world_normal;
uniform sampler2D u_textureEntity;

void main()
{
	// Set the ouput color per pixel
	vec3 color = normalize(v_world_normal);
    
	// Cargamos los colores de la entidad en función de las coordenadas.
	vec4 t0 = texture2D(u_textureEntity,  v_uv);
	gl_FragColor = t0;
}
