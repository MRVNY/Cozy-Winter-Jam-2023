// Fill out your copyright notice in the Description page of Project Settings.


#include "SnowBall.h"

#include "AbsorbableObject.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASnowBall::ASnowBall()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	SphereComp->SetMobility(EComponentMobility::Movable);
	SphereComp->SetupAttachment(RootComponent);
	*/

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetMobility(EComponentMobility::Movable);
	Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASnowBall::BeginPlay()
{
	Super::BeginPlay();

	Speed = InitialSpeed;

	AbsorbedObjectList = TArray<AAbsorbableObject*>();


	FVector Origin;
	FVector BoxExtent;
	UKismetSystemLibrary::GetComponentBounds(Mesh,Origin,BoxExtent,CurrentSphereRadius);
	
	RndUnitVector = FVector::UpVector;
}

// Called every frame
void ASnowBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Origin;
	FVector BoxExtent;
	float SphereRadius;
	UKismetSystemLibrary::GetComponentBounds(Mesh,Origin,BoxExtent,SphereRadius);

	DrawDebugSphere(GetWorld(),Mesh->GetComponentLocation()+RndUnitVector*CurrentSphereRadius,5.0f,20,FColor::Red,false,0.1f);
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
	Mesh->AddForce(DeltaLocation,TEXT(""),false);
}

void ASnowBall::MoveRight(float axisValue)
{
    FVector DeltaLocation = FVector::ZeroVector;
    DeltaLocation.Y = axisValue*Speed;
	Mesh->AddForce(DeltaLocation,TEXT(""),false);
}


void ASnowBall::GrowTest()
{
	ASnowBall::Grow(TestGrowModifRate,TestSpeedModifRate);
}
void ASnowBall::Grow(float ModifGrowCoef, float ModifSpeedCoef)
{
	FVector RelativeScale = Mesh->GetRelativeScale3D();
	RelativeScale *= (1+ModifGrowCoef);
	Mesh->SetRelativeScale3D(RelativeScale);

	CurrentSphereRadius*= (1+ModifGrowCoef);
	Speed*=(1+ModifSpeedCoef);

	for(AAbsorbableObject* AbsorbedObject : AbsorbedObjectList)
	{
		UStaticMeshComponent* ObjMesh = AbsorbedObject->ObjMesh;
		FVector LocalOffset = ObjMesh->GetComponentLocation() - Mesh->GetComponentLocation();
		LocalOffset /= LocalOffset.Length();
		ObjMesh->SetWorldLocation(Mesh->GetComponentLocation()+LocalOffset*CurrentSphereRadius);
	}
}

void ASnowBall::OnHitObject(float IncreaseModifCoef,AAbsorbableObject* AbsorbedObject)
{
	
	//grow object

	//add absorbed object 3D model to snowball
	UStaticMeshComponent* ObjMesh = AbsorbedObject->ObjMesh;

	//disable collision
	ObjMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AbsorbedObjectList = TArray<AAbsorbableObject*>();

	//attach to snowball
	ObjMesh->AttachToComponent(Mesh,FAttachmentTransformRules::KeepWorldTransform);

	// //move the object slightly inwards
	// FVector RelativeLocation = ObjMesh->GetRelativeLocation();
	// RelativeLocation=RelativeLocation*0.5f;
	//
	// //interpolate the object to the new position
	// ObjMesh->SetRelativeLocation(RelativeLocation,true);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Snowball is absorbingabsor!"));
}



void ASnowBall::OnOverlapAbsorbable(AAbsorbableObject* AbsorbedObject)
{
	//grow object

	//add absorbed object 3D model to snowball
	UStaticMeshComponent* ObjMesh = AbsorbedObject->ObjMesh;


	//disable collision
	ObjMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	//attach to snowball
	ObjMesh->AttachToComponent(Mesh,FAttachmentTransformRules::KeepWorldTransform);

	// move the object slightly inwards

	// //interpolate the object to the new position
	FVector LocalOffset = ObjMesh->GetComponentLocation() - Mesh->GetComponentLocation();
	AbsorbedObject->SnowBallLocalUnitVector = LocalOffset/LocalOffset.Length();
	ObjMesh->SetWorldLocation(Mesh->GetComponentLocation()+AbsorbedObject->SnowBallLocalUnitVector*CurrentSphereRadius);


	AbsorbedObjectList.Add(AbsorbedObject);


	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Snowball is absorbingabsor!"));
}