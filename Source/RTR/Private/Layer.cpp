#include "Layer.h"

//--------------------------------------------------------------
// baseLayer
//--------------------------------------------------------------

BaseLayer::BaseLayer(string label, FMOD_SYSTEM* system, FMOD_CHANNELGROUP* channelGroup)
{
	this->label = label;
	this->system = system;
	this->channelGroup = channelGroup;
}

BaseLayer::~BaseLayer()
{
}

void BaseLayer::stopSounds()
{
	for (int i = 0; i < sounds.size(); i++) {
		FMOD_Channel_Stop(channels[i]);
	}
}

void BaseLayer::setVolume(float volume)
{
	for (int i = 0; i < sounds.size(); i++) {
		FMOD_Channel_SetVolume(channels[i], volume);
	}
}

void BaseLayer::setFrequency(float frequency)
{
	for (int i = 0; i < sounds.size(); i++) {
		FMOD_Channel_SetFrequency(channels[i], frequency);
	}
}

float BaseLayer::getFrequency()
{
	float out = 0;
	for (int i = 0; i < sounds.size(); i++) {
		FMOD_Channel_GetFrequency(channels[i], &out);
	}
	return out;
}

//--------------------------------------------------------------
// impactLayer
//--------------------------------------------------------------

ImpactLayer::ImpactLayer(string label, FMOD_SYSTEM* system, FMOD_CHANNELGROUP* channelGroup) : BaseLayer(label, system, channelGroup)
{
}

ImpactLayer::~ImpactLayer()
{
}

void ImpactLayer::startSounds()
{
	int randValue = rand() % 3; 	// randomise sub and impact selection
	FMOD_System_PlaySound(system, sounds[randValue], channelGroup, false, &channels[randValue]);
}

//--------------------------------------------------------------
// loopLayer
//--------------------------------------------------------------

LoopLayer::LoopLayer(string label, FMOD_SYSTEM* system, FMOD_CHANNELGROUP* channelGroup) : BaseLayer(label, system, channelGroup)
{
	positionPitchMod.modType = linear;
	setVolume(0);
}

LoopLayer::~LoopLayer()
{
}

void LoopLayer::startSounds()
{
	for (int i = 0; i < sounds.size(); i++) {
		//FMOD_System_PlaySound(_system, FMOD_CHANNEL_FREE, _sounds[i], false, &_channels[i]);
		FMOD_System_PlaySound(system, sounds[i], channelGroup, false, &channels[i]);
	}
}

float LoopLayer::gainModulation(float inputValue, float positionTrigger, float timeTrigger, float actionTrigger, float actionInputValue)
{
	float actOuput = actionGainMod.CalculateModulation(actionInputValue, actionTrigger);
	float output = positionGainMod.CalculateModulation(inputValue, positionTrigger) * timeGainMod.CalculateModulation(timeTrigger) * actOuput;

	return output;
}

float LoopLayer::pitchModulation(float inputValue, float positionTrigger, float timeTrigger)
{
	float output = positionPitchMod.CalculateModulation(inputValue, positionTrigger);
	return output;
}