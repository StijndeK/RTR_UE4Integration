#include "JsonSystem.h"
#include "fmod.h"

void JsonSystem::readJson(FString filename, AudioSystem& audioSystem)
{
    FString pluginname = "RTR/Content/";
    const FString JsonFilePath = FPaths::ProjectPluginsDir() + pluginname + filename;

    FString JsonString; //Json converted to FString
    FFileHelper::LoadFileToString(JsonString, *JsonFilePath);

    //Displaying the json in a string format inside the output log
    GLog->Log(JsonString);

    //Create a json object to store the information from the json string
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

    //The json reader is used to deserialize the json object
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
    {
        GLog->Log("gain: " + FString::SanitizeFloat(JsonObject->GetNumberField("gain")));
    }
    else
    {
        GLog->Log("couldn't deserialize");
    }

    // set all values
    initialiseAllValues(audioSystem, JsonObject);
}

void JsonSystem::initialiseAllValues(AudioSystem& audioSystem, TSharedPtr<FJsonObject>& jsonObject)
{
    // Initialise all values 
    audioSystem.setGain(jsonObject->GetNumberField("gain"));
    audioSystem.setOffset(jsonObject->GetNumberField("offset"));
    audioSystem.setPositionModifier(jsonObject->GetNumberField("attack decrease modifier"));
    audioSystem.setPositionGainModulation(jsonObject->GetNumberField("range in s"));
    audioSystem.setPositionPitchModulation(jsonObject->GetNumberField("range in s"));

    audioSystem.setAttack(jsonObject->GetNumberField("attack"));
    audioSystem.setRelease(jsonObject->GetNumberField("release"));

    audioSystem.setTimeModulationThreshold(jsonObject->GetNumberField("tm threshold"), jsonObject->GetNumberField("range in s"));
    audioSystem.setTimeModulationLength(jsonObject->GetNumberField("tm length"), jsonObject->GetNumberField("range in s"));
    audioSystem.setActionModulationThreshold(jsonObject->GetNumberField("am threshold"));
    audioSystem.setActionModulationLength(jsonObject->GetNumberField("am length"), jsonObject->GetNumberField("range in s"));

    audioSystem.setPosition(1);

    string loopLayerNames[] = { "Pad: Start", "Pad: End", "Fx", "Noise", "Shepards" };
    for (int i = 0; i < 5; i++) {
        audioSystem.getLayerByName(loopLayerNames[i])->_onOff = (bool)jsonObject->GetNumberField(loopLayerNames[i].c_str());
    }
}
