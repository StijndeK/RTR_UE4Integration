#include "Layer.h"

//--------------------------------------------------------------
// baseLayer
//--------------------------------------------------------------

BaseLayer::BaseLayer(string label, FMOD_SYSTEM* system, FMOD_CHANNELGROUP* channelGroup)
{
	_label = label;
	FMOD_CHANNEL* arr[] = { _channel, channel2, channel3, channel4, channel5 };
	_channels.insert(_channels.end(), arr, arr + 5);
	_system = system;
	_channelGroup = channelGroup;
}

BaseLayer::~BaseLayer()
{
}

void BaseLayer::stopSounds()
{
	for (int i = 0; i < _sounds.size(); i++) {
		FMOD_Channel_Stop(_channels[i]);
	}
}

void BaseLayer::setVolume(float volume)
{
	for (int i = 0; i < _sounds.size(); i++) {
		FMOD_Channel_SetVolume(_channels[i], volume);
	}
}

void BaseLayer::setFrequency(float frequency)
{
	for (int i = 0; i < _sounds.size(); i++) {
		FMOD_Channel_SetFrequency(_channels[i], frequency);
	}
}

float BaseLayer::getFrequency()
{
	float out = 0;
	for (int i = 0; i < _sounds.size(); i++) {
		FMOD_Channel_GetFrequency(_channels[i], &out);
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
	//FMOD_System_PlaySound(_system, FMOD_CHANNEL_FREE, _sounds[randValue], false, &_channels[randValue]);
	FMOD_System_PlaySound(_system, _sounds[randValue], _channelGroup, false, &_channels[randValue]);
}

//--------------------------------------------------------------
// loopLayer
//--------------------------------------------------------------

LoopLayer::LoopLayer(string label, FMOD_SYSTEM* system, FMOD_CHANNELGROUP* channelGroup) : BaseLayer(label, system, channelGroup)
{
	_label = label;
	positionPitchMod.modType = linear;
}

LoopLayer::~LoopLayer()
{
}

void LoopLayer::startSounds()
{
	for (int i = 0; i < _sounds.size(); i++) {
		//FMOD_System_PlaySound(_system, FMOD_CHANNEL_FREE, _sounds[i], false, &_channels[i]);
		FMOD_System_PlaySound(_system, _sounds[i], _channelGroup, false, &_channels[i]);
	}
}

float LoopLayer::gainModulation(float inputValue, float positionTrigger, float timeTrigger, float actionTrigger, float actionInputValue)
{
	float actOuput = actionGainMod.CalculateModulation(actionInputValue, actionTrigger);
	float output = positionGainMod.CalculateModulation(inputValue, positionTrigger) * timeGainMod.CalculateModulation(timeTrigger);

	return output;
}

float LoopLayer::pitchModulation(float inputValue, float positionTrigger, float timeTrigger)
{
	float output = positionPitchMod.CalculateModulation(inputValue, positionTrigger);
	return output;
}