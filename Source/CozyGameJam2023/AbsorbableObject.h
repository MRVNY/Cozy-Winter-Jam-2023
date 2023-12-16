// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbsorbableObject.generated.h"

UCLASS()
class COZYGAMEJAM2023_API AAbsorbableObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAbsorbableObject();

	UFUNCTION(BlueprintCallable, Category="Object")
	void OnHit();

	class UStaticMeshComponent* ObjMesh;

	float OnAbsorbGrowCoeff = 0.2f;

	FVector SnowBallLocalUnitVector;

	float AbsorbedRadius;

private:
	// UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
