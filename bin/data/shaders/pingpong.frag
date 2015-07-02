#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable
//uniform vec2 resolution;
uniform sampler2DRect tex0;
//uniform int kernelSize;
uniform float alphaDamping;
//uniform int velX;
//uniform int velY;

float normpdf(in float x, in float sigma)
{
	return 0.39894*exp(-0.5*x*x/(sigma*sigma))/sigma;
}

void main(void)
{
	vec4 c = texture2DRect(tex0, gl_FragCoord.xy).rgba;

	//declare stuff
		//int mSize = 11;
		//if (kernelSize != 0)

/*				
			int	mSize = kernelSize;
			int kSize = (mSize-1)/2;
			float kernel[50];
			vec3 final_colour = vec3(0.0);
			
			//create the 1-D kernel
			float sigma = 7.0;
			float Z = 0.0;
			for (int j = 0; j <= kSize; ++j)
			{
				kernel[kSize+j] = kernel[kSize-j] = normpdf(float(j), sigma);
			}
			
			//get the normalization factor (as the gaussian has been clamped)
			for (int j = 0; j < mSize; ++j)
			{
				Z += kernel[j];
			}
			//read out the texels
			for (int i=-kSize; i <= kSize; ++i)
			{
				for (int j=-kSize; j <= kSize; ++j)
				{
					final_colour += kernel[kSize+j]*kernel[kSize+i]*texture2DRect(tex0, (gl_FragCoord.xy+vec2(float(i + velX),float(j + velY)))).rgb;
		
				}
			}
			
*/			
			//gl_FragColor = vec4(final_colour/(Z*Z) * alphaDamping, 1.0);
			//gl_FragColor = vec4(final_colour/(Z*Z), alphaDamping);
			//gl_FragColor = vec4(final_colour/(Z*Z), 1.0);

			//gl_FragColor = vec4( (final_colour/(Z*Z)) * alphaDamping , c.a);
			gl_FragColor = vec4( c.rgb * alphaDamping , c.a);

}