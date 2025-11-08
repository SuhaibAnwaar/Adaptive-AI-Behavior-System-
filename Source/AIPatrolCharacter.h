// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIPatrolCharacter.generated.h"

UCLASS()
class COURSEWORK_CMP302_API AAIPatrolCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIPatrolCharacter();

	//stores next checkpoint, set in unreal editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ACheckPointActor* NextCheckpoint;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
