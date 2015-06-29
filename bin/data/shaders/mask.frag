#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect tex0;
uniform sampler2DRect mask;

void main(void)
{
	vec2 pos = gl_FragCoord.xy;
	vec3 mask = texture2DRect(mask, pos).xyz;
	vec4 tex = texture2DRect(tex0, pos);
	
	if ((mask.x + mask.y + mask.z) == 0)
		gl_FragColor = vec4(0,0,0,1.0);
	else
		gl_FragColor = tex;
}
