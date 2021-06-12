// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prop.h"
#include "Components/TimelineComponent.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORATERIS_API ADoor : public AProp
{
	GENERATED_BODY()
	
private:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	int Health;

	int NrOfLogs;

	bool ReadyState;

public:
	ADoor();

	UPROPERTY(EditAnywhere)
		UCurveFloat* DoorCurve;

	UFUNCTION()
		void ControlDoor();

	UFUNCTION()
		void ToggleDoor();

	UFUNCTION()
		void SetState();

	bool Opened;

	UFUNCTION(BlueprintPure, Category = "State")
		bool GetReadyState();

	UFUNCTION(BlueprintCallable, Category = "State")
		void SetReadyState(bool NewValue);

	float RotateValue;

	float CurveFloatValue;

	float TimelineValue;

	FTimeline DoorTimeline;

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
