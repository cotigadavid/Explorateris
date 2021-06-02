// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prop.h"
#include "Cabin.generated.h"


UCLASS()
class EXPLORATERIS_API ACabin : public AProp
{
	GENERATED_BODY()
	
public:
	ACabin();

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Text)  
		//class UTextRenderComponent* LogCountText;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:

	int Health;

	int NrOfLogs;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AdjustPosition();

	void StartDestroy();

	UFUNCTION(BlueprintPure, Category = "------")
		int GetNrOfLogs();

	UFUNCTION(BlueprintCallable, Category = "------")
		void SetNrOfLogs(int NewValue);

	UFUNCTION(BlueprintPure, Category = "Health")
		int GetHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetHealth(int NewValue);
};
