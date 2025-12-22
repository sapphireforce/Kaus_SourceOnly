#pragma once
#include "CommonPlayerController.h"
#include "KausPlayerController.generated.h"

/**
 * AKausPlayerController
 *
 *	The base player controller class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base player controller class used by this project."))
class KAUS_API AKausPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()
	
	
public:
	AKausPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface

	
};
