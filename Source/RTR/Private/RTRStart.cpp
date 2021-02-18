// Fill out your copyright notice in the Description page of Project Settings.

#include "RTRStart.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARTRStart::ARTRStart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Start Component"));
	FVector boxEx;
	boxEx.X = 100;
	boxEx.Y = 100;
	boxEx.Z = 100;
	collisionBox->InitBoxExtent(boxEx);
	collisionBox->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = collisionBox;

	collisionBox->OnComponentBeginOverlap.AddDynamic(this, &ARTRStart::OnOverlapBegin);
}

ARTRStart::~ARTRStart()
{
	if (URTRBPLibrary::playing == true) {
		GLog->Log("overlap stop");
		URTRBPLibrary::playing = false;
		URTRBPLibrary::audioSystem.stopRiser();
	}
}
// Called when the game starts or when spawned
void ARTRStart::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARTRStart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARTRStart::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// check if overlap is with the player
	if (OtherActor == UGameplayStatics::GetPlayerPawn(this, 0)) {
		if (URTRBPLibrary::playing != true) {
			GLog->Log("overlap start");
			URTRBPLibrary::playing = true;
			URTRBPLibrary::playAudio();

			// start update timer
			GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ARTRStart::update, 1.0f / 60.0f, true);
		}
	}
}

void ARTRStart::update()
{
	URTRBPLibrary::update();
}