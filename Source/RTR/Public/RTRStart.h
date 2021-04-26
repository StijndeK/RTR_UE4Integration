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
	ARTRStart();
	~ARTRStart();

	float CalculateDistance(FVector* location1, FVector* location2);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(visibleAnywhere)
		class UBoxComponent* collisionBox;

	UPROPERTY(visibleAnywhere)
		class UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere)
		AActor* Destination;

	UPROPERTY(EditAnywhere)
		APawn* Player;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void update();

	float boxHeight;
	float boxWidth;

	FTimerHandle timerHandle;
};
