/*
 *  drawMidiNotes.h
 *  midiCannamReader
 *
 *  Created by Andrew on 17/07/2011.
 *  Copyright 2011 QMUL. All rights reserved.
 *
 */


#include "ofMain.h"

class drawMidiNotes{
public:
	drawMidiNotes();
	void updatePlayPosition();
	
	typedef std::vector<double> DoubleVector;
	typedef std::vector<DoubleVector> DoubleMatrix;
	
	DoubleMatrix beatPeriodMatrix;
	
	typedef std::vector<int> IntVector;
	typedef std::vector<IntVector> IntMatrix;
	
	void drawFile(const IntMatrix& noteOnMatrix);
	void reset();
	
	int ticksPerScreen;
	int tickLocation;
	int noteArrayIndex;
	
	int noteMinimum, noteMaximum;
	int screenWidth, screenHeight;
	float noteHeight;
	float tempo;
	double period;
	int pulsesPerQuarternote;
	double lastPeriodUpdateTime;
					 
	
};