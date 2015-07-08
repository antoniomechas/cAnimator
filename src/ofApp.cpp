#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);

	width = ofGetWidth();
	height = ofGetHeight();

    img1.loadImage("img1.jpg");
    img2.loadImage("img2.jpg");
	
	ofDisableArbTex();
	img.loadImage("img/wes.jpg");
	ofEnableArbTex();

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


	ofDisableArbTex();
	fboOut.allocate(ofGetWidth(), ofGetHeight(),GL_RGBA32F);
	//FilterChain * charcoal = new FilterChain(ofGetWidth(), ofGetHeight(), "Charcoal");
 //   //charcoal->addFilter(new BilateralFilter(width, height, 4, 4));
 //   charcoal->addFilter(new GaussianBlurFilter(ofGetWidth(), ofGetHeight(), 2.f ));
 //   //charcoal->addFilter(new DoGFilter(ofGetWidth(), ofGetHeight(), 12, 1.2, 8, 0.99, 4));
 //   _filters.push_back(charcoal);
   // _filters.push_back(new KuwaharaFilter(6));
	//_filters.push_back(new GaussianBlurFilter(ofGetWidth(), ofGetHeight(), 2.f ));
 
	setupFilters();

	ofEnableArbTex();
	
	setupGui();
}

//--------------------------------------------------------------
void ofApp::setupGui(){

	gui.setup("");
	gui.add(contourAnimator.paramStop.setup("Stop",false));
	gui.add(contourAnimator.paramNumPuntos.setup("num Puntos",1000, 20, 2000));
	gui.add(contourAnimator.paramNoiseFreq.setup("noise freq",0.001, 0.001, 10));
	gui.add(contourAnimator.paramNoiseMult.setup("noise mult",0.01, 0.01, 40));
	gui.add(contourAnimator.paramPingPong.setup("Ping Pong",false));
	gui.add(contourAnimator.paramPingPongStep.setup("Ping Pong Step",1,1,30));
	gui.add(contourAnimator.paramAlphaDamping.setup("alpha damping",0.01, 0.1, 1.0));
	gui.add(contourAnimator.paramSmooth.setup("smooth",1, 0.0, 10.0));
	gui.add(contourAnimator.paramRayoNumRayos.setup("Rayo Num Rayos",1, 1, 20));
	gui.add(contourAnimator.paramRayoDisplace.setup("Rayo Displace",1, 1, 200));
	gui.add(contourAnimator.paramRayoMinDist.setup("Rayo MinDist",1, 1, 50));
	gui.add(contourAnimator.paramRayoMult.setup("Rayo Mult",1, 0.01, 10));

	guiFilters.setup("","",gui.getPosition().x + gui.getWidth() + 10);


	for (int i = 0 ; i < filterNames.size() ; i++)
	{
		filterToggles.push_back( new ofxToggle );
		filterToggles.back()->addListener(this, &ofApp::toggleFilterPressed);
		guiFilters.add(filterToggles.back()->setup(filterNames[i],false));
	}

}

//--------------------------------------------------------------
void ofApp::setupFilters()
{
	filterNames.push_back("Halftone");
	filterNames.push_back("Crosshatch");
	filterNames.push_back("Kuwahara");
	filterNames.push_back("SobelEdgeDetection");
	filterNames.push_back("Bilateral");
	filterNames.push_back("Sketch");
	filterNames.push_back("Dilation");
	filterNames.push_back("PerlinPixellation");
	filterNames.push_back("XYDerivative");
	filterNames.push_back("ZoomBlur");
	filterNames.push_back("Emboss");
	filterNames.push_back("SmoothToon");
	filterNames.push_back("TiltShift");
	filterNames.push_back("Voronoi");
	filterNames.push_back("CGAColorspace");
	filterNames.push_back("Erosion");
	filterNames.push_back("Vignette");
	filterNames.push_back("Posterize");
	filterNames.push_back("Laplacian");
	filterNames.push_back("Pixelate");
	filterNames.push_back("HarrisCornerDetection");
	filterNames.push_back("MotionDetection");
	filterNames.push_back("LowPass");

	//chains
	filterNames.push_back("charcoal");
	filterNames.push_back("foggedTexturedGlass");
	filterNames.push_back("WaterColor");

	//ofDisableArbTex();
	fc = new FilterChain(width, height, "fc");
	//ofEnableArbTex();
	//FilterChain * fc2 = new FilterChain(width, height, "Monet");
	//fc2->addFilter(new KuwaharaFilter(9));
	//fc2->addFilter(new LookupFilter(width, height, "img/lookup_miss_etikate.png"));
	//fc2->addFilter(new BilateralFilter(width, height));
	//fc2->addFilter(new PoissonBlendFilter("img/canvas_texture.jpg", width, height, 2.0));
	//fc2->addFilter(new VignetteFilter());
	//_filtersTmp.push_back(fc2);
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
    
	//fboOut.draw(0,0);

	//ofEnableAlphaBlending();	
	
	if (_filters.size() > 0)
	{
		ofDisableArbTex();
		ofPushMatrix();
		ofScale(-1, 1);
		ofTranslate(-ofGetWidth(), 0);
		_filters[0]->begin();
		//fboOut.draw(0,0);
		img.draw(0,0,width,height);
		_filters[0]->end();
		ofPopMatrix();
		ofEnableArbTex();
	}
	else
		fboOut.draw(0,0);

	
	ofSetColor(255);
	gui.draw();
	guiFilters.draw();
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


void ofApp::exit()
{

	//for (int i = 0 ; i < filterNames.size() ; i++)
	//{
	//	filterToggles[i].removeListener(this, &ofApp::toggleFilterPressed);
	//}

}

void ofApp::toggleFilterPressed( bool & value )
{

	ofDisableArbTex();

 //   _filters.push_back(new );
 //   _filters.push_back(new (width, height));
 //   _filters.push_back(new (width, height));
 //   _filters.push_back(new (width, height));
 //   _filters.push_back(new (width, height));
 //   _filters.push_back(new ());
 //   _filters.push_back(new (width, height, 2.f));
 //   _filters.push_back(new (width, height));
 //   _filters.push_back(new TiltShiftFilter(_video.getTextureReference()));
 //   _filters.push_back(new VoronoiFilter(_video.getTextureReference()));    
 //   _filters.push_back(new ());
 //   _filters.push_back(new (width, height));
 //   _filters.push_back(new ());
 //   _filters.push_back(new );
 //   _filters.push_back(new );
 //   _filters.push_back(new (width, height));
 //   _filters.push_back(new HarrisCornerDetectionFilter(_video.getTextureReference()));
 //   _filters.push_back(new MotionDetectionFilter(_video.getTextureReference()));
 //   _filters.push_back(new LowPassFilter(width, height, 0.9));
	_filters.clear();
	
	fc->_filters.clear();
	
	bool bHay = false;

	int width = ofGetWidth();
	int height = ofGetHeight();

	for (int i = 0 ; i < filterToggles.size() ; i++)
	{
		int b = *filterToggles[i];
		if (b == 1)
		{
			cout << "Añadido filtro : " << filterNames[i] << endl;
			bHay = true;
			switch (i)
			{
				case 0: //Halftone
					fc->addFilter(new HalftoneFilter(width, height, 0.001));
					break;

				case 1:
					 fc->addFilter(new CrosshatchFilter(width, height));
					 break;

				case 2:
					 fc->addFilter(new KuwaharaFilter(6));
					 break;
			
				case 3:
					 fc->addFilter(new SobelEdgeDetectionFilter(width, height));
					 break;

				case 4:
					 fc->addFilter(new BilateralFilter(width, height));
					 break;

				case 5:
					 fc->addFilter(new SketchFilter(width, height));
					 break;

				case 6:
					 fc->addFilter(new DilationFilter(width, height));
					 break;

				case 7:
					 fc->addFilter(new PerlinPixellationFilter(width, height));
					 break;

				case 8:
					 fc->addFilter(new XYDerivativeFilter(width, height));
					 break;

				case 9:
					 fc->addFilter(new ZoomBlurFilter());
					 break;

				case 10:
					 fc->addFilter(new EmbossFilter(width, height, 2.0f));
					 break;

				case 11:
					 fc->addFilter(new SmoothToonFilter(width, height));
					 break;

				case 12:
					// fc->addFilter(new TiltShiftFilter(width, height));
					 break;

				case 13:
					// fc->addFilter(new VoronoiFilter(width, height));
					 break;

				case 14:
					 fc->addFilter(new CGAColorspaceFilter());
					 break;

				case 15:
					 fc->addFilter(new ErosionFilter(width, height));
					 break;

				case 16:
					 fc->addFilter(new VignetteFilter());
					 break;

				case 17:
					 fc->addFilter(new PosterizeFilter(8));
					 break;

				case 18:
					 fc->addFilter(new LaplacianFilter(width, height, ofVec2f(1, 1)));
					 break;

				case 19:
					 fc->addFilter(new PixelateFilter(width, height));
					 break;
			
				case 20:
					 //fc->addFilter(new HarrisCornerDetectionFilter(width, height));
					 break;
			
				case 21:
					//fc->addFilter(new MotionDetectionFilter(width, height));
					 break;
			
				case 22:
					 fc->addFilter(new LowPassFilter(width, height, 0.9));
					 break;

				case 23: //charcoal
					fc->addFilter(new BilateralFilter(width, height, 4, 4));
					fc->addFilter(new GaussianBlurFilter(width, height, 2.f ));
					fc->addFilter(new DoGFilter(width, height, 12, 1.2, 8, 0.99, 4));
					break;

				case 24: //foggyTexturedGlass
					fc->addFilter(new PerlinPixellationFilter(width, height, 13.f));
					fc->addFilter(new EmbossFilter(width, height, 0.5));
					fc->addFilter(new GaussianBlurFilter(width, height, 3.f));
					break;

				case 25: //waterColor
					fc->addFilter(new KuwaharaFilter(9));
					fc->addFilter(new LookupFilter(width, height, "img/lookup_miss_etikate.png"));
					fc->addFilter(new BilateralFilter(width, height));
					fc->addFilter(new PoissonBlendFilter("img/canvas_texture.jpg", width, height, 2.0));
					fc->addFilter(new VignetteFilter());
			}
		}
	}
	
	if (bHay)
	{
		cout << "Número de filtros en el filter Chain: " << fc->_filters.size() << endl;
		_filters.push_back(fc);
	}

	ofEnableArbTex();


}