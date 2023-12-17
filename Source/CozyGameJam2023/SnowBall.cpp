// Fill out your copyright notice in the Description page of Project Settings.


#include "SnowBall.h"

#include "AbsorbableObject.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "NPC.h"
#include "Components/CapsuleComponent.h"
#include "DynamicMesh/ColliderMesh.h"
#include "Kismet/KismetMathLibrary.h"

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

	AbsorbedNpcList = TArray<ANPC*>();

	//get sphere radius at init
	FVector Origin;
	FVector BoxExtent;
	UKismetSystemLibrary::GetComponentBounds(Mesh,Origin,BoxExtent,CurrentSphereRadius);

	//count all the object of the class AbsorbableObject
	TArray<AActor*> FoundActors;
	TArray<AActor*> FoundNpcs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AAbsorbableObject::StaticClass(),FoundActors);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ANPC::StaticClass(),FoundNpcs);
	AbsorbCounter = FoundActors.Num();
	AbsorbCounter += FoundNpcs.Num();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, std::to_string(AbsorbCounter).data());
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


void ASnowBall::Grow(ESize AbsorbableSize)
{

	float GrowthFactor = 0.0f;

	switch (AbsorbableSize)
	{
		case ESize::SE_Tiny:
			GrowthFactor = TinyAbsorbGrowth;
			break;
		case ESize::SE_Small:
			GrowthFactor = SmallAbsorbGrowth;
			break;
		case ESize::SE_Mid:
			GrowthFactor = MidAbsorbGrowth;
			break;
		case ESize::SE_Big:
			GrowthFactor = BigAbsorbGrowth;
			break;
		case ESize::SE_Huge:
			GrowthFactor = HugeAbsorbGrowth;
			break;
		case ESize::SE_Enormous:
			GrowthFactor = EnormousAbsorbGrowth;
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


	for(ANPC* Npc : AbsorbedNpcList)
	{
		USkeletalMeshComponent* ObjMesh = Npc->GetMesh();
		FVector LocalOffset = ObjMesh->GetComponentLocation() - Mesh->GetComponentLocation();
		LocalOffset /= LocalOffset.Length();
		ObjMesh->SetWorldLocation(Mesh->GetComponentLocation()+LocalOffset*Npc->AbsorbedRadius);
	}

	UE_LOG(LogTemp, Warning, TEXT("CurrentSphereRadius = %f"),CurrentSphereRadius);
}


bool ASnowBall::CanAbsorbObject(ESize AbsorbableSize) const
{
	bool canAbsorb = false;
	switch (AbsorbableSize)
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

void ASnowBall::UpdateProgression()
{
	AbsorbCounter--;
	
	//getwidget
	// UE::Widget* Widget = Cast<UE::Widget>(UGameplayStatics::GetPlayerController(GetWorld(),0)->GetHUD()->GetUserWidgetObject());

	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, std::to_string(AbsorbCounter).data());
	if(AbsorbCounter<=0)
	{
		//end game
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You win!"));
	}
}


void ASnowBall::OnOverlapAbsorbable(AAbsorbableObject* AbsorbedObject)
{
	bool canAbsorb = CanAbsorbObject(AbsorbedObject->Size);
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


	Grow(AbsorbedObject->Size);


	//add absorbed object 3D model to snowball
	//disable collision
	ObjMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	//attach to snowball
	ObjMesh->AttachToComponent(Mesh,FAttachmentTransformRules::KeepWorldTransform);
	
	AbsorbedObject->AbsorbedRadius = CurrentSphereRadius;


	AbsorbedObjectList.Add(AbsorbedObject);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Snowball absorbed object!"));
	UpdateProgression();
}


void ASnowBall::OnOverlapAbsorbableNPC(ANPC* AbsorbedNPC)
{

	ESize NpcSize = AbsorbedNPC->Size;
	bool canAbsorb = CanAbsorbObject(NpcSize);
	if(!canAbsorb)
	{
		return;
	}
	USkeletalMeshComponent* ObjMesh = AbsorbedNPC->GetMesh();
	
	//disableCollider
	UCapsuleComponent* NpcCollider = AbsorbedNPC->GetCapsuleComponent();
	NpcCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//grow object
	Grow(NpcSize);


	//// add absorbed object 3D model to snowball ////
	//stop navigation
	AbsorbedNPC->GetController()->StopMovement();
	//stop animation
	AbsorbedNPC->OnAbsorbNpcAnimation();


	//disable collision
	ObjMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);




	// move the object slightly inwards

	// //interpolate the object to the new position
	FVector LocalOffset = ObjMesh->GetComponentLocation() - Mesh->GetComponentLocation();
	LocalOffset = LocalOffset/LocalOffset.Length();
	//ObjMesh->SetWorldLocation(Mesh->GetComponentLocation()+LocalOffset*CurrentSphereRadius);


	//rotate mesh
	/*LocalOffset += TestAbsorbOffset;
	FRotator Rotation = FRotator::MakeFromEuler(LocalOffset);
	ObjMesh->SetWorldRotation(Rotation);
	AbsorbedNPC->SetActorRotation(Rotation);*/



	//attach to snowball
	ObjMesh->AttachToComponent(Mesh,FAttachmentTransformRules::KeepWorldTransform);

	AbsorbedNPC->AbsorbedRadius = CurrentSphereRadius;
	AbsorbedNPC->isAbsorbed = true;

	AbsorbedNpcList.Add(AbsorbedNPC);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Snowball absorbed NPC!"));
	UpdateProgression();
}

void ASnowBall::OnOverlapText(AAbsorbableObject* AbsorbedObject)
{
	
	// //disable collision
	// AbsorbedObject->Children[0]->FindComponentByClass
	// 	->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	//attach to snowball
	AbsorbedObject->AttachToComponent(Mesh,FAttachmentTransformRules::KeepWorldTransform);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Snowball absorbed Text!"));
}

