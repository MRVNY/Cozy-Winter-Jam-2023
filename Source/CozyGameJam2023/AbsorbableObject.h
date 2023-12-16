// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbsorbableObject.generated.h"


UENUM(BlueprintType)
enum class ESize : uint8 
{
	SE_Tiny		UMETA(DisplayName="Tiny"), //glasses, soccer ball, toys, mushrooms
	SE_Small	UMETA(DisplayName="Small"), //christmas elf, small characters, furniture
	SE_Mid      UMETA(DisplayName="Mid"), //tall characters, small trees
	SE_Big      UMETA(DisplayName="Big"), //cars, trees
	SE_Huge		UMETA(DisplayName="Huge"), // big trees, small buildings
	SE_Enormous		UMETA(DisplayName="Enormous") //big buildings
};

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESize Size;

private:
	// UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
