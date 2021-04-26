#pragma once
#include "fmod.h"
#include "CoreMinimal.h"
#include <vector>
#include <string>
#include "ModulationData.h"

class AudioBase
{
public:
	static void debugMessage(std::string message);
	static const char* getAudioPath(FString filename);
	static void getDriverInfo(int& ofxFmodNumDevices, FMOD_SYSTEM* system, std::vector<std::string>& deviceNames);

private:
	static int samplerate;
	static int channels;
	static FMOD_SPEAKERMODE speakerMode;
};

