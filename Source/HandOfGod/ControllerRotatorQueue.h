// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Queue.h"
#include "ControllerRotatorQueue.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HANDOFGOD_API UControllerRotatorQueue : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UControllerRotatorQueue();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MaxFrameCount;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TQueue<FQuat> queue;
	int count;
	FQuat average;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Enqueue(FRotator rotation);

	UFUNCTION(BlueprintCallable)
	FRotator GetAverageRotator();
	
};
