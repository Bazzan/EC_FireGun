#include "AbilitySystem/Projectile/ECProjectile.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EC_GameplayTags.h"
#include "Engine/World.h"
#include "TimerManager.h"

AECProjectile::AECProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	CollisionComponent->SetSphereRadius(CollisionRadius);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	CollisionComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void AECProjectile::InitDamage(
	TSubclassOf<UGameplayEffect> InDamageEffect,
	float InDamage,
	UAbilitySystemComponent* InSourceASC)
{
	DamageEffectClass = InDamageEffect;
	DamageValue = InDamage;
	SourceASC = InSourceASC;
}

void AECProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->SetSphereRadius(CollisionRadius);
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;

	if (GetInstigator())
	{
		CollisionComponent->IgnoreActorWhenMoving(GetInstigator(), !bCanDamageInstigator);
	}

	CollisionComponent->OnComponentHit.AddDynamic(this, &AECProjectile::OnHit);
}

void AECProjectile::OnHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (bHasHit)
	{
		return;
	}
	bHasHit = true;

	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovement->StopMovementImmediately();

	if (HasAuthority() && OtherActor)
	{
		ApplyDamageToActor(OtherActor);
	}

	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		const FVector Impulse = GetVelocity().GetSafeNormal() * PhysicsForce;
		OtherComp->AddImpulseAtLocation(Impulse, Hit.ImpactPoint);
	}

	BP_OnImpact(Hit);

	if (DestroyDelay > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			DestroyTimerHandle, this, &AECProjectile::OnDestroyTimer, DestroyDelay, false);
	}
	else
	{
		Destroy();
	}
}

void AECProjectile::ApplyDamageToActor(AActor* Target)
{
	if (!Target || !DamageEffectClass || !SourceASC.IsValid())
	{
		return;
	}

	if (!bCanDamageInstigator && Target == GetInstigator())
	{
		return;
	}

	const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Target);
	if (!ASI)
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = ASI->GetAbilitySystemComponent();
	if (!TargetASC)
	{
		return;
	}

	FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
	Context.AddSourceObject(this);

	FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1, Context);
	if (Spec.IsValid())
	{
		Spec.Data->SetSetByCallerMagnitude(EC_GameplayTags::Data_Damage, -DamageValue);
		SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data, TargetASC);
	}
}

void AECProjectile::OnDestroyTimer()
{
	Destroy();
}
