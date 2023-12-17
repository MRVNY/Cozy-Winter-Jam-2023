// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SnowBall.generated.h"


class USphereComponent;
class ANPC;
enum class ESize : uint8;;

UCLASS()
class COZYGAMEJAM2023_API ASnowBall : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASnowBall();

	UFUNCTION(BlueprintCallable, Category="Absorption")
	void OnOverlapAbsorbable(AAbsorbableObject* AbsorbedObject);

	UFUNCTION(BlueprintCallable, Category="Absorption")
	void OnOverlapAbsorbableNPC(ANPC* AbsorbedNPC);

	UFUNCTION(BlueprintCallable, Category="Components")
	void SetSphereCollider(USphereComponent* SphereCollider);
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
	//UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	class UStaticMeshComponent* Mesh;

	// Absorbing object list
	TArray<AAbsorbableObject*> AbsorbedObjectList;
	
	TArray<class ANPC*> AbsorbedNpcList;

	void MoveForward(float axisValue);
	void MoveRight(float axisValue);
	void Grow(float GrowModifCoef, float SpeedCoef);
	void Grow(float ModifGrowCoef);
	void Grow(ESize AbsorbableSize);
	void GrowTest();
	bool CanAbsorbObject(ESize AbsorbableSize) const;

	int AbsorbCounter;
	void UpdateProgression();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Movement")
	float InitialSpeed = 60000.0f;

	float Speed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Movement")
	float TestGrowModifRate = 0.5f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Movement")
	float TestSpeedModifRate = 0.1f;

	float CurrentSphereRadius=0.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "AbsorbSystem")
	float SmallAbsorbRadius;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "AbsorbSystem")
	float MidAbsorbRadius;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "AbsorbSystem")
	float BigAbsorbRadius;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "AbsorbSystem")
	float HugeAbsorbRadius;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "AbsorbSystem")
	float EnormousAbsorbRadius;
};
