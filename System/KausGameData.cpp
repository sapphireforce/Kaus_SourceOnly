#include "KausGameData.h"
#include "KausAssetManager.h"

UKausGameData::UKausGameData()
{
}

const UKausGameData& UKausGameData::UKausGameData::Get()
{
	return UKausAssetManager::Get().GetGameData();
}
