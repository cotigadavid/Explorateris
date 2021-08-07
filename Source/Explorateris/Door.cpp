// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MyBoxComponent"));
	BoxComp->InitBoxExtent(FVector(150, 100, 100));
	BoxComp->SetCollisionProfileName("Trigger");

	FHitResult SetLocationResult;
	//BoxComp->SetRelativeLocation(FVector(100, 0, 210), false, &SetLocationResult, ETeleportType::None);
	BoxComp->SetWorldLocation(FVector(100, 0, 210));
	//RootComponent = BoxComp;

	StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 50.0f, -100.0f));
	StaticMeshComponent->SetWorldScale3D(FVector(1.f));

	isClosed = true;

	Opening = false;
	Closing = false;

	DotP = 0.0f;
	MaxDegree = 0.0f;
	AddRotation = 0.0f;
	PosNeg = 0.0f;
	DoorCurrentRotation = 0.0f;

	SetNrOfLogs(0);
	SetHealth(100);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	//DrawDebugBox(GetWorld(), GetActorLocation(), BoxComp->GetScaledBoxExtent(), FQuat(GetActorRotation()), FColor::Turquoise, true, -1, 0, 2);
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Opening)
	{
		OpenDoor(DeltaTime);
	}

	if (Closing)
	{
		CloseDoor(DeltaTime);
	}
}

void ADoor::OpenDoor(float dt)
{
	DoorCurrentRotation = StaticMeshComponent->GetRelativeRotation().Yaw;

	AddRotation = PosNeg * dt * 80;

	if (FMath::IsNearlyEqual(DoorCurrentRotation, MaxDegree, 1.5f))
	{
		Closing = false;
		Opening = false;
	}
	else if (Opening)
	{
		FRotator NewRotation = FRotator(0.0f, AddRotation, 0.0f);
		StaticMeshComponent->AddRelativeRotation(FQuat(NewRotation), false, 0, ETeleportType::None);
	}
}

void ADoor::CloseDoor(float dt)
{
	DoorCurrentRotation = StaticMeshComponent->GetRelativeRotation().Yaw;

	if (DoorCurrentRotation > 0)
	{
		AddRotation = -dt * 80;
	}
	else
	{
		AddRotation = dt * 80;
	}

	if (FMath::IsNearlyEqual(DoorCurrentRotation, 0.0f, 1.5f))
	{
		Closing = false;
		Opening = false;
	}
	else if (Closing)
	{
		FRotator NewRotation = FRotator(0.0f, AddRotation, 0.0f);
		StaticMeshComponent->AddRelativeRotation(FQuat(NewRotation), false, 0, ETeleportType::None);
	}
}

void ADoor::ToggleDoor(FVector ForwardVector)
{
	DotP = FVector::DotProduct(BoxComp->GetForwardVector(), ForwardVector);

	// get 1 or -1 from the the dot product
	PosNeg = FMath::Sign(DotP);

	// degree to clamp at
	MaxDegree = PosNeg * 80.0f;

	// toggle bools
	if (isClosed) {
		isClosed = false;
		Closing = false;
		Opening = true;

	}
	else {
		Opening = false;
		isClosed = true;
		Closing = true;
	}
}

