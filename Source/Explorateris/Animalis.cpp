// Fill out your copyright notice in the Description page of Project Settings.


#include "Animalis.h"

// Sets default values
AAnimalis::AAnimalis()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Health = 100;
}

// Called when the game starts or when spawned
void AAnimalis::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAnimalis::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAnimalis::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int AAnimalis::GetHealth()
{
	return Health;
}

void AAnimalis::SetHealth(int newHealth)
{
	Health = newHealth;
}

