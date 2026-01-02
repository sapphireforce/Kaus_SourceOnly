#include "KausGameData.h"
#include "KausAssetManager.h"
#include "DataTable/KausUnitStatsRow.h"
#include "NativeGameplayTags.h"
#include "Logs/KausLogChannels.h"

UKausGameData::UKausGameData()
{
}

const UKausGameData& UKausGameData::UKausGameData::Get()
{
	return UKausAssetManager::Get().GetGameData();
}

const FKausUnitStatsRow* UKausGameData::GetUnitStats(FGameplayTag UnitTag) const
{
	if (!UnitTag.IsValid())
	{
		return nullptr;
	}

	if (const FKausUnitStatsRow* CachedRow = UnitStatsCache.Find(UnitTag))
	{
		return CachedRow;
	}

	if (UnitStatsTable)
	{
		const TMap<FName, uint8*>& RowMap = UnitStatsTable->GetRowMap();
		for (const auto& KeyValue : RowMap)
		{
			const FKausUnitStatsRow* Row = reinterpret_cast<const FKausUnitStatsRow*>(KeyValue.Value);
			if (Row && Row->UnitTag == UnitTag)
			{
				return Row;
			}
		}
	}

	UE_LOG(LogKaus, Warning, TEXT("Could not find stats for tag [%s] in DataTable."), *UnitTag.ToString());
	return nullptr;
}

void UKausGameData::PostLoad()
{
    Super::PostLoad();

    BuildUnitStatsCache();

#if WITH_EDITOR
	if (UnitStatsTable)
	{
		UnitStatsTable->OnDataTableChanged().AddUObject(this, &ThisClass::HandleDataTableChanged);
	}
#endif
}

#if WITH_EDITOR
void UKausGameData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UKausGameData, UnitStatsTable))
	{
		BuildUnitStatsCache();

		if (UnitStatsTable)
		{
			UnitStatsTable->OnDataTableChanged().RemoveAll(this);
			UnitStatsTable->OnDataTableChanged().AddUObject(this, &ThisClass::HandleDataTableChanged);
		}
	}
}
void UKausGameData::HandleDataTableChanged()
{
	UE_LOG(LogKaus, Log, TEXT("KausGameData: Detected DataTable change. Rebuilding UnitStatsCache."));
	BuildUnitStatsCache();
}
#endif

void UKausGameData::BuildUnitStatsCache()
{
    UnitStatsCache.Empty();
    if (!UnitStatsTable) return;

    const TMap<FName, uint8*>& RowMap = UnitStatsTable->GetRowMap();
    for (const auto& KeyValue : RowMap)
    {
        const FKausUnitStatsRow* Row = reinterpret_cast<const FKausUnitStatsRow*>(KeyValue.Value);
        if (Row && Row->UnitTag.IsValid())
        {
            if (UnitStatsCache.Contains(Row->UnitTag))
            {
                UE_LOG(LogKaus, Warning, TEXT("Duplicate UnitTag [%s] detected in DataTable Row [%s]."), *Row->UnitTag.ToString(), *KeyValue.Key.ToString());
            }
            UnitStatsCache.Add(Row->UnitTag, *Row);
        }
    }
}
