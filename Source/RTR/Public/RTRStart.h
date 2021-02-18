// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RTRBPLibrary.h"
#include "RTRStart.generated.h"

UCLASS()
class RTR_API ARTRStart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARTRStart();
	//virtual void BeginDestroy();
	~ARTRStart();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(visibleAnywhere)
		class UBoxComponent* collisionBox;

	UPROPERTY(visibleAnywhere)
		class UStaticMeshComponent* mesh;

	float boxHeight;
	float boxWidth;

	FTimerHandle timerHandle;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void update();
};
