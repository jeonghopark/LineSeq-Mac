//
//  NoteBlock.cpp
//  timeLineTrigger
//
//  Created by JeongHoPark on 20.11.13.
//
//

#include "NoteBlock.h"

NoteBlock::NoteBlock(){
    onPos = ofVec3f( ofRandom( 100, 1024-100 ), ofRandom( 50, 768-50 ), 0 );
}

NoteBlock::~NoteBlock(){
    
}

void NoteBlock::update(){
    
}

void NoteBlock::noteInput(int _pitch, int _velocity, int _duration){
    
    midiPitch = _pitch;
    midiVelocity = _velocity;
    blockRotationZ = round(ofRandom( 1 ));
    
    if (blockRotationZ==0) {
        if (onPos.x>1024/2) {
            offPos.x =  onPos.x - _duration * 0.3;
            offPos.y = onPos.y - ofMap( _velocity, 0, 127, 0, 5 );
            
            pos1 = ofVec3f( offPos.x, offPos.y, 0 );
            pos2 = ofVec3f( onPos.x, offPos.y, 0 );
            pos3 = ofVec3f( onPos.x, onPos.y, 0 );
            pos4 = ofVec3f( offPos.x, onPos.y, 0 );
            
        } else {
            offPos.x = _duration * 0.3 + onPos.x;
            offPos.y = ofMap( _velocity, 0, 127, 0, 5 ) + onPos.y;
            
            pos1 = ofVec3f( onPos.x, onPos.y, 0 );
            pos2 = ofVec3f( offPos.x, onPos.y, 0 );
            pos3 = ofVec3f( offPos.x, offPos.y, 0 );
            pos4 = ofVec3f( onPos.x, offPos.y, 0 );
        }
        
    } else {
        if (onPos.y>768/2) {
            offPos.x = ofMap( _velocity, 0, 127, 0, 5 ) + onPos.x;
            offPos.y = onPos.y - _duration * 0.3;
            
            pos1 = ofVec3f( onPos.x, offPos.y, 0 );
            pos2 = ofVec3f( offPos.x, offPos.y, 0 );
            pos3 = ofVec3f( offPos.x, onPos.y, 0 );
            pos4 = ofVec3f( onPos.x, onPos.y, 0 );
            
        } else {
            offPos.x = ofMap( _velocity, 0, 127, 0, 5 ) + onPos.x;
            offPos.y = _duration * 0.3 + onPos.y;
            
            pos1 = ofVec3f( onPos.x, onPos.y, 0 );
            pos2 = ofVec3f( offPos.x, onPos.y, 0 );
            pos3 = ofVec3f( offPos.x, offPos.y, 0 );
            pos4 = ofVec3f( onPos.x, offPos.y, 0 );
        }
    }
    
    
}

void NoteBlock::contact(ofVec3f _input){
    
    if (blockRotationZ==0) {
        if ( ((_input.x>pos1.x)&&(_input.x<pos3.x))&&((_input.y>pos1.y-(pos3.y-pos1.y))&&(_input.y<pos3.y+(pos3.y-pos1.y))) ) {
            contactBlock = true;
        }
    } else {
        if ( ((_input.x>pos1.x-(pos3.x-pos1.x))&&(_input.x<pos3.x+(pos3.x-pos1.x)))&&((_input.y>pos1.y)&&(_input.y<pos3.y)) ) {
            contactBlock = true;
        }
    }
    
}

void NoteBlock::drawing(){
    ofPushMatrix();
    ofPushStyle();
    
    if (contactBlock) {
        ofSetColor( ofColor::fromHsb(0, 0, 255, ofMap(midiVelocity,0,127,0,255)) );
        contactBlock = !contactBlock;
    } else {
        ofSetColor( ofColor::fromHsb(0, 0, 0, ofMap(midiVelocity,0,127,0,180)) );
    }
    
    ofBeginShape();
    ofVertex( pos1 );
    ofVertex( pos2 );
    ofVertex( pos3 );
    ofVertex( pos4 );
    ofEndShape();
    
    //    ofEllipse(pos1, 10, 10);
    //    ofEllipse(pos2, 10, 10);
    //    ofEllipse(pos3, 10, 10);
    //    ofEllipse(pos4, 10, 10);
    
    ofPopStyle();
    ofPopMatrix();
    
}