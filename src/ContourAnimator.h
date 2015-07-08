#ifndef CONTOURANIMATOR_H
#define CONTOURANIMATOR_H

#include "ofMain.h"
#include "ofxBlobTracker.h"
#include "ShaderMask.h"
#include "ofxGui.h"
#include "ofxKCloth.h"

class ContourAnimator
{
    public:

        void    setup					( ofxBlobTracker *blobTracker, int width, int height);
        void    animateFromTo			( int idFrom, int idTo);
		void	addAnimationRadial		( int idBlob, int durationMilis = 0, float velocidad = 0.5f);
		void	addAnimationVertical	( int idBlob, int durationMilis = 0, float velocidad = 0.5f);
		void	addAnimationHorizontal	( int idBlob, int durationMilis = 0, float velocidad = 0.5f);
		void	addAnimationRayos		( int idBlob, ofPoint origen, int durationMilis = 0, float velocidad = 0.5f);
        void    update();
        void    draw					( ofFbo *fboOut );


        ContourAnimator();
        virtual ~ContourAnimator();

		ofxToggle		paramStop;
		ofxToggle		paramPingPong;
        ofxIntSlider    paramPingPongStep;
        ofxIntSlider    paramNumPuntos;
        ofxFloatSlider  paramNoiseFreq;
        ofxFloatSlider  paramNoiseMult;
		ofxFloatSlider	paramAlphaDamping;
		ofxFloatSlider	paramSmooth;
		ofxIntSlider	paramRayoNumRayos;
		ofxFloatSlider  paramRayoDisplace;
		ofxFloatSlider	paramRayoMinDist;
		ofxFloatSlider  paramRayoMult;

    protected:

        bool    getUnifiedContour	( int idBlob, ofPolyline &poly, int numPuntos ); //devuelve false si el blob ya no existe
        void    drawPoly			( ofPolyline &poly );
        ofPoint getInterpolated		( ofPoint p1, ofPoint p2, float value );
        void    dampPoly			( ofPolyline &pAct, ofPolyline &pDst, float paramDamp, ofPolyline &pResultado );
		void	drawAnimation		( int idAnimation );
		void	drawBlobMask		( int idBlob );
		void	drawDesdeHasta		( int idAnimation );
		void	drawVertical		( int idAnimation );
		void	drawHorizontal		( int idAnimation );
		void	drawRadial			( int idAnimation );
		void	drawContourOnda		( int idAnimation );
		void	drawLine			( ofPoint p1, ofPoint p2, int idAnimacion = 0);
		void	drawContourRayos	( int idAnimation );

		void	drawRayo			( ofPoint p1, ofPoint p2);
		void	drawRayoRec			( ofPoint p1, ofPoint p2, float displace);

        float width;
        float height;

		enum TIPOANIMACION
		{
			TIPOA_DESDE_HASTA,
			TIPOA_RADIAL,
			TIPOA_VERTICAL,
			TIPOA_HORIZONTAL,
			TIPOA_RAYOS
		};

        struct C_ANIMATION
        {
			TIPOANIMACION	tipoAnimacion;
            int				idFrom;
            int				idTo;
            double			timeStart;
            bool			active;
            float			timeDuration;
            ofPolyline		polyActual;
			float			velocidad;
			ofPoint			punto;			//punto del que salen los rayos, por ejemplo
        };

        vector<C_ANIMATION>	animations;

        ofxBlobTracker		*blobTracker;

		ShaderMask			shaderMask;
		ofFbo				fboOut1;
		ofFbo				fboOut2;
		ofFbo				*ping;
		ofFbo				*pong;

		ofShader			shaderPingPong;

		ClothController		clothController;
        
		int					ticks;

       // int cuenta;
       // int timeCuenta;

    private:

};

#endif // CONTOURANIMATOR_H
