// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "fmod.h"
#include "CoreMinimal.h"
#include "AudioSystem.h"

class JsonSystem
{
public:
	static void readJson(FString filename, AudioSystem& audioSystem);
	void initialiseAllValues();
};