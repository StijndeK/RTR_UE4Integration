#include "AudioBase.h"

int AudioBase::samplerate = 44800;
FMOD_SPEAKERMODE AudioBase::speakerMode = FMOD_SPEAKERMODE_STEREO;
int AudioBase::channels = 2;

void AudioBase::debugMessage(std::string message)
{
	FString mes(message.c_str());
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, mes);
	GLog->Log(mes);
}

const char* AudioBase::getAudioPath(FString filename)
{
	FString pathToFile = FPaths::ProjectPluginsDir();
	pathToFile += "RTR/Content/";
	pathToFile += filename;
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
