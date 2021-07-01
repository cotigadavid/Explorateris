// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prop.h"
#include "ToolBase.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORATERIS_API AToolBase : public AProp
{
	GENERATED_BODY()
	
public:

	AToolBase();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:

	int Type;

public:

	UFUNCTION(BlueprintPure, Category = "Type")
		int GetType();

	UFUNCTION(BlueprintCallable, Category = "Type")
		void SetType(int NewType);

	void Delete();
};
