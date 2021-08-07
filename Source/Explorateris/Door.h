// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prop.h"
#include "ConstructableItem.h"
#include "Components/TimelineComponent.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORATERIS_API ADoor : public AConstructableItem
{
	GENERATED_BODY()
	
private:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	ADoor();

	UFUNCTION()
		void OpenDoor(float dt);

	UFUNCTION()
		void CloseDoor(float dt);

	// declare sphere comp
	UPROPERTY(VisibleAnywhere, Category = "BoxComp")
		class UBoxComponent* BoxComp;

	UFUNCTION()
		void ToggleDoor(FVector ForwardVector);

	bool Opening;
	bool Closing;
	bool isClosed;

	float DotP;
	float MaxDegree;
	float AddRotation;
	float PosNeg;
	float DoorCurrentRotation;

};
