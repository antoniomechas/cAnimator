#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxBlobTracker.h"
#include "ofxGui.h"
#include "ofxContourAnalysis.h"

#define MAX_BLOBS_HISTORY	150			// Cuantos analizadores de blobs guardamos en el historico

class ContourManager
{

	public:

						ContourManager			( );
		void			setup					( int camWidth, int camHeight);
		void			update					( ofxCvGrayscaleImage img );
		void			draw					( float x, float y );

		void			convexHullBlob			( int i, vector <ofPoint> &contourOut);

		int				getBlobCloserToPoint	( ofPoint p );								// Devuelve el id del blob cuyo centroid está mas cerca del punto dado

		float			height;
		float			width;

		ofxBlobTracker	blobTracker;			// El blobTracker actual

		ofxToggle		paramFindHoles;
		ofxToggle		paramUseApproximation;

	protected:

		void						blobAdded				( ofxBlob &_blob );
		void						blobMoved				( ofxBlob &_blob );
		void						blobDeleted				( ofxBlob &_blob );

		int							getIndexHistoryAnterior	( int i );			// Devuelve el indice dentro de blobTrackerHistory anterior al especificado

		ofxCvGrayscaleImage			grayImg;

		ofxContourAnalysis			contourAnalysis;

};
