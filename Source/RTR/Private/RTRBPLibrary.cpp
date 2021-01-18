// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTRBPLibrary.h"
#include "RTR.h"
using namespace std;

AudioSystem URTRBPLibrary::audioSystem;
JsonSystem URTRBPLibrary::jsonSystem;

URTRBPLibrary::URTRBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

// read current json settings and start the riser
void URTRBPLibrary::playAudio()
{
	jsonSystem.readJson("example.json", audioSystem);
	audioSystem.startRiser();
}

void URTRBPLibrary::stopAudio()
{
	audioSystem.startRelease();
}

void URTRBPLibrary::setupRTR(float minimumDistance, float maximumDistance, float fastedTimeInMs)
{
	// currentposition is a constantly changing value
	// minim and max distance is used for calculation
	// get distance to gedeelte kan voor nu nog wel in bp, en later helemaal calculated

	// initialise the FMOD system // TODO: this only needs to happen once
	audioSystem.initFMODSystem();

	// read initial json values and set values
	jsonSystem.readJson("example.json", audioSystem);

	// set the range with the fasted possible time
	audioSystem.setGainModulation(fastedTimeInMs);
	audioSystem.setPitchModulation(fastedTimeInMs);
	audioSystem.debugMessage("set time in ms: " + to_string(fastedTimeInMs));
}

void URTRBPLibrary::resetRTR()
{
	audioSystem.stopAudioLayers(audioSystem.layerLoops);
	audioSystem.stopAudioLayers(audioSystem.layerImpacts);
}

void URTRBPLibrary::update(float playerPosition)
{
	// set the position to get to
	//audioSystem.setPosition(playerPosition);

	audioSystem.update();
}

