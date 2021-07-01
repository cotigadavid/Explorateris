// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prop.h"
#include "ConstructableItem.h"
#include "Cabin.generated.h"


UCLASS()
class EXPLORATERIS_API ACabin : public AConstructableItem
{
	GENERATED_BODY()
	
public:
	ACabin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartDestroy();
	
};
