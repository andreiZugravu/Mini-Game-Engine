precision mediump float;

uniform sampler2D u_texture;

varying vec2 v_uv;

uniform vec3 u_camera_coord;
uniform float u_r;
uniform float u_R;
uniform vec3 u_fog_color;

varying vec3 v_position;

uniform vec3 u_ambiental_light_color;
uniform vec3 u_diffusal_light_color;

void main()
{
    vec4 color = texture2D(u_texture, v_uv);
    if(color.a < 0.5)
        discard;
    float alpha = clamp((distance(v_position, u_camera_coord) - u_r) / (u_R - u_r), 0.0, 1.0);
	
    vec4 comp_amb = color * vec4(u_ambiental_light_color, 1.0); //also use the ratio?
    vec4 comp_diff = color * vec4(u_diffusal_light_color, 1.0);

    gl_FragColor = comp_amb * (1.0 - alpha) + vec4(u_fog_color * alpha, 1.0);
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    //gl_FragColor = vec4(v_uv.x, v_uv.y, 0.0, 1.0);
}
