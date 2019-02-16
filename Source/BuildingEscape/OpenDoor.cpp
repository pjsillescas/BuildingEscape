// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/TriggerVolume.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	OpenAngle = 90;
	LastDoorOpenTime = 0;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	Owner = GetOwner();
}

void UOpenDoor::SetDoorAngle(int32 Angle)
{
	FRotator NewRotation(0, Angle, 0);
	Owner->SetActorRotation(NewRotation);
}

void UOpenDoor::OpenDoor()
{
	float Angle = 180 - OpenAngle;
	SetDoorAngle(Angle);
}

void UOpenDoor::CloseDoor()
{
	float Angle = 180;
	SetDoorAngle(Angle);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	
	// Poll the trigger volume 
	// If the actor that opens is in the volume,
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it's time to close the door
	float Time = GetWorld()->GetTimeSeconds();
	if (fabs(Time - LastDoorOpenTime) >= DoorCloseDelay)
	{
		CloseDoor();
	}
}

