#ifndef CONTOURANIMATOR_H
#define CONTOURANIMATOR_H

#include "ofMain.h"
#include "ofxBlobTracker.h"
#include "ShaderMask.h"

class ContourAnimator
{
    public:

        void    setup					( ofxBlobTracker *blobTracker, int width, int height);
        void    animateFromTo			( int idFrom, int idTo);
		void	addAnimationRadial		( int idBlob, int durationMilis = 0, float velocidad = 0.5f);
		void	addAnimationVertical	( int idBlob, int durationMilis = 0, float velocidad = 0.5f);
		void	addAnimationHorizontal	( int idBlob, int durationMilis = 0, float velocidad = 0.5f);
        void    update();
        void    draw();


        ContourAnimator();
        virtual ~ContourAnimator();

        int     paramNumPuntos;
        float   paramNoiseMult;

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
		void	drawLine			( ofPoint p1, ofPoint p2, int idAnimacion = 0);

        float width;
        float height;

		enum TIPOANIMACION
		{
			TIPOA_DESDE_HASTA,
			TIPOA_RADIAL,
			TIPOA_VERTICAL,
			TIPOA_HORIZONTAL
		};

        struct ANIMATION
        {
			TIPOANIMACION tipoAnimacion;
            int idFrom;
            int idTo;
            double timeStart;
            bool active;
            float timeDuration;
            ofPolyline polyActual;
			float velocidad;
            //float dist;
        };

        vector<ANIMATION> animations;

        ofxBlobTracker *blobTracker;

		ShaderMask		shaderMask;


       // int cuenta;
       // int timeCuenta;

    private:

};

#endif // CONTOURANIMATOR_H
