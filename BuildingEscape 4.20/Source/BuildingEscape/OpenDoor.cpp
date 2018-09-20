// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

#define OUT		// replace OUT with nothing. The macro does nothing but it marks an out parameter

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	if (!PressurePlate) {
	
		UE_LOG(LogTemp, Error, TEXT("%s missing PressurePlate (nullptr in OpenDoor.ccp)"), *(GetOwner()->GetName()));
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	if((GetTotalMassOnPlate() >= TriggerMass) && (!ReverseDoorAngle))
	{
		OnOpenNormal.Broadcast();
		DoorTriggered = true;
	}
	else if ((GetTotalMassOnPlate() >= TriggerMass) && (ReverseDoorAngle)) 
	{
		OnOpenReverse.Broadcast();
		DoorTriggered = true;
	}
	else if(DoorTriggered)
	{
		OnClose1.Broadcast();
		OnClose2.Broadcast();
		DoorTriggered = false;
	}

}

// returns the total mass overlapping the trigger volume to decide if door should open
float UOpenDoor::GetTotalMassOnPlate()
{
	
	float TotalMass = 0.f;
	
	// find all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);		// initializes TArray


	// iterate through them to find total mass on plate
	for (const auto* CurrentActor : OverlappingActors)				// cycles through TArray
	{
		TotalMass += CurrentActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("%s is on plate"), *(CurrentActor->GetName()));

	}
	
	return TotalMass;
}


//void UOpenDoor::OpenTheDoor()
//{
//
//	AActor* Owner = GetOwner();				// Find the owning Actor
//	FRotator NewRotation = FRotator(0.0, -(OpenAngle), 0.0);		// Create a new rotator
//	Owner->SetActorRotation(NewRotation);	// Set Door rotation
//
//	OnOpen.Broadcast();
//}