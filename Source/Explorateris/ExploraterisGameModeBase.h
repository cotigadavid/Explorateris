// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ExploraterisGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORATERIS_API AExploraterisGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	AExploraterisGameModeBase();

	// ---------- spawn prop
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<APawn> TreeRockToSpawn;

	void SpawnPawn(int type);

	// ---------- spawn prop

protected:
	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "HUD", Meta = (BlueprintProtected = "true"));
	TSubclassOf<class UUserWidget> InGameHUD;

	UPROPERTY()
		class UUserWidget* CurrentWidget;

private:
	bool IsBuildBookOpen;

	UFUNCTION(BlueprintPure, Category = "BuildBook")
		bool GetIsBuildBookOpen();

	UFUNCTION(BlueprintCallable, Category = "BuildBook")
		void SetIsBuildBookOpen(bool NewState);
};
