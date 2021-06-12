// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prop.h"
#include "ConstructableItem.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORATERIS_API AConstructableItem : public AProp
{
	GENERATED_BODY()

public:
	AConstructableItem();

private:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	int Health;

	int NrOfLogs;

public:
	void AdjustPosition();

	UFUNCTION(BlueprintPure, Category = "Build")
		int GetNrOfLogs();

	UFUNCTION(BlueprintCallable, Category = "Build")
		void SetNrOfLogs(int NewValue);

	UFUNCTION(BlueprintPure, Category = "Health")
		int GetHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetHealth(int NewValue);
};
