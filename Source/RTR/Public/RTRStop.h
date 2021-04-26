// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RTRBPLibrary.h"
#include "RTRStop.generated.h"

UCLASS()
class RTR_API ARTRStop : public AActor
{
	GENERATED_BODY()
	
public:	
	ARTRStop();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(visibleAnywhere)
		class UBoxComponent* collisionBox;

	UPROPERTY(visibleAnywhere)
		class UStaticMeshComponent* mesh;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	float boxHeight;
	float boxWidth;
};
