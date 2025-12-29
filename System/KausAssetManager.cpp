#include "KausAssetManager.h"
#include "KausGameData.h"
#include "Logs/KausLogChannels.h"

UKausAssetManager::UKausAssetManager()
{
}

UKausAssetManager& UKausAssetManager::Get()
{
	check(GEngine);

	if (UKausAssetManager* Singleton = Cast<UKausAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogKaus, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to KausAssetManager!"));

	return *NewObject<UKausAssetManager>();
}

void UKausAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	GetGameData();
}

const UKausGameData& UKausAssetManager::GetGameData()
{
	if (CachedGameData)
	{
		return *CachedGameData;
	}

	UKausGameData* LoadedData = nullptr;
	if (!KausGameDataPath.IsNull())
	{
		LoadedData = KausGameDataPath.LoadSynchronous();
	}

	if (LoadedData)
	{
		CachedGameData = LoadedData;
		LoadedAssets.Add(CachedGameData);
	}
	else
	{
		UE_LOG(LogKaus, Error, TEXT("Failed to load KausGameData! Check DefaultEngine.ini"));
		CachedGameData = NewObject<UKausGameData>();
	}

	return *CachedGameData;
}