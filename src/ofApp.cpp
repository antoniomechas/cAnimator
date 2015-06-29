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
}

//--------------------------------------------------------------
void ofApp::update(){
    contourManager.update(imagen);
    contourAnimator.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
    //contourImg1.draw(0,0);
    //contourImg2.draw(ofGetWidth()/2, 0);
    //imagen.draw(0,0);
    //contourManager.draw(0,0);
    contourAnimator.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (contourManager.blobTracker.trackedBlobs.size() > 1)
    {
        //contourAnimator.animateFromTo(contourManager.blobTracker[0].id, contourManager.blobTracker[1].id);
		contourAnimator.addAnimationVertical(contourManager.blobTracker[1].id, 0, ofRandom(0.01,1.0));
		contourAnimator.addAnimationHorizontal(contourManager.blobTracker[1].id, 0, ofRandom(0.01,1.0));

		//contourAnimator.addAnimationVertical(contourManager.blobTracker[0].id, 0, ofRandom(0.01,1.0));
		contourAnimator.addAnimationRadial(contourManager.blobTracker[0].id, 0, ofRandom(0.01,1.0));
        //contourAnimator.animateFromTo(contourManager.blobTracker[1].id, contourManager.blobTracker[0].id);
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
