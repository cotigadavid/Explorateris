// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolBase.h"

AToolBase::AToolBase() {

}

void AToolBase::BeginPlay()
{

}

void AToolBase::Tick(float DeltaTime)
{

}

int AToolBase::GetType()
{
	return Type;
}

void AToolBase::SetType(int NewType)
{
	Type = NewType;
}

void AToolBase::Delete()
{
	FVector Location = GetActorLocation();
	
	Location.Z -= 500;

	SetActorLocation(Location);
}
