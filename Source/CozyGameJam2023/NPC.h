// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPC.generated.h"

enum class ESize : uint8;;

UCLASS()
class COZYGAMEJAM2023_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFleeing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESize Size;

	float AbsorbedRadius;

	UFUNCTION(BlueprintImplementableEvent, Category = "NPC")
	void TestFunction();

	UFUNCTION(BlueprintImplementableEvent, Category = "NPC")
	void Idle();

	UFUNCTION(BlueprintImplementableEvent, Category = "NPC")
	void Walk();

	UFUNCTION(BlueprintImplementableEvent, Category = "NPC")
	void Run();

private:
	APawn* Snowball;
	AAIController* NPCController;
	//animator
	UAnimInstance* Animator;

	FVector RandomDirection;
	FVector FleeDirection;


	float AbsorbedRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESize Size;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

};
