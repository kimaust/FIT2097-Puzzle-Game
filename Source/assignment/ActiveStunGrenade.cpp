// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveStunGrenade.h"

#include "AIEnemyCharacter.h"
#include "AIEnemyController.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AActiveStunGrenade::AActiveStunGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;

	// Enable physics and dampings to simulate real grenade projectile motions. 
	BaseMesh->SetSimulatePhysics(true);
	BaseMesh->SetLinearDamping(0.7f);
	BaseMesh->SetAngularDamping(0.7f);

	// Use sphere object for the mesh for now (a grenade mesh can be placed if needed).
	ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshObject(
		TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraObject(TEXT(
		"/Game/FX/ExplosionFX.ExplosionFX"));

	if (SphereMeshObject.Succeeded()) {
		BaseMesh->SetStaticMesh(SphereMeshObject.Object);
	}
	if (NiagaraObject.Succeeded()) {
		ExplosionFX = NiagaraObject.Object;
	}
	Duration = 3;
	StunDuration = 7;
	ExplosionRadius = 1000;
}

// Called when the game starts or when spawned
void AActiveStunGrenade::BeginPlay()
{
	Super::BeginPlay();

	// Set up the grenade explosion timer to explode after the specified duration.
	GetWorldTimerManager().SetTimer(GrenadeTimer, this, &AActiveStunGrenade::Explode, Duration);
}

// Called every frame
void AActiveStunGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// NOTE: Can update material to pulse periodically if needed.
}

void AActiveStunGrenade::Explode()
{
	GetWorldTimerManager().ClearTimer(GrenadeTimer);

	// Generate a spherical overlap around the explosion point to get list of enemies that has been hit.
	TArray<AActor*> OverlappedActors;
	TArray<AActor*> IgnoredActors = TArray<AActor*>{this};
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		ExplosionRadius, ObjectTypes,
		AAIEnemyCharacter::StaticClass(),
		IgnoredActors,
		OverlappedActors);

	// Generate explosion effect.
	UNiagaraComponent* Effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		ExplosionFX,
		GetActorLocation(),
		FRotator::ZeroRotator,
		FVector(1),
		true,
		true,
		ENCPoolMethod::AutoRelease,
		true);
	
	// Stun all enemies that has been hit.
	for (auto Actor : OverlappedActors) {
		auto Enemy = Cast<AAIEnemyCharacter>(Actor);
		if (Enemy) {
			auto AIController = Cast<AAIEnemyController>(Enemy->GetController());
			AIController->Stun(StunDuration);
		}
	}
	Destroy();
}