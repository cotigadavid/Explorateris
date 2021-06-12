// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmsCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Apple.h"
#include "LogWood.h"
#include "Stone.h"
#include "Cabin.h"
#include "TreeRockEtc.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Math/Vector.h"
#include "Kismet/GameplayStatics.h"
#include "ExploraterisGameModeBase.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Door.h"
#include "../../MyLibrary/Source/MyLibrary.h"
//#include "MyLibrary.h"

// Sets default values
AArmsCharacter::AArmsCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false;

	FakeCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FakeCamera"));
	FakeCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FakeCamera->bUsePawnControlRotation = false;

	CabinToPlaceMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CabinToPlaceMeshComponent"));
	CabinToPlaceMeshComponent->SetupAttachment(RootComponent);
	CabinToPlaceMeshComponent->SetVisibility(false);

	DoorToPlaceMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorToPlaceMeshComponent"));
	DoorToPlaceMeshComponent->SetupAttachment(RootComponent);
	DoorToPlaceMeshComponent->SetVisibility(false);

	EatAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EatAudioComponent"));
	EatAudioComponent->SetupAttachment(RootComponent);

	HitAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HitAudioComponent"));
	HitAudioComponent->SetupAttachment(RootComponent);

	PlaceAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PlaceAudioComponent"));
	PlaceAudioComponent->SetupAttachment(RootComponent);

	Inventory.Init(0, 10);
	NrOfElem.Init(0, 10);

	Select1 ();

	InventoryIsAccessed = 0;

	SetHealth(100);
	SetHunger(100);

	PlaceMode = false;
}

// Called when the game starts or when spawned
void AArmsCharacter::BeginPlay()
{
	Super::BeginPlay();

	IsPickingUp = false;

	if (EatSoundCue) {
		EatAudioComponent->SetSound(EatSoundCue);
	}
	if (HitSoundCue) {
		HitAudioComponent->SetSound(HitSoundCue);
	}
	if (PlaceSoundCue) {
		PlaceAudioComponent->SetSound(PlaceSoundCue);
	}

	//FTimerHandle handle;
	//GetWorldTimerManager().SetTimer(handle, this, &AArmsCharacter::Temp, 0.01f, true);
	
}

void AArmsCharacter::DestroySpecificCabin(ACabin* CabinToDestroy) {
	CabinToDestroy->Destroy();
}

void AArmsCharacter::Temp() {

	if (PlaceMode == true) {
		FVector SpawnLocation = FVector(0, 0, 0);
		FRotator SpawnRotation = FRotator(0, 0, 0);
		FActorSpawnParameters SpawnInfo;

		if (FakeCamera) {
			CameraLocation = FakeCamera->GetComponentLocation();
			CameraForwardVector = FakeCamera->GetForwardVector();

			CameraForwardVector = CameraForwardVector * 500.0f;
			EndLocation = CameraLocation + CameraForwardVector;

			DrawDebugLine(GetWorld(), CameraLocation, EndLocation, FColor::Green, false, 1, 0, 1);

			if (GetWorld()->LineTraceSingleByChannel(OutHit, CameraLocation, EndLocation, ECC_Visibility, CollisionParams)) {
				if (OutHit.GetActor()) {

					SpawnLocation = OutHit.ImpactPoint;

					SpawnLocation.Z = 500;
					FRotator rotate = FRotator(0, 0, 0);
					FActorSpawnParameters SpawnInfo2;

					ACabin* CabinProp = Cast<ACabin>(GetWorld()->SpawnActor<APawn>(CabinPlaceToSpawn, SpawnLocation, rotate, SpawnInfo));
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("-"));

					if (CabinProp) {
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("+"));
						CabinProp->AdjustPosition();

						CabinProp->StartDestroy();
					}
				}
			}
		}
	}
}

// Called every frame
void AArmsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHunger(GetHunger() - 0.01f);

	if (GetHunger() == 0)
		SetHealth(GetHealth() - 0.03f);

	
}

// Called to bind functionality to input
void AArmsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &AArmsCharacter::PickUp);
	PlayerInputComponent->BindAction("Place", IE_Pressed, this, &AArmsCharacter::Place);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AArmsCharacter::Jump);
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AArmsCharacter::Use);

	PlayerInputComponent->BindAxis("MoveForward", this, &AArmsCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AArmsCharacter::MoveRight);

	PlayerInputComponent->BindAxis("TurnRight", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Select1", IE_Pressed, this, &AArmsCharacter::Select1);
	PlayerInputComponent->BindAction("Select2", IE_Pressed, this, &AArmsCharacter::Select2);
	PlayerInputComponent->BindAction("Select3", IE_Pressed, this, &AArmsCharacter::Select3);
	PlayerInputComponent->BindAction("Select4", IE_Pressed, this, &AArmsCharacter::Select4);
	PlayerInputComponent->BindAction("Select5", IE_Pressed, this, &AArmsCharacter::Select5);


	PlayerInputComponent->BindAction("AccessInventory", IE_Pressed, this, &AArmsCharacter::AccessInventory);

}

void AArmsCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AArmsCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}


bool AArmsCharacter::GetIsPickingUp()
{
	return IsPickingUp;
}


void AArmsCharacter::PickUp()
{
	IsPickingUp = true;

	FTimerHandle handle1;
	GetWorldTimerManager().SetTimer(handle1, [this]() {
		IsPickingUp = false;
		}, 1.43f, false);

	if (FakeCamera) {
		CameraLocation = FakeCamera->GetComponentLocation();
		CameraForwardVector = FakeCamera->GetForwardVector();

		CameraForwardVector = CameraForwardVector * 500.0f;
		EndLocation = CameraLocation + CameraForwardVector;


		DrawDebugLine(GetWorld(), CameraLocation, EndLocation, FColor::Green, false, 1, 0, 1);

		if (GetWorld()->LineTraceSingleByChannel(OutHit, CameraLocation, EndLocation, ECC_Visibility, CollisionParams)) {
			if (OutHit.GetActor()) {

				AApple* AppleHit = Cast<AApple>(OutHit.GetActor());
				ATreeRockEtc* TreeRockHit = Cast<ATreeRockEtc>(OutHit.GetActor());
				ALogWood* LogWoodHit = Cast<ALogWood>(OutHit.GetActor());
				AStone* StoneHit = Cast<AStone>(OutHit.GetActor());
				ADoor* DoorHit = Cast<ADoor>(OutHit.GetActor());
				ACabin* CabinHit = Cast<ACabin>(OutHit.GetActor());

				if (AppleHit) {

					int i, ok = 0;
					for (i = 0; i < 5; ++i) {
						//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("%d %d %d \n"), i, Inventory[i], NrOfElem[i]));
						if (GetInventory(i) == 1 && GetNrOfElem(i) < 6) {
							SetNrOfElem(i, GetNrOfElem(i) + 1);
							ok = 1;
							break;
						}
					}

					if (!ok) {
						for (i = 0; i < 5 && GetInventory(i) != 0; ++i) {}
						SetInventory(i, 1);
						SetNrOfElem(i, GetNrOfElem(i) + 1);
					}

					AppleHit->Destroy();
				}

				if (TreeRockHit) {
					int type = TreeRockHit->GetType();

					if (type < 5) {
						TreeRockHit->SetHealthPoints(TreeRockHit->GetHealthPoints() - 15);

						if (TreeRockHit->GetHealthPoints() <= 0) {

							int i, ok = 0;
							for (i = 0; i < 5; ++i) {
								if (GetInventory(i) == 2 && GetNrOfElem(i) < 6) {
									SetNrOfElem(i, GetNrOfElem(i) + 1);
									ok = 1;
									break;
								}
							}

							if (!ok) {
								for (i = 0; i < 5 && GetInventory(i) != 0; ++i) {}
								SetInventory(i, 2);
								SetNrOfElem(i, GetNrOfElem(i) + 1);
							}

							TreeRockHit->Destroy();
						}

						PlayHitSound();

					}
					else {
						TreeRockHit->SetHealthPoints(TreeRockHit->GetHealthPoints() - 10);

						if (TreeRockHit->GetHealthPoints() <= 0) {

							int i, ok = 0;
							for (i = 0; i < 5; ++i) {
								if (GetInventory(i) == 3 && GetNrOfElem(i) < 6) {
									SetNrOfElem(i, GetNrOfElem(i) + 1);
									ok = 1;
									break;
								}
							}

							if (!ok) {
								for (i = 0; i < 5 && GetInventory(i) != 0; ++i) {}
								SetInventory(i, 3);
								SetNrOfElem(i, GetNrOfElem(i) + 1);
							}

							TreeRockHit->Destroy();
						}
						PlayHitSound();
					}
				}

				if (LogWoodHit) {
					int i, ok = 0;
					for (i = 0; i < 5; ++i) {
						if (GetInventory(i) == 2 && GetNrOfElem(i) < 6) {
							SetNrOfElem(i, GetNrOfElem(i) + 1);
							ok = 1;
							break;
						}
					}

					if (!ok) {
						for (i = 0; i < 5 && GetInventory(i) != 0; ++i) {}
						SetInventory(i, 2);
						SetNrOfElem(i, GetNrOfElem(i) + 1);
					}

					LogWoodHit->Destroy();
				}

				if (StoneHit) {
					int i, ok = 0;
					for (i = 0; i < 5; ++i) {
						if (GetInventory(i) == 3 && GetNrOfElem(i) < 6) {
							SetNrOfElem(i, GetNrOfElem(i) + 1);
							ok = 1;
							break;
						}
					}

					if (!ok) {
						for (i = 0; i < 5 && GetInventory(i) != 0; ++i) {}
						SetInventory(i, 3);
						SetNrOfElem(i, GetNrOfElem(i) + 1);
					}

					StoneHit->Destroy();
				}

				if (DoorHit) {

					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Door"));

					DoorHit->ToggleDoor();
				}

				if (CabinHit) {
					CabinHit->SetHealth(CabinHit->GetHealth() - 10);
					PlayHitSound();
				}
			}
		}
	}
}

int AArmsCharacter::GetInventory(int index)
{
	if (index >= 0 && index < 5)
		return Inventory[index];
	return -1;
}

void AArmsCharacter::SetInventory(int index, int val)
{
	Inventory[index] = val;
}

int AArmsCharacter::GetNrOfElem(int index)
{
	if (index >= 0 && index < 5)
		return NrOfElem[index];
	return -1;
}

void AArmsCharacter::SetNrOfElem(int index, int val)
{
	NrOfElem[index] = val;
}

void AArmsCharacter::Select1()
{
	SetSelectedNR(1);
}

void AArmsCharacter::Select2()
{
	SetSelectedNR(2);
}

void AArmsCharacter::Select3()
{
	SetSelectedNR(3);
}

void AArmsCharacter::Select4()
{
	SetSelectedNR(4);
}

void AArmsCharacter::Select5()
{
	SetSelectedNR(5);
}

void AArmsCharacter::SelectHoldObject()
{
	
}

int AArmsCharacter::GetSelectedNR()
{
	return SelectedNr;
}

void AArmsCharacter::SetSelectedNR(int NewNumber)
{
	SelectedNr = NewNumber;
}

void AArmsCharacter::Place()
{

	FVector SpawnLocation = FVector(0, 0, 0);
	FRotator SpawnRotation = FRotator(0, 0, 0);
	FActorSpawnParameters SpawnInfo;

	if (GetPlaceMode())
		PlaceInPlaceMode();

	else if (FakeCamera) {
		CameraLocation = FakeCamera->GetComponentLocation();
		CameraForwardVector = FakeCamera->GetForwardVector();

		CameraForwardVector = CameraForwardVector * 500.0f;
		EndLocation = CameraLocation + CameraForwardVector;

		DrawDebugLine(GetWorld(), CameraLocation, EndLocation, FColor::Green, false, 1, 0, 1);

		if (GetWorld()->LineTraceSingleByChannel(OutHit, CameraLocation, EndLocation, ECC_Visibility, CollisionParams)) {
			if (OutHit.GetActor()) {
				SpawnLocation = OutHit.ImpactPoint;

				if (GetInventory(SelectedNr - 1) != 0 && GetNrOfElem(SelectedNr - 1) > 0) {

					if (AppleToSpawn && GetInventory(SelectedNr - 1) == 1) {
						SpawnLocation.Z += 20;
						GetWorld()->SpawnActor<APawn>(AppleToSpawn, SpawnLocation, SpawnRotation, SpawnInfo);
					}

					if (LogToSpawn && GetInventory(SelectedNr - 1) == 2) {
						SpawnLocation.Z += 10;

						ACabin* CabinInConstruction = Cast<ACabin>(OutHit.GetActor());
						ADoor* DoorInConstruction = Cast<ADoor>(OutHit.GetActor());

						if (CabinInConstruction && CabinInConstruction->GetNrOfLogs() < 5) {
							CabinInConstruction->SetNrOfLogs(CabinInConstruction->GetNrOfLogs() + 1);
						}
						else if (DoorInConstruction && DoorInConstruction->GetNrOfLogs() < 1) {
							DoorInConstruction->SetNrOfLogs(DoorInConstruction->GetNrOfLogs() + 1);
						}
						else {
							GetWorld()->SpawnActor<APawn>(LogToSpawn, SpawnLocation, SpawnRotation, SpawnInfo);
						}
					}

					if (StoneToSpawn && GetInventory(SelectedNr - 1) == 3) {
						SpawnLocation.Z += 10;
						GetWorld()->SpawnActor<APawn>(StoneToSpawn, SpawnLocation, SpawnRotation, SpawnInfo);
					}

					SetNrOfElem(SelectedNr - 1, GetNrOfElem(SelectedNr - 1) - 1);
					PlayPlaceSound();
				}
			}
		}

		for (int i = 0; i < 5; ++i) {
			if (GetNrOfElem(i) == 0)
				Inventory[i] = 0;
		}
	}
}

void AArmsCharacter::PlaceInPlaceMode() {

	FVector SpawnLocation = FVector(0, 0, 0);
	FRotator SpawnRotation = FRotator(0, 0, 0);
	FActorSpawnParameters SpawnInfo;

	if (FakeCamera) {
		CameraLocation = FakeCamera->GetComponentLocation();
		CameraForwardVector = FakeCamera->GetForwardVector();

		CameraForwardVector = CameraForwardVector * 1500.0f;
		EndLocation = CameraLocation + CameraForwardVector;

		DrawDebugLine(GetWorld(), CameraLocation, EndLocation, FColor::Green, false, 1, 0, 1);

		if (GetWorld()->LineTraceSingleByChannel(OutHit, CameraLocation, EndLocation, ECC_Visibility, CollisionParams)) {
			if (OutHit.GetActor()) {
				SpawnLocation = OutHit.ImpactPoint;

				if (ItemToPlace == 0) {
					ACabin* CabinProp = Cast<ACabin>(GetWorld()->SpawnActor<APawn>(CabinPlaceToSpawn, SpawnLocation, SpawnRotation, SpawnInfo));
					SetPlaceMode(false);
					CabinToPlaceMeshComponent->SetVisibility(false);

					if (CabinProp) {
						CabinProp->AdjustPosition();
					}
				}

				if (ItemToPlace == 1) {
					SpawnRotation.Yaw = GetActorRotation().Yaw + 90;

					ADoor* DoorProp = Cast<ADoor>(GetWorld()->SpawnActor<APawn>(DoorPlaceToSpawn, SpawnLocation, SpawnRotation, SpawnInfo));
					SetPlaceMode(false);
					DoorToPlaceMeshComponent->SetVisibility(false);

					if (DoorProp) {
						DoorProp->AdjustPosition();
					}
				}
			}
		}
	}
}

void AArmsCharacter::AccessInventory()
{

	if (!InventoryIsAccessed) {
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

		InventoryIsAccessed = 1;
	}
	else {
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

		InventoryIsAccessed = 0;
	}
}

bool AArmsCharacter::GetInventoryIsAccessed()
{
	return InventoryIsAccessed;
}

void AArmsCharacter::SetInventoryIsAccessed(bool NewState)
{
	InventoryIsAccessed = NewState;
}

void AArmsCharacter::Swap(int index1, int index2)
{
	MyLibrary::swap(Inventory[index1], Inventory[index2]);
	MyLibrary::swap(NrOfElem[index1], NrOfElem[index2]);
}

float AArmsCharacter::GetHealth()
{
	return Health;
}

void AArmsCharacter::SetHealth(float HealthP)
{
	Health = HealthP;

	if (Health > 100)
		Health = 100;
	if (Health < 0)
		Health = 0;
}

float AArmsCharacter::GetHunger()
{
	return Hunger;
}

void AArmsCharacter::SetHunger(float HungerP)
{
	Hunger = HungerP;

	if (Hunger > 100)
		Hunger = 100;
	if (Hunger < 0)
		Hunger = 0;
}

void AArmsCharacter::Use()
{
	if (GetInventory(SelectedNr - 1) == 1 && GetNrOfElem(SelectedNr - 1) > 0) {
		SetNrOfElem(SelectedNr - 1, GetNrOfElem(SelectedNr - 1) - 1);
		SetHunger(GetHunger() + 20);

		PlayEatSound();
	}

	for (int i = 0; i < 5; ++i) {
		if (GetNrOfElem(i) == 0)
			Inventory[i] = 0;
	}
}

void AArmsCharacter::PlayHitSound()
{
	if (HitAudioComponent && HitSoundCue)
		HitAudioComponent->Play(0.0f);
}

void AArmsCharacter::PlayPlaceSound()
{
	if (PlaceAudioComponent && PlaceSoundCue)
		PlaceAudioComponent->Play(0.0f);
}

bool AArmsCharacter::GetPlaceMode()
{
	return PlaceMode;
}

void AArmsCharacter::SetPlaceMode(bool NewState)
{
	PlaceMode = NewState;
}

int AArmsCharacter::GetItemToPlace()
{
	return ItemToPlace;
}

void AArmsCharacter::SetItemToPlace(int NewCode)
{
	ItemToPlace = NewCode;
}

void AArmsCharacter::PlayEatSound()
{
	if (EatAudioComponent && EatSoundCue)
		EatAudioComponent->Play(0.0f);
}
