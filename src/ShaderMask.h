#pragma once

#include "ofMain.h"

class ShaderMask
{

	public:

		void setup			(int width, int height);
		
		void setMaskBegin	();
		void setMaskEnd		();
		
		void drawBegin		();
		void drawEnd		();

		void draw			();
		void drawMask		();

	private:

		int			width;
		int			height;

		ofShader	shaderMask;
		ofFbo		fboMask;
		ofFbo		fboDraw;
		ofFbo		fboOut;

};