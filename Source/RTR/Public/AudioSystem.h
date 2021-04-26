//// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "fmod.h"
#include "Layer.h"
#include "AudioBase.h"
#include "Envelopes.h"
#include "Modulation.h"
#include "ThresholdChecker.h"

class AudioSystem : public AudioBase 
{
public:
	AudioSystem();
	~AudioSystem();

	static void initFMODSystem();
	static void loadAudio();
	static void update();

	static void startRiser();
	static void startRelease();
	static void stopRiser();
	static void startAudioLayers(vector<ImpactLayer*> layersToStart);
	static void startAudioLayers(vector<LoopLayer*> layersToStart);
	static void stopAudioLayers(vector<LoopLayer*> layersToStop);
	static void stopAudioLayers(vector<ImpactLayer*> layersToStop);

	// setters
	static void setGain(float gain);
	static void setPositionGainModulation(float attack);
	static void setPositionPitchModulation(float attack);
	static void setPositionModifier(float modifier);
	static void setAttack(float attack);
	static void setRelease(float release); // move to layer
	static void setModulationCurve(float startValue);
	static void setPosition(float position);

	static void setTimeModulationThreshold(float threshold, float minimumLength);
	static void setTimeModulationLength(float lengthInMs, float minimumLength);
	static void setActionModulationThreshold(float threshold);
	static void setActionModulationLength(float lengthInMs, float minimumLength);

	static void triggerTimeModulation();
	static void triggerActionModulation();
	static void setActionModulationPosition(float position);

	static void checkLessModifier(float value);
	static void setOffset(float offset);

	static positionModulation timeMod;

	static void soundDesign();

	// getters
	static string getAudioName(FMOD_SOUND* sound);
	static LoopLayer* getLayerByName(string name);

	static bool audioLoaded;
	static bool systemInitialised;

	static bool modulationTrigger;		// true on attack when playing
	static bool timeModulationTrigger;
	static bool actionModulationTrigger;
	static float actionInput;
	static bool envelopeTrigger;		// true on start, then immediatly false
	static bool playing;				// true while audio is playing
	static bool releasePhase;			// true while release is playing, to get notified when to stop audio

	static float _gain;
	static float gainSnapshot;

	// holds values to check deviation for lessGain
	static vector<float> lastValues;
	static int currentValue;
	static int currentTick;

	// vectors holding collections of layers initialised in load audio
	static vector<ImpactLayer*> layerImpacts;
	static vector<LoopLayer*> layerLoops;

	// data on the modulation, such as player position etc
	static ModulationData modData;

	// values for UI plotters
	static float mainOutputGainAllLayers;
	static float mainFrequencyAllLayers;

	// FMOD
	static int ofxFmodNumDevices;
	static int ofxFmodPreferedDevice;
	static int ofxFmodDevice;
	static unsigned int buffersize;

	static std::vector<std::string> ofxFmodDeviceNames;

	static FMOD_CHANNELGROUP* channelgroup;
	static FMOD_SYSTEM* sys;
private:
	// attack only envelope for the start of the sound
	static Envelopes attackEnv;

	static Timer releaseTimer;			// Timer on when to stop all audio after impact
	static Timer timeModulationTimer;	// Timer to start decreasing intensity if the riser is taking long
	static ActionCalculator positionActionCalculator;

	static float frequencyStandard;

	static float positionModifier;
};
