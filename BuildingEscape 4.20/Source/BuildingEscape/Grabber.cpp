// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include  "Runtime/Engine/Public/CollisionQueryParams.h "

#define OUT		// replace OUT with nothing. The macro does nothing but it marks an out parameter


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandler();		//Look for Physics handle component
	SetupInputComponent();		//Look for Pawn Input Component
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandler) { return; }
	// if physics handle attached, move holding object
	if (PhysicsHandler->GrabbedComponent) 
	{
		FVector LineTraceEnd = LineLocation();
		PhysicsHandler->SetTargetLocation(LineTraceEnd);
	}
	
}


// grabs an object
void UGrabber::Grab()
{
	// UE_LOG(LogTemp, Warning, TEXT("You Pressed a button!"));
	FHitResult HitResult = FirstObjectHit();	// draws line and finds nearest physics object
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	
	// if hit something, attaches physics handle
	if (ActorHit) {			// same as (ActorHit != nullptr)
		
		if (!PhysicsHandler) { return; }
		PhysicsHandler->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true	// add rotation
		);


	}
}

// releases the held object
void UGrabber::Release()
{
	if (!PhysicsHandler) { return; }
	PhysicsHandler->ReleaseComponent();
}


//Look for Physics handle component
void UGrabber::FindPhysicsHandler()
{
	FString ObjectName = GetOwner()->GetName();
	PhysicsHandler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();	// looks at owner, then searches by componet for physics handler
	if (PhysicsHandler == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s missing Physics Handle component"), *(ObjectName));
	}
}


//Look for Pawn Input Component
void UGrabber::SetupInputComponent()
{
	PawnInput = GetOwner()->FindComponentByClass<UInputComponent>();

	if (PawnInput)
	{
		//bind input axis
		PawnInput->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);	//action name, event, component, reference to function when activated
		PawnInput->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Did not find Input Component!"));
	}
}


// draws line and finds nearest physics object
const FHitResult UGrabber::FirstObjectHit()
{
	// Get player view point (every tick)
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(			// rather than return values, GetPlayerViewPoint stores values in passed variables
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = LineLocation();

	// Setup query parameters
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	// Line-trace (ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return Hit;

	// draws red line

	//UE_LOG(LogTemp, Warning, TEXT("Location: %s   Rotation: %s"), 
	//	*(PlayerViewPointLocation.ToString()), 
	//	*(PlayerViewPointRotation.ToString())
	//);

	// Draws a red line in front of player
	//DrawDebugLine(						// draws a line
	//	GetWorld(),						// the world
	//	PlayerViewPointLocation,		// the location of the line (location of player)
	//	LineTraceEnd,					// end of the line
	//	FColor(255, 0, 0),				// color of line
	//	false,
	//	0.f,
	//	0.f,
	//	10.f							// line thickness
	//);

	
}

// returns the end of the line
FVector UGrabber::LineLocation() 
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(			// rather than return values, GetPlayerViewPoint stores values in passed variables
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	return LineTraceEnd;
}