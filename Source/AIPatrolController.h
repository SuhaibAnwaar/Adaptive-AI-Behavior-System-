// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIPatrolController.generated.h"

/**
 * 
 */
UCLASS()
class COURSEWORK_CMP302_API AAIPatrolController : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIPatrolController();

	virtual void BeginPlay() override;

	//function runs when Ai Controller possess bot 
	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaSeconds) override;

	//Ai preception component to point in the direction the bot is facing 
	virtual FRotator GetControlRotation() const override;

	//process any detected objects
	UFUNCTION()
		void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

	//check if bot is inside the light or shadow
	UFUNCTION()
		bool GetInsideLight();

	//Sight radius of bot
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightRadius = 500.0f;

	//length at which player is no longer in bot's radius after it is seen
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AILoseSightRadius = AISightRadius + 25.0f;

	//The amount of time which the bot has forgotten about the player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightAge = 1.0f;

	//field of view of the bot, set to 90 so the bot can only see infront
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AIFieldOfView = 90.0f;

	//how close the player must stay from its last known location to keep being seen
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AILastSeenLocation = 550.f;

	//sight preception component so the bot can see and detect player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		class UAISenseConfig_Sight* SightConfig;

	//does the bot detect the player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool IsPlayerDetected = false;

	//the bot's distance from player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float DistanceFromPlayer = 0.0f;
};
