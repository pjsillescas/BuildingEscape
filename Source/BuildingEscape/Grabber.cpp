// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"

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

	UE_LOG(LogTemp,Warning,TEXT("Grabber reporting for duty"));

	// Look for attached physics handle
	PhysicsHandle = Cast<UPhysicsHandleComponent>(GetOwner()->GetComponentByClass(UPhysicsHandleComponent::StaticClass()));
	if (PhysicsHandle != nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("Physics handle found!!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber: No physics handle found in actor '%s'"),*(GetOwner()->GetName()));
	}
	
	InputComponent = Cast<UInputComponent>(GetOwner()->GetComponentByClass(UInputComponent::StaticClass()));
	if (InputComponent != nullptr)
	{
		// Bind inputs
		InputComponent->BindAction("Grab", IE_Pressed,this,&UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber: No input component found in actor '%s'"), *(GetOwner()->GetName()));
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Releasing grabbed object"));
}

void UGrabber::Grab()
{
	/*
	FVector Location;
	FRotator Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Location, OUT Rotation);
	
	FVector LineTraceEnd = Location + Reach * Rotation.Vector();
	FCollisionQueryParams QueryParams(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, Location, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), QueryParams);

	AActor* ActorHit = Hit.GetActor();
	// See what we hit
	if (ActorHit != nullptr)
	{
		FString Name = ActorHit->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Hitting actor %s"), *Name);
	}
	*/
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	// Get player view point
	FVector Location;
	FRotator Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT Location,OUT Rotation);
	
	/// Draw a red trace in the world to visual
	FVector LineTraceEnd = Location + Reach * Rotation.Vector();
	DrawDebugLine(GetWorld(),Location,LineTraceEnd,FColor::Red,false,0,0,10);

	// Setup query parameters
	FCollisionQueryParams QueryParams(FName(TEXT("")),false,GetOwner());

	// Ray cast out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(OUT Hit,Location,LineTraceEnd,FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),QueryParams);

	AActor* ActorHit = Hit.GetActor();
	// See what we hit
	if (ActorHit != nullptr)
	{
		FString Name = ActorHit->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Hitting actor %s"), *Name);
	}
}

