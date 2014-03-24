

#pragma once

#include "ofMain.h"
#include "ofxiOS.h"
#include "ofxiOSExtras.h"

//#include "NoteBlock.h"

#include "ofxTonic.h"

#include "PixelBlock.h"

using namespace Tonic;


struct TriggerLine {
    ofVec2f start;
    ofVec2f stop;
    vector<ofVec2f> linePath;
    bool trigger;
};

class ofApp : public ofxiOSApp{
    
    ofxTonicSynth synth;
    int scaleDegree;
    void trigger(int _pitch);
    void setScaleDegreeBasedOnMouseX();
    
public:
    void setup();
    void update();
    void draw();
    void exit();
	
    void touchDown(ofTouchEventArgs & touch);
    void touchMoved(ofTouchEventArgs & touch);
    void touchUp(ofTouchEventArgs & touch);
    void touchDoubleTap(ofTouchEventArgs & touch);
    void touchCancelled(ofTouchEventArgs & touch);
    
    void lostFocus();
    void gotFocus();
    void gotMemoryWarning();
    void deviceOrientationChanged(int newOrientation);
    
//    void keyPressed  (int key);
//    void keyReleased(int key);
//    void mouseMoved(int x, int y );
//    void mouseDragged(int x, int y, int button);
//    void mousePressed(int x, int y, int button);
//    void mouseReleased(int x, int y, int button);
    
    void audioRequested (float * output, int bufferSize, int nChannels);
    
    
	void startPlaying();
	void stopPlaying();
    
	typedef std::vector<int> IntVector;
	typedef std::vector<double> DoubleVector;
    
	IntVector v;
    
	int cannamMainFunction();
	string midiFileName;
    
	bool playing;
    
	int screenWidth, screenHeight;
    
    // Trigger Line
    vector<TriggerLine> triggerLine;
    ofVec2f touchUpPos;
    ofVec2f touchDownPos;
    ofVec2f touchMov;
    void drawPreviewLine();
    void triggerLineDraw();
    
    
    // Trigger Point;
    float triggerMovingFactor;
    ofVec2f triggerPosOnLine;
    bool triggerHoverX, triggerHoverY;
    
    bool reload;
    
    
    //Video
    
    ofVideoGrabber videoGrabber;
    ofTexture tex;
    unsigned char * pix;

    vector<float> pixelColor;
    vector<float> pixelNote;
    
    int pixelNum;
    int pixelSize;
    int widthRes, heightRes;
    int numWidthPixel, numHeightPixel;
    
    
    // Pixel
    vector<PixelBlock> pixelBlocks;
    
    void triggerLinePixel();
    
    bool contactPixel(float _x, float _y, float _xD, float _yD);
    bool bPixelContact;
    
    vector<bool> triggerPixel;
    
    ofPoint triggerPosOnLines[8];
    
};


