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
void ASnowBall::SetSphereCollider(USphereComponent* SphereCollider)
{
	SphereComp = SphereCollider;
}
// Called when the game starts or when spawned
void ASnowBall::BeginPlay()
{
	Super::BeginPlay();

	Speed = InitialSpeed;

	AbsorbedObjectList = TArray<AAbsorbableObject*>();


	//get sphere radius at init
	FVector Origin;
	FVector BoxExtent;
	UKismetSystemLibrary::GetComponentBounds(Mesh,Origin,BoxExtent,CurrentSphereRadius);
	
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

void ASnowBall::Grow(float ModifGrowCoef)
{
	ASnowBall::Grow(ModifGrowCoef,0.f);
}


void ASnowBall::Grow(const AAbsorbableObject* AbsorbedObject)
{

	float GrowthFactor = 0.0f;

	switch (AbsorbedObject->Size)
	{
		case ESize::SE_Tiny:
			GrowthFactor = 0.02f;
			break;
		case ESize::SE_Small:
			GrowthFactor = 0.04f;
			break;
		case ESize::SE_Mid:
			GrowthFactor = 0.08f;
			break;
		case ESize::SE_Big:
			GrowthFactor = 0.12f;
			break;
		case ESize::SE_Huge:
			GrowthFactor = 0.16f;
			break;
		case ESize::SE_Enormous:
			GrowthFactor = 0.24f;
			break;
		default:
			break;
	}

	ASnowBall::Grow(GrowthFactor,-GrowthFactor/30);
}

void ASnowBall::Grow(float ModifGrowCoef, float ModifSpeedCoef)
{
	FVector RelativeScale = Mesh->GetRelativeScale3D();
	RelativeScale *= (1+ModifGrowCoef);
	Mesh->SetRelativeScale3D(RelativeScale);

	CurrentSphereRadius*= (1+ModifGrowCoef);
	Speed*=(1+ModifSpeedCoef);

	//reset absorbed objects local position to in account the scaling up of the parent
	for(AAbsorbableObject* AbsorbedObject : AbsorbedObjectList)
	{
		UStaticMeshComponent* ObjMesh = AbsorbedObject->ObjMesh;
		FVector LocalOffset = ObjMesh->GetComponentLocation() - Mesh->GetComponentLocation();
		LocalOffset /= LocalOffset.Length();
		ObjMesh->SetWorldLocation(Mesh->GetComponentLocation()+LocalOffset*AbsorbedObject->AbsorbedRadius);
	}

	UE_LOG(LogTemp, Warning, TEXT("CurrentSphereRadius = %f"),CurrentSphereRadius);
}


bool ASnowBall::CanAbsorbObject(const AAbsorbableObject* AbsorbableObject) const
{
	bool canAbsorb = false;
	switch (AbsorbableObject->Size)
	{
		case ESize::SE_Tiny:
			canAbsorb = true;
			break;
		case ESize::SE_Small:
			canAbsorb = CurrentSphereRadius > SmallAbsorbRadius;
			break;
		case ESize::SE_Mid:
			canAbsorb = CurrentSphereRadius > MidAbsorbRadius;
			break;
		case ESize::SE_Big:
			canAbsorb = CurrentSphereRadius > BigAbsorbRadius;
			break;
		case ESize::SE_Huge:
			canAbsorb = CurrentSphereRadius > HugeAbsorbRadius;
			break;
		case ESize::SE_Enormous:
			canAbsorb = CurrentSphereRadius > EnormousAbsorbRadius;
			break;
		default:
			break;
	}
	return canAbsorb;
}


void ASnowBall::OnOverlapAbsorbable(AAbsorbableObject* AbsorbedObject)
{
	bool canAbsorb = CanAbsorbObject(AbsorbedObject);
	if(!canAbsorb)
	{
		return;
	}
	UStaticMeshComponent* ObjMesh = AbsorbedObject->ObjMesh;

	//grow object
	FVector Origin;
	FVector BoxExtent;
	float SphereRadius;
	UKismetSystemLibrary::GetComponentBounds(Mesh,Origin,BoxExtent,SphereRadius);

	float Volume = BoxExtent.X*BoxExtent.Y*BoxExtent.Z/FMath::Pow(100.f,3);


	Grow(AbsorbedObject);


	//add absorbed object 3D model to snowball
	//disable collision
	ObjMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	//attach to snowball
	ObjMesh->AttachToComponent(Mesh,FAttachmentTransformRules::KeepWorldTransform);

	// move the object slightly inwards

	// //interpolate the object to the new position
	//FVector LocalOffset = ObjMesh->GetComponentLocation() - Mesh->GetComponentLocation();
	//AbsorbedObject->SnowBallLocalUnitVector = LocalOffset/LocalOffset.Length();
	//ObjMesh->SetWorldLocation(Mesh->GetComponentLocation()+AbsorbedObject->SnowBallLocalUnitVector*CurrentSphereRadius);

	AbsorbedObject->AbsorbedRadius = CurrentSphereRadius;


	AbsorbedObjectList.Add(AbsorbedObject);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Snowball is absorbingabsor!"));
}