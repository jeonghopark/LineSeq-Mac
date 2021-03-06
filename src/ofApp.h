#pragma once

#include "ofMain.h"
#include "ofxTonic.h"

#include "PixelBlock.h"
//#include "NoteBlock.h"

#define TRIGGER_LINE_NUM 8

using namespace Tonic;


struct TriggerLine {
    ofVec2f start;
    ofVec2f stop;
    vector<ofVec2f> linePath;
    bool trigger;
};

class ofApp : public ofBaseApp{
    
    ofxTonicSynth synth;
    int scaleDegree;
    void trigger(int _pitch);
    void setScaleDegreeBasedOnMouseX();
    
public:
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
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
    float triggerMovingFactor[TRIGGER_LINE_NUM];
    float triggerMovingSpeed[TRIGGER_LINE_NUM];
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
    
    bool fullscreen;
  
    void informationText();
    
    void colorValueView();
    bool bColorValueView;
    
};


