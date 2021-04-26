// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "fmod.h"
#include "AudioSystem.h"
#include "JsonIn.h"
#include <Runtime/Core/Private/Windows/MinimalWindowsApi.cpp>
#include <Runtime/Core/Public/Windows/MinimalWindowsApi.h>
#include <profileapi.h>
#include "RTRBPLibrary.generated.h"

UCLASS()
class URTRBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RTR playAudio", Keywords = "audio"), Category = "RTR")
		static void playAudio();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RTR stopAudio", Keywords = "audio"), Category = "RTR")
		static void stopAudio();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RTR Setup", Keywords = "json, audio, rtr"), Category = "RTR")
		static void setupRTR(float minimumDistance, float maximumDistance);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RTR reset", Keywords = "json, audio, rtr"), Category = "RTR")
		static void resetRTR();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RTR set player position", Keywords = "update, audio, rtr, player, position"), Category = "RTR")
		static void setPlayerPosition(float playerPosition);
	UFUNCTION()
		static void update();

	static JsonIn jsonSystem;
	static AudioSystem audioSystem;
	static bool playing;
	static float plpos;
};