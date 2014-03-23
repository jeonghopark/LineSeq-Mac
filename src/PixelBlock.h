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
    
    void setup(int _size);
    void pixelMovUpdate();
    void pixelUpdate(ofPoint _pos, ofColor _c);
    void drawPixelRect(int _size);
    void contact();
    
    
    bool contactPixel(float _x, float _y);
    
    ofPoint pixelPos;
    ofColor pixelColor;
    
    ofVec3f rotXYZ;
    
    void touchMoved(ofTouchEventArgs & touch);
  
//    float pixelSize;
    float rPixelSize;
    bool bRotateX;

    static float pixelSize;
    static ofPoint touchPos;
    static ofPoint sPixelPos;



};


#endif /* defined(__lineSeqMus__PixelBlock__) */
