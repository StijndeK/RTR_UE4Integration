// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTRBPLibrary.h"
#include "RTR.h"
using namespace std;

AudioSystem* URTRBPLibrary::audioSystem;

URTRBPLibrary::URTRBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float URTRBPLibrary::RTRSampleFunction(float Param)
{
	return -1;
}

void URTRBPLibrary::loadAudio()
{
	audioSystem->loadAudio();
}

void URTRBPLibrary::playAudio()
{
	// audioSystem->playAudio();
}

void URTRBPLibrary::setJson()
{
	jsonSystem->readJson("example.json", *audioSystem);
}

void URTRBPLibrary::setupRTR(float playerPosition, float minimumDistance, float maximumDistance)
{
}

void URTRBPLibrary::initFMODSystem()
{
	audioSystem->initFMODSystem();
}
