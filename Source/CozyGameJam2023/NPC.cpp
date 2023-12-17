// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	Snowball = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	NPCController = GetController<AAIController>();
	Animator = GetMesh()->GetAnimInstance();

	// Animator->

	//execute funtion in blueprint class
	TestFunction();

	Walk();
	
	//show on screen

	RandomDirection = FMath::VRand().GetSafeNormal2D() * 500.f + GetActorLocation();
	//draw RandomDirection in scene as debug
	//DrawDebugLine(GetWorld(), GetActorLocation(), RandomDirection, FColor::Red, false, 1.f, 0, 1.f);
	
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, std::to_string(GetCharacterMovement()->MaxWalkSpeed).data());
	
	FleeDirection = RandomDirection;

}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(isAbsorbed)
	{
		return;
	}

	//Check distance with the Snowball
	float Distance = GetDistanceTo(Snowball);
	
	if (Distance < 500.f)
	{
		if(!IsFleeing)
		{
			// FleeDirection = GetActorLocation() + (GetActorLocation() - Snowball->GetActorLocation()).GetSafeNormal() * 500.f;
			// DrawDebugLine(GetWorld(), GetActorLocation(), FleeDirection, FColor::Red, false, 10.f, 0, 1.f);
			IsFleeing = true;
			//max walk speed
			GetCharacterMovement()->MaxWalkSpeed = 200.f;
			
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, std::to_string(GetCharacterMovement()->MaxWalkSpeed).data());
			Run();
		}
		FleeDirection = GetActorLocation() + (GetActorLocation() - Snowball->GetActorLocation()).GetSafeNormal() * 500.f;
		NPCController->MoveToLocation(FleeDirection);
		//speed
	}

	if (Distance >= 500.f)
	{
		if(IsFleeing)
		{
			IsFleeing = false;
			GetCharacterMovement()->MaxWalkSpeed = 100.f;
			Walk();
			//go to random direction
			RandomDirection = FMath::VRand().GetSafeNormal2D() * 500.f + GetActorLocation();
			//DrawDebugLine(GetWorld(), GetActorLocation(), RandomDirection, FColor::Red, false, 10.f, 0, 1.f);
		}
		NPCController->MoveToLocation(RandomDirection);
	}

}



// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

