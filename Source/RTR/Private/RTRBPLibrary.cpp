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

void URTRBPLibrary::playAudio()
{
	// read settings and starting the riser
	jsonSystem.readJson("example.json", audioSystem);
	audioSystem.startRiser();
}

void URTRBPLibrary::stopAudio()
{
	audioSystem.startRelease();
}

void URTRBPLibrary::setupRTR(float playerPosition, float minimumDistance, float maximumDistance)
{
	GLog->Log("setup RTR");
	audioSystem.initFMODSystem();
	jsonSystem.readJson("example.json", audioSystem);
}

void URTRBPLibrary::resetRTR()
{
	audioSystem.stopAudioLayers(audioSystem.layerLoops);
	audioSystem.stopAudioLayers(audioSystem.layerImpacts);
}

void URTRBPLibrary::update()
{
	//GLog->Log("tick");
	audioSystem.update();
}

