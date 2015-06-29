#include "ShaderMask.h"

void ShaderMask::setup(int width, int height)
{
	this->width = width;
	this->height = height;

	fboMask.allocate(width, height);
	fboDraw.allocate(width, height);
	fboOut.allocate(width, height);

	fboMask.begin();
		ofClear(0,0,0,1);
	fboMask.end();

	fboDraw.begin();
		ofClear(0,0,0,1);
	fboDraw.end();
	
	fboOut.begin();
		ofClear(0,0,0,1);
	fboOut.end();

	shaderMask.load("shaders/mask");
	
}

void ShaderMask::setMaskBegin()
{
	fboMask.begin();
		ofClear(0,1);
}

void ShaderMask::setMaskEnd()
{
	fboMask.end();
}

void ShaderMask::drawBegin()
{
	fboDraw.begin();
		ofClear(0,0,0,1);
}

void ShaderMask::drawEnd()
{
	fboDraw.end();
}

void ShaderMask::draw()
{

	fboOut.begin();
		ofClear(0,0,0,1);
		shaderMask.begin();
			shaderMask.setUniformTexture("tex0", fboDraw.getTextureReference(), 0);
			shaderMask.setUniformTexture("mask", fboMask.getTextureReference(), 1);
			fboDraw.draw(0,0);
		shaderMask.end();
	fboOut.end();

	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	//ofDisableAlphaBlending();
	fboOut.draw(0,0);
	
	//fboMask.draw(width/2,0,width/2,height/2);
}

void ShaderMask::drawMask()
{
	ofDisableAlphaBlending();
	ofSetColor(255);
	fboMask.draw(width/2,0,width/2,height/2);
}