// Fill out your copyright notice in the Description page of Project Settings.

#include "RTRStop.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ARTRStop::ARTRStop()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;

	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Stop Component"));
	FVector boxEx;
	boxEx.X = 100;
	boxEx.Y = 100;
	boxEx.Z = 100;
	collisionBox->InitBoxExtent(boxEx);
	collisionBox->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = collisionBox;

	collisionBox->OnComponentBeginOverlap.AddDynamic(this, &ARTRStop::OnOverlapBegin);
}

// Called when the game starts or when spawned.
void ARTRStop::BeginPlay()
{
	Super::BeginPlay();

}

void ARTRStop::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if overlap is with the player.
	if (OtherActor == UGameplayStatics::GetPlayerPawn(this, 0)) {
		// Stop the riser
		if (URTRBPLibrary::playing == true) 
		{
			GLog->Log("overlap stop");
			URTRBPLibrary::playing = false;
			URTRBPLibrary::stopAudio();
		}
	}
}