// Fill out your copyright notice in the Description page of Project Settings.


#include "HoedField.h"


// Sets default values
AHoedField::AHoedField()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHoedField::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHoedField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

