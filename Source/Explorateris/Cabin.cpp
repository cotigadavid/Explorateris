// Fill out your copyright notice in the Description page of Project Settings.


#include "Cabin.h"
//#include "Components/TextRenderComponent.h"

ACabin::ACabin() 
{

	/*LogCountText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("LogCount"));
	LogCountText->SetupAttachment(RootComponent);

	LogCountText->SetText(TEXT("0"));
	LogCountText->SetTextRenderColor(FColor::Black);
	LogCountText->SetXScale(1.0f);
	LogCountText->SetYScale(1.0f);*/

	NrOfLogs = 0;

	Health = 100;
}

void ACabin::BeginPlay()
{
	Super::BeginPlay();
}

void ACabin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Health <= 0)
		Destroy();
}

void ACabin::AdjustPosition()
{
	FVector StartLocation = GetActorLocation();
	FVector UpVector = { 0, 0, -1 };

	FVector EndLocation = StartLocation + (UpVector * 1000.0f);
	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, ECC_Visibility, CollisionParams)) {
		if (OutHit.GetActor()) {

			ACabin* HitActor = Cast<ACabin>(OutHit.GetActor());

			if (!HitActor) {
				SetActorLocation(OutHit.ImpactPoint);
			}
		}
	}
}

void ACabin::StartDestroy()
{
	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, [this]() {
		Destroy();
		}, 0.01f, false);
}

int ACabin::GetNrOfLogs()
{
	return NrOfLogs;
}

void ACabin::SetNrOfLogs(int NewValue)
{
	NrOfLogs = NewValue;
}

int ACabin::GetHealth()
{
	return Health;
}

void ACabin::SetHealth(int NewValue)
{
	Health = NewValue;
}
