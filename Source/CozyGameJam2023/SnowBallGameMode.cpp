// Fill out your copyright notice in the Description page of Project Settings.


#include "SnowBallGameMode.h"
#include "SnowBall.h"
#include "Kismet/GameplayStatics.h"

void ASnowBallGameMode::BeginPlay()
{
    Super::BeginPlay();
    SnowBall = Cast<ASnowBall>(UGameplayStatics::GetPlayerPawn(this,0));

    if(SnowBall)
    {
        UE_LOG(LogTemp, Warning, TEXT("SnowBall Actor : %s"),*SnowBall->GetActorNameOrLabel());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SnowBall Actor not found"));
    }
}