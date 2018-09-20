// Fill out your copyright notice in the Description page of Project Settings.

#include "TickTacOpenDoors.h"


// Sets default values for this component's properties
UTickTacOpenDoors::UTickTacOpenDoors()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTickTacOpenDoors::BeginPlay()
{
	Super::BeginPlay();
	
	// checking for nullptr
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing PressurePlate (nullptr)"), *(GetOwner()->GetName()));
	}
	
	if (!OpeningActor) {
		UE_LOG(LogTemp, Error, TEXT("%s missing OpeningActor (nullptr"), *(GetOwner()->GetName()));
	}
}


// Called every frame
void UTickTacOpenDoors::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// guarding against nullptr
	if (!PressurePlate) { return; }
	if (!OpeningActor) { return; }

	if (PressurePlate->IsOverlappingActor(OpeningActor))
	{
		OpenDoor();
		CloseTrigger = true;
	}
	else if (CloseTrigger) 
	{
		CloseDoor();
		CloseTrigger = false;
	}
}


void UTickTacOpenDoors::OpenDoor()
{

	// Find the owning Actor
	AActor* Owner = GetOwner();

	// Create a new rotator
	FRotator NewRotation = FRotator(0.0, OpenAngle, 0.0);

	// Set Door rotation
	Owner->SetActorRotation(NewRotation);

}

void UTickTacOpenDoors::CloseDoor()
{

	// Find the owning Actor
	AActor* Owner = GetOwner();

	// Create a new rotator
	FRotator NewRotation = FRotator(0.0, CloseAngle, 0.0);

	// Set Door rotation
	Owner->SetActorRotation(NewRotation);

}