// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorEvent);

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
	FOnDoorEvent OnOpenRequest;
	
	UPROPERTY(BlueprintAssignable)
	FOnDoorEvent OnCloseRequest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OpenAngle;
private:

	UPROPERTY(EditAnywhere)
	class ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	float TriggerMass;

	class AActor* Owner;

	// Returns total mass in kg
	float GetTotalMassOfActorsOnPlate();
};
