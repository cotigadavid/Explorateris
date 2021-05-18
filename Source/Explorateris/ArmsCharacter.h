// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArmsCharacter.generated.h"

UCLASS()
class EXPLORATERIS_API AArmsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))  //meta
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) // meta
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) // meta
		class UCameraComponent* FakeCamera;

	// --- sound
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio)
		class USoundCue* EatSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio)
		class USoundCue* HitSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio)
		class USoundCue* PlaceSoundCue;

public:
	// Sets default values for this character's properties
	AArmsCharacter();

	void MoveForward(float Value);

	void MoveRight(float Value);

	void PickUp();

	bool IsPickingUp;

	UFUNCTION(BlueprintPure, Category = "PickingUp")
		bool GetIsPickingUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// ------- PickUp

	FVector CameraLocation;
	FVector CameraForwardVector;
	FVector EndLocation;
	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	// ------- PickUp

	TArray<int> Inventory;
	TArray<int> NrOfElem;

	UFUNCTION(BlueprintPure, Category = "Inventory")
		int GetInventory(int index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void SetInventory(int index, int val);

	UFUNCTION(BlueprintPure, Category = "Inventory")
		int GetNrOfElem(int index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void SetNrOfElem(int index, int val);

	// ------- Selection

	int SelectedNr;

	void Select1();

	void Select2();

	void Select3();

	void Select4();

	void Select5();

	void SelectHoldObject();

	UFUNCTION(BlueprintPure, Category = "Selection")
		int GetSelectedNR();

	// -------- Place

	void Place();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> AppleToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> LogToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> StoneToSpawn;

	// -------- Inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void AccessInventory();

	bool InventoryIsAccessed;

	UFUNCTION(BlueprintPure, Category = "Inventory")
		bool GetInventoryIsAccessed();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void Swap(int index1, int index2);

	AActor* ActorInHand;

	// --------- Health And Hunger

	float Health;
	float Hunger;

	UFUNCTION(BlueprintPure, Category = "Health")
		float GetHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetHealth(float HealthP);

	UFUNCTION(BlueprintPure, Category = "Hunger")
		float GetHunger();

	UFUNCTION(BlueprintCallable, Category = "Hunger")
		void SetHunger(float HungerP);

	// --------- Health And Hunger

	void Use();

	// ---------  Sound

	class UAudioComponent* EatAudioComponent;
	class UAudioComponent* HitAudioComponent;
	class UAudioComponent* PlaceAudioComponent;

	void PlayEatSound();
	void PlayHitSound();
	void PlayPlaceSound();
};
