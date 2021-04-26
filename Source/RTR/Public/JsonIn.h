#pragma once
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "fmod.h"
#include "CoreMinimal.h"
#include "AudioSystem.h"

class JsonIn
{
public:
	static void readJson(FString filename, AudioSystem& audioSystem);
	static void initialiseAllValues(AudioSystem& audioSystem, TSharedPtr<FJsonObject>& jsonObject);
};