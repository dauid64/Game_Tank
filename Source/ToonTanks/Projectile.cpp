// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));//declarando o projetil e adicionando o component movement
	ProjectileMovementComponent->MaxSpeed = 1300.f;//inicial e maxima velocidade
	ProjectileMovementComponent->InitialSpeed = 1300.f;

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));//creando a particulo q segue a bala
	TrailParticles->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);//se atingir ativa a função OnHit

	if (LaunchSound) {
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {

	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr) {
		Destroy();
		return;
	}
	 AController* MyOwnerInstigator = MyOwner-> GetInstigatorController();//conseguindo o controlador/instigador do projetil
	 UClass* DamageTypeClass = UDamageType::StaticClass();//tipo do dano

	 if (OtherActor && OtherActor != this && OtherActor != MyOwner) {//primeira verificação para nao darmos danos a nos mesmo
		 UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);//chamamos o dano
		 if (HitParticles) {
			 UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
		 }
		 if (HitSound) {
			 UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		 }
		 if (HitCameraShakeClass) {
			 GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(HitCameraShakeClass);
		 }
	 }
	 Destroy();//destroir o projetil
}

