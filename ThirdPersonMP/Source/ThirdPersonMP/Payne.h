// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Payne.generated.h"

UCLASS(config=Game)
class APayne : public ACharacter
{
	GENERATED_BODY()

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pickup, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollectionSphere;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite,Category = Pickup, meta = (AllowPrivateAccess = "true"))
	float CollectionSphereRadius;

	

protected:
	UPROPERTY(EditAnywhere, Category="Health")
	float MaxHealth;
	
	UPROPERTY(ReplicatedUsing=OnRep_CurrentHealth)
	float CurrentHealth;

	/* projectile */
	UPROPERTY(EditDefaultsOnly, Category="Gameplay|Projectile")
	TSubclassOf<class AThirdPersonMPProjectile> ProjectileClass;

	/** 射击之间的延迟，单位为秒。用于控制测试发射物的射击速度，还可防止服务器函数的溢出导致将SpawnProjectile直接绑定至输入。*/
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	float FireRate;

	/** 若为true，则正在发射投射物。*/
	bool bIsFiringWeapon;

	/** 定时器句柄，用于提供生成间隔时间内的射速延迟。*/
	FTimerHandle FiringTimer;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite,Category = Pickup, meta = (AllowPrivateAccess = "true"))
	float InitialPower;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentPower, EditAnywhere, BlueprintReadWrite,Category = Pickup, meta = (AllowPrivateAccess = "true"))
	float CurrentPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Power", meta = (BlueprintProtected = "true"))
	float BaseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Power", meta = (BlueprintProtected = "true"))
	float SpeedFactor;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;


private:
	UFUNCTION()
	void OnRep_CurrentPower();


protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	
	UFUNCTION()
	void OnRep_CurrentHealth();

	/** 响应要更新的生命值。修改后，立即在服务器上调用，并在客户端上调用以响应RepNotify*/
	void OnHealthUpdate();
	

	/** 用于启动武器射击的函数。*/
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void StartFire();

	/** 用于结束武器射击的函数。一旦调用这段代码，玩家可再次使用StartFire。*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();  

	/** 用于生成投射物的服务器函数。*/
	UFUNCTION(Server, Reliable)
	void HandleFire();

	UFUNCTION(BlueprintCallable, Category="Pickup")
	void CollectPickups();

	UFUNCTION(Reliable,Server,WithValidation)
	void ServerCollecPickups();

	UFUNCTION(BlueprintImplementableEvent, Category="Power")
	void PowerChangeEffect();

public:
	APayne();
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class USphereComponent* GetCollectionSphere() const {return CollectionSphere;}

	/* nova */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure, Category="Health")
	FORCEINLINE float GetMaxHealth() const {return MaxHealth;}

	UFUNCTION(BlueprintPure, Category="Health")
	FORCEINLINE float GetCurrentHealth() const {return CurrentHealth;}

	UFUNCTION(BlueprintCallable, Category="Health")
	void SetCurrentHealth(float healthValue);

	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage( float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser ) override;

	UFUNCTION(BlueprintPure, Category="Power")
	float GetInitialPower();

	UFUNCTION(BlueprintPure, Category="Power")
	float GetCurrentPower();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Power")
	void UpdatePower(float DeltaPower);
	
	UFUNCTION(NetMulticast, Reliable)
	void OnPlayerDeath();

	UFUNCTION(BlueprintCallable, Category="SeamlessTravel")
	void SeamlessTravelTo(FString URL);

	
	UFUNCTION(Reliable,Server,WithValidation)
	void ServerSeamlessTravelTo(const FString &URL);
};

