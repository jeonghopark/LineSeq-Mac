/*
 *  DynamicVector.cpp
 *  midiCannamReader
 *
 *  Created by Andrew on 18/07/2011.
 *  Copyright 2011 QMUL. All rights reserved.
 *
 */

#include "DynamicVector.h"

DynamicVector::DynamicVector(){
	length = 0;
	arraySize = 0;
	maximumValue = 0;
	MAPestimate = 0;
	offset = 0;
	scalar = 1;
}

void DynamicVector::copyFromDynamicVector(const DynamicVector& dynamicVec){
	if (dynamicVec.length == length){
		for (int i = 0;i < length;i++)
			array[i] = dynamicVec.array[i];
	}
	else{
		printf("CANNOT COPY VECTORS OF NON SAME LENGTH!!\n");
	}
}

void DynamicVector::createVector(int len){
	array.clear();
	for (int i = 0; i < len;i++){
		array.push_back(0);
	}
	length = len;
	arraySize = array.size();
}


double DynamicVector::getMaximum(){
	int i;
	double max = 0;
	for (i=0;i < length;i++){	
		if (array[i] > max){
			max = array[i];
			MAPestimate = i;
		}
	}
	maximumValue = max;
	return max;
}

void DynamicVector::zero(){
	for (int i = 0;i < array.size();i++)
		array[i] = 0;
}

void DynamicVector::renormalise(){
	double tmpMax = getMaximum();
	if (tmpMax > 0){
//	printf("renormalise : max is %f and size is %i\n", tmpMax, arraySize);		
		for (int i = 0;i < array.size();i++)
			array[i] /= tmpMax;

	}
	//printArray();
}

void DynamicVector::doProduct(DynamicVector& arrayOne, DynamicVector& arrayTwo){

	for (int i = 0;i < arrayOne.length;i++)
		array[i] = arrayOne.array[i] * arrayTwo.array[i];
}


void DynamicVector::printArray(){
	for (int i = 0;i < arraySize;i++){
		printf("[%i] = %f\n", i, array[i]);
	}
}

void DynamicVector::translateDistribution(int translationIndex){
	int tmpIndex;
	DoubleVector tmpArray;
	int i;

	for (i=0;i < arraySize;i++){
		tmpArray.push_back(array[i]);
	}
	//translate values
	for (i=0;i < arraySize;i++){
		tmpIndex = (i + translationIndex + arraySize)%arraySize;
		array[tmpIndex] = tmpArray[i]; 
	}
	tmpArray.clear();
	//now delete tmp array
}

void DynamicVector::addGaussianShape(double mean, double StdDev, double factor){
	int i;
	for (i=0;i<array.size();i++){
		array[i] += factor*(1/(StdDev*sqrt(2*PI)))*exp(-1*(i-mean)*(i-mean)/(2*StdDev*StdDev));
	}
	//printf("ADDED GAUSSIAN SHAPE %i\n", (int)array.size());
}

void DynamicVector::addConstant(double value){
	for (int i=0;i<array.size();i++){
		array[i] += value;
	}
}


void DynamicVector::addToIndex(int index, double constant){
	array[index] += constant;
}


double DynamicVector::getIndexInRealTerms(const int& index){
	if (index < arraySize)
		return (offset + scalar*index);
	else
		return 0;
}

double DynamicVector::getRealTermsAsIndex(double value){
	value -= offset;
	value /= scalar;
	
	return value;
	
}

void DynamicVector::drawVector(const int& minIndex, const int& maxIndex){

		
		double stepSize = 1024 / (double)(maxIndex - minIndex);
		double screenHeight = 768;
		double maxVal = getMaximum();
		
		for (int i = max(1,minIndex+1);i < min(maxIndex, (int)array.size());i++){
		ofLine (stepSize*(i-1), screenHeight * (1 - array[i-1] / maxVal), stepSize*i, screenHeight * (1 - array[i] / maxVal) );
		}
	
}


void DynamicVector::drawConstrainedVector(const int& minIndex, const int& maxIndex, const int& minScreenIndex, const int& maxScreenIndex){
	//constrain the height and width
	
	double stepSize = (maxScreenIndex - minScreenIndex) / (double)(maxIndex - minIndex);//step size in pixels per array bin 
	double screenHeight = 768;
	double maxVal = getMaximum();

	//OPTIMIZE!! XXX could just add stepsize each time
	//not add minindex each time
	int i = max(1,minIndex+1);
//	ofDrawBitmapString("i = "+ofToString(i)+"  :: screen min: "+ofToString(minScreenIndex + stepSize*(i-minIndex-1)), 20, 640);
	
	while ((minScreenIndex + stepSize*(i-minIndex)) < 0)
		i++;//only draw what is on the screen
	
	for ( ; i < min(maxIndex+1, (int)array.size());i++){
		ofLine (minScreenIndex + (stepSize*(i-minIndex-1)), screenHeight * (1 - array[i-1] / maxVal), 
				minScreenIndex + (stepSize*(i-minIndex)),	 screenHeight * (1 - array[i] / maxVal) );
		
	}
	
	ofLine(minScreenIndex, screenHeight, minScreenIndex, screenHeight/2);
	ofLine(maxScreenIndex, screenHeight, maxScreenIndex, screenHeight/2);
	
//	ofDrawBitmapString(ofToString(stepSize, 2)+"  "+ofToString(maxScreenIndex - minScreenIndex, 0), 20, 600);

}
