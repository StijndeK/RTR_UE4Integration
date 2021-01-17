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
	audioSystem.startRiser();
}

void URTRBPLibrary::stopAudio()
{
	audioSystem.startRelease();
}

void URTRBPLibrary::setupRTR(float playerPosition, float minimumDistance, float maximumDistance)
{
	GLog->Log("setup RTR");
	//audioSystem = new AudioSystem;
	audioSystem.initFMODSystem();
	jsonSystem.readJson("example.json", audioSystem);
}

void URTRBPLibrary::resetRTR()
{
}

void URTRBPLibrary::update()
{
	//GLog->Log("tick");
	audioSystem.update();
}

