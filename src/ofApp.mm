#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(10);
    ofSetFrameRate(60);
    ofSetOrientation(OF_ORIENTATION_90_RIGHT);
    
    ofxAccelerometer.setup();               //accesses accelerometer data
    ofxiPhoneAlerts.addListener(this);      //allows elerts to appear while app is running
    //	ofRegisterTouchEvents(this);            //method that passes touch events
    
    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationFade];
    
    pixelNum = 20;
    pixelSize = 20;
    videoGrabber.setDeviceID(1);
    videoGrabber.initGrabber(480, 360);
    widthRes = videoGrabber.getWidth()/pixelNum;
    heightRes = videoGrabber.getHeight()/pixelNum;
    numWidthPixel = videoGrabber.getWidth()/pixelSize;
    numHeightPixel = videoGrabber.getHeight()/pixelSize;
    
	tex.allocate(numWidthPixel, numHeightPixel, GL_RGB);
	
	pix = new unsigned char[ (int)( numWidthPixel * numHeightPixel * 3.0) ];
    
	midiFileName = "entertainer.mid";
	int retVal = cannamMainFunction();
    
	playing = false;
    
	screenWidth = 1024;
	screenHeight = 768;
	midiEvents.screenWidth = &screenWidth;
	midiEvents.screenHeight = &screenHeight;
    
    
    triggerMovingFactor = 0;
    
    
    // Tonic
    ofSoundStreamSetup(2, 0, this, 44100, 256, 4);
    
    ControlGenerator midiNote = synth.addParameter("midiNumber");
    ControlGenerator noteFreq =  ControlMidiToFreq().input(midiNote);
    Generator tone = RectWave().freq( noteFreq );
    tone = LPF12().input(tone).Q(10).cutoff((noteFreq * 2) + SineWave().freq(10) * 0.5 * noteFreq);
    
    ControlGenerator envelopeTrigger = synth.addParameter("trigger");
    Generator toneWithEnvelope = tone * ADSR().attack(0).decay(1.5).sustain(0).release(0).trigger(envelopeTrigger).legato(true);
    Generator toneWithDelay = StereoDelay(0.5, 0.75).input(toneWithEnvelope).wetLevel(0.1).feedback(0.2);
    
    synth.setOutputGen( toneWithEnvelope );
    
    pixelBlocks.resize(numWidthPixel * numHeightPixel);

    
}

//--------------------------------------------------------------
void ofApp::update(){
    
	videoGrabber.update();
	
	unsigned char * src = videoGrabber.getPixels();
	int totalPix = videoGrabber.getWidth() * videoGrabber.getHeight() * 3;
    
    pixelColor.clear();
    int _pixIndex=0;

//    for (int i=0; i<videoGrabber.getHeight(); i+=pixelSize) {
//        for (int j=0; j<videoGrabber.getWidth(); j+=pixelSize) {
//            int _index = (j + i * numWidthPixel * pixelSize) * 3;
//            pix[_pixIndex  ] = src[_index];
//            pix[_pixIndex+1] = src[_index+1];
//            pix[_pixIndex+2] = src[_index+2];
//            _pixIndex += 3;
//        }
//    }
    
    
    for (int i=videoGrabber.getHeight()-1; i>0; i-=pixelSize) {
        for (int j=0; j<videoGrabber.getWidth(); j+=pixelSize) {
            int _index = (j + i * numWidthPixel * pixelSize) * 3;
            pixelColor.push_back(src[_index]);
            pixelColor.push_back(src[_index+1]);
            pixelColor.push_back(src[_index+2]);
        }
    }
    
    PixelBlock _pixelBlockE;
//    pixelBlocks.clear();
    int _drawPixelSize = pixelSize*1.5;

    
    int _pixelIndex = 0;
    for (int i=0; i<numHeightPixel; i++) {
        for (int j=0; j<numWidthPixel; j++) {
            
            int _index = j * pixelSize * 3 + (numHeightPixel-1-i) * pixelSize * numWidthPixel * pixelSize * 3;
            
//            _pixelBlockE.pixelColor = ofColor(src[_index], src[_index+1], src[_index+2]);
//            _pixelBlockE.pixelPos = ofPoint(j*_drawPixelSize, i*_drawPixelSize);
//            pixelBlocks.push_back(_pixelBlockE);
            
            pixelBlocks[_pixelIndex].pixelColor = ofColor(src[_index], src[_index+1], src[_index+2]);
            pixelBlocks[_pixelIndex].pixelPos = ofPoint(j*_drawPixelSize, i*_drawPixelSize);
            _pixelIndex++;
        }
    }

    for (int i=0; i<pixelBlocks.size(); i++) {
        pixelBlocks[i].pixelMovUpdate();
    }

    cout << pixelBlocks.size() << endl;
    
    pixelNote = pixelColor;


    //	for(int k = 0; k < totalPix; k+= 3){
    //		pix[k  ] = 255 - src[k];
    //		pix[k+1] = 255 - src[k+1];
    //		pix[k+2] = 255 - src[k+2];
    //	}
	
    tex.loadData(pix, tex.getWidth(), tex.getHeight(), GL_RGB);
    
    // MIDI
	if (playing){
		midiEvents.updatePlayPosition();
    }
    
    triggerMovingFactor = triggerMovingFactor + 0.012;
    if (triggerMovingFactor>1) triggerMovingFactor = 0;
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //    midiEvents.drawFile();
    //    tex.draw( 0, 0, ofGetWidth(), ofGetHeight() );
    
    int _drawPixelSize = pixelSize*1.5;
    
    ofPushMatrix();
    float _xPos = ofGetWidth()/2-((numWidthPixel-1)*_drawPixelSize)/2;
    float _yPos = ofGetHeight()/2-((numHeightPixel-1)*_drawPixelSize)/2;
    ofTranslate( _xPos, _yPos );
    ofPushStyle();
    for (int i=0; i<numHeightPixel; i++) {
        for (int j=0; j<numWidthPixel; j++) {
            int _index = (j + i * numWidthPixel);
            ofPushMatrix();
            pixelBlocks[_index].drawPixelRect(_drawPixelSize);
            ofPopMatrix();
        }
    }
    ofPopStyle();
    ofPopMatrix();
    
    drawPreviewLine();
    triggerLineDraw();
    
    //    for (int i=0; i<noteBlock.size(); i++) {
    //        noteBlock[i].drawing();
    //    }
    
    ofDrawBitmapString( ofToString( ofGetFrameRate(),2), 10, ofGetHeight()-20 );
    
//    videoGrabber.draw(0, 0, 160, 120);
}


void ofApp::randomMidiInput(int _p, int _v, int _d){
    
    NoteBlock noteBlock_e;
    noteBlock_e.noteInput(_p, _v, _d);
    noteBlock.push_back(noteBlock_e);
    
}



void ofApp::drawPreviewLine(){
    
    ofPushMatrix();
    ofPushStyle();
    ofSetColor( ofColor::fromHsb(0, 255, 255, 150) );
    ofSetLineWidth(3);
    
    ofLine( touchDownPos, touchUpPos );
    
    ofPopStyle();
    ofPopMatrix();
}


void ofApp::triggerLineDraw(){
    
    ofPushMatrix();
    ofPushStyle();
    ofSetColor( ofColor::fromHsb(0, 255, 255, 150) );
    ofSetLineWidth(3);
    for (int i=0; i<triggerLine.size(); i++) {
        ofLine( triggerLine[i].start, triggerLine[i].stop );
        
        triggerPosOnLine = ( triggerLine[i].stop - triggerLine[i].start ) * triggerMovingFactor + triggerLine[i].start;
        
        for (int i=0; i<noteBlock.size(); i++) {
            noteBlock[i].contact(triggerPosOnLine);
            if (noteBlock[i].contactBlock) {
                trigger( noteBlock.at(i).midiPitch );
            }
        }
        
        ofPushStyle();
        ofEllipse( triggerPosOnLine.x, triggerPosOnLine.y, 10, 10 );
        
        ofPopStyle();
    }
    
    ofPopStyle();
    ofPopMatrix();
}


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
//void ofApp::mouseMoved(int x, int y ){
//
//}
//
////--------------------------------------------------------------
//void ofApp::mouseDragged(int x, int y, int button){
//    touchUpPos.x = x;
//    touchUpPos.y = y;
//}
//
////--------------------------------------------------------------
//void ofApp::mousePressed(int x, int y, int button){
//    touchUpPos.x = x;
//    touchUpPos.y = y;
//    touchDownPos.x = x;
//    touchDownPos.y = y;
//}
//
////--------------------------------------------------------------
//void ofApp::mouseReleased(int x, int y, int button){
//    TriggerLine triggerLine_e;
//    triggerLine_e.start.x = touchDownPos.x;
//    triggerLine_e.start.y = touchDownPos.y;
//    triggerLine_e.stop.x = touchUpPos.x;
//    triggerLine_e.stop.y = touchUpPos.y;
//    triggerLine.push_back(triggerLine_e);
//
//    if (triggerLine.size()>9) {
//        triggerLine.erase(triggerLine.begin());
//    }
//}
//
////--------------------------------------------------------------
//void ofApp::windowResized(int w, int h){
//	screenWidth = w;
//	screenHeight = h;
//	midiEvents.noteHeight = screenHeight / (float)(midiEvents.noteMaximum - midiEvents.noteMinimum);
//
//}


//--------------------------------------------------------------
void ofApp::exit(){
    pix = NULL;
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
    
    
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
    
    
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::lostFocus(){
    
}

//--------------------------------------------------------------
void ofApp::gotFocus(){
    
}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){
    
}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
    
}


void ofApp::startPlaying(){
	playing = !playing;
	midiEvents.reset();
	midiEvents.setStartPlayingTimes();
	//this is where we stop and start playing
}

void ofApp::stopPlaying(){
	playing = false;
}

bool ofApp::getFilenameFromDialogBox(string* fileNameToSave){
	//this uses a pointer structure within the loader and returns true if the dialogue box was used successfully
	// first, create a string that will hold the URL
	string URL;
    
	// openFile(string& URL) returns 1 if a file was picked
	// returns 0 when something went wrong or the user pressed 'cancel'
    //	int response = ofxFileDialogOSX::openFile(URL);
    //	if(response){
    //		// now you can use the URL
    //		*fileNameToSave = URL;
    //		//printf("\n filename is %s \n", soundFileName.c_str());
    //		return true;
    //	}
    //	else {
    //		//	soundFileName = "OPEN canceled. ";
    //		printf("\n open file cancelled \n");
    //		return false;
    //	}
    
    
    
}




int ofApp::cannamMainFunction(){
    
    
	midiEvents.recordedNoteOnMatrix.clear();//where we store the note ons
    
	//int main(int argc, char **argv)
	//{
	//	if (argc != 2) {
	//		cerr << "Usage: midifile <file.mid>" << endl;
	//		return 1;
	//	}
    
	std::string filename = midiFileName;//argv[1];
    
    MIDIFileReader fr( ofToDataPath(filename));
    if (!fr.isOK()) {
        std::cerr << "Error: " << fr.getError().c_str() << std::endl;
        return 1;
    }
    
    MIDIComposition c = fr.load();
    
    switch (fr.getFormat()) {
        case MIDI_SINGLE_TRACK_FILE: cout << "Format: MIDI Single Track File" << endl; break;
        case MIDI_SIMULTANEOUS_TRACK_FILE: cout << "Format: MIDI Simultaneous Track File" << endl; break;
        case MIDI_SEQUENTIAL_TRACK_FILE: cout << "Format: MIDI Sequential Track File" << endl; break;
        default: cout << "Format: Unknown MIDI file format?" << endl; break;
    }
    
    cout << "Tracks: " << c.size() << endl;
    
    int td = fr.getTimingDivision();
    if (td < 32768) {
        cout << "Timing division: " << fr.getTimingDivision() << " ppq" << endl;
        
        midiEvents.pulsesPerQuarternote = fr.getTimingDivision();
    } else {
        int frames = 256 - (td >> 8);
        int subframes = td & 0xff;
        cout << "SMPTE timing: " << frames << " fps, " << subframes << " subframes" << endl;
    }
    
    for (MIDIComposition::const_iterator i = c.begin(); i != c.end(); ++i) {
        
        cout << "Start of track: " << i->first+1 << endl;
        
        for (MIDITrack::const_iterator j = i->second.begin(); j != i->second.end(); ++j) {
            
            unsigned int t = j->getTime();
            int ch = j->getChannelNumber();
            
            if (j->isMeta()) {
                int code = j->getMetaEventCode();
                string name;
                bool printable = true;
                switch (code) {
                        
                    case MIDI_END_OF_TRACK:
                        cout << t << ": End of track" << endl;
                        break;
                        
                    case MIDI_TEXT_EVENT: name = "Text"; break;
                    case MIDI_COPYRIGHT_NOTICE: name = "Copyright"; break;
                    case MIDI_TRACK_NAME: name = "Track name"; break;
                    case MIDI_INSTRUMENT_NAME: name = "Instrument name"; break;
                    case MIDI_LYRIC: name = "Lyric"; break;
                    case MIDI_TEXT_MARKER: name = "Text marker"; break;
                    case MIDI_SEQUENCE_NUMBER: name = "Sequence number"; printable = false; break;
                    case MIDI_CHANNEL_PREFIX_OR_PORT: name = "Channel prefix or port"; printable = false; break;
                    case MIDI_CUE_POINT: name = "Cue point"; break;
                    case MIDI_CHANNEL_PREFIX: name = "Channel prefix"; printable = false; break;
                    case MIDI_SEQUENCER_SPECIFIC: name = "Sequencer specific"; printable = false; break;
                    case MIDI_SMPTE_OFFSET: name = "SMPTE offset"; printable = false; break;
                        
                    case MIDI_SET_TEMPO:
                    {
                        int m0 = j->getMetaMessage()[0];
                        int m1 = j->getMetaMessage()[1];
                        int m2 = j->getMetaMessage()[2];
                        long tempo = (((m0 << 8) + m1) << 8) + m2;
                        
                        cout << t << ": Tempo: " << 60000000.0 / double(tempo) << endl;
                        midiEvents.tempo = 60000000.0 / double(tempo);
                        midiEvents.period = double(tempo)/1000.0;
                        
                        printf("period double is %f\n", midiEvents.period);
                    }
                        break;
                        
                    case MIDI_TIME_SIGNATURE:
                    {
                        int numerator = j->getMetaMessage()[0];
                        int denominator = 1 << (int)j->getMetaMessage()[1];
                        
                        cout << t << ": Time signature: " << numerator << "/" << denominator << endl;
                    }
                        
                    case MIDI_KEY_SIGNATURE:
                    {
                        int accidentals = j->getMetaMessage()[0];
                        int isMinor = j->getMetaMessage()[1];
                        bool isSharp = accidentals < 0 ? false : true;
                        accidentals = accidentals < 0 ? -accidentals : accidentals;
                        cout << t << ": Key signature: " << accidentals << " "
                        << (isSharp ?
                            (accidentals > 1 ? "sharps" : "sharp") :
                            (accidentals > 1 ? "flats" : "flat"))
                        << (isMinor ? ", minor" : ", major") << endl;
                    }
                        
                }
                
                
                if (name != "") {
                    if (printable) {
                        cout << t << ": File meta event: code " << code
                        << ": " << name << ": \"" << j->getMetaMessage()
                        << "\"" << endl;
                    } else {
                        cout << t << ": File meta event: code " << code
                        << ": " << name << ": ";
                        for (int k = 0; k < j->getMetaMessage().length(); ++k) {
                            cout << (int)j->getMetaMessage()[k] << " ";
                        }
                    }
                }
                continue;
            }
            
            switch (j->getMessageType()) {
                    
                case MIDI_NOTE_ON:
                    cout << t << ": Note: channel " << ch
                    << " duration " << j->getDuration()
                    << " pitch " << j->getPitch()
                    << " velocity " << j->getVelocity()
                    << "event time " << midiEvents.getEventTimeMillis(t) << endl;
                    v.clear();
                    v.push_back(t);
                    v.push_back(j->getPitch());
                    v.push_back(j->getVelocity());
                    v.push_back(j->getDuration());
                    midiEvents.recordedNoteOnMatrix.push_back(v);
                    midiEvents.recordedEventTimes.push_back(midiEvents.getEventTimeMillis(t));
                    
                    randomMidiInput( j->getPitch(), j->getVelocity(), j->getDuration() );
                    
                    break;
                    
                case MIDI_POLY_AFTERTOUCH:
                    cout << t << ": Polyphonic aftertouch: channel " << ch
                    << " pitch " << j->getPitch()
                    << " pressure " << j->getData2() << endl;
                    break;
                    
                case MIDI_CTRL_CHANGE:
                {
                    int controller = j->getData1();
                    string name;
                    switch (controller) {
                        case MIDI_CONTROLLER_BANK_MSB: name = "Bank select MSB"; break;
                        case MIDI_CONTROLLER_VOLUME: name = "Volume"; break;
                        case MIDI_CONTROLLER_BANK_LSB: name = "Bank select LSB"; break;
                        case MIDI_CONTROLLER_MODULATION: name = "Modulation wheel"; break;
                        case MIDI_CONTROLLER_PAN: name = "Pan"; break;
                        case MIDI_CONTROLLER_SUSTAIN: name = "Sustain"; break;
                        case MIDI_CONTROLLER_RESONANCE: name = "Resonance"; break;
                        case MIDI_CONTROLLER_RELEASE: name = "Release"; break;
                        case MIDI_CONTROLLER_ATTACK: name = "Attack"; break;
                        case MIDI_CONTROLLER_FILTER: name = "Filter"; break;
                        case MIDI_CONTROLLER_REVERB: name = "Reverb"; break;
                        case MIDI_CONTROLLER_CHORUS: name = "Chorus"; break;
                        case MIDI_CONTROLLER_NRPN_1: name = "NRPN 1"; break;
                        case MIDI_CONTROLLER_NRPN_2: name = "NRPN 2"; break;
                        case MIDI_CONTROLLER_RPN_1: name = "RPN 1"; break;
                        case MIDI_CONTROLLER_RPN_2: name = "RPN 2"; break;
                        case MIDI_CONTROLLER_SOUNDS_OFF: name = "All sounds off"; break;
                        case MIDI_CONTROLLER_RESET: name = "Reset"; break;
                        case MIDI_CONTROLLER_LOCAL: name = "Local"; break;
                        case MIDI_CONTROLLER_ALL_NOTES_OFF: name = "All notes off"; break;
                    }
                    cout << t << ": Controller change: channel " << ch
                    << " controller " << j->getData1();
                    if (name != "") cout << " (" << name << ")";
                    cout << " value " << j->getData2() << endl;
                }
                    break;
                    
                case MIDI_PROG_CHANGE:
                    cout << t << ": Program change: channel " << ch
                    << " program " << j->getData1() << endl;
                    break;
                    
                case MIDI_CHNL_AFTERTOUCH:
                    cout << t << ": Channel aftertouch: channel " << ch
                    << " pressure " << j->getData1() << endl;
                    break;
                    
                case MIDI_PITCH_BEND:
                    cout << t << ": Pitch bend: channel " << ch
                    << " value " << (int)j->getData2() * 128 + (int)j->getData1() << endl;
                    break;
                    
                case MIDI_SYSTEM_EXCLUSIVE:
                    cout << t << ": System exclusive: code "
                    << (int)j->getMessageType() << " message length " <<
                    j->getMetaMessage().length() << endl;
                    break;
            }
        }
    }
    
	//}
}//end cannam midi main


void ofApp::audioRequested (float * output, int bufferSize, int nChannels){
    synth.fillBufferOfFloats(output, bufferSize, nChannels);
}
