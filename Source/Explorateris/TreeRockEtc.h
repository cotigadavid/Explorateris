// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TreeRockEtc.generated.h"

UCLASS()
class EXPLORATERIS_API ATreeRockEtc : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATreeRockEtc();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* StaticMeshComponent;


	UFUNCTION(BlueprintPure, Category = "Type")
		int GetType();

	UFUNCTION(BlueprintCallable, Category = "Type")
		void SetType(int val);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AdjustPosition();

	// ------------ Health

	int HealthPoints;

	UFUNCTION(BlueprintPure, Category = "Health")
		int GetHealthPoints();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetHealthPoints(int Health);

	// ------------ Health

private:
	UPROPERTY(VisibleAnywhere)
		int type;
};
