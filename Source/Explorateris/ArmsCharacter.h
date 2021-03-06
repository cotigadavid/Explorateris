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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* CabinToPlaceMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* DoorToPlaceMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* ToolComponent;

	// --- sound
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio)
		class USoundCue* EatSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio)
		class USoundCue* HitSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio)
		class USoundCue* PlaceSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio)
		class USoundCue* CowSoundCue;

	// ------ Niagara

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UNiagaraComponent* EatNiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* HitWoodNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* HitStoneNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* HitCowNiagaraSystem;

	// Sets default values for this character's properties
	AArmsCharacter();

	void MoveForward(float Value);

	void MoveRight(float Value);

	void PickUp();

	UFUNCTION(BlueprintPure, Category = "PickingUp")
		bool GetIsPickingUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	// --------- Inter

	FVector CameraLocation;
	FVector CameraForwardVector;
	FVector EndLocation;
	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;
	
	// --------- Inter

	bool IsPickingUp;

	bool CanPickUp;

	TArray<int> Inventory;
	TArray<int> NrOfElem;

	int SelectedNr;

	bool InventoryIsAccessed;

	float Health;
	float Hunger;

	bool PlaceMode;

	int ItemToPlace;

	// ---------- AI

	class UAIPerceptionStimuliSourceComponent* stimulus;

	void SetupStimulus();

public:
	void Temp();

	void DestroySpecificCabin(class ACabin* CabinToDestroy);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category = "Inventory")
		int GetInventory(int index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void SetInventory(int index, int val);

	UFUNCTION(BlueprintPure, Category = "Inventory")
		int GetNrOfElem(int index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void SetNrOfElem(int index, int val);

	// ------- Selection

	void Select1();

	void Select2();

	void Select3();

	void Select4();

	void Select5();

	void SelectHoldObject();

	UFUNCTION(BlueprintPure, Category = "Selection")
		int GetSelectedNR();

	UFUNCTION(BlueprintCallable, Category = "Selection")
		void SetSelectedNR(int NewNumber);

	// -------- PickUp

	UFUNCTION(BlueprintPure, Category = "PickUp")
		bool GetCanPickUp();

	UFUNCTION(BlueprintCallable, Category = "PickUp")
		void SetCanPickUp(bool NewValue);


	// -------- Place

	void Place();

	void PlaceInPlaceMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> AppleToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> LogToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> StoneToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> CabinPlaceToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> DoorPlaceToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> AxeToSpawn;


	// -------- Inventory
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void AccessInventory();

	UFUNCTION(BlueprintPure, Category = "Inventory")
		bool GetInventoryIsAccessed();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void SetInventoryIsAccessed(bool NewState);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void Swap(int index1, int index2);

	// --------- Health And Hunger

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
	class UAudioComponent* CowAudioComponent;

	void PlayEatSound();
	void PlayHitSound();
	void PlayPlaceSound();

	// --------  PlaceCabinOrSo

	UFUNCTION(BlueprintPure, Category = "Place")
		bool GetPlaceMode();

	UFUNCTION(BlueprintCallable, Category = "Place")
		void SetPlaceMode(bool NewState);

	UFUNCTION(BlueprintPure, Category = "Place")
		int GetItemToPlace();

	UFUNCTION(BlueprintCallable, Category = "Place")
		void SetItemToPlace(int NewCode);


};
