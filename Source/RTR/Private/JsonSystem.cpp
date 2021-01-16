#include "JsonSystem.h"
#include "fmod.h"

void JsonSystem::readJson(FString filename, AudioSystem& audioSystem)
{
    bool loaded = true;

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

    GLog->Log("Set gain: " + FString::SanitizeFloat(JsonObject->GetNumberField("gain")));
    audioSystem.setGain(JsonObject->GetNumberField("gain"));
}
