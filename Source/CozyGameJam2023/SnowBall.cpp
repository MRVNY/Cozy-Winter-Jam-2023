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

	/*Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);*/

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Capsule Collider"));
	SphereComp->SetMobility(EComponentMobility::Movable);
	SphereComp->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetMobility(EComponentMobility::Movable);
	Mesh->SetupAttachment(SphereComp);

	/*
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetMobility(EComponentMobility::Movable);
	SpringArmComponent->SetupAttachment(Root);

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetMobility(EComponentMobility::Movable);
	TopDownCameraComponent->SetupAttachment(SpringArmComponent);
	*/
}

// Called when the game starts or when spawned
void ASnowBall::BeginPlay()
{
	Super::BeginPlay();
	
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
}

void ASnowBall::MoveForward(float axisValue)
{
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    FVector DeltaLocation = FVector::ZeroVector;
    DeltaLocation.X = axisValue*Speed;
	SphereComp->AddForce(DeltaLocation,TEXT(""),false);
}

void ASnowBall::MoveRight(float axisValue)
{
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    FVector DeltaLocation = FVector::ZeroVector;
    DeltaLocation.Y = axisValue*Speed;
	SphereComp->AddForce(DeltaLocation,TEXT(""),false);
}
