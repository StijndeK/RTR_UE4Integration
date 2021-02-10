#pragma once
#include "CoreMinimal.h"
#include "fmod.h"
#include <string>
#include <vector>
#include "Envelopes.h"
#include "Modulation.h"
#include "ModulationTypes.h"
using namespace std;

class BaseLayer : public ModulationTypes
{
public:
	BaseLayer(string label, FMOD_SYSTEM* system, FMOD_CHANNELGROUP* channelGroup);
	~BaseLayer();

	FMOD_SYSTEM* _system;
	FMOD_CHANNELGROUP* _channelGroup;

	void stopSounds();
	void setVolume(float volume);
	void setFrequency(float frequency);
	float getFrequency();

	vector<FMOD_SOUND*> _sounds;
	vector<FMOD_CHANNEL*> _channels;
	FMOD_CHANNEL* _channel;
	FMOD_CHANNEL* channel2;
	FMOD_CHANNEL* channel3;
	FMOD_CHANNEL* channel4;
	FMOD_CHANNEL* channel5;

	string _label;
	int _currentSoundIdentifier = 0;
	bool _onOff = 0;
};

class ImpactLayer : public BaseLayer
{
public:
	ImpactLayer(string label, FMOD_SYSTEM* system, FMOD_CHANNELGROUP* channelGroup);
	~ImpactLayer();

	void startSounds();
};

class LoopLayer : public BaseLayer
{
public:
	LoopLayer(string label, FMOD_SYSTEM* system, FMOD_CHANNELGROUP* channelGroup);
	~LoopLayer();

	void startSounds();
	float gainModulation(float inputValue, float positionTrigger, float timeTrigger, float actionTrigger, float actionInputValue);
	float pitchModulation(float inputValue, float positionTrigger, float timeTrigger);

	PositionModulation positionGainMod;
	PositionModulation positionPitchMod;
	TimeModulation timeGainMod;
	ActionModulation actionGainMod;

	// set if pitch mod should be on, main gain mod and less gain mod is always on
	bool mainPitchModToggle = false;
	float frequencyRange = 1.5;
};