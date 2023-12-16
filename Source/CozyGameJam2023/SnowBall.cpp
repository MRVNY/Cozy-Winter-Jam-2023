// Fill out your copyright notice in the Description page of Project Settings.


#include "SnowBall.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASnowBall::ASnowBall()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Capsule Collider"));
	SphereComp->SetMobility(EComponentMobility::Movable);
	SphereComp->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetMobility(EComponentMobility::Movable);
	Mesh->SetupAttachment(SphereComp);
}

// Called when the game starts or when spawned
void ASnowBall::BeginPlay()
{
	Super::BeginPlay();

	Speed = InitialSpeed;
}

// Called every frame
void ASnowBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASnowBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&ASnowBall::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this,&ASnowBall::MoveRight);
	PlayerInputComponent->BindAction(TEXT("TestGrow"),EInputEvent::IE_Pressed, this ,&ASnowBall::GrowTest);
}

void ASnowBall::MoveForward(float axisValue)
{
    FVector DeltaLocation = FVector::ZeroVector;
    DeltaLocation.X = axisValue*Speed;
	SphereComp->AddForce(DeltaLocation,TEXT(""),false);
}

void ASnowBall::MoveRight(float axisValue)
{
    FVector DeltaLocation = FVector::ZeroVector;
    DeltaLocation.Y = axisValue*Speed;
	SphereComp->AddForce(DeltaLocation,TEXT(""),false);
}


void ASnowBall::GrowTest()
{
	ASnowBall::Grow(TestGrowModifRate,TestSpeedModifRate);
}
void ASnowBall::Grow(float IncreaseModifCoef, float IncreaseSpeedCoef)
{
	FVector RelativeScale = SphereComp->GetRelativeScale3D();
	RelativeScale=RelativeScale*(1+IncreaseModifCoef);

	Speed*=(1+IncreaseSpeedCoef);
	SphereComp->SetRelativeScale3D(RelativeScale);
}

void ASnowBall::AbsorbObject(float IncreaseModifCoef,UClass ObjectClass)
{
	//grow object

	//add absorbed object 3D model to snowball
}