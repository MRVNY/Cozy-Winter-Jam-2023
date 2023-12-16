// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SnowBall.generated.h"

UCLASS()
class COZYGAMEJAM2023_API ASnowBall : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASnowBall();

	UFUNCTION(BlueprintCallable, Category="Object")
	void OnHitObject(float IncreaseModifCoef, AAbsorbableObject* AbsorbedObject);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//collider
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	class USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	class UStaticMeshComponent* Mesh;

	// Absorbing object list
	TArray<AAbsorbableObject*> AbsorbedObjectList;

	/*
	//Root
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;

	//Top down camera/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	//Spring Arm positioning the camera above the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;
	*/

	void MoveForward(float axisValue);
	void MoveRight(float axisValue);
	void Grow(float IncreaseModifCoef, float IncreaseSpeedCoef);
	void GrowTest();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Movement")
	float InitialSpeed = 60000.0f;

	float Speed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Movement")
	float TestGrowModifRate = 0.5f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Movement")
	float TestSpeedModifRate = 0.1f;
};
