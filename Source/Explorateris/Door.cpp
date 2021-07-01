// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

ADoor::ADoor()
{
    PrimaryActorTick.bCanEverTick = true;

	Opened = false;
	ReadyState = false;

	SetNrOfLogs(0);
	SetHealth(100);
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
