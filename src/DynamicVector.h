/*
 *  DynamicVector.h
 *  midiCannamReader
 *
 *  Created by Andrew on 18/07/2011.
 *  Copyright 2011 QMUL. All rights reserved.
 *
 */

//OPTIMIZE CONSTRAINED VECTOR

#include "stdlib.h"
#include "ofMain.h"

#ifndef _DYNAMIC_VECTOR
#define _DYNAMIC_VECTOR

class DynamicVector{
public:
	DynamicVector();

	void createVector(int len);
	void renormalise();
	void translateDistribution(int translationIndex);
	typedef std::vector<double> DoubleVector;
	DoubleVector array;
	double getMaximum();
	void drawVector(const int& minIndex, const int& maxIndex);
	void drawConstrainedVector(const int& minIndex, const int& maxIndex, const int& minScreenIndex, const int& maxScreenIndex);

	void addConstant(double value);
	void addGaussianShape(double mean, double stddev, double factor);
	void addToIndex(int index, double constant);
	
	void doProduct(DynamicVector& arrayOne, DynamicVector& arrayTwo);
		
	double getIndexInRealTerms(const int& index);
	double getRealTermsAsIndex(double value);
	
	void printArray();
	void zero();
	
	void copyFromDynamicVector(const DynamicVector& dynamicVec);
	
	//variables
	int length, arraySize;
	double maximumValue;
	int MAPestimate;
	
	double offset;
	double scalar;//each array point is this much of the quantity
	//i.e. array[index] contributes to (offset + scalar*index) in real terms

};

#endif
