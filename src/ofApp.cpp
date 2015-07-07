#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);

    img1.loadImage("img1.jpg");
    img2.loadImage("img2.jpg");

    colorImg1.setFromPixels(img1.getPixelsRef());
    colorImg2.setFromPixels(img2.getPixelsRef());

    //contourImg1.setFromPixels(img1.getPixelsRef());
    contourImg1 = colorImg1;
    contourImg2 = colorImg2;

    contourImg1.scale(0.5,0.5);
    contourImg2.resize(contourImg2.getWidth() * 2, contourImg2.getHeight()*2);
    contourImg2.scale(1,1);
    contourImg2.invert();

    imagen.allocate(ofGetWidth(), ofGetHeight());
    imagen.setROI(0,0,contourImg1.getWidth(), contourImg1.getHeight());
    imagen.setRoiFromPixels(contourImg1.getPixels(),contourImg1.getWidth(), contourImg1.getHeight() );

    imagen.setROI(imagen.getWidth() / 2,0,contourImg2.getWidth(), contourImg2.getHeight());
    imagen.setRoiFromPixels(contourImg2.getPixels(),contourImg2.getWidth(), contourImg2.getHeight() );
    imagen.resetROI();
    imagen.threshold(100);
    contourManager.setup(ofGetWidth(), ofGetHeight());
    contourAnimator.setup(&contourManager.blobTracker, ofGetWidth(), ofGetHeight());

	gui.setup("");
	gui.add(contourAnimator.paramStop.setup("Stop",false));
	gui.add(contourAnimator.paramNumPuntos.setup("num Puntos",1000, 20, 2000));
	gui.add(contourAnimator.paramNoiseFreq.setup("noise freq",0.001, 0.001, 10));
	gui.add(contourAnimator.paramNoiseMult.setup("noise mult",0.01, 0.01, 40));
	gui.add(contourAnimator.paramPingPong.setup("Ping Pong",false));
	gui.add(contourAnimator.paramAlphaDamping.setup("alpha damping",0.01, 0.1, 1.0));
	gui.add(contourAnimator.paramSmooth.setup("smooth",1, 0.0, 10.0));
	gui.add(contourAnimator.paramRayoNumRayos.setup("Rayo Num Rayos",1, 1, 20));
	gui.add(contourAnimator.paramRayoDisplace.setup("Rayo Displace",1, 1, 200));
	gui.add(contourAnimator.paramRayoMinDist.setup("Rayo MinDist",1, 1, 50));
	gui.add(contourAnimator.paramRayoMult.setup("Rayo Mult",1, 0.01, 10));

	
	ofDisableArbTex();
	fboOut.allocate(ofGetWidth(), ofGetHeight(),GL_RGBA32F);
	//FilterChain * charcoal = new FilterChain(ofGetWidth(), ofGetHeight(), "Charcoal");
 //   //charcoal->addFilter(new BilateralFilter(_video.getWidth(), _video.getHeight(), 4, 4));
 //   charcoal->addFilter(new GaussianBlurFilter(ofGetWidth(), ofGetHeight(), 2.f ));
 //   //charcoal->addFilter(new DoGFilter(ofGetWidth(), ofGetHeight(), 12, 1.2, 8, 0.99, 4));
 //   _filters.push_back(charcoal);
    _filters.push_back(new KuwaharaFilter(6));

	ofEnableArbTex();

}

//--------------------------------------------------------------
void ofApp::update(){
    contourManager.update(imagen);
    contourAnimator.update();
	//ofSetBackgroundAuto(false);
	//ofSetBackgroundColor(0);
	ofSetWindowTitle(ofToString(ofGetFrameRate()) + " FPS");
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	//contourImg1.draw(0,0);
    //contourImg2.draw(ofGetWidth()/2, 0);
    //imagen.draw(0,0);
	//ofSetColor(0);
    //contourManager.draw(0,0);
    //ofCircle(ofPoint(100,100),ofRandom(200));
	
	contourAnimator.draw(&fboOut);
    
	ofEnableAlphaBlending();	
	
	ofDisableArbTex();
    ofPushMatrix();
    ofScale(-1, 1);
    ofTranslate(-ofGetWidth(), 0);
    _filters[0]->begin();
	fboOut.draw(0,0);
    _filters[0]->end();
    ofPopMatrix();
	ofEnableArbTex();

	
	ofSetColor(255);
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (contourManager.blobTracker.trackedBlobs.size() > 1)
    {
        //contourAnimator.animateFromTo(contourManager.blobTracker[0].id, contourManager.blobTracker[1].id);
		//contourAnimator.addAnimationVertical(contourManager.blobTracker[1].id, 0, ofRandom(0.01,1.0));
		//contourAnimator.addAnimationHorizontal(contourManager.blobTracker[1].id, 0, ofRandom(0.01,1.0));
		//contourAnimator.addAnimationRadial(contourManager.blobTracker[1].id, 0, ofRandom(0.01,1.0));

		//contourAnimator.addAnimationVertical(contourManager.blobTracker[0].id, 0, ofRandom(0.01,1.0));
        if (key == 'f')
			contourAnimator.animateFromTo(contourManager.blobTracker[1].id, contourManager.blobTracker[0].id);

		if (key == 'r')
		{
			ofPoint p(ofGetWidth() / 2, ofGetHeight() /2);
			p.x = p.x + ofSignedNoise(ofGetElapsedTimeMillis()) * 100;
			p.y = p.y + ofSignedNoise(10, ofGetElapsedTimeMillis()) * 100;
			contourAnimator.addAnimationRayos(contourManager.blobTracker[1].id, p	);
		}
        cout << "blob desde = " << contourManager.blobTracker[0].id << ", blob hasta =" << contourManager.blobTracker[1].id << endl;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    imagen.set(0);
    imagen.setROI(0,0,contourImg1.getWidth(), contourImg1.getHeight());
    imagen.setRoiFromPixels(contourImg1.getPixels(),contourImg1.getWidth(), contourImg1.getHeight() );

    imagen.setROI( x, y, contourImg2.getWidth(), contourImg2.getHeight());
    imagen.setRoiFromPixels(contourImg2.getPixels(),contourImg2.getWidth(), contourImg2.getHeight() );
    imagen.resetROI();
    imagen.threshold(100);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){


}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
