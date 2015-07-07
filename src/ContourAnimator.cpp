#include "ContourAnimator.h"

//----------------------------------------------------------------------
//
ContourAnimator::ContourAnimator()
{
    //ctor
}

//----------------------------------------------------------------------
//
ContourAnimator::~ContourAnimator()
{
    //dtor
}

//----------------------------------------------------------------------
//
void ContourAnimator::setup (ofxBlobTracker *bTracker, int w, int h)
{
    width = w;
    height = h;
    blobTracker = bTracker;

	shaderMask.setup( w, h);
	shaderMask.setMaskBegin();
		ofClear(1,1,1,1);
		//ofBackground(255);		
	shaderMask.setMaskEnd();


	fboOut1.allocate(w,h,GL_RGBA32F_ARB);
	fboOut1.begin();
		ofClear(0,0,0,1);
	fboOut1.end();

	fboOut2.allocate(w,h,GL_RGBA32F_ARB);
	fboOut2.begin();
		ofClear(0,0,0,1);
	fboOut2.end();
	ping = &fboOut1;
	pong = &fboOut2;

	shaderPingPong.load("shaders/pingpong");

	paramAlphaDamping = 0.9f;
    paramNumPuntos = 2000;
    paramNoiseMult = 1;
	paramRayoMinDist = 4;
	paramRayoMult = 1;

}

//----------------------------------------------------------------------
//
void ContourAnimator::animateFromTo(int idFrom, int idTo)
{
    C_ANIMATION a;
	a.tipoAnimacion = TIPOANIMACION::TIPOA_DESDE_HASTA;
    a.idFrom = idFrom;
    a.idTo = idTo;
    a.timeStart = ofGetElapsedTimeMillis();
    a.active = true;
    a.timeDuration = ofRandom(300,1000);
    animations.push_back(a);
    //cuenta = 0;
    //timeCuenta = ofGetElapsedTimeMillis();
}

//----------------------------------------------------------------------
//
void ContourAnimator::addAnimationRadial(int idBlob, int durationMilis, float velocidad)
{
    C_ANIMATION a;
	a.tipoAnimacion = TIPOANIMACION::TIPOA_RADIAL;
    a.idFrom = idBlob;
    a.idTo = 0;
    a.timeStart = ofGetElapsedTimeMillis();
    a.active = true;
    a.timeDuration = durationMilis;
	a.velocidad = velocidad;
    animations.push_back(a);
    //cuenta = 0;

}

//----------------------------------------------------------------------
//
void ContourAnimator::addAnimationVertical(int idBlob, int durationMilis, float velocidad)
{
    C_ANIMATION a;
	a.tipoAnimacion = TIPOANIMACION::TIPOA_VERTICAL;
    a.idFrom = idBlob;
    a.idTo = 0;
    a.timeStart = ofGetElapsedTimeMillis();
    a.active = true;
    a.timeDuration = durationMilis;
	a.velocidad = velocidad;
    animations.push_back(a);
    //cuenta = 0;

}


//----------------------------------------------------------------------
//
void ContourAnimator::addAnimationHorizontal(int idBlob, int durationMilis, float velocidad)
{
    C_ANIMATION a;
	a.tipoAnimacion = TIPOANIMACION::TIPOA_HORIZONTAL;
    a.idFrom = idBlob;
    a.idTo = 0;
    a.timeStart = ofGetElapsedTimeMillis();
    a.active = true;
    a.timeDuration = durationMilis;
	a.velocidad = velocidad;
    animations.push_back(a);
    //cuenta = 0;

}

//----------------------------------------------------------------------
//
void ContourAnimator::addAnimationRayos(int idBlob, ofPoint origen, int durationMilis, float velocidad)
{
    C_ANIMATION a;
	a.tipoAnimacion = TIPOANIMACION::TIPOA_RAYOS;
    a.punto = origen;
	a.idFrom = idBlob;
    a.idTo = 0;
    a.timeStart = ofGetElapsedTimeMillis();
    a.active = true;
    a.timeDuration = durationMilis;
	a.velocidad = velocidad;
    animations.push_back(a);
    //cuenta = 0;

}



//----------------------------------------------------------------------
//
void ContourAnimator::update()
{
    if (paramStop)
	{
		for (int i = 0; i < animations.size() ; i++)
			animations[i].active = false;
		animations.clear();
		paramStop = false;
	}
	
	int i = 0;
    while (i < animations.size())
    {
		if ((animations[i].tipoAnimacion == TIPOA_DESDE_HASTA) && (animations[i].timeDuration < ofGetElapsedTimeMillis() - animations[i].timeStart))
            animations[i].active = false;
        
		if (!animations[i].active)
            animations.erase(animations.begin() + i);
        else
            i++;
    }



}

//----------------------------------------------------------------------
//
void ContourAnimator::draw(ofFbo *fboOut)
{
	//ofBackground(0);
	ofPushStyle();

	//ofEnableSmoothing();

	ofEnableAntiAliasing();
	
	ofEnableAlphaBlending();
	
	if (paramPingPong)
	{
		ping->begin();
			//ofClear(1,1,1,0);
			for (int i = 0 ; i < animations.size() ; i++)
			{
				if (animations[i].active)
					drawAnimation(i);
			}
			drawRayo(ofPoint(100,100), ofPoint(800,200));
		ping->end();

		ofDisableAntiAliasing();
		//ofDisableSmoothing();
		ofPopStyle();
	
		//ofDisableAlphaBlending();

		pong->begin();
			shaderPingPong.begin();
				//shaderPingPong.setUniform1i("kernelSize", paramKernelSize);
				shaderPingPong.setUniform1f("alphaDamping", paramAlphaDamping);
				//shader.setUniform1i("velX", paramVelX);
				//shader.setUniform1i("velY", paramVelY);
				ping->draw(0,0);
			shaderPingPong.end();
		pong->end();

		ofDisableAlphaBlending();
		ofSetColor(255);
		
		fboOut->begin();
			pong->draw(0,0);
		fboOut->end();

		swap(ping, pong);
	}
	else
	{
		fboOut->begin();
			ofClear(0,1);
			for (int i = 0 ; i < animations.size() ; i++)
			{
				if (animations[i].active)
					drawAnimation(i);
			}
		fboOut->end();
		//drawRayo(ofPoint(100,100), ofPoint(800,200));
	}


}

//----------------------------------------------------------------------
//
void ContourAnimator::drawBlobMask( int idBlob )
{
	ofBackground(0);
	ofPolyline poly1;
    for (int i = 0 ; i < blobTracker->trackedBlobs.size() ; i++)
    {
        if (blobTracker->trackedBlobs[i].id == idBlob)
            poly1.addVertices(blobTracker->trackedBlobs[i].pts);
    }
	poly1.close();
	ofSetColor(255);
	ofFill();
	ofBeginShape();
	for (int i = 0 ; i < poly1.size() ; i++)
	{
		ofPoint p = poly1[i] * ofPoint(width, height);
		ofVertex( p.x, p.y );
	}
	ofEndShape();
}


//----------------------------------------------------------------------
//
void ContourAnimator::drawAnimation( int idAnimation )
{

	ofPolyline poly1;
    
	switch(animations[idAnimation].tipoAnimacion)
	{
		case TIPOA_DESDE_HASTA:
			drawDesdeHasta(idAnimation);
			break;

		case TIPOA_RADIAL:
			drawRadial(idAnimation);
			break;

		case TIPOA_RAYOS:
			drawContourRayos(idAnimation);
			break;

		case TIPOA_VERTICAL:
			shaderMask.setMaskBegin();
				drawBlobMask(animations[idAnimation].idFrom);
			shaderMask.setMaskEnd();
			shaderMask.drawBegin();
				drawVertical(idAnimation);
			shaderMask.drawEnd();
			ofSetColor(255);
			
			shaderMask.draw();
			
			if (getUnifiedContour(animations[idAnimation].idFrom, poly1, 400))
				poly1.draw();

			//shaderMask.drawMask();
			break;

		case TIPOA_HORIZONTAL:
			shaderMask.setMaskBegin();
				drawBlobMask(animations[idAnimation].idFrom);
			shaderMask.setMaskEnd();
			shaderMask.drawBegin();
				drawHorizontal(idAnimation);
			shaderMask.drawEnd();
			ofSetColor(255);
			shaderMask.draw();
			if (getUnifiedContour(animations[idAnimation].idFrom, poly1, 400))
				poly1.draw();

			//shaderMask.drawMask();
			break;

	}

}

//----------------------------------------------------------------------
//Linea horizontal que sube y baja
//
void ContourAnimator::drawVertical( int idAnimation )
{
	ofPoint pos;
    int index = -1;
	int idBlob = animations[idAnimation].idFrom;
	for (int i = 0 ; i < blobTracker->trackedBlobs.size() ; i++)
    {
        if (blobTracker->trackedBlobs[i].id == idBlob)
			index = i;
    }
	if (index == -1)
		return;

	ofRectangle r = blobTracker->trackedBlobs[index].boundingRect;

	float t = (ofGetElapsedTimeMillis() - animations[idAnimation].timeStart) * animations[idAnimation].velocidad * 0.001f;
	float numLineas = 8;
	for ( int k = 0 ; k < numLineas ; k++)
	{
		float py = sin(t + k * ((PI*2)/numLineas));
		pos.y = r.getCenter().y * height + (py*((r.getHeight() * height) / 2.0));
		pos.x = r.getCenter().x * width;
		drawLine(pos - ofPoint(500,0),pos + ofPoint(500,0), idAnimation);
	}


}

//----------------------------------------------------------------------
//Linea vertical de izq a derecha
//
void ContourAnimator::drawHorizontal( int idAnimation )
{
	ofPoint pos;
    int index = -1;
	int idBlob = animations[idAnimation].idFrom;
	for (int i = 0 ; i < blobTracker->trackedBlobs.size() ; i++)
    {
        if (blobTracker->trackedBlobs[i].id == idBlob)
			index = i;
    }
	if (index == -1)
		return;

	ofRectangle r = blobTracker->trackedBlobs[index].boundingRect;

	float t = (ofGetElapsedTimeMillis() - animations[idAnimation].timeStart) * animations[idAnimation].velocidad * 0.001f;
	float numLineas = 8;
	for ( int k = 0 ; k < numLineas ; k++)
	{
		float py = sin(t + k * ((PI*2)/numLineas));
		pos.y = r.getCenter().y * height;
		pos.x = r.getCenter().x * width + (py*((r.getWidth() * width) / 2.0));
		drawLine(pos - ofPoint(0,500),pos + ofPoint(0,500), idAnimation);
	}


}


//----------------------------------------------------------------------
//de dentro pa fuera y viceversa
//
void ContourAnimator::drawRadial( int idAnimation )
{
	ofPoint pos;
    int index = -1;
	int idBlob = animations[idAnimation].idFrom;
	for (int i = 0 ; i < blobTracker->trackedBlobs.size() ; i++)
    {
        if (blobTracker->trackedBlobs[i].id == idBlob)
			index = i;
    }
	if (index == -1)
		return;

	ofPoint centroid = blobTracker->trackedBlobs[index].centroid * ofPoint(width,height);
	//ofPoint centroid = blobTracker->trackedBlobs[index].boundingRect.getTopLeft();

	float t = (ofGetElapsedTimeMillis() - animations[idAnimation].timeStart) * animations[idAnimation].velocidad * 0.001f;
	float numContornos = 8;
	ofPolyline poly1;
	if (!getUnifiedContour(animations[idAnimation].idFrom, poly1, paramNumPuntos))
		return;
	poly1.draw();
	for (int i = 0 ; i < poly1.size() ; i++)
	{
		poly1[i] -= centroid;
		poly1[i] += ofPoint(ofSignedNoise(i, ofGetElapsedTimeMillis() * paramNoiseFreq) * paramNoiseMult, ofSignedNoise(ofGetElapsedTimeMillis() * paramNoiseFreq, i)* paramNoiseMult);
	}

	for ( int k = 0 ; k < numContornos ; k++)
	{
		float py = abs(sin(t + k * ((PI*2)/numContornos)));
		ofPushMatrix();
		ofTranslate(centroid);
		ofScale(py,py);
		poly1.draw();
		ofPopMatrix();
	}


}


//----------------------------------------------------------------------
//Dibuja el contorno con una forma sinusoidal
//
void ContourAnimator::drawContourOnda( int idAnimation )
{
	ofPoint pos;
    int index = -1;
	int idBlob = animations[idAnimation].idFrom;
	for (int i = 0 ; i < blobTracker->trackedBlobs.size() ; i++)
    {
        if (blobTracker->trackedBlobs[i].id == idBlob)
			index = i;
    }
	if (index == -1)
		return;

	ofPolyline poly1;
	if (!getUnifiedContour(animations[idAnimation].idFrom, poly1, paramNumPuntos))
		return;

	for (int i = 0 ; i < poly1.size() ; i++)
	{
		
	}



}

//----------------------------------------------------------------------
// Dibuja rayos que surgen desde un punto fijo hacia el contorno
//
void ContourAnimator::drawContourRayos( int idAnimation )
{
	ofPoint pos;
    int index = -1;
	int idBlob = animations[idAnimation].idFrom;
	for (int i = 0 ; i < blobTracker->trackedBlobs.size() ; i++)
    {
        if (blobTracker->trackedBlobs[i].id == idBlob)
			index = i;
    }
	if (index == -1)
		return;

	ofPolyline poly1;
	if (!getUnifiedContour(animations[idAnimation].idFrom, poly1, paramNumPuntos))
		return;
	//poly1.draw();
	for (int i = 0 ; i < poly1.size() ; i++)
	{
		drawRayo(animations[idAnimation].punto, poly1[i]);
	}



}

//----------------------------------------------------------------------
//
void ContourAnimator::drawDesdeHasta( int idAnimation )
{
	//paramNumPuntos = 700 + ofNoise(idAnimation, ofGetElapsedTimef() * 0.1) * 50;
    ofPolyline poly1;
    if (!getUnifiedContour(animations[idAnimation].idFrom, poly1, paramNumPuntos))
        return;
    ofPolyline poly2;
    if (!getUnifiedContour(animations[idAnimation].idTo, poly2, paramNumPuntos))
        return;
    ofSetColor(255,0,0);
    drawPoly(poly1);
    ofSetColor(0,255,0);
    drawPoly(poly2);
    ofPolyline polyDst;
	ofPoint puntoAnt(-1,-1);
    for (int k = 0 ; k < poly1.size() ; k++)
    {
        float timeElapsed = ofGetElapsedTimeMillis() - animations[idAnimation].timeStart;
        float pct = ofMap(timeElapsed, 0, animations[idAnimation].timeDuration,0,1);
        ofPoint punto = getInterpolated(poly1[k],poly2[k],pct);
        if (puntoAnt.x != -1)
		{
			ofVec2f dir = punto-puntoAnt;
			dir.normalize();
			punto = punto + dir * ofSignedNoise(k, ofGetElapsedTimef() * paramNoiseFreq) * paramNoiseMult;
			//punto.x = punto.x + ofSignedNoise(k,ofGetElapsedTimef()) * paramNoiseMult;
			//punto.y = punto.y + ofSignedNoise(ofGetElapsedTimef(),k) * paramNoiseMult;
		
		}
		
        polyDst.addVertex(punto);
		puntoAnt = punto;
        //ofLine(poly1[k],poly2[k]);
    }
    ofSetColor(255);
    polyDst.close();
    ofPolyline polyResultado;
    float paramDamp = 0.9f;
    //dampPoly(animations[i].polyActual, polyDst, paramDamp, polyResultado);
    //animations[i].polyActual = polyResultado;
    //drawPoly(polyResultado);
    drawPoly(polyDst);
//            ofCircle(poly1[cuenta],5);
//            ofCircle(poly2[cuenta],5);
//            ofCircle(polyDst[cuenta],5);
    //if (ofGetElapsedTimeMillis() - timeCuenta > 50)
    //{
    //    timeCuenta = ofGetElapsedTimeMillis();
    //    cuenta++;
    //    if (cuenta == poly1.size()-1)
    //        cuenta = 0;
    //}

}

//----------------------------------------------------------------------
//Dibuja un rayo entre los dos puntos especificados
//
void ContourAnimator::drawRayo(ofPoint p1, ofPoint p2)
{
	for (int i = 0 ; i < paramRayoNumRayos ; i++)
		drawRayoRec(p1, p2, paramRayoDisplace);
}

void ContourAnimator::drawRayoRec(ofPoint p1, ofPoint p2, float displace)
{
	if (displace < paramRayoMinDist)
	{	
		ofLine(p1,p2);
	}
	else
	{
		ofVec2f v = p2-p1;
		ofPoint p3 = p2.getMiddle(p1);
	
		ofVec2f vp = v.perpendicular();
		vp.normalize();
		//p1 = p1 + vp * ofRandom(-paramRayoMult,paramRayoMult) * displace;
		//p2 = p2 + vp * ofRandom(-paramRayoMult,paramRayoMult) * displace;
		p3 = p3 + vp * ofRandom(-paramRayoMult,paramRayoMult) * displace;
		drawRayoRec(p1,p3, displace / 2);
		drawRayoRec(p3,p2, displace / 2);
	}
}


//----------------------------------------------------------------------
//
bool ContourAnimator::getUnifiedContour(int idBlob, ofPolyline &poly, int numPuntos)
{
    ofPolyline poly1;
    ofPolyline poly2;
    for (int i = 0 ; i < blobTracker->trackedBlobs.size() ; i++)
    {
        if (blobTracker->trackedBlobs[i].id == idBlob)
            poly1.addVertices(blobTracker->trackedBlobs[i].pts);
    }
    if (poly1.size() == 0)
    {
        cout << "Error en getUnifiedContour, id blob no encontrado: " << idBlob << endl;
        return false;
    }
    //busca el punto con la y mas pequeña

    float yMin = 10000;
    int posMin = 0;
    for (int i = 0 ; i < poly1.size() ; i++)
    {
        poly1[i].x = poly1[i].x * width;
        poly1[i].y = poly1[i].y * height;
        if (poly1[i].y < yMin)
        {
            posMin = i;
            yMin = poly1[i].y;
        }
    }
    //cout << "getUnifiedContour id=" << idBlob << ", yMin=" << yMin << endl;

    for (int i = posMin ; i < poly1.size() ; i++)
        poly2.addVertex(poly1[i]);

    for (int i = 0 ; i < posMin ; i++)
        poly2.addVertex(poly1[i]);

    poly2.addVertex(poly1[posMin]);

    poly = poly2.getResampledByCount(numPuntos);
    if (poly.size() != numPuntos)
    {
        //cout << "GetUnifiedContour error: numero de puntos = " << poly.size() << ", se esperaban " << numPuntos << ", se añaden hasta completar." << endl;
        while (poly.size() < numPuntos)
            poly.addVertex(poly[0]);
    }
    //poly.close();
    //cout << "resampled blob id " << idBlob << ", size=" << poly.size() << endl;
    return true;
}

//----------------------------------------------------------------------
//
void ContourAnimator::drawPoly(ofPolyline &poly)
{
//    for (int i = 0 ; i < poly.size(); i++)
//    {
//        ofCircle(poly[i],1);
//    }
    ofPolyline poly2;
	poly2.addVertex(poly[0]);
	for (int i = 0 ; i < poly.size(); i++)
    {
        poly2.curveTo(poly[i]);
    }
    poly2.curveTo(poly[0]);
    poly2.curveTo(poly[0]);
    poly2.getSmoothed(paramSmooth);
    poly2.draw();
//    poly.draw();
}

//----------------------------------------------------------------------
//
ofPoint ContourAnimator::getInterpolated(ofPoint p1, ofPoint p2, float value)
{
    ofVec2f v = p1;
    ofPoint p = v.interpolate(p2, value);
    return p;
}



//----------------------------------------------------------------------
//
void ContourAnimator::dampPoly(ofPolyline &pAct, ofPolyline &pDst, float paramDamp, ofPolyline &pResultado)
{
    if (pAct.size() == 0)
    {
        pResultado = pDst;
        return;
    }

    pResultado.clear();

    for (int i=0 ; i < pAct.size() ; i++)
    {
        ofVec2f v = pAct[i];
        ofPoint p = v.interpolate(pDst[i], paramDamp);
        pResultado.addVertex(p);
    }

}


void ContourAnimator::drawLine(ofPoint p1, ofPoint p2, int idAnimacion)
{
	int numDiv = 200;
	float amplitude = paramNoiseMult;
	float speed = paramNoiseFreq;

	ofPolyline poly;
	poly.addVertex(p1);
	poly.addVertex(p2);
	
	ofPolyline poly2 = poly.getResampledByCount(numDiv);
	ofVec2f linea = p2-p1;
	ofVec2f lineaP = linea.normalized().perpendicular();
	for (int i = 0 ; i < poly2.size() ; i++)
	{
		poly2[i] += lineaP * ofSignedNoise((idAnimacion * 100) + i,ofGetElapsedTimeMillis() * speed) * amplitude;
		//poly2[i] += ofPoint(ofNoise(i,ofGetElapsedTimeMillis()) * amplitude, ofNoise(ofGetElapsedTimeMillis(),i) * amplitude);
	}
	ofSetLineWidth(ofNoise(idAnimacion, ofGetElapsedTimeMillis() * speed) * 5.0);
	poly2.draw();
	ofSetLineWidth(1.0);
	//ofLine(p1,p2);
}