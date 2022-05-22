// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	if (InFireRange()) {
		RotateTurret(Tank->GetActorLocation());//gira a torre
	}
}

void ATower::HandleDestruction() {

	Super::HandleDestruction();
	Destroy();
}

void ATower::BeginPlay() {

	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);//disparos a cada 2seg
}

void ATower::CheckFireCondition() {
	if (Tank==nullptr) {
		return;
		}
	if (InFireRange() && Tank->bAlive) {
		Fire();
	}
}

bool ATower::InFireRange() {//checa se esta existe o tank e se esta na distancia de fogo
	if (Tank) {
		float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());//loc da torre e do tank
		if (Distance <= FireRange) {
			return true;
		}
	}

	return false;
}