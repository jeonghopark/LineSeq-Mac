//
//  NoteBlock.h
//  timeLineTrigger
//
//  Created by JeongHoPark on 20.11.13.
//
//

#ifndef __timeLineTrigger__NoteBlock__
#define __timeLineTrigger__NoteBlock__

#include "ofMain.h"

class NoteBlock{
    
public:
    NoteBlock();
    ~NoteBlock();
    
    void update();
    void noteInput(int pitch, int velocity, int duration);
    void contact(ofVec3f input);
    void drawing();
    
    int midiPitch;
    int midiVelocity;
    int midiDuration;

    int blockRotationZ;
    
    ofVec3f onPos;
    ofVec3f offPos;
    
    bool contactBlock;
    
    ofVec3f pos1, pos2, pos3, pos4;
    
};

#endif /* defined(__timeLineTrigger__NoteBlock__) */
