precision mediump float;

uniform samplerCube u_cube_texture;

varying vec3 v_coord;

void main()
{
    vec4 color = textureCube(u_cube_texture, v_coord);
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    gl_FragColor = color;
    //gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    //gl_FragColor = vec4(v_uv.x, v_uv.y, 0.0, 1.0);
}
