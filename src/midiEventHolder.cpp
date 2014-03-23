/*
 *  midiEventHolder.cpp
 *  midiCannamReader3
 *
 *  Created by Andrew on 19/07/2011.
 *  Copyright 2011 QMUL. All rights reserved.
 *
 */

#include "midiEventHolder.h"

midiEventHolder::midiEventHolder(){
//	recordedNoteOnIndex = 0;
	
	width = 1024;
	height = 768;
	screenWidth= &width;
	screenHeight = &height;
	
	ticksPerScreen = 4000;
	tickLocation = 0;
	pulsesPerQuarternote = 240;
	noteArrayIndex = 0;
	noteMinimum = 30;
	noteMaximum = 96;
	
	minimumMatchSpeed = 0.5;
	maximumMatchSpeed = 2.0;
	likelihoodWidth = 100;
	likelihoodToNoiseRatio = 50;
	
	matchWindowWidth = 6000;//window size for matching in ms 

	
	noteHeight = (*screenHeight) / (float)(noteMaximum - noteMinimum);
}



void midiEventHolder::reset(){
	noteArrayIndex = 0;
	tickLocation = 0;
	lastPeriodUpdateTime = ofGetElapsedTimeMillis();

	numberOfScreensIn = 0;

	playedEventTimes.clear();
	playedNoteOnMatrix.clear();
	matchMatrix.clear();
	
}

void midiEventHolder::printNotes(){
	printf("RECORDED MATRIX");
	for (int i = 0;i < recordedNoteOnMatrix.size();i++){
		printf("%i :: %i @ %f\n", recordedNoteOnMatrix[i][0], recordedNoteOnMatrix[i][1], recordedEventTimes[i]);
	}
}


double midiEventHolder::getEventTimeTicks(double millis){
	return (millis * pulsesPerQuarternote / period);
}

double midiEventHolder::getEventTimeMillis(double ticks){
	return (period * ticks / (double) pulsesPerQuarternote);
}

void midiEventHolder::updatePlayPosition(){
	
	//in actual fact if we are changing the speed of the play position 
	//we will need to update this via the file
	
	double timeDifference = ofGetElapsedTimeMillis() - lastPeriodUpdateTime;
	//this is time diff in milliseconds
	//then we have 
	double quarterNoteIntervals = (timeDifference / period);
	tickLocation = quarterNoteIntervals * pulsesPerQuarternote; 
	
	playPositionInMillis = timeDifference;//based on updating from when we change period
	//this to be added
	
	
}


void midiEventHolder::drawFile(){
	//draws midi file on scrolling screen
	int size = recordedNoteOnMatrix.size();
	if (size > 0){
		
		numberOfScreensIn = 0;
		
		timeOffsetForScreen = getEventTimeMillis(numberOfScreensIn * ticksPerScreen);
		
		while (noteArrayIndex < recordedNoteOnMatrix.size() && tickLocation > recordedNoteOnMatrix[noteArrayIndex][0] )
			noteArrayIndex++;
		
		
		while (noteArrayIndex > 0 && noteArrayIndex < size && tickLocation < recordedNoteOnMatrix[noteArrayIndex][0])
			noteArrayIndex--;
		
		//need to start where we currently are in file
		int maxNoteIndexToPrint	= noteArrayIndex;
		int minNoteIndexToPrint = noteArrayIndex;
		
		while (maxNoteIndexToPrint < recordedNoteOnMatrix.size() && recordedNoteOnMatrix[maxNoteIndexToPrint][0] < (numberOfScreensIn+1)*ticksPerScreen )
			maxNoteIndexToPrint++;
		
		while (minNoteIndexToPrint > 0 && minNoteIndexToPrint < size && recordedNoteOnMatrix[minNoteIndexToPrint][0] > numberOfScreensIn*ticksPerScreen)
			minNoteIndexToPrint--;
		
		for (int tmpIndex = max(0,minNoteIndexToPrint);tmpIndex < min(maxNoteIndexToPrint, (int)recordedNoteOnMatrix.size());tmpIndex++){
			
			
				ofSetColor(255,255,255);

	//		 XXX replace ofgetwidth below
			//if (tmpIndex >= 0 && tmpIndex < size)
			int xLocation = (float)(recordedNoteOnMatrix[tmpIndex][0] - numberOfScreensIn*ticksPerScreen)*(*screenWidth)/(float)ticksPerScreen;
			int duration = (float)(recordedNoteOnMatrix[tmpIndex][3]*(*screenWidth))/(float)ticksPerScreen;
			
			
			int yLocation = (*screenHeight) - ((recordedNoteOnMatrix[tmpIndex][1] - noteMinimum )*(*screenHeight)/ (float)(noteMaximum - noteMinimum));						 
			ofRect(xLocation,yLocation, duration,  noteHeight);
			
		}
		
		
		int xLocation;// = getLocationFromTicks(tickLocation);
	//	ofLine(xLocation, 0, xLocation, (*screenHeight));
		
		//orange line at best estimate

		xLocation = getLocationFromMillis(playPositionInMillis);
		ofSetColor(250,100,0);
		ofLine(xLocation, 0, xLocation, (*screenHeight));
		
		
		//lines where matching window start and end are 
		ofSetColor(0,100,255);
		xLocation = getLocationFromMillis(windowStartTime);
		ofLine(xLocation, 0, xLocation, (*screenHeight));
		xLocation = getLocationFromMillis(windowStartTime+matchWindowWidth);
		ofLine(xLocation, 0, xLocation, (*screenHeight));
		 
		
	}	
	
//	ofDrawBitmapString(ofToString(timeOffsetForScreen, 1), 20,20);
//	
//	ofDrawBitmapString(timeString, 20, 60);
//		
//	ofDrawBitmapString(matchString, 20, ofGetHeight() - 20);
	
}

int midiEventHolder::getLocationFromTicks(double tickPosition){
	return (int)((float)(tickPosition - numberOfScreensIn*ticksPerScreen)*(*screenWidth)/(float)ticksPerScreen);
}

int midiEventHolder::getLocationFromMillis(double millisPosition){
	//(getEventTimeTicks(windowStartTime+matchWindowWidth) - numberOfScreensIn*ticksPerScreen)*(*screenWidth) / (double)ticksPerScreen
	return (millisPosition - timeOffsetForScreen)*(*screenWidth)/getEventTimeMillis(ticksPerScreen);
}



void midiEventHolder::setStartPlayingTimes(){
	lastPeriodUpdateTime = ofGetElapsedTimeMillis();

	startTime = lastPeriodUpdateTime;
	
	matchString = "";
}


void midiEventHolder::printMatchMatrix(){
	printf("match matrix:\n");
	for (int i = 0;i < matchMatrix.size();i++){
		for (int k = 0;k < matchMatrix[i].size();k++){
			printf("%i , ", matchMatrix[i][k]);
		}
		printf("\n");
	}
	
	
}