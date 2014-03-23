

#pragma once

#include "ofMain.h"
#include "ofxiOS.h"
#include "ofxiOSExtras.h"

#include "MIDIFileReader.h"
//#include "ofxFileDialogOSX.h"
#include "drawMidiNotes.h"

//#include <iostream>
#include "midiEventHolder.h"

#include "NoteBlock.h"

#include "ofxTonic.h"

#include "PixelBlock.h"

using namespace Tonic;

using namespace std;
using namespace MIDIConstants;

struct MidiNoteRandomDraw {
    int pitch;
    int velocity;
    int duration;
    float onPosX;
    float onPosY;
    float offPosX;
    float offPosY;
    int rotationZ;
};


struct TriggerLine {
    ofVec2f start;
    ofVec2f stop;
    
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
//    void windowResized(int w, int h);
    
    void audioRequested (float * output, int bufferSize, int nChannels);
    
    
	void startPlaying();
	void stopPlaying();
	bool getFilenameFromDialogBox(string* fileNameToSave);
    
	typedef std::vector<int> IntVector;
	typedef std::vector<double> DoubleVector;
    
	IntVector v;
    
	midiEventHolder midiEvents;
	
	int cannamMainFunction();
	string midiFileName;
    
	bool playing;
    
	int screenWidth, screenHeight;
    
    
    // NoteBlock
    vector<NoteBlock> noteBlock;
    
    // MIDI Note new Drawing
    MidiNoteRandomDraw midiNoteRandomDraw;
    void randomMidiDrawing();
    void randomMidiInput(int _p, int _v, int _d);
    
    // Trigger Line
    vector<TriggerLine> triggerLine;
    ofVec2f touchUpPos;
    ofVec2f touchDownPos;
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
    
};


