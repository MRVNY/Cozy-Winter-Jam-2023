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

private:
	// UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	class UStaticMeshComponent* Cube;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
