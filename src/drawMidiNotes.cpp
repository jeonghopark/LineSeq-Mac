/*
 *  drawMidiNotes.cpp
 *  midiCannamReader
 *
 *  Created by Andrew on 17/07/2011.
 *  Copyright 2011 QMUL. All rights reserved.
 *
 */

#include "drawMidiNotes.h"

drawMidiNotes::drawMidiNotes(){

	
	ticksPerScreen = 8000;
	tickLocation = 0;
	pulsesPerQuarternote = 240;
	noteArrayIndex = 0;
	noteMinimum = 30;
	noteMaximum = 96;
	screenWidth = 1024;
	screenHeight = 768;
	noteHeight = screenHeight / (float)(noteMaximum - noteMinimum);
}

void drawMidiNotes::reset(){
	noteArrayIndex = 0;
	tickLocation = 0;
	lastPeriodUpdateTime = ofGetElapsedTimeMillis();
	
}

void drawMidiNotes::updatePlayPosition(){
	double timeDifference = ofGetElapsedTimeMillis() - lastPeriodUpdateTime;
	//this is time diff in milliseconds
	//then we have 
	double quarterNoteIntervals = (timeDifference / period);
	tickLocation = quarterNoteIntervals * pulsesPerQuarternote; 

}

void drawMidiNotes::drawFile(const IntMatrix& noteOnMatrix){
	int size = noteOnMatrix.size();
	if (size > 0){

		int numberOfScreensIn = tickLocation / ticksPerScreen;
		
	while (noteArrayIndex < noteOnMatrix.size() && tickLocation > noteOnMatrix[noteArrayIndex][0] )
		noteArrayIndex++;
	
	while (noteArrayIndex > 0 && noteArrayIndex < size && tickLocation < noteOnMatrix[noteArrayIndex][0])
		noteArrayIndex--;
		
		//need to start where we currently are in file
		int maxNoteIndexToPrint	= noteArrayIndex;
		int minNoteIndexToPrint = noteArrayIndex;
		
		
		while (maxNoteIndexToPrint < noteOnMatrix.size() && noteOnMatrix[maxNoteIndexToPrint][0] < (numberOfScreensIn+1)*ticksPerScreen )
			maxNoteIndexToPrint++;
		
		while (minNoteIndexToPrint > 0 && minNoteIndexToPrint < size && noteOnMatrix[minNoteIndexToPrint][0] > numberOfScreensIn*ticksPerScreen)
			minNoteIndexToPrint--;
		
		
		for (int tmpIndex = minNoteIndexToPrint;tmpIndex < maxNoteIndexToPrint;tmpIndex++){
		int xLocation = (float)(noteOnMatrix[tmpIndex][0] - numberOfScreensIn*ticksPerScreen)*screenWidth/(float)ticksPerScreen;
			int duration = (float)(noteOnMatrix[tmpIndex][3]*screenWidth)/(float)ticksPerScreen;
			
			
		int yLocation = screenHeight - ((noteOnMatrix[tmpIndex][1] - noteMinimum )*screenHeight/ (float)(noteMaximum - noteMinimum));						 
		ofRect(xLocation,yLocation, duration,  noteHeight);
		
		}
		
			int xLocation = (float)(tickLocation - numberOfScreensIn*ticksPerScreen)*screenWidth/(float)ticksPerScreen;
		ofLine(xLocation, 0, xLocation, screenHeight);
	
	//	if (noteArrayIndex < size )
	//		printf("tick %i :: note array :%i: %i\n", tickLocation, noteArrayIndex, noteOnMatrix[noteArrayIndex][0]);
	//	else
	//		printf("end of file\n");

		
	}	
		
}