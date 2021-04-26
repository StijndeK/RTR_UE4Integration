// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTRBPLibrary.h"
#include "RTR.h"

AudioSystem URTRBPLibrary::audioSystem;
JsonIn URTRBPLibrary::jsonSystem;
bool URTRBPLibrary::playing = false;
float URTRBPLibrary::plpos;

URTRBPLibrary::URTRBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

// Read current json settings and start the riser.
void URTRBPLibrary::playAudio()
{
	jsonSystem.readJson("example.json", audioSystem);
	audioSystem.startRiser();
}

void URTRBPLibrary::stopAudio()
{
	audioSystem.startRelease();
}

void URTRBPLibrary::setupRTR(float minimumDistance, float maximumDistance)
{
	// Initialise the FMOD system 
	// TODO: this only needs to happen once
	audioSystem.initFMODSystem();

	// Read initial json values and set values.
	jsonSystem.readJson("example.json", audioSystem);

	// Set modulation data.
	audioSystem.modData.positionGoal = maximumDistance;
	audioSystem.modData.positionStart = minimumDistance;
	audioSystem.modData.totalDistance = maximumDistance - minimumDistance;
}

void URTRBPLibrary::resetRTR()
{
	audioSystem.stopAudioLayers(audioSystem.layerLoops);
	audioSystem.stopAudioLayers(audioSystem.layerImpacts);
}

void URTRBPLibrary::setPlayerPosition(float playerPosition)
{
	plpos = playerPosition;
}

void URTRBPLibrary::update()
{
	// Set the position to get to.
	float pos = audioSystem.modData.ConvertToDecimalData(plpos);
	audioSystem.setPosition(pos);

	audioSystem.update();
}

