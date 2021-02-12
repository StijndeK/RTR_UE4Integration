#pragma once
#include "CoreMinimal.h"
#include "AudioSystem.h"
#include "JsonSystem.h"

class RTRData
{
public:
	RTRData();
	~RTRData();

	JsonSystem jsonSystem;
	AudioSystem audioSystem;
private:

};