// Copyright Epic Games, Inc. All Rights Reserved.

#include "Payne.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
/* nova */
//	network
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
//	projectile
#include "BatteryPickup.h"
#include "Pickup.h"
#include "ThirdPersonMPProjectile.h"
#include "Components/SphereComponent.h"


//////////////////////////////////////////////////////////////////////////
// APayne

APayne::APayne()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// collsctionsphere
	CollectionSphereRadius = 200.f;

	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(CollectionSphereRadius);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	/* nova */
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	//初始化投射物类
	ProjectileClass = AThirdPersonMPProjectile::StaticClass();
	//初始化射速
	FireRate = 0.25f;
	bIsFiringWeapon = false;

	InitialPower = 2000.f;
	CurrentPower = InitialPower;
}

//////////////////////////////////////////////////////////////////////////
// Input

void APayne::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APayne::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APayne::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APayne::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APayne::LookUpAtRate);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APayne::StartFire);

	PlayerInputComponent->BindAction("CollectPickups", IE_Pressed, this, &APayne::CollectPickups);

	
}




void APayne::OnResetVR()
{
	// If ThirdPersonMP is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in ThirdPersonMP.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void APayne::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void APayne::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void APayne::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APayne::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APayne::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APayne::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

/* nova */
void APayne::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APayne, CurrentHealth);
	DOREPLIFETIME(APayne, CollectionSphereRadius);
	DOREPLIFETIME(APayne, CurrentPower);
	DOREPLIFETIME(APayne, InitialPower);
}

void APayne::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}


void APayne::OnHealthUpdate()
{
	// only client 
	if (IsLocallyControlled())
	{
		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."),CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,healthMessage);

		if (CurrentHealth <= 0)
		{
			FString deathMessage = FString::Printf(TEXT("You die"));
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Blue,deathMessage);
		}
	}
	//only server
	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,healthMessage);
	}

	//在所有机器上都执行的函数。 
	/*  
		因任何因伤害或死亡而产生的特殊功能都应放在这里。 
	*/
}

void APayne::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		OnHealthUpdate();
	}
}

float APayne::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageApplied = CurrentHealth - DamageTaken;
	SetCurrentHealth(damageApplied);
	return damageApplied;
}

float APayne::GetInitialPower()
{
	return InitialPower;
}

float APayne::GetCurrentPower()
{
	return CurrentPower;
}

void APayne::UpdatePower(float DeltaPower)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentPower += DeltaPower;
	}
}

void APayne::StartFire()
{
	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FiringTimer,this,&APayne::StopFire,FireRate,false);
		HandleFire();
	}
}

void APayne::StopFire()
{
	bIsFiringWeapon = false;
}

void APayne::CollectPickups()
{
	//FString healthMessage = FString::Printf(TEXT("Pressc"));         GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	ServerCollecPickups();
}

void APayne::ServerCollecPickups_Implementation()
{
	
	if (GetLocalRole() == ROLE_Authority)
	{
		TArray<AActor*> CollectedActorsArray;
		CollectionSphere->GetOverlappingActors(CollectedActorsArray);
		for (int i = 0; i < CollectedActorsArray.Num(); ++i)
		{
			APickup* const tmp = Cast<APickup>(CollectedActorsArray[i]);
			if (tmp != NULL && !tmp->IsPendingKill() && tmp->IsActive())
			{
				if (ABatteryPickup* tmpB = Cast<ABatteryPickup>(tmp))
				{
					UpdatePower(tmpB->GetPower());
				}
				tmp->PickUpBy(this);
				tmp->SetActive(false);
				// tmp->WasCollected();
				
			}
		}
	}
}

bool APayne::ServerCollecPickups_Validate()
{
	return true;
}

void APayne::HandleFire_Implementation()
{
	FVector spawnLocation = GetActorLocation() + ( GetControlRotation().Vector()  * 100.0f ) + (GetActorUpVector() * 50.0f);
	FRotator spawnRotation = GetControlRotation();

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	AThirdPersonMPProjectile* spawnedProjectile = GetWorld()->SpawnActor<AThirdPersonMPProjectile>(spawnLocation, spawnRotation, spawnParameters);
}


