// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseRequest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetDoorAngle(int32 Angle);
	
	UPROPERTY(BlueprintAssignable)
	FOnOpenRequest OnOpenRequest;
	
	UPROPERTY(BlueprintAssignable)
	FOnCloseRequest OnCloseRequest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OpenAngle;
private:

	UPROPERTY(EditAnywhere)
	class ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1;

	UPROPERTY(EditAnywhere)
	float TriggerMass;

	float LastDoorOpenTime;

	class AActor* Owner;

	void OpenDoor();
	void CloseDoor();

	// Returns total mass in kg
	float GetTotalMassOfActorsOnPlate();
};
