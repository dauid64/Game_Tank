// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank() {
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));//adiciona o distanciamento da camera
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));//adiciona a camera com o distanciamento
    Camera->SetupAttachment(SpringArm);
}
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {

    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);//passa o valor 1 para frente e -1 para tras para a funcao move
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);//passa o valor 1 e -1 para virar

    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire); //Toda vez que clicar ira atirar
}

void ATank::Tick(float DeltaTime) {

    Super::Tick(DeltaTime);
     
    if (PlayerControllerRef) {

        FHitResult HitResult;
        PlayerControllerRef->GetHitResultUnderCursor(
            ECollisionChannel::ECC_Visibility,
            false, 
            HitResult
            );

        RotateTurret(HitResult.ImpactPoint);//gira a torre ministrada pelo mouse, indo ate a BasePawn aonde esta essa função

    }
}

void ATank::HandleDestruction() {
    
    Super::HandleDestruction();

    SetActorHiddenInGame(true);//tank fica invisivel
    SetActorTickEnabled(false);
    bAlive = false;
}

// Called when the game starts or when spawned
void ATank::BeginPlay() {
    Super::BeginPlay();

    PlayerControllerRef = Cast<APlayerController>(GetController());//conversão para não armazenar um componentente filho em um pai
    
}
void ATank::Move(float Value) {//vincula com o mapeamento de eixos para se movimentar

    FVector DeltaLocation(0.f);
    DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);//Está multiplicando o valor de 1 para ficar mais rapido e multiplicando com o delta time para não mudar a velocidade para cada taxa de quadros
    AddActorLocalOffset(DeltaLocation, true);// true habilitamos a varredura e não vamos mais ultrapassar paredes

}

void ATank::Turn(float Value) {//Funçao para virar o tank

    FRotator DeltaRotation(0.f);
    DeltaRotation.Yaw = Value * TurnRate* UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(DeltaRotation, true);
}

