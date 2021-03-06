#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
#ifdef DEBUG
    
#else
    ofSetDataPathRoot("../Resources/data");
#endif
    
    
    ofBackground(0);
    ofSetFrameRate(60);
    
    pixelSize = 20;
//    videoGrabber.setDeviceID(1);
    videoGrabber.initGrabber(480, 360);
    numWidthPixel = videoGrabber.getWidth()/pixelSize;
    numHeightPixel = videoGrabber.getHeight()/pixelSize;
    
	tex.allocate(numWidthPixel, numHeightPixel, GL_RGB);
	pix = new unsigned char[ (int)( numWidthPixel * numHeightPixel * 3.0) ];
    pixelBlocks.resize(numWidthPixel * numHeightPixel);
    for (int i=0; i<pixelBlocks.size(); i++) {
        pixelBlocks[i].setup();
    }

	playing = false;
    
    for (int i=0; i<TRIGGER_LINE_NUM; i++) {
        triggerMovingFactor[i] = 0;
        triggerMovingSpeed[i] = ofRandom(1) * 0.024;
    }
    
    // Tonic
    
    ControlGenerator midiNote = synth.addParameter("midiNumber");
    ControlGenerator noteFreq =  ControlMidiToFreq().input(midiNote);
    Generator tone = RectWave().freq( noteFreq );
    tone = LPF12().input(tone).Q(10).cutoff((noteFreq * 2) + SineWave().freq(10) * 0.5 * noteFreq);
    
    ControlGenerator envelopeTrigger = synth.addParameter("trigger");
    Generator toneWithEnvelope = tone * ADSR().attack(0).decay(1.5).sustain(0).release(0).trigger(envelopeTrigger).legato(true);
    Generator toneWithDelay = StereoDelay(0.5, 0.75).input(toneWithEnvelope).wetLevel(0.1).feedback(0.2);
    
    synth.setOutputGen( toneWithEnvelope );
    
    ofSoundStreamSetup(2, 0, this, 44100, 256, 4);
    
    fullscreen = false;

    bColorValueView = false;
    
}




//--------------------------------------------------------------
void ofApp::update(){
    
	videoGrabber.update();
	unsigned char * src = videoGrabber.getPixels().getData();
    
    int _drawPixelSize = pixelSize * 1.5;
    
    float _xPos = ofGetWidth()/2.0 - ( (numWidthPixel-1) * _drawPixelSize ) / 2.0;
    float _yPos = ofGetHeight()/2.0 - ( (numHeightPixel-1) * _drawPixelSize ) / 2.0;
    ofPoint _pixelChangePos = ofPoint( _xPos, _yPos );
    

    
    for (int i=0; i<numHeightPixel; i++) {
        for (int j=0; j<numWidthPixel; j++) {
            
            int _index = j * pixelSize * 3 + (numHeightPixel-1-i) * pixelSize * numWidthPixel * pixelSize * 3;

            int _indexPixel = j + i * numWidthPixel;
            
            pixelBlocks[_indexPixel].pixelColor = ofColor(src[_index], src[_index+1], src[_index+2]);
            
            for (int k=0; k<triggerLine.size(); k++) {
                triggerPosOnLines[k] = ( triggerLine[k].stop - triggerLine[k].start ) * triggerMovingFactor[k] + triggerLine[k].start;
                
                pixelBlocks[_indexPixel].contactPixel(triggerPosOnLines[k].x, triggerPosOnLines[k].y);
                
                if (pixelBlocks[_indexPixel].bPixelContact) {
                    pixelBlocks[_indexPixel].pixelColor = ofColor::fromHsb(0, 255, 180, 180);
                    trigger( ofMap(src[_index], 0, 255, 35, 105) );
                }
                
            }

            ofPoint _pixelPos = ofPoint(j*_drawPixelSize, i*_drawPixelSize) + _pixelChangePos;

            pixelBlocks[(numHeightPixel * numWidthPixel)-_indexPixel].pixelPos = _pixelPos;
            pixelBlocks[_indexPixel].pixelSize  = _drawPixelSize;
            pixelBlocks[_indexPixel].pixelMovUpdate();

        }
    }
    
    
    for (int i=0; i<TRIGGER_LINE_NUM; i++) {
        triggerMovingFactor[i] = triggerMovingFactor[i] + triggerMovingSpeed[i];
        if (triggerMovingFactor[i]>1) triggerMovingFactor[i] = 0;
    }
    
    
}




//--------------------------------------------------------------
void ofApp::draw(){
    
    int _drawPixelSize = pixelSize * 1.5;
    
    ofPushMatrix();
    ofPushStyle();
    
    for (int i=0; i<numHeightPixel; i++) {
        for (int j=0; j<numWidthPixel; j++) {
            int _index = j + i * numWidthPixel;
            pixelBlocks[_index].drawPixelRect(_drawPixelSize);
        }
    }


    
    
    if (bColorValueView) {

        for (int i=0; i<numHeightPixel; i++) {
            for (int j=0; j<numWidthPixel; j++) {
                int _index = (j + i * numWidthPixel);
                int _cR = pixelBlocks[_index].pixelColor.r;
                int _cG = pixelBlocks[_index].pixelColor.g;
                int _cB = pixelBlocks[_index].pixelColor.b;
                
                string _c = ofToString(_cR) + " " + ofToString(_cG) + " " + ofToString(_cB);
                ofDrawBitmapString(ofToString(_cR), pixelBlocks[_index].pixelPos);
                ofDrawBitmapString(ofToString(_cG), pixelBlocks[_index].pixelPos + ofVec2f(0, 10));
                ofDrawBitmapString(ofToString(_cB), pixelBlocks[_index].pixelPos + ofVec2f(0, 20));
            }
        }
        
    }

    
    ofPopStyle();
    ofPopMatrix();
    
    drawPreviewLine();
    triggerLineDraw();
    
    
    informationText();
    
//    videoGrabber.draw(0, 0, 160, 120);
    
}





//--------------------------------------------------------------
void ofApp::exit(){
    
    std::exit(0);
    
}





//--------------------------------------------------------------
void ofApp::informationText(){
 
    ofDrawBitmapString( ofToString( ofGetFrameRate(),2), 10, ofGetHeight()-20 );
    ofDrawBitmapString( "f : full screen", 10, ofGetHeight() - 40 );
    ofDrawBitmapString( "r : reset lines", 10, ofGetHeight() - 60 );
    ofDrawBitmapString( "c : reset lines", 10, ofGetHeight() - 80 );
    
}


//--------------------------------------------------------------
void colorValueView(){
    
    
    
}




//--------------------------------------------------------------
bool ofApp::contactPixel(float _x, float _y, float _xD, float _yD){
    
    int _drawPixelSize = pixelSize*1.5;
    
    if (((_x>_xD)&&(_x<_xD+_drawPixelSize))&&((_y>_yD)&&(_y<_yD+_drawPixelSize))) {
        return true;
    } else {
        return false;
    }
    
}



//--------------------------------------------------------------
void ofApp::drawPreviewLine(){
    
    ofPushMatrix();
    ofPushStyle();

    ofSetColor( ofColor::fromHsb(120, 255, 180, 180) );
    ofSetLineWidth(2);
    
    ofDrawLine( touchDownPos, touchUpPos );
    
    ofPopStyle();
    ofPopMatrix();
    
}


//--------------------------------------------------------------
void ofApp::triggerLineDraw(){
    
    ofPushMatrix();
    ofPushStyle();
    ofSetColor( ofColor::fromHsb(0, 255, 180, 180));
    ofSetLineWidth(3);
    
    for (int i=0; i<triggerLine.size(); i++) {
        ofDrawLine( triggerLine[i].start, triggerLine[i].stop );
        
        triggerPosOnLine = ( triggerLine[i].stop - triggerLine[i].start ) * triggerMovingFactor[i] + triggerLine[i].start;
        
        //        for (int i=0; i<noteBlock.size(); i++) {
        //            noteBlock[i].contact(triggerPosOnLine);
        //            if (noteBlock[i].contactBlock) {
        //                trigger( noteBlock.at(i).midiPitch );
        //            }
        //        }
        
        ofPushStyle();
        ofDrawCircle( triggerPosOnLine.x, triggerPosOnLine.y, 10, 10 );
        ofPopStyle();
    }
    
    ofPopStyle();
    ofPopMatrix();
    
}


//--------------------------------------------------------------
void ofApp::triggerLinePixel(){
    
    for (int i=0; i<triggerLine.size(); i++) {
        triggerPosOnLine = ( triggerLine[i].stop - triggerLine[i].start ) * triggerMovingFactor[i] + triggerLine[i].start;
        
        for (int j=0; j<pixelBlocks.size(); j++) {
            pixelBlocks[j].contactPixel(triggerPosOnLine.x, triggerPosOnLine.y);
            pixelBlocks[j].pixelColorUpdate();
        }
    }
    
}



//--------------------------------------------------------------
void ofApp::trigger(int _pitch){
    synth.setParameter("midiNumber", _pitch);
    synth.setParameter("trigger", 1);
}



//--------------------------------------------------------------
void ofApp::setScaleDegreeBasedOnMouseX(){
    //    int newScaleDegree = ofGetMouseX() * NUMBER_OF_KEYS / ofGetWindowWidth();
    //    if(ofGetMousePressed() && ( newScaleDegree != scaleDegree )){
    //        scaleDegree = newScaleDegree;
    //        trigger();
    //    }else{
    //        scaleDegree = newScaleDegree;
    //    }
}


//--------------------------------------------------------------
//void ofApp::keyPressed(int key){
//
//
//	if (key == ' '){
//		startPlaying();
//	}
//
//
//	if (key == OF_KEY_RETURN)
//		stopPlaying();
//
//	if (key == OF_KEY_UP){
//		if (midiEvents.ticksPerScreen >= 4000)
//            midiEvents.ticksPerScreen += 2000;
//		else
//			midiEvents.ticksPerScreen += 500;
//	}
//
//	if (key == 'm'){
//        //		midiEvents.findMatch(84, 0, 10000);
//	}
//
//	if (key == OF_KEY_DOWN){
//		if (midiEvents.ticksPerScreen >= 4000)
//            midiEvents.ticksPerScreen -= 2000;
//        else if (midiEvents.ticksPerScreen > 500)
//            midiEvents.ticksPerScreen -= 500;
//	}
//
//	if (key == 'w')
//		midiEvents.printMatchMatrix();
//
//	if (key == 'p'){
//		midiEvents.printNotes();
//	}
//
//	if (key == 'o'){
//		//open audio file
//		string *filePtr;
//		filePtr = &midiFileName;
//
//		if (getFilenameFromDialogBox(filePtr)){
//			printf("Midifile: Loaded name okay :\n'%s' \n", midiFileName.c_str());
//			cannamMainFunction();
//		}
//
//	}
//
//
//    if (key=='x') {
//        triggerLine.clear();
//    }
//
//}

//--------------------------------------------------------------
//void ofApp::keyReleased(int key){
//
//    if (key=='r') {
//        reload = true;
//        noteBlock.clear();
//        int retVal = cannamMainFunction();
//    }
//
//}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if (key=='f') {

        fullscreen = !fullscreen;
        ofSetFullscreen(fullscreen);

    } else if (key == 'r' ) {
    
        triggerLine.clear();
        touchUpPos.x = 0;
        touchUpPos.y = 0;
        touchDownPos.x = 0;
        touchDownPos.y = 0;
        
    } else if (key == 'c') {
        bColorValueView = !bColorValueView;
    }
    
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    touchUpPos.x = x;
    touchUpPos.y = y;
    touchDownPos.x = x;
    touchDownPos.y = y;
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    touchMov.x = x;
    touchMov.y = y;
    
    touchUpPos.x = x;
    touchUpPos.y = y;
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    TriggerLine triggerLine_e;
    triggerLine_e.start.x = touchDownPos.x;
    triggerLine_e.start.y = touchDownPos.y;
    triggerLine_e.stop.x = touchUpPos.x;
    triggerLine_e.stop.y = touchUpPos.y;
    triggerLine.push_back(triggerLine_e);
    
    if (triggerLine.size()>8) {
        triggerLine.erase(triggerLine.begin());
    }
    
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



//--------------------------------------------------------------
void ofApp::startPlaying(){
	playing = !playing;
}

//--------------------------------------------------------------
void ofApp::stopPlaying(){
	playing = false;
}


//--------------------------------------------------------------
void ofApp::audioRequested (float * output, int bufferSize, int nChannels){
    synth.fillBufferOfFloats(output, bufferSize, nChannels);
}
