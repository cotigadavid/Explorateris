// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructableItem.h"
#include "TreeRockEtc.h"

AConstructableItem::AConstructableItem() {

	NrOfLogs = 0;

	Health = 100;
}

void AConstructableItem::BeginPlay() 
{
	Super::BeginPlay();
}

void AConstructableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AConstructableItem::AdjustPosition()
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

int AConstructableItem::GetNrOfLogs()
{
	return NrOfLogs;
}

void AConstructableItem::SetNrOfLogs(int NewValue)
{
	NrOfLogs = NewValue;
}

int AConstructableItem::GetHealth()
{
	return Health;
}

void AConstructableItem::SetHealth(int NewValue)
{
	Health = NewValue;
}
