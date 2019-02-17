// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsHandleComponent.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	PhysicsHandle = nullptr;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	// Look for attached physics handle
	PhysicsHandle = Cast<UPhysicsHandleComponent>(GetOwner()->GetComponentByClass(UPhysicsHandleComponent::StaticClass()));
	if (PhysicsHandle != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Physics handle found!!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber: No physics handle found in actor '%s'"), *(GetOwner()->GetName()));
	}

}

void UGrabber::SetupInputComponent()
{
	InputComponent = Cast<UInputComponent>(GetOwner()->GetComponentByClass(UInputComponent::StaticClass()));
	if (InputComponent != nullptr)
	{
		// Bind inputs
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber: No input component found in actor '%s'"), *(GetOwner()->GetName()));
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector Location;
	FRotator Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Location, OUT Rotation);

	FVector LineTraceEnd = Location + Reach * Rotation.Vector();
	FCollisionQueryParams QueryParams(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, Location, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), QueryParams);

	return Hit;
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Releasing grabbed object"));

	// Release physics handle
	if(PhysicsHandle != nullptr)
		PhysicsHandle->ReleaseComponent();
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));

	// Line trace and see if we reach any actors with physics body collision channel set
	FHitResult Hit = GetFirstPhysicsBodyInReach();

	// If we hit something, then attach a physics handle
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit != nullptr)
	{
		auto ComponentToGrab = Hit.GetComponent();
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None,ComponentToGrab->GetOwner()->GetActorLocation(),true /* Allow rotation */);
		
		FString Name = ActorHit->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Hitting actor %s"), *Name);
	}
	// TODO attach a physics handle
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	// if the physics handle is attached
	if (PhysicsHandle != nullptr && PhysicsHandle->GrabbedComponent != nullptr)
	{
		// move the object that we're holding
		FVector Location;
		FRotator Rotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Location, OUT Rotation);

		FVector LineTraceEnd = Location + Reach * Rotation.Vector();
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

