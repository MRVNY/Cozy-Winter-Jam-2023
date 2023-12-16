// Fill out your copyright notice in the Description page of Project Settings.


#include "AbsorbableObject.h"


// Sets default values
AAbsorbableObject::AAbsorbableObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//link with alreay created mesh
	// Cube2 = 
	// Cube2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube2"));
	// UStaticMesh* Cube = 
}

// Called when the game starts or when spawned
void AAbsorbableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAbsorbableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAbsorbableObject::OnHit()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm Object!"));
	// log on screen
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("I'm Object!"));
}

