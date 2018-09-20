// Fill out your copyright notice in the Description page of Project Settings.

#include "FinalDoor.h"


// Sets default values for this component's properties
UFinalDoor::UFinalDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFinalDoor::BeginPlay()
{
	Super::BeginPlay();

	// checking for nullptr
	if (!WoodPressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing WoodPressurePlate (nullptr)"), *(GetOwner()->GetName()));
	}
	if (!PaperPressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing PaperPressurePlate (nullptr)"), *(GetOwner()->GetName()));
	}
	if (!OpeningWoodActor) {
		UE_LOG(LogTemp, Error, TEXT("%s missing OpeningWoodActor (nullptr)"), *(GetOwner()->GetName()));
	}
	if (!OpeningPaperActor) {
		UE_LOG(LogTemp, Error, TEXT("%s missing OpeningPaperActor (nullptr)"), *(GetOwner()->GetName()));
	}
	if (!WaterPressurePlate1) {
		UE_LOG(LogTemp, Error, TEXT("%s missing WaterPressurePlate1 (nullptr)"), *(GetOwner()->GetName()));
	}
	if (!WaterPressurePlate2) {
		UE_LOG(LogTemp, Error, TEXT("%s missing WaterPressurePlate2 (nullptr)"), *(GetOwner()->GetName()));
	}
	if (!WaterPressurePlate3) {
		UE_LOG(LogTemp, Error, TEXT("%s missing WaterPressurePlate3 (nullptr)"), *(GetOwner()->GetName()));
	}
}


// Called every frame
void UFinalDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// guarding against nullptr
	if (!WoodPressurePlate) { return; }
	if (!PaperPressurePlate) { return; }
	if (!OpeningWoodActor) { return; }
	if (!OpeningPaperActor) { return; }
	if (!WaterPressurePlate1) { return; }
	if (!WaterPressurePlate2) { return; }
	if (!WaterPressurePlate3) { return; }

	// fire room requirements
	if (WoodPressurePlate->IsOverlappingActor(OpeningWoodActor) &&
		PaperPressurePlate->IsOverlappingActor(OpeningPaperActor)) 
	{
		fireRoom = true;
		fireTrigger = true;
	}
	else if (fireTrigger) 
	{
		fireRoom = false;
		fireTrigger = false;
	}
	
	
	// water room requirements
	if (
		(GetTotalMassOnPlate(WaterPressurePlate1) >= 20.f) &&
		(GetTotalMassOnPlate(WaterPressurePlate2) >= 20.f) &&
		(GetTotalMassOnPlate(WaterPressurePlate3) >= 20.f))
	{
		waterRoom = true;
		waterTrigger = true;
	}
	else if (waterTrigger)
	{
		waterRoom = false;
		waterTrigger = false;
	}
	
	// final door requirements
	if (fireRoom && waterRoom) 
	{
		OpenDoor();
		finalTrigger = true;
	}
	else if (finalTrigger)
	{
		CloseDoor();
		finalTrigger = false;
	}
	
}


void UFinalDoor::OpenDoor()
{
	// Find the owning Actor
	AActor* Owner = GetOwner();

	// Create a new rotator
	FRotator NewRotation = FRotator(0.0, -90, 0.0);

	// Set Door rotation
	Owner->SetActorRotation(NewRotation);
}


void UFinalDoor::CloseDoor()
{
	// Find the owning Actor
	AActor* Owner = GetOwner();

	// Create a new rotator
	FRotator NewRotation = FRotator(0.0, 0.0, 0.0);

	// Set Door rotation
	Owner->SetActorRotation(NewRotation);
}


float UFinalDoor::GetTotalMassOnPlate(ATriggerVolume* PressurePlate)
{

	float TotalMass = 0.f;

	// find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);		// initializes TArray
																	// iterate through them to find total mass on plate
	for (const auto* CurrentActor : OverlappingActors)				// cycles through TArray
	{
		TotalMass += CurrentActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}