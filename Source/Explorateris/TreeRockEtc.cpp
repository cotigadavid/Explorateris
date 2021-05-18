// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeRockEtc.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ATreeRockEtc::ATreeRockEtc()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	SetHealthPoints(100);
}

int ATreeRockEtc::GetType()
{
	return type;
}

void ATreeRockEtc::SetType(int val)
{
	type = val;
}

// Called when the game starts or when spawned
void ATreeRockEtc::BeginPlay()
{
	Super::BeginPlay();

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("%d \n"), type));
}

// Called every frame
void ATreeRockEtc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATreeRockEtc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATreeRockEtc::AdjustPosition()
{
	FVector StartLocation = GetActorLocation();
	FVector UpVector = { 0, 0, -1 };

	FVector EndLocation = StartLocation + (UpVector * 1000.0f);
	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, ECC_Visibility, CollisionParams)) {
		if (OutHit.GetActor()) {

			ATreeRockEtc* HitActor = Cast<ATreeRockEtc>(OutHit.GetActor());

			if (!HitActor) {
				SetActorLocation(OutHit.ImpactPoint);
			}
		}
	}
}

int ATreeRockEtc::GetHealthPoints()
{
	return HealthPoints;
}

void ATreeRockEtc::SetHealthPoints(int Health)
{
	HealthPoints = Health;
}
