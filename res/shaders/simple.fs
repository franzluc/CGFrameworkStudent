// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
//varying vec3 v_world_normal;
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;
uniform sampler2D u_textureEntity;

void main()
{
	// Set the ouput color per pixel
	vec4 textureColor = texture2D(u_textureEntity, v_uv);

	gl_FragColor = textureColor;
}
