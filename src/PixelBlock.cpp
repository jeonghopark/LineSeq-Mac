//
//  PixelBlock.cpp
//  lineSeqMus
//
//  Created by JH.Park on 23.03.14.
//
//

#include "PixelBlock.h"

PixelBlock::PixelBlock(){

}


void PixelBlock::setup(){
    pixelColorChange = pixelColor;
}

void PixelBlock::pixelMovUpdate(){
    
    rPixelSize = pixelSize;
    rPixelPos = pixelPos;

}

void PixelBlock::pixelColorUpdate(){
    
    if (bPixelContact) pixelColorChange = ofColor_<unsigned char>::black;
    else pixelColorChange = pixelColor;

}

void PixelBlock::drawPixelRect(int _size){
    
    ofPushMatrix();
    ofPushStyle();
    
    if (bPixelContact) {
        ofSetColor( 0 );
    } else {
    }
    
    ofSetColor( pixelColor );

    ofRect( pixelPos.x, pixelPos.y, _size, _size);

    ofPopStyle();
    ofPopMatrix();

}


void PixelBlock::contact(){
    
}


void PixelBlock::contactPixel(float _x, float _y){

    if (((_x>rPixelPos.x)&&(_x<rPixelPos.x+rPixelSize))&&((_y>rPixelPos.y)&&(_y<rPixelPos.y+rPixelSize))) {
        bPixelContact = true;
    } else {
        bPixelContact = false;
    }
    
}

