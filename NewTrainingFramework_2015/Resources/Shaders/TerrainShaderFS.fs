precision mediump float;

uniform sampler2D u_blend_map;

varying vec2 v_uv;
varying vec2 v_uv_map;

uniform sampler2D u_color_rock;
uniform sampler2D u_color_dirt;
uniform sampler2D u_color_grass;

uniform vec3 u_camera_coord;
uniform float u_r;
uniform float u_R;
uniform vec3 u_fog_color;

varying vec3 v_position;

void main()
{
    vec3 color = texture2D(u_blend_map, v_uv_map).rgb;
    //if(color.a < 0.5)
    //    discard;
	//gl_FragColor = vec4(color_test, 1.0);
    float alpha = clamp((distance(v_position, u_camera_coord) - u_r) / (u_R - u_r), 0.0, 1.0);
    vec3 groundColor = vec3(color.r * texture2D(u_color_rock, v_uv) + color.g * texture2D(u_color_grass, v_uv) + color.b * texture2D(u_color_dirt, v_uv));
    gl_FragColor = vec4(groundColor * (1.0 - alpha) + u_fog_color * alpha, 1.0);
    //gl_FragColor = vec4(v_uv.x, v_uv.y, 0.0, 1.0);
}
