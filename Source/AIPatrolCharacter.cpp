// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPatrolCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AAIPatrolCharacter::AAIPatrolCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//bot faces direction of acceleration
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//rotation speed
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
}

// Called when the game starts or when spawned
void AAIPatrolCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIPatrolCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

// Called to bind functionality to input
void AAIPatrolCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

