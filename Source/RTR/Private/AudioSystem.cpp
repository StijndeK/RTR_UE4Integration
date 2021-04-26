#include "AudioSystem.h"

// Initialise static FMOD variables.
FMOD_CHANNELGROUP* AudioSystem::channelgroup;
FMOD_SYSTEM* AudioSystem::sys;

int AudioSystem::ofxFmodNumDevices = 0;
int AudioSystem::ofxFmodPreferedDevice = 0;
int AudioSystem::ofxFmodDevice = 0;
unsigned int AudioSystem::buffersize = 1024;

std::vector<std::string> AudioSystem::ofxFmodDeviceNames;

// Initialise other static members.
bool AudioSystem::audioLoaded = false;
bool AudioSystem::systemInitialised = false;

int AudioSystem::currentValue = 0;
int AudioSystem::currentTick = 0;
vector<float> AudioSystem::lastValues(40, 0.f);

float AudioSystem::mainOutputGainAllLayers;
float AudioSystem::mainFrequencyAllLayers;

float AudioSystem::_gain = 1;
float AudioSystem::gainSnapshot = 1;

bool AudioSystem::modulationTrigger = 0;			// true on attack when playing
bool AudioSystem::timeModulationTrigger = 0;		// true after timeModulation threshold is passed
bool AudioSystem::actionModulationTrigger = 1;		// true after actionModulation threshold is passed
float AudioSystem::actionInput = 1;
bool AudioSystem::envelopeTrigger = 0;				// true on start, then immediatly false
bool AudioSystem::playing = false;					// true while audio is playing
vector<ImpactLayer*> AudioSystem::layerImpacts;
vector<LoopLayer*> AudioSystem::layerLoops;

float AudioSystem::frequencyStandard = 44100;

Envelopes AudioSystem::attackEnv;
Timer AudioSystem::releaseTimer;
Timer AudioSystem::timeModulationTimer;
ActionCalculator AudioSystem::positionActionCalculator;
ModulationData AudioSystem::modData;

float AudioSystem::positionModifier = 1;

//--------------------------------------------------------------
AudioSystem::AudioSystem()
{
	// initialise threshold modulators
	releaseTimer.setFunctionToCall(stopRiser);
	timeModulationTimer.setFunctionToCall(triggerTimeModulation);
	positionActionCalculator.setFunctionsToCall(triggerActionModulation, setActionModulationPosition);
}

AudioSystem::~AudioSystem()
{
	for (auto layer : layerLoops) delete layer;
	layerLoops.clear();
	for (auto layer : layerImpacts) delete layer;
	layerImpacts.clear();
}

//--------------------------------------------------------------
// audio loading and initialising
//--------------------------------------------------------------

void AudioSystem::initFMODSystem() 
{
	if (systemInitialised == false) 
	{
		// init
		FMOD_System_Create(&sys);

		// select device
		static bool listCreated = false;

		if (listCreated == false) 
		{
			FMOD_System_GetNumDrivers(sys, &ofxFmodNumDevices);

			getDriverInfo(ofxFmodNumDevices, sys, ofxFmodDeviceNames);

			listCreated = true;
		}

		if (ofxFmodPreferedDevice < ofxFmodNumDevices) 
		{
			ofxFmodDevice = ofxFmodPreferedDevice;
		}
		else {
			ofxFmodDevice = 0;
		}

		// Set buffersize, keep number of buffers.
		unsigned int bsTmp;
		int nbTmp;
		FMOD_System_GetDSPBufferSize(sys, &bsTmp, &nbTmp);
		FMOD_System_SetDSPBufferSize(sys, buffersize, nbTmp);

		FMOD_System_SetDriver(sys, ofxFmodDevice);

		FMOD_System_Init(sys, 104, FMOD_INIT_NORMAL, 0);
		FMOD_System_GetMasterChannelGroup(sys, &channelgroup);
		FMOD_System_Update(sys);
		systemInitialised = true;

		debugMessage("Fmod initialised");
	}

	FMOD_ChannelGroup_SetVolume(channelgroup, 0);
	loadAudio();
}

void AudioSystem::loadAudio() 
{
	if (audioLoaded == false) 
	{
		// Initialise layers.
		string loopLayerNames[] = { "Pad: Start", "Pad: End", "Fx", "Noise", "Shepards" };
		for (int i = 0; i < sizeof(loopLayerNames) / sizeof(loopLayerNames[0]); i++) layerLoops.push_back(new LoopLayer(loopLayerNames[i], sys, channelgroup));

		string impactLayerNames[] = { "Hit", "Sub" }; 
		for (int i = 0; i < sizeof(impactLayerNames) / sizeof(impactLayerNames[0]); i++) layerImpacts.push_back(new ImpactLayer(impactLayerNames[i], sys, channelgroup));

		// Get directory.
		IFileManager& _FileManager = IFileManager::Get();
		FString pathToFile = FPaths::ProjectPluginsDir() + "RTR/Content/";

		const TCHAR* RootPath = &pathToFile[0];
		TArray<FString> AudioFiles;
		const TCHAR* extension = _T("*.wav");

		_FileManager.FindFiles(AudioFiles, RootPath, extension);

		// Load sounds into layers.
		for (int i = 0; i < AudioFiles.Num(); i++) 
		{
			// Get path and name.
			FString tempName = FPaths::GetCleanFilename(TCHAR_TO_UTF8(*AudioFiles[i]));
			GLog->Log("tempname: " + tempName);

			// Create sound and channel to add to layer.
			FMOD_SOUND* tempSound;

			FMOD_System_CreateSound(sys, getAudioPath(*AudioFiles[i]), FMOD_DEFAULT, 0, &tempSound);

			// Initialise layers with their names and FMOD_SOUNDS.
			if (tempName[0] == 'I') 
			{
				layerImpacts[0]->channels.push_back(nullptr);
				layerImpacts[0]->sounds.push_back(tempSound);
			}
			else if (tempName[0] == 'S') 
			{	
				layerImpacts[1]->channels.push_back(nullptr);
				layerImpacts[1]->sounds.push_back(tempSound);
			}
			else if (tempName[0] == 'L') 
			{		
				FMOD_Sound_SetMode(tempSound, FMOD_LOOP_NORMAL); 

				if (tempName[2] == 'P') 
				{		
					layerLoops[0]->channels.push_back(nullptr);
					layerLoops[0]->sounds.push_back(tempSound);
					GLog->Log("Pad: " + tempName);
				}
				else if (tempName[2] == 'E') 
				{	
					layerLoops[1]->channels.push_back(nullptr);
					layerLoops[1]->sounds.push_back(tempSound);
					GLog->Log("Pad: " + tempName);
				}
				else if (tempName[2] == 'F') 
				{	
					layerLoops[2]->channels.push_back(nullptr);
					layerLoops[2]->sounds.push_back(tempSound);
					GLog->Log("Fx: " + tempName);
				}
				else if (tempName[2] == 'N') 
				{	
					layerLoops[3]->channels.push_back(nullptr);
					layerLoops[3]->sounds.push_back(tempSound);
					GLog->Log("Noise: " + tempName);
				}
				else if (tempName[2] == 'S') 
				{	
					layerLoops[4]->channels.push_back(nullptr);
					layerLoops[4]->sounds.push_back(tempSound);
					GLog->Log("Shepard: " + tempName);
				}
				else 
				{
					GLog->Log("Error: Loopname not found: " + tempName);
				}
			}
			else 
			{
				GLog->Log("Error: Name not found: " + tempName);
			}
		}

		// Set on for frequency.
		getLayerByName("Pad: Start")->mainPitchModToggle = true;
		getLayerByName("Pad: End")->mainPitchModToggle = true;

		audioLoaded = true;
		debugMessage("audio loaded");

		soundDesign();
	}
}

//--------------------------------------------------------------
// update
//--------------------------------------------------------------
void AudioSystem::update() 
{
	FMOD_System_Update(sys);

	if (playing) 
	{
		// Get the player's position in float.
		float decimalValue = setDecimalValue(modData);

		// Attack envelope
		float attackedGain = attackEnv.arAttackExp(_gain, envelopeTrigger);

		// Plotting
		int onLayers = 0;
		mainOutputGainAllLayers = 0;
		mainFrequencyAllLayers = 0;

		for (auto layer : layerLoops) 
		{
			if (layer->onOff) 
			{

				// Gain modulation
				float outputGain = attackedGain * layer->gainModulation(decimalValue, modulationTrigger, timeModulationTrigger, actionModulationTrigger, actionInput);
				layer->setVolume(outputGain);

				// Pitch modulation
				if (layer->mainPitchModToggle) 
				{
					float outputPitch = (layer->frequencyRange * frequencyStandard) * layer->pitchModulation(decimalValue, modulationTrigger, timeModulationTrigger);
					layer->setFrequency(outputPitch);
				}

				// Get main amplitude and pitch (used for plotting).
				onLayers++;
				mainOutputGainAllLayers += attackedGain * outputGain;
				mainFrequencyAllLayers += layer->getFrequency();
			}
		}

		// Set main amplitude and pitch (used for plotting).
		mainOutputGainAllLayers /= onLayers;
		mainFrequencyAllLayers /= onLayers;

		debugMessage("position to get to: " + to_string(modData.currentDistanceToGetTo));
		debugMessage("output" + to_string(mainOutputGainAllLayers));

		// Update treshold checkers.
		releaseTimer.update();
		timeModulationTimer.update();
		positionActionCalculator.update(decimalValue);

		// Reset trigger for envelopes.
		if (envelopeTrigger == 1) envelopeTrigger = 0;
	}
}

//--------------------------------------------------------------
// playback
//--------------------------------------------------------------

void AudioSystem::startRiser()
{
	stopRiser();

	debugMessage("start riser");

	// Create snapshot of gain for envelopes.
	gainSnapshot = _gain;

	// Set trigger bools.
	modulationTrigger = 1;
	envelopeTrigger = 1;
	playing = true;

	timeModulationTimer.startTimer();
	positionActionCalculator.startActionCalculator();

	FMOD_ChannelGroup_SetVolume(channelgroup, 1);

	startAudioLayers(layerLoops);
}

// Begin release fase of riser, using a timer that checks when to stop the riser.
void AudioSystem::startRelease() {
	debugMessage("start stopping audio");

	// reset triggers
	modulationTrigger = 0;
	timeModulationTrigger = 0;

	releaseTimer.startTimer();

	// play impacts
	startAudioLayers(layerImpacts);
}

void AudioSystem::stopRiser()
{
	debugMessage("stop riser");

	stopAudioLayers(layerLoops);
	stopAudioLayers(layerImpacts);
	FMOD_ChannelGroup_SetVolume(channelgroup, 0);

	// Reset timers in case they are currently running.
	releaseTimer.stopTimer();
	timeModulationTimer.stopTimer();
	positionActionCalculator.stopActionCalculator();

	playing = false;
}

//--------------------------------------------------------------
// Riser start and stops
//--------------------------------------------------------------

void AudioSystem::startAudioLayers(vector<LoopLayer*> layersToStart) 
{
	debugMessage("start Audio loops");
	for (auto layer : layersToStart) 
	{
		if (layer->onOff) 
		{
			layer->setVolume(0);
			layer->startSounds();
		}
	}
}

void AudioSystem::startAudioLayers(vector<ImpactLayer*> layersToStart) 
{
	debugMessage("start Audio impacts");
	for (auto layer : layersToStart) layer->startSounds();
}

void AudioSystem::stopAudioLayers(vector<LoopLayer*> layersToStop) 
{
	debugMessage("stop Audio loops");
	for (auto layer : layersToStop) layer->stopSounds();
}

void AudioSystem::stopAudioLayers(vector<ImpactLayer*> layersToStop) 
{
	debugMessage("stop Audio impacts");
	for (auto layer : layersToStop) layer->stopSounds();
}

//--------------------------------------------------------------
// Calculate (and set) values
//--------------------------------------------------------------

void AudioSystem::setGain(float gain) {
	debugMessage("setGain: " + to_string(pow(10, gain / 20)));
	_gain = pow(10, gain / 20);
}

void AudioSystem::setPositionGainModulation(float attack)
{
	debugMessage("setPositionModulation: Amp. " + to_string(attack));
	for (auto layer : layerLoops) 
	{
		layer->positionGainMod.calculateAttackStepSize(attack);
		layer->positionGainMod.calculateAttackDecreaseStepSize(positionModifier * attack);
	}
}

void AudioSystem::setPositionPitchModulation(float attack)
{
	debugMessage("setPositionModulation: Pitch. " + to_string(attack));
	for (auto layer : layerLoops) 
	{
		if (layer->mainPitchModToggle) 
		{
			layer->positionPitchMod.calculateAttackStepSize(attack);
			layer->positionPitchMod.calculateAttackDecreaseStepSize(positionModifier * attack);
		}
	}
}

void AudioSystem::setPositionModifier(float modifier)
{
	debugMessage("setPositionModifier: " + to_string(modifier));
	positionModifier = modifier;
}

void AudioSystem::setAttack(float attack) {
	debugMessage("setAttack: " + to_string(attack));
	attackEnv.setARExp(attack, 0);
}

void AudioSystem::setRelease(float release)
{
	debugMessage("setRelease: " + to_string(release));

	for (auto layer : layerLoops) layer->positionGainMod.calculateReleaseStepSize(release);
	for (auto layer : layerLoops) if (layer->mainPitchModToggle) layer->positionPitchMod.calculateReleaseStepSize(release);

	// set timer length
	releaseTimer.setLength(release + 50);
}

void AudioSystem::setModulationCurve(float startValue)
{
	debugMessage("setcurve: " + to_string(startValue));
	for (auto layer : layerLoops) layer->positionGainMod.curveRatio = startValue;
}

void AudioSystem::setPosition(float position)
{
	modData.currentDistanceToGetTo = position;
}

void AudioSystem::setTimeModulationThreshold(float threshold, float minimumLength)
{
	debugMessage("setTimeModulationThreshold: " + to_string(threshold * minimumLength));
	timeModulationTimer.setLength(threshold * minimumLength);
}

void AudioSystem::setTimeModulationLength(float lengthInMs, float minimumLength)
{
	debugMessage("setTimeModulationLength " + to_string(lengthInMs * minimumLength));
	for (auto layer : layerLoops) layer->timeGainMod.calculateAttackDecreaseStepSize(lengthInMs * minimumLength);
}

void AudioSystem::setActionModulationThreshold(float threshold)
{
	debugMessage("setActionModulationThreshold: " + to_string(threshold));
	positionActionCalculator.setThreshold(threshold);
}

void AudioSystem::setActionModulationLength(float lengthInMs, float minimumLength)
{
	float modifier = 1.5;

	debugMessage("setActionModulation: Amp. " + to_string(lengthInMs * minimumLength));
	for (auto layer : layerLoops) 
	{
		layer->actionGainMod.calculateAttackStepSize(modifier * lengthInMs * minimumLength);
		layer->actionGainMod.calculateAttackDecreaseStepSize(lengthInMs * minimumLength);
	}
}

//--------------------------------------------------------------
// getters
//--------------------------------------------------------------

string AudioSystem::getAudioName(FMOD_SOUND* sound) 
{
	char name[256];
	FMOD_Sound_GetName(sound, name, 256);
	string outName = name;
	return name;
}

LoopLayer* AudioSystem::getLayerByName(string name) 
{
	for (auto l : layerLoops) 
	{
		if (l->label == name) 
		{
			debugMessage("getLayerByName: " + l->label);
			return l;
		}
	}
	return nullptr;
}

//--------------------------------------------------------------
// modulation triggers
//--------------------------------------------------------------

void AudioSystem::triggerTimeModulation()
{
	debugMessage("start time modulation");
	timeModulationTrigger = 1;
}

void AudioSystem::triggerActionModulation()
{
	debugMessage("start action modulation");
	actionModulationTrigger = !actionModulationTrigger;
}

void AudioSystem::setActionModulationPosition(float position)
{
	actionInput = position;
}

//--------------------------------------------------------------
// action modulation
//--------------------------------------------------------------

// after how long and how much defiation in player position should the riser slow down
void AudioSystem::checkLessModifier(float value) 
{
	// check every 10 ticks to set lessGain
	if (currentTick == 10) {
		// use setcurrentdistance value to convert the value to a 0 1 value in the engine (if this is not done before)
		//float currentDistanceValue = setCurrentDistanceValue(decimalValue, mainOutputGainAllLayers);

		checkLessModifier(value);

		currentTick = 0;
	}
	else {
		currentTick++;
	}


	float deviationValue = 0.5; // if only distance moved changed half, start decreasing intensity of the riser

	lastValues[currentValue] = value;

	// set value in currentValue
	currentValue = (currentValue + 1) % lastValues.size();

	float currentDeviation = 0;
	for (float v : lastValues) {
		currentDeviation += v;
	}
	currentDeviation /= lastValues.size();

	float threshold = 0.1; // TODO: set this value from input

	//if (currentDeviation - value <= threshold && currentDeviation - value >= 0 - threshold) {
	//	debugMessage("current deviation" + to_string(currentDeviation) = " | value: " + to_string(value));
	//}
	// check only for if its bigger
	// then check if has not changed enough
}

//--------------------------------------------------------------
// Offset
//--------------------------------------------------------------

void AudioSystem::setOffset(float offset) 
{
	// check if offset is necessary
	// check if max offset is necessary (maybe only needs half of the time, as the sound does need to get more intense)
	// set values for offset to happen and sound to modulate to its conclusion
}

//--------------------------------------------------------------
// Sound design
//--------------------------------------------------------------

// set all the modulation
void AudioSystem::soundDesign()
{
	debugMessage("set sound design");

	// position modulation
	getLayerByName("Pad: Start")->positionGainMod.curveRatio = 1;

	// time modulation
	getLayerByName("Fx")->timeGainMod.curveRatio = 1;
	getLayerByName("Fx")->timeGainMod.range = 0.3;
	getLayerByName("Shepards")->timeGainMod.curveRatio = 1;
	getLayerByName("Shepards")->timeGainMod.range = 0.3;

	// action modulation
	getLayerByName("Pad: Start")->actionGainMod.curveRatio = 0.5;
	getLayerByName("Pad: Start")->actionGainMod.range = 0;
	getLayerByName("Pad: End")->actionGainMod.range = 0;
	getLayerByName("Noise")->actionGainMod.range = 0;
	getLayerByName("Fx")->actionGainMod.curveRatio = 1;
	getLayerByName("Fx")->actionGainMod.range = 1;
	getLayerByName("Shepards")->actionGainMod.curveRatio = 0.5;
	getLayerByName("Shepards")->actionGainMod.range = 1;
}