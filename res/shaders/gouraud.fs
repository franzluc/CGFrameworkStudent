// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec2 v_uv;
varying vec3 v_world_normal;
varying vec3 v_cv;
uniform sampler2D u_textureEntity;

void main()
{
	// Set the ouput color per pixel
	vec3 color = normalize(v_world_normal);
    
	gl_FragColor = vec4(v_cv, 1.0);	
}
