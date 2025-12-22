
#pragma once
#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "KausAssetManager.generated.h"

class UKausGameData;

/**
 * 
 */
UCLASS(Config = Game)
class KAUS_API UKausAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	UKausAssetManager();
	
	static UKausAssetManager& Get();

	template<typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	const UKausGameData& GetGameData();

protected:
	//~UAssetManager interface
	virtual void StartInitialLoading() override;
	//~End of UAssetManager interface

private:
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	UPROPERTY(Config)
	TSoftObjectPtr<UKausGameData> KausGameDataPath;

	UPROPERTY(Transient)
	TObjectPtr<UKausGameData> CachedGameData;
};

template<typename AssetType>
AssetType* UKausAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(AssetPath.TryLoad());
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepInMemory)
		{
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template<typename AssetType>
TSubclassOf<AssetType> UKausAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(AssetPath.TryLoad());
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}