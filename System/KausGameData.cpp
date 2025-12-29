#include "KausGameData.h"
#include "KausAssetManager.h"
#include "Data/KausUnitStatsRowData.h"
#include "NativeGameplayTags.h"

UKausGameData::UKausGameData()
{
}

const UKausGameData& UKausGameData::UKausGameData::Get()
{
	return UKausAssetManager::Get().GetGameData();
}

const FKausUnitStatsRow* UKausGameData::GetUnitStats(FGameplayTag UnitTag) const
{
    if (!UnitStatsTable || !UnitTag.IsValid())
    {
        return nullptr;
    }

    return UnitStatsTable->FindRow<FKausUnitStatsRow>(FName(*UnitTag.ToString()), TEXT("GetUnitStats"));
}
