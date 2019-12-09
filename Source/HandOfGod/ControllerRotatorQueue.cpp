// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerRotatorQueue.h"

// Sets default values for this component's properties
UControllerRotatorQueue::UControllerRotatorQueue()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UControllerRotatorQueue::BeginPlay()
{
	Super::BeginPlay();

	count = 0;
}


// Called every frame
void UControllerRotatorQueue::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!queue.IsEmpty())
	{
		FQuat rot;
		queue.Dequeue(rot);
		average *= rot.Inverse();
		count--;
	}
}

void UControllerRotatorQueue::Enqueue(FRotator rotation)
{
	if (count == MaxFrameCount)
	{
		FQuat rot;
		queue.Dequeue(rot);
		average *= rot.Inverse();
		count--;
	}
	FQuat rot_new = FQuat::Slerp(FQuat::Identity, rotation.Quaternion, 1.0 / MaxFrameCount);
	queue.Enqueue(rot_new);
	average = rot_new * average;
	count++;
}

FRotator UControllerRotatorQueue::GetAverageRotator()
{
	return average.Rotator();
}


