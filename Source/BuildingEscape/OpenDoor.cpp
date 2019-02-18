// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/TriggerVolume.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	OpenAngle = 90;
	TriggerMass = 50;
	PressurePlate = nullptr;
	Owner = nullptr;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Owner = GetOwner();

	if (PressurePlate == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("Pressure plate is not initialized in %s"),*(Owner->GetName()));
	}
}

void UOpenDoor::SetDoorAngle(int32 Angle)
{
	FRotator NewRotation(0, Angle, 0);
	Owner->SetActorRotation(NewRotation);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	
	// Poll the trigger volume 
	// If the actor that opens is in the volume,
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OnOpenRequest.Broadcast();
	}
	else
	{
		OnCloseRequest.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0;
	
	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;

	if (PressurePlate != nullptr)
	{
		PressurePlate->GetOverlappingActors(OUT OverlappingActors);

		// Iterate through them adding their masses
		for (const auto Actor : OverlappingActors)
		{
			UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Actor->GetComponentByClass(UPrimitiveComponent::StaticClass()));
			TotalMass += PrimitiveComponent->GetMass();
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Total mass of %.5f kg"), TotalMass);
	}

	return TotalMass;
}