// Fill out your copyright notice in the Description page of Project Settings.

#include "AIPatrolController.h"
#include "AIPatrolCharacter.h"
#include "CheckPointActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "CourseWork_CMP302Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/PointLight.h"
#include "Components/PointLightComponent.h"

AAIPatrolController::AAIPatrolController()
{
	PrimaryActorTick.bCanEverTick = true;

	//Create sight configuration object
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	//Initialise sight configuration object
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 550.f;

	//what types of objects that can be detected
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());

	//Call function when something is preceived by the bot
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAIPatrolController::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AAIPatrolController::BeginPlay()
{
	Super::BeginPlay();

	//check perception component works
	if (GetPerceptionComponent() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetPerceptionComponent: working"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetPerceptionComponent: not working"));
	}
}

void AAIPatrolController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AAIPatrolController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AAIPatrolCharacter* PatrolCharacter = Cast<AAIPatrolCharacter>(GetPawn());

	//When bot is inside shadow set perception radius to 100.f and when inside light reset to 500.f
	if (GetInsideLight())
	{
		SightConfig->SightRadius = 500.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 550.f;

		if (GetPerceptionComponent())
		{
			//Call function when something is preceived by the bot
			GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAIPatrolController::OnPawnDetected);
			GetPerceptionComponent()->ConfigureSense(*SightConfig);
		}
	}
	else if (!GetInsideLight())
	{
		SightConfig->SightRadius = 300.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 350.f;

		if (GetPerceptionComponent())
		{
			//Call function when something is preceived by the bot
			GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAIPatrolController::OnPawnDetected);
			GetPerceptionComponent()->ConfigureSense(*SightConfig);
		}
	}

	//if player goes out off the bot sight radius, the bot stops chasing player
	if (DistanceFromPlayer > SightConfig->LoseSightRadius)
	{
		IsPlayerDetected = false;
	}

	if (PatrolCharacter->NextCheckpoint != nullptr && IsPlayerDetected == false)
	{
		//Move to CheckPoint
		MoveToActor(PatrolCharacter->NextCheckpoint, 5.0f);

		//Make Dummy walk at 100.f when moving to each checkpoint
		PatrolCharacter->GetCharacterMovement()->MaxWalkSpeed = 100.f;
	}
	else if (IsPlayerDetected == true)
	{
		ACourseWork_CMP302Character* Player = Cast<ACourseWork_CMP302Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Player)
		{
			//Chase player
			MoveToActor(Player, 5.0f);

			//make Dummy run at 500.f when chasing player
			PatrolCharacter->GetCharacterMovement()->MaxWalkSpeed = 500.f;
		}
	}
}

FRotator AAIPatrolController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);

}

void AAIPatrolController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	for (size_t i = 0; i < DetectedPawns.Num(); i++)
	{
		//store distance from bot to player
		DistanceFromPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);
	}

	//bot now detects player
	IsPlayerDetected = true;
}

bool AAIPatrolController::GetInsideLight()
{
	bool isInsideLight = false;

	//get lights
	TArray<AActor*> PointLights;
	UGameplayStatics::GetAllActorsOfClass(this, APointLight::StaticClass(), PointLights);

	//loop through lights
	for (int32 i = 0; i < PointLights.Num(); i++)
	{
		APointLight* Light = Cast<APointLight>(PointLights[i]);
		if (Light)
		{
			FVector EyeLoc;
			FRotator EyeRot;

			//calculate distance from bots eyes
			GetActorEyesViewPoint(EyeLoc, EyeRot);

			//Distance between the ai eyes and Point light 
			float Distance = (Light->GetActorLocation() - EyeLoc).Size();
			
			//if true, the bot is inside the light
			if (Distance < Light->PointLightComponent->AttenuationRadius)
			{
				//use collision to check for any objects that are infront of the light
				FCollisionQueryParams QueryParams;
				QueryParams.AddIgnoredActor(this);
				QueryParams.bTraceComplex = true;

				FHitResult Hit;
				//if true, no object inbetween light and ai bot
				if (!GetWorld()->LineTraceSingleByChannel(Hit, EyeLoc, Light->GetActorLocation(), ECC_Visibility, QueryParams))
				{
					//we are visible to a light
					isInsideLight = true;
					//when seen by a light, break from for loop
					break;
				}
			}
		}
		else if (!Light)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Controller: Light is nullptr");
		}

	}

	return isInsideLight;
}