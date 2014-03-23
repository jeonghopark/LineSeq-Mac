/*
 *  midiEventHolder.h
 *  midiCannamReader3
 *
 *  Created by Andrew on 19/07/2011.
 *  Copyright 2011 QMUL. All rights reserved.
 *
 */
#include "ofMain.h"

class midiEventHolder{

public:
	
	midiEventHolder();
	void printNotes();
	
	typedef std::vector<int> IntVector;
	typedef std::vector<IntVector> IntMatrix;
	
	typedef std::vector<bool> BoolVector;
	
	typedef std::vector<double> DoubleVector;
	typedef std::vector<DoubleVector> DoubleMatrix;
	
	//the rehearsal version
	IntMatrix recordedNoteOnMatrix;//note, velocity, duration
	IntVector matchesFound;
	BoolVector noteOnMatches;
//	int recordedNoteOnIndex;
	DoubleVector recordedEventTimes;
	
	IntMatrix playedNoteOnMatrix;
	DoubleVector playedEventTimes;
	int playedNoteIndex;
	IntMatrix matchMatrix;
	
	
	double minimumMatchSpeed , maximumMatchSpeed;
	
	double period, pulsesPerQuarternote;
	double getEventTimeMillis(double ticks);
	double getEventTimeTicks(double millis);
	
	int getLocationFromTicks(double tickPosition);
	int getLocationFromMillis(double millisPosition);
	
	double windowStartTime;
	
	
	double likelihoodWidth;
	double likelihoodToNoiseRatio;
	
	void printMatchMatrix();
	
	void setMatchLikelihoods(int numberOfMatches);
	
	void setStartPlayingTimes();
	
	int width, height;
	/////
	string matchString;
	void updatePlayPosition();
	
	DoubleMatrix beatPeriodMatrix;
	
	void drawFile();
	void reset();
	
	int ticksPerScreen;
	int tickLocation;
	int numberOfScreensIn;
	int noteArrayIndex;
	
	int matchWindowWidth;
	
	int noteMinimum, noteMaximum;
	int* screenWidth;
	int* screenHeight;
	float noteHeight;
	float tempo;
	double lastPeriodUpdateTime;
	
	double playPositionInMillis;
	
	double timeOffsetForScreen;
	
	double recentNoteOnTime;


	string timeString;
	double startTime;
	
};