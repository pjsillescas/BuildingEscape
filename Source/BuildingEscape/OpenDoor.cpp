// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/TriggerVolume.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	OpenAngle = 90;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	SetDoorAngle(60);
}

void UOpenDoor::SetDoorAngle(int32 Angle)
{
	AActor* Owner = GetOwner();
	FRotator ActorRotator = Owner->GetActorRotation();

	FRotator NewRotation(0, Angle, 0);
	Owner->SetActorRotation(NewRotation);

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

