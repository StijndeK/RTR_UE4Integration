#pragma once
#include "CoreMinimal.h"
#include "AudioSystem.h"
#include "JsonIn.h"

class RTRData
{
public:
	RTRData();
	~RTRData();

	JsonIn jsonSystem;
	AudioSystem audioSystem;
private:

};