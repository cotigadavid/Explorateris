// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

ADoor::ADoor()
{
    PrimaryActorTick.bCanEverTick = true;

	Opened = false;
	ReadyState = false;

	NrOfLogs = 0;
	Health = 100;
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DoorTimeline.TickTimeline(DeltaTime);

	if (GetHealth() <= 0)
		Destroy();
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	RotateValue = 1.0f;

	if (DoorCurve) {

		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, FName("ControlDoor"));
		TimelineFinishedCallback.BindUFunction(this, FName("SetState"));

		DoorTimeline.AddInterpFloat(DoorCurve, TimelineCallback);
		DoorTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);

	}
}

void ADoor::ControlDoor()
{
	TimelineValue = DoorTimeline.GetPlaybackPosition();
	CurveFloatValue = RotateValue * DoorCurve->GetFloatValue(TimelineValue);

	FQuat NewRotation = FQuat(FRotator(0.f, CurveFloatValue, 0.f));

	StaticMeshComponent->SetRelativeRotation(NewRotation);
}

void ADoor::ToggleDoor()
{
    if (ReadyState)
    {
        Opened = !Opened;

        if (Opened)
        {
            RotateValue = 1.0f;

            ReadyState = false;
            DoorTimeline.PlayFromStart();
        }
        else
        {
            ReadyState = false;
            DoorTimeline.Reverse();
        }
    }
}

void ADoor::SetState()
{
	ReadyState = true;
}

bool ADoor::GetReadyState()
{
	return ReadyState;
}

void ADoor::SetReadyState(bool NewValue)
{
	ReadyState = NewValue;
}

void ADoor::AdjustPosition()
{
	FVector StartLocation = GetActorLocation();
	FVector UpVector = { 0, 0, -1 };

	FVector EndLocation = StartLocation + (UpVector * 1000.0f);
	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, ECC_Visibility, CollisionParams)) {
		if (OutHit.GetActor()) {

			ADoor* HitActor = Cast<ADoor>(OutHit.GetActor());

			if (!HitActor) {
				SetActorLocation(OutHit.ImpactPoint);
			}
		}
	}
}

int ADoor::GetNrOfLogs()
{
	return NrOfLogs;
}

void ADoor::SetNrOfLogs(int NewValue)
{
	NrOfLogs = NewValue;
}

int ADoor::GetHealth()
{
	return Health;
}

void ADoor::SetHealth(int NewValue)
{
	Health = NewValue;
}
