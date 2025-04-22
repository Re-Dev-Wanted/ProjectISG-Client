// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCrop.h"


// Sets default values
ATestCrop::ATestCrop()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetCropTotalGrowDay(1);
	SetWaterDuration(12);
}

// Called when the game starts or when spawned
void ATestCrop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestCrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

