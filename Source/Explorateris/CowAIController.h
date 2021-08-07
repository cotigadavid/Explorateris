// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "CowAIController.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORATERIS_API ACowAIController : public AAIController
{
	GENERATED_BODY()

public:

	ACowAIController();

	void BeginPlay() override;

	void OnPossess(APawn* Pawn) override;

	//class UBlackboardComponent* GetBlackBoard() const;

	void Tick(float DeltaSeconds) override;

	FRotator GetControlRotation() const override;

/*private:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* BehaviourTreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* BehaviourTree;

	class UBlackboardComponent* BlackBoard;

	class UAISenseConfig_Sight* SightConfig;

	class UAISenseConfig_Hearing* HearingConfig;

	UFUNCTION()
		void on_updated(TArray<AActor*> const& updated_actors);

	UFUNCTION()
		void on_target_updated(AActor* actor, FAIStimulus const stimulus);

	void setup_perception_system();*/
	
	UFUNCTION()
		void OnUpdated(TArray<AActor*> const& DetectedPawns);

	UFUNCTION()
		void OnPawnDetected(TArray<AActor*> DetectedPawns);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightRadius = 500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightAge = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AILoseSightRadius = AISightRadius + 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AIFieldOfView = 90.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool bIsPlayerDetected = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float DistanceToPlayer = 0.0f;

	bool IsMoving;
};
