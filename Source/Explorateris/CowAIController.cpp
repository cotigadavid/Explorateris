// Fill out your copyright notice in the Description page of Project Settings.


#include "CowAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Cow.h"
#include "ArmsCharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"


/*ACowAIController::ACowAIController(FObjectInitializer const& object_initializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/Animals/Cow/AI/CowAITree.CowAITree'"));

	if (obj.Succeeded())
	{
		BehaviourTree = obj.Object;
	}

	BehaviourTreeComponent = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));

	BlackBoard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	setup_perception_system();
}

void ACowAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviourTree);

	BehaviourTreeComponent->StartTree(*BehaviourTree);
}

void ACowAIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);

	if (BlackBoard)
	{
		BlackBoard->InitializeBlackboard(*BehaviourTree->BlackboardAsset);
	}
}

UBlackboardComponent* ACowAIController::GetBlackBoard() const
{
	return BlackBoard;
}

void ACowAIController::on_updated(TArray<AActor*> const& updated_actors)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("VAZUT"));
	for (size_t x = 0; x < updated_actors.Num(); ++x)
	{
		FActorPerceptionBlueprintInfo info;
		GetPerceptionComponent()->GetActorsPerception(updated_actors[x], info);
		for (size_t k = 0; k < info.LastSensedStimuli.Num(); ++k)
		{
			FAIStimulus const stim = info.LastSensedStimuli[k];
			if (stim.Tag == tags::noise_tag)
			{

				GetBlackBoard()->SetValueAsBool(bb_keys::is_investigating, stim.WasSuccessfullySensed());
				GetBlackBoard()->SetValueAsVector(bb_keys::target_location, stim.StimulusLocation);
			}
			else if (stim.Type.Name == "Default__AISense_Sight")
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("VAZUT"));
				GetBlackBoard()->SetValueAsBool(bb_keys::can_see_player, stim.WasSuccessfullySensed());
			}
		}
	}
}

void ACowAIController::on_target_updated(AActor* actor, FAIStimulus const stimulus) {
	
	auto const ch = Cast<ACowAIController>(actor);
	
	if (ch) {
		GetBlackBoard()->SetValueAsBool(bb_keys::can_see_player, stimulus.WasSuccessfullySensed());
	}
}

void ACowAIController::setup_perception_system()
{
	// create and initialise sight configuration object
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 500.0f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.0f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		SightConfig->SetMaxAge(5.0f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;
		SightConfig->DetectionByAffiliation.bDetectEnemies =
			SightConfig->DetectionByAffiliation.bDetectFriendlies =
			SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		// add sight configuration component to perception component
		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}

	// create and initialise hearing config object
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing config"));
	if (HearingConfig)
	{
		HearingConfig->HearingRange = 3000.0f;
		HearingConfig->DetectionByAffiliation.bDetectEnemies =
			HearingConfig->DetectionByAffiliation.bDetectFriendlies =
			HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

		// add sight configuration component to perception component
		GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ACowAIController::on_updated);
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	}
}*/


ACowAIController::ACowAIController() {

	PrimaryActorTick.bCanEverTick = true;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ACowAIController::OnUpdated);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

	IsMoving = false;
}

void ACowAIController::BeginPlay() {
	Super::BeginPlay();
}

void ACowAIController::OnPossess(APawn* PawnX) {

	Super::OnPossess(PawnX);
}

void ACowAIController::Tick(float DeltaSeconds){

	Super::Tick(DeltaSeconds);

	DistanceToPlayer = GetPawn()->GetDistanceTo(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (DistanceToPlayer > AISightRadius)
	{
		bIsPlayerDetected = false;
	} 

	if (GetPawn()->GetVelocity().X == 0 && GetPawn()->GetVelocity().Y == 0 && IsMoving) {
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, [this]() {IsMoving = false; }, 5.0f, false);
	}

	//Move to Waypoint
	if (bIsPlayerDetected == false && IsMoving == false)
	{
		MoveToLocation(FVector(FMath::RandRange(-12700, 12500), FMath::RandRange(-12700, 12500), 100));
		IsMoving = true;

	}

	else if (bIsPlayerDetected == true)
	{
		//AArmsCharacter* Player = Cast<AArmsCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		//MoveToActor(Player, 5.0f);

		FVector PlayerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
		FVector CowLocation = GetPawn()->GetTargetLocation();

		FVector Target;
		
		Target.X = CowLocation.X + (CowLocation.X - PlayerLocation.X);
		Target.Y = CowLocation.Y + (CowLocation.Y - PlayerLocation.Y);
		Target.Z = CowLocation.Z;

		MoveToLocation(Target);
		IsMoving = true;

		bIsPlayerDetected = false;
	}
}

FRotator ACowAIController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);

}

void ACowAIController::OnPawnDetected(TArray<AActor*> DetectedPawns)
{
	for (size_t i = 0; i < DetectedPawns.Num(); i++)
	{
		DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);

		UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToPlayer);
	}

	bIsPlayerDetected = true;
}

void ACowAIController::OnUpdated(TArray<AActor*> const& DetectedPawns)
{
	for (size_t i = 0; i < DetectedPawns.Num(); i++)
	{
		DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);

		UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToPlayer);
	}

	bIsPlayerDetected = true;
}