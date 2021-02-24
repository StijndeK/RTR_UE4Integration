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

	// check if stop reference is set
	if (Destination == nullptr) {
		GLog->Log("RTR ERROR: please provide a reference to the RTRStop actor to the RTRStart actor");
	}
	else {
		// calculate distance
		FVector loc1 = GetActorLocation();
		FVector loc2 = Destination->GetActorLocation();
		float startStopDistance = CalculateDistance(&loc1, &loc2);
		URTRBPLibrary::setupRTR(400, startStopDistance);
		UE_LOG(LogTemp, Warning, TEXT("startStopDistance, %f"), startStopDistance);
	}
}

// calculate distance
float ARTRStart::CalculateDistance(FVector* location1, FVector* location2)
{
	// TODO: acount for offset

	float distancex = (location2[0].X - location1[0].X) * (location2[0].X - location1[0].X);
	float distancey = (location2[0].Y - location1[0].Y) * (location2[0].Y - location1[0].Y);
	float distance = sqrt(distancex + distancey);

	return distance;
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
	// update player position
	FVector loc1 = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation();
	FVector loc2 = Destination->GetActorLocation();
	float playerDistance = CalculateDistance(&loc1, &loc2);
	UE_LOG(LogTemp, Warning, TEXT("player position: , %f"), playerDistance);
	URTRBPLibrary::setPlayerPosition(playerDistance);

	// update audio
	URTRBPLibrary::update();
}