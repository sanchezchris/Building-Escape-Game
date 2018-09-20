// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"

#include "FinalDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UFinalDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFinalDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	void OpenDoor();
	void CloseDoor();
	float GetTotalMassOnPlate(ATriggerVolume*);

	UPROPERTY(EditAnywhere)
	ATriggerVolume* WoodPressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PaperPressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* OpeningWoodActor = nullptr;
	
	UPROPERTY(EditAnywhere)
	AActor* OpeningPaperActor = nullptr;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* WaterPressurePlate1 = nullptr;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* WaterPressurePlate2 = nullptr;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* WaterPressurePlate3 = nullptr;

	bool fireRoom = false;
	bool fireTrigger = false;
	bool waterRoom = false;
	bool waterTrigger = false;
	bool finalTrigger = false;

};
