// Copyright Epic Games, Inc. All Rights Reserved.


#include "ExploraterisGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "TreeRockEtc.h"
#include "Math/UnrealMathUtility.h"


AExploraterisGameModeBase::AExploraterisGameModeBase()
{

}

void AExploraterisGameModeBase::SpawnPawn(int nType)
{
	int x = FMath::RandRange(-12600, 12770);
	int y = FMath::RandRange(-12600, 12770);
	int z = 500;

	FVector location = FVector(x, y, z);
	FRotator rotate = FRotator(0, 0, 0);
	FActorSpawnParameters SpawnInfo;


	//APawn* Temp = GetWorld()->SpawnActor<APawn>(TreeRockToSpawn, location, rotate, SpawnInfo);
	ATreeRockEtc* Prop = Cast<ATreeRockEtc>(GetWorld()->SpawnActor<APawn>(TreeRockToSpawn, location, rotate, SpawnInfo));

	if (Prop) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Loaded"));
		Prop->SetType(nType);
		Prop->AdjustPosition();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("%d \n"), Prop->GetType()));
	}
	//GetWorld()->SpawnActor<APawn>(TreeRockToSpawn, location, rotate, SpawnInfo);
}

void AExploraterisGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (InGameHUD != NULL) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), InGameHUD);

		if (InGameHUD != NULL)
			CurrentWidget->AddToViewport();
	}

	for (int type = 0; type <= 4; ++type)
		for (int i = 0; i < 50; ++i)
			SpawnPawn(type);

	for (int type = 5; type <= 7; ++type)
		for (int i = 0; i < 50; ++i)
			SpawnPawn(type);

	//for (int i = 0; i < ind; ++i)
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("%d "), TreeRockList[i]->GetType()));
}

void AExploraterisGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AExploraterisGameModeBase::GetIsBuildBookOpen()
{
	return IsBuildBookOpen;
}

void AExploraterisGameModeBase::SetIsBuildBookOpen(bool NewState)
{
	IsBuildBookOpen = NewState;
}
