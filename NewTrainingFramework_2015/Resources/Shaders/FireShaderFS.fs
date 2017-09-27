precision mediump float;

uniform sampler2D u_disp;
uniform sampler2D u_texture_fire;
uniform sampler2D u_texture_fire_mask;
uniform float u_disp_max;

varying vec2 v_uv;

uniform vec3 u_camera_coord;
uniform float u_r;
uniform float u_R;
uniform vec3 u_fog_color;

varying vec3 v_position;

uniform float u_time;

void main()
{
    vec2 disp = texture2D(u_disp, vec2(v_uv.x, v_uv.y + u_time)).rg;

    vec2 offset = 2.0 * u_disp_max * disp - vec2(u_disp_max);

    vec2 v_uv_displaced = v_uv + offset;

    vec4 color_fire  = texture2D(u_texture_fire, v_uv_displaced);
    vec4 color_alpha = texture2D(u_texture_fire_mask, v_uv);
    
    color_fire.a *= color_alpha.r;

    //if(color.a < 0.5)
    //    discard;

    float alpha = clamp((distance(v_position, u_camera_coord) - u_r) / (u_R - u_r), 0.0, 1.0);
	
    gl_FragColor = color_fire * (1.0 - alpha) + vec4(u_fog_color, 1.0) * alpha;
    gl_FragColor.a = color_fire.a;
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    //gl_FragColor = vec4(v_uv.x, v_uv.y, 0.0, 1.0);
}
