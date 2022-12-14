// Fill out your copyright notice in the Description page of Project Settings.


#include "RepulsiveFireBall.h"

#include <Utility/Utility.h>


#include "MonoHitBehaviours.h"
#include "BaseCharacter.h"
#include "Players/BasePlayer.h"

#define COLLISION_CHANNEL_FIREBALL ECC_GameTraceChannel4

void ARepulsiveFireBall::OnFireBallBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!DestroyOnlyIfGroundTagFound || OtherComp->ComponentHasTag(TEXT("FireBallDestroyable")))
    {
        Destroy();
    }
}

void ARepulsiveFireBall::OnChocWaveZoneBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{ 
    if (UNLIKELY(!OtherComp->ComponentHasTag(TEXT("Body")) && !OtherComp->ComponentHasTag(TEXT("PlayerBody"))))
        return;

    /*Add the actor on if is has not already hit by the fire ball*/
    if (UNLIKELY(MonoHitBehavioursComponent->CheckIfAlreadyExistAndAdd(OtherActor)))
       return;

    OnFireBallHitActor.Broadcast(OtherActor);
    
    ABaseCharacter* pCharacter = Cast<ABaseCharacter>(OtherActor);

    FVector LaunchForce = OtherActor->GetActorLocation() - GetActorLocation();
    const float ChocForceWithDistance = ChocForce * (ChocForceDependingOfDistance ? 1.f : 1.f - (LaunchForce
    .SizeSquared() / (ChocWaveZone->GetScaledSphereRadius() * ChocWaveZone->GetScaledSphereRadius())));
    LaunchForce.Normalize();
    LaunchForce *= ChocForceWithDistance;
    LaunchForce.Z = ChocForceHeightRatio * ChocForceWithDistance;

    ABasePlayer* pPlayer = Cast<ABasePlayer>(OtherActor);

    if (UNLIKELY(pPlayer)) //More chance to hit trash than the player
    {
        OnFireBallHitPlayer.Broadcast(pPlayer);
        pPlayer->TakeDamageCharacter(Damage);
    }

    pCharacter->LaunchAndStun(LaunchForce, true, true);
    
    OnChocWave.Broadcast(); 
}

ARepulsiveFireBall::ARepulsiveFireBall()
{
    ChocWaveZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ChocWaveZone->OnComponentBeginOverlap.AddDynamic(this, &ARepulsiveFireBall::OnChocWaveZoneBeginOverlap);

    MonoHitBehavioursComponent = CreateDefaultSubobject<UMonoHitBehaviours>(TEXT("MonoHitBehavioursComponent"));
}
