//
//  PixelBlock.h
//  lineSeqMus
//
//  Created by JH.Park on 23.03.14.
//
//
#pragma once

#ifndef __lineSeqMus__PixelBlock__
#define __lineSeqMus__PixelBlock__

#include "ofMain.h"

class PixelBlock{
    
public:
    
    PixelBlock();
    
    void setup();
    void pixelMovUpdate();
    void pixelColorUpdate();
    void drawPixelRect(int _size);
    void contact();
    
    
    void contactPixel(float _x, float _y);
    
    ofPoint pixelPos;
    ofColor pixelColor;
    
    ofVec3f rotXYZ;
    
    bool bRotateX;
    
    float pixelSize;
    
    ofPoint rPixelPos;
    float rPixelSize;

    ofColor pixelColorChange;
    bool bPixelContact;
    
};


#endif /* defined(__lineSeqMus__PixelBlock__) */
