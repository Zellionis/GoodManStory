// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WaveZone.h"
#include "Engine/World.h"
#include "GoodManStory/Wave/EnemyState.h"

#include "EnemiesManager.generated.h"

class UDataTable;

UCLASS()
class GOODMANSTORY_API AEnemiesManager : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AEnemiesManager();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    class UDataTable* WaveDataTable = nullptr;

    UPROPERTY(VisibleAnywhere)
    TArray<class ABaseEnemy*> DeathEnemyContainer;

    UPROPERTY(VisibleAnywhere)
    TArray<class AActor*> DeathWeaponContainer;

    UPROPERTY(EditAnywhere)
    uint16 MaxDeathEnemies = 100;

    UPROPERTY(EditAnywhere)
    uint16 MaxDeathWeapon = 100;
    
    UPROPERTY(EditAnywhere)
    TArray<FEnemyState> EnemiesStatsContainer;

    UPROPERTY(EditAnywhere)
    TArray<AActor*> SpawnersContainer;

    UPROPERTY(EditAnywhere)
    TArray<AWaveZone*> ZonesContainer;

    FTimerHandle TimerActuMinionSpawn;

    bool bWaveSpawnerIsRunning  = false;
    bool bPlayerCanStartTheWave = false;
    bool bCurrentWaveIsDone = true; //True because the first wave dosn't start

    UPROPERTY(VisibleAnywhere)
    uint16 WaveIndex = 0;

    /**
     * @brief Pointor to the struct of the current wave. If nullptr, there is not wave
     */
    struct FWaveInfo* pCurrentWave = nullptr;

    UFUNCTION(BlueprintCallable)
    void CheckIfCurrentWaveSpawnerIsEmpty();

    UFUNCTION(BlueprintCallable)
    bool IsAllEnemiesDied();

    UFUNCTION(BlueprintCallable)
    void CheckIfPlayerCanStartTheNextWave();

    UFUNCTION(BlueprintCallable)
    void Spawn(float DeltaTime);

    UFUNCTION(BlueprintCallable)
    void NextWave();

    UFUNCTION(BlueprintCallable)
    bool CanSpawnTheEntityWithoutExcesMaxNumber(const TSubclassOf<class ABaseEnemy>& Type) const;

    UFUNCTION(BlueprintCallable)
    void MoveLivingEnemyOnDeathContainer(class ABaseCharacter* pCharacter);

    UFUNCTION(BlueprintCallable)
    void ResetCurrentWave();
    

    FActorSpawnParameters SpawnParams;
    bool                  Spawning = true;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
