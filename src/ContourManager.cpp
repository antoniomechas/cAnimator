#include "ContourManager.h"


//-------------------------------------------------------
//
ContourManager::ContourManager( )
{
	paramFindHoles = false;
	paramUseApproximation = true;
	//indexHistory = 0;
}

//-------------------------------------------------------
//
void ContourManager::setup(int camWidth, int camHeight )
{
	width = camWidth;
	height = camHeight;

	grayImg.allocate(width, height);

	ofAddListener(blobTracker.blobAdded, this, &ContourManager::blobAdded);
    ofAddListener(blobTracker.blobMoved, this, &ContourManager::blobMoved);
    ofAddListener(blobTracker.blobDeleted, this, &ContourManager::blobDeleted);

}

//-------------------------------------------------------
//
void ContourManager::update ( ofxCvGrayscaleImage img)
{
	grayImg = img;
	// -1 le indica que no haga threshold
	// hullPress no sirve
	blobTracker.update(grayImg, -1, 20, width * height, 5, 20, paramFindHoles, paramUseApproximation);

	//blobTrackerHistory[indexHistory] = blobTracker;
	//indexHistory++;
	//if (indexHistory == MAX_BLOBS_HISTORY)
	//	indexHistory = 0;

}


//-------------------------------------------------------
//
void ContourManager::draw ( float x, float y )
{

	//blobTracker.draw( x, y );

	//for (int i = 0; i < blobTracker.size(); i++)
	//{
	//	ofFill();
	//	ofSetColor(255,0,0);
	//	ofCircle(blobTracker[i].centroid.x * width,
	//				blobTracker[i].centroid.y * height,
	//				10);
	//	ofSetColor(127,127,127);
	//	ofDrawBitmapString(ofToString( blobTracker[i].id ),
	//						blobTracker[i].centroid.x * width,
	//						blobTracker[i].centroid.y * height);
	//}

	for (int i = 0 ; i < blobTracker.size() ; i++)
	{
		blobTracker[i].drawContours(0,0, width, height);
	}

}


//-------------------------------------------------------
//
void ContourManager::convexHullBlob( int i, vector <ofPoint> &contourOut)
{
	vector<ofPoint> tmp;

	for (int k = 0 ; k < blobTracker[i].nPts ; k ++)
	{
		bool valido = false;
		ofPoint p = blobTracker[i].pts[k];
		p.x = p.x * width;
		p.y = p.y * height;
		tmp.push_back( p );
	}
	contourAnalysis.convexHull(tmp, contourOut);

}


//-------------------------------------------------------
// Devuelve el index del blob cuyo centroid está mas cerca del punto p
// el punto viene en coordenadas de proyección
// el blob guarda sus puntos normalizados
// hay que convertir los centroids a escala kinect y luego convertirlos a coordenadas de proyeccion
//
int ContourManager::getBlobCloserToPoint( ofPoint p )
{

	float dist = 9999999;
	int idDest = 0;

	for (int i = 0 ; i < blobTracker.size() ; i++)
	{
		ofPoint centroid = blobTracker[i].centroid;
		centroid.x = centroid.x * width;
		centroid.y = centroid.y * height;
		ofPoint cProy = centroid ;
		if (p.distance(cProy) < dist)
		{
			dist = p.distance(cProy);
			idDest = blobTracker[i].id;
		}
	}
	//cout << "blob closer: " << idDest << ", punto: "<< p.x << "," << p.y << endl;
	return idDest;

}


//-------------------------------------------------------
//
int ContourManager::getIndexHistoryAnterior ( int i )
{
	if ( i == 0 )
		i = MAX_BLOBS_HISTORY - 1;
	else
		i = i - 1;
	return (i);
}

//-------------------------------------------------------
//
void ContourManager::blobAdded(ofxBlob &_blob)
{
    ofLog(OF_LOG_NOTICE, "Blob ID " + ofToString(_blob.id) + " added" );


}

//-------------------------------------------------------
//
void ContourManager::blobMoved(ofxBlob &_blob)
{
    //ofLog(OF_LOG_NOTICE, "Blob ID " + ofToString(_blob.id) + " moved" );
}

//-------------------------------------------------------
//
void ContourManager::blobDeleted(ofxBlob &_blob)
{

	ofLog(OF_LOG_NOTICE, "Blob ID " + ofToString(_blob.id) + " deleted" );

}
