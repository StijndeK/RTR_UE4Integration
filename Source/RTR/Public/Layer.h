#pragma once
#include "CoreMinimal.h"
#include "fmod.h"
#include <string>
#include <vector>
#include "Envelopes.h"
#include "Modulation.h"
#include "ModulationTypes.h"
using namespace std;

class BaseLayer
{
public:
	BaseLayer(string label, FMOD_SYSTEM* system, FMOD_CHANNELGROUP* channelGroup);

	FMOD_SYSTEM* system;
	FMOD_CHANNELGROUP* channelGroup;

	void stopSounds();
	void setVolume(float volume);
	void setFrequency(float frequency);
	float getFrequency();

	vector<FMOD_SOUND*> sounds;
	vector<FMOD_CHANNEL*> channels;

	string label;
	bool onOff = 0;
};

class ImpactLayer : public BaseLayer
{
public:
	ImpactLayer(string label, FMOD_SYSTEM* system, FMOD_CHANNELGROUP* channelGroup);

	void startSounds();
};

class LoopLayer : public BaseLayer
{
public:
	LoopLayer(string label, FMOD_SYSTEM* system, FMOD_CHANNELGROUP* channelGroup);

	void startSounds();
	float gainModulation(float inputValue, float positionTrigger, float timeTrigger, float actionTrigger, float actionInputValue);
	float pitchModulation(float inputValue, float positionTrigger, float timeTrigger);

	positionModulation positionGainMod;
	positionModulation positionPitchMod;
	timeModulation timeGainMod;
	actionModulation actionGainMod;

	// Set if pitch mod should be on. Main gain mod and less gain mod are always on.
	bool mainPitchModToggle = false;
	float frequencyRange = 1.5;
};