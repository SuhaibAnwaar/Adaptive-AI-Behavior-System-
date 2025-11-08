// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPointActor.h"
#include "AIPatrolCharacter.h"

// Sets default values
ACheckPointActor::ACheckPointActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set up root component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(Root);

	//Trigger box represent a checkpoint
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	BoxComponent->SetupAttachment(GetRootComponent());

	//if bot enters trigger box call function
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACheckPointActor::OnBotEnter);

	//resize trigger box
	BoxComponent->InitBoxExtent(FVector(80.f, 80.f, 90.f));
}

// Called when the game starts or when spawned
void ACheckPointActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheckPointActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckPointActor::OnBotEnter(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAIPatrolCharacter* PatrolCharacter = nullptr;

	if (OtherActor != nullptr)
	{
		PatrolCharacter = Cast<AAIPatrolCharacter>(OtherActor);
		if (PatrolCharacter != nullptr)
		{
			//set patrol ai to next checkpoint
			PatrolCharacter->NextCheckpoint = NextCheckpoint;
		}
	}
}
