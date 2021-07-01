// Fill out your copyright notice in the Description page of Project Settings.


#include "Cabin.h"

ACabin::ACabin() 
{

	SetNrOfLogs(0);

	SetHealth(100);
}

void ACabin::BeginPlay()
{
	Super::BeginPlay();
}

void ACabin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetHealth() <= 0)
		Destroy();
}



void ACabin::StartDestroy()
{
	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, [this]() {
		Destroy();
		}, 0.01f, false);
}
