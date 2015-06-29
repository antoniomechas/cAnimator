#pragma once

#include "ofMain.h"
#include "ContourManager.h"
#include "ContourAnimator.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    protected:

        ContourManager contourManager;

        ofImage img1;
        ofImage img2;
        ofxCvColorImage colorImg1;
        ofxCvColorImage colorImg2;
        ofxCvGrayscaleImage contourImg1;
        ofxCvGrayscaleImage contourImg2;

        ofxCvGrayscaleImage imagen;

        ContourAnimator contourAnimator;

};
