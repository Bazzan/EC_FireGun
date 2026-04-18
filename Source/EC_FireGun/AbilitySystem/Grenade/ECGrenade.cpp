#include "AbilitySystem/Grenade/ECGrenade.h"
#include "AbilitySystem/Abilities/Ultimate/UltimateAbilityBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EC_GameplayTags.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"
#include "TimerManager.h"

AECGrenade::AECGrenade()
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
	ProjectileMovement->InitialSpeed = InitialSpeed;
	ProjectileMovement->MaxSpeed = 0.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 1.0f;
}

void AECGrenade::InitDamage(
	TSubclassOf<UGameplayEffect> InDamageEffect,
	float InDamage,
	UAbilitySystemComponent* InSourceASC)
{
	DamageEffectClass = InDamageEffect;
	DamageValue = InDamage;
	SourceASC = InSourceASC;
}

void AECGrenade::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->SetSphereRadius(CollisionRadius);
	ProjectileMovement->InitialSpeed = InitialSpeed;

	if (GetInstigator())
	{
		CollisionComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	}

	CollisionComponent->OnComponentHit.AddDynamic(this, &AECGrenade::OnHit);

	if (HasAuthority() && MaxLifetime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			LifetimeTimerHandle, this, &AECGrenade::OnLifetimeExpired, MaxLifetime, false);
	}
}

void AECGrenade::OnHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (bHasExploded || !HasAuthority())
	{
		return;
	}

	Explode();
}

void AECGrenade::OnLifetimeExpired()
{
	if (!bHasExploded && HasAuthority())
	{
		Explode();
	}
}

void AECGrenade::Explode()
{
	if (bHasExploded)
	{
		return;
	}
	bHasExploded = true;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(LifetimeTimerHandle);
	}

	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovement->StopMovementImmediately();

	const FVector ExplosionLocation = GetActorLocation();

	UWorld* World = GetWorld();
	if (World)
	{
		TArray<FOverlapResult> Overlaps;
		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(GrenadeExplosion), false, this);

		const bool bAnyOverlap = World->OverlapMultiByObjectType(
			Overlaps,
			ExplosionLocation,
			FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::AllDynamicObjects),
			FCollisionShape::MakeSphere(ExplosionRadius),
			QueryParams);

		if (bAnyOverlap)
		{
			TSet<AActor*> ProcessedActors;
			for (const FOverlapResult& Overlap : Overlaps)
			{
				AActor* HitActor = Overlap.GetActor();
				if (!HitActor || ProcessedActors.Contains(HitActor))
				{
					continue;
				}
				ProcessedActors.Add(HitActor);

				ApplyExplosionDamageToActor(HitActor, ExplosionLocation);

				if (UPrimitiveComponent* HitPrim = Overlap.GetComponent();
					HitPrim && HitPrim->IsSimulatingPhysics() && PhysicsForce > 0.0f)
				{
					const FVector Dir = (HitActor->GetActorLocation() - ExplosionLocation).GetSafeNormal();
					HitPrim->AddImpulse(Dir * PhysicsForce, NAME_None, true);
				}
			}
		}
	}

	BP_OnExplode(ExplosionLocation);

	if (DestroyDelay > 0.0f && World)
	{
		World->GetTimerManager().SetTimer(
			DestroyTimerHandle, this, &AECGrenade::OnDestroyTimer, DestroyDelay, false);
	}
	else
	{
		Destroy();
	}
}

void AECGrenade::ApplyExplosionDamageToActor(AActor* Target, const FVector& ExplosionLocation)
{
	if (!IsValid(Target) || !DamageEffectClass || !SourceASC.IsValid())
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
		// Per project rule: do not fall back to UGameplayStatics::ApplyDamage.
		UE_LOG(LogClassAbility, Verbose,
			TEXT("AECGrenade: target %s has no ASC, skipping damage."), *Target->GetName());
		return;
	}

	UAbilitySystemComponent* TargetASC = ASI->GetAbilitySystemComponent();
	if (!TargetASC)
	{
		return;
	}

	float Damage = DamageValue;
	if (bUseDamageFalloff && ExplosionRadius > 0.0f)
	{
		const float Distance = FVector::Dist(Target->GetActorLocation(), ExplosionLocation);
		const float Alpha = FMath::Clamp(Distance / ExplosionRadius, 0.0f, 1.0f);
		const float Scale = FMath::Lerp(1.0f, MinDamageScale, Alpha);
		Damage *= Scale;
	}

	FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
	Context.AddSourceObject(this);

	const FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1, Context);
	if (Spec.IsValid())
	{
		Spec.Data->SetSetByCallerMagnitude(EC_GameplayTags::Data_Damage, -Damage);
		SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data, TargetASC);
	}
}

void AECGrenade::OnDestroyTimer()
{
	Destroy();
}
