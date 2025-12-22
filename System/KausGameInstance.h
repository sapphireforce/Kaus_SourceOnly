/*


*/

#pragma once

#include "CoreMinimal.h"
#include "CommonGameInstance.h"
#include "KausGameInstance.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class KAUS_API UKausGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()
	
public:
	UKausGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	

	virtual void Init() override;
	virtual void Shutdown() override;
	
};
