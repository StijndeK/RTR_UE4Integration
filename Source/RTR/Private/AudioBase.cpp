#include "AudioBase.h"

int samplerate = 44800;
FMOD_SPEAKERMODE speakerMode = FMOD_SPEAKERMODE_STEREO;
int channels = 2;

void AudioBase::debugMessage(string message)
{
	FString mes(message.c_str());
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, mes);
	GLog->Log(mes);
}

const char* AudioBase::getAudioPath(FString filename)
{
	FString pathToFile = FPaths::ProjectPluginsDir();
	pathToFile += "RTR/Content/";
	//pathToFile += filename;
	return TCHAR_TO_ANSI(*pathToFile);
}

void AudioBase::getDriverInfo(int& ofxFmodNumDevices, FMOD_SYSTEM* system, std::vector<std::string>& deviceNames)
{
	for (int i = 0; i < ofxFmodNumDevices; i++)
	{
		char name[256];
		FMOD_System_GetDriverInfo(system, i, name, 256, 0, &samplerate, &speakerMode, &channels);
		deviceNames.push_back(name);
	}
}


// this value to get to in the audio == the player position scaled to a value between 0 and 1
// in mock no need to convert, use the currentPositionSlider value (which is already between 0 and 1)
// in engine you would set the value like this: data.ConvertToDecimalData();
float AudioBase::setDecimalValue(ModulationData& data)
{
	float decimalValue = data.currentDistanceToGetTo;
	return decimalValue;
}

// map value to a value between 0 and 1 in UE4
// if in tool, use gain (and decimalValue in engine), because slider can be moved instantly opposite to the player that has to take time to move
float AudioBase::setCurrentDistanceValue(float decimValue, float gainValue)
{
	return gainValue;
}
