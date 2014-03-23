//
//  PixelBlock.cpp
//  lineSeqMus
//
//  Created by JH.Park on 23.03.14.
//
//

#include "PixelBlock.h"


float PixelBlock::pixelSize;
ofPoint PixelBlock::touchPos;
ofPoint PixelBlock::sPixelPos;


PixelBlock::PixelBlock(){

    ofAddListener(ofEvents().touchMoved, this, &PixelBlock::touchMoved);
    rotXYZ.x = 1;
}


void PixelBlock::setup(int _size){
    
}

void PixelBlock::pixelMovUpdate(){
    
    rotXYZ.x *= 0.97;
}


void PixelBlock::pixelUpdate(ofPoint _pos, ofColor _c){
    

}


void PixelBlock::drawPixelRect(int _size){
    
    ofTranslate( pixelPos.x, pixelPos.y );

    ofPushMatrix();
    ofPushStyle();
    
    
    if(contactPixel(touchPos.x, touchPos.y)){
        rotXYZ.x *= 0.96;
        ofRotateX(ofRadToDeg(PI*0.5));
    } else {
        rotXYZ.x = 0;
        ofRotateX(ofRadToDeg(0));
    }
    ofTranslate( 0, -_size/2 );

    ofSetColor( pixelColor );
    
    ofRect( 0, 0, _size, _size);

    ofPopStyle();
    ofPopMatrix();

    pixelSize = _size;
    sPixelPos = pixelPos;

    
}


void PixelBlock::contact(){
    
}

void PixelBlock::touchMoved(ofTouchEventArgs & touch){
    
    if (contactPixel(touch.x, touch.y)) {
        bRotateX = true;
        rotXYZ.x = 1;
    }
    
    touchPos.x = touch.x;
    touchPos.y = touch.y;
}


bool PixelBlock::contactPixel(float _x, float _y){

    if (((_x>sPixelPos.x)&&(_x<sPixelPos.x+pixelSize))&&((_y>sPixelPos.y)&&(_y<sPixelPos.y+pixelSize))) {
        return true;
    } else {
        return false;
    }
    
}

