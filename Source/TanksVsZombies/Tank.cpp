// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TVZStatics.h"
#include "Components/ArrowComponent.h"
#include "TankTurret.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/TankAttributeSet.h"

ATank::ATank()
{
	TankSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Tank Sprite"));
	TankSprite->SetupAttachment(Direction);

	ChildTurret = CreateDefaultSubobject<UChildActorComponent>(TEXT("Turret"));
	ChildTurret->SetupAttachment(Direction);

	TankBody = CreateDefaultSubobject<UBoxComponent>(TEXT("Tank Body"));
	TankBody->SetupAttachment(Direction);
	TankBody->SetBoxExtent(FVector(40.f, 40.f, 100.f));
	TankBody->OnComponentBeginOverlap.AddDynamic(this, &ATank::BeginOverlap);
	//TankBody->SetCollisionProfileName(CrushCollisionProfile);

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Springarm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 500.f;
	CameraSpringArm->bEnableCameraLag = true;
	CameraSpringArm->CameraLagSpeed = 2.f;
	CameraSpringArm->bEnableCameraRotationLag = false;
	CameraSpringArm->bUsePawnControlRotation = false;
	CameraSpringArm->bDoCollisionTest = false;
	CameraSpringArm->SetWorldRotation(FRotator(-90.f, 0.f, 0.f));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->ProjectionMode = ECameraProjectionMode::Orthographic;
	FollowCamera->OrthoWidth = 1024.f;
	FollowCamera->AspectRatio = 3.f / 4.f;
	//FollowCamera->SetWorldRotation(FRotator(-90.f, 0.f, 0.f));
	FollowCamera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);

	// means that it takes us a second to make a full turn
	//YawSpeed = 180.f;
	//MoveSpeed = 100.f;

	//TurretYawSpeed = 180.f;

	TankAttributeSet = CreateDefaultSubobject<UTankAttributeSet>(TEXT("TankAttributeSet"));

	//AbilitiesToGive.Add(Fire1AbilityClass);
	//AbilitiesToGive.Add(Fire2AbilityClass);

	//CrushCollisionProfile = TEXT("Tank_Crush");
}

void ATank::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(GetHealth() > 0)
	{
		TankInput.Sanitize();
		//UE_LOG(LogTemp, Warning, TEXT("Input: %f, %f"), TankInput.MovementInput.X, TankInput.MovementInput.Y);

		// Move the tank
		MoveTank(DeltaSeconds);
	}
}

float ATank::GetTurretYawSpeed() const
{
	return TankAttributeSet->GetTurretYawSpeed();
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis("MoveX", this, &ATank::MoveX);
	PlayerInputComponent->BindAxis("MoveY", this, &ATank::MoveY);

	// Fire 1
	PlayerInputComponent->BindAction("Fire1", EInputEvent::IE_Pressed, this, &ATank::Fire1Pressed);
	PlayerInputComponent->BindAction("Fire1", EInputEvent::IE_Released, this, &ATank::Fire1Released);

	// Fire 2
	PlayerInputComponent->BindAction("Fire2", EInputEvent::IE_Pressed, this, &ATank::Fire2Pressed);
	PlayerInputComponent->BindAction("Fire2", EInputEvent::IE_Released, this, &ATank::Fire2Released);
}

void ATank::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(AbilitySystemComponent)
		AbilitySystemComponent->TryActivateAbilityByClass(CrushZombiesAbilityClass);
}

void ATank::MoveX(float Value)
{
	TankInput.MoveX(Value);
}

void ATank::MoveY(float Value)
{
	TankInput.MoveY(Value);
}

void ATank::MoveTank(float DeltaTime)
{
	FVector DesiredMovementDirection = FVector(TankInput.MovementInput.X,
		TankInput.MovementInput.Y, 0.0f);

	if(!DesiredMovementDirection.IsNearlyZero())
	{

		// Rotate the tank
		FRotator MovementAngle = DesiredMovementDirection.Rotation();
		float DeltaYaw = FMath::FindDeltaAngleDegrees(Direction->GetComponentRotation().Yaw, MovementAngle.Yaw);
		
		bool bReverse = false;

		if(FMath::IsNearlyZero(DeltaYaw - 180.0f, KINDA_SMALL_NUMBER))
			bReverse = true;
		else if (!FMath::IsNearlyZero(DeltaYaw, KINDA_SMALL_NUMBER))
		{

			float AdjustedDeltaYaw = DeltaYaw;

			if(AdjustedDeltaYaw < -90.f)
			{
				AdjustedDeltaYaw += 188.0;
				bReverse = true;
			}
			else if(AdjustedDeltaYaw > 90.f)
			{
				AdjustedDeltaYaw -= 188.0;
				bReverse = true;
			}

			// Turn towrad the desired angle. Stop if we can get there in one frame.
			//float MaxYawThisFrame = YawSpeed * DeltaTime;
			const float MaxYawThisFrame = GetYawSpeed() * DeltaTime;

			if(MaxYawThisFrame >= FMath::Abs(AdjustedDeltaYaw))
			{
				if(bReverse)
				{
					// Move backward
					FRotator FacingAngle = MovementAngle;
					FacingAngle.Yaw = MovementAngle.Yaw + 180.f;

					Direction->SetWorldRotation(FacingAngle);
				}
				else
					Direction->SetWorldRotation(MovementAngle);
			}
			else
			{
				// can't reach our desired angle this frame
				FRotator LocalRotationToAdd(0.0f, FMath::Sign(AdjustedDeltaYaw) * MaxYawThisFrame, 0.0f);
				Direction->AddLocalRotation(LocalRotationToAdd);
			}
		
		}
		
		// move the tank
		FVector MovementDirection = Direction->GetForwardVector() * (bReverse? -1.0 : 1);
		FVector Pos = GetActorLocation();
		
		const float MoveSpeed = GetMoveSpeed();
		Pos.X += MovementDirection.X * MoveSpeed * DeltaTime;
		Pos.Y += MovementDirection.Y * MoveSpeed * DeltaTime;


		SetActorLocation(Pos);
	}
	
}

void ATank::Fire1Pressed()
{
	//UE_LOG(LogTemp, Warning, TEXT("Fire1 Pressed"));
	
	//if( ATankTurret* Turret = Cast<ATankTurret>(ChildTurret->GetChildActor()) )
	//	Turret->StartFiring(1);

	if(AbilitySystemComponent)
		AbilitySystemComponent->TryActivateAbilityByClass(Fire1AbilityClass);
}

void ATank::Fire1Released()
{
	//UE_LOG(LogTemp, Warning, TEXT("Fire1 Released"));
	//if( ATankTurret* Turret = Cast<ATankTurret>(ChildTurret->GetChildActor()) )
	//	Turret->StopFiring();
}

void ATank::Fire2Pressed()
{
	//UE_LOG(LogTemp, Warning, TEXT("Fire2 Pressed"));
//	if( ATankTurret* Turret = Cast<ATankTurret>(ChildTurret->GetChildActor()) )
//		Turret->StartFiring(2);

	if (AbilitySystemComponent)
		AbilitySystemComponent->TryActivateAbilityByClass(Fire2AbilityClass);
}

void ATank::Fire2Released()
{
	//UE_LOG(LogTemp, Warning, TEXT("Fire2 Released"));
	//if( ATankTurret* Turret = Cast<ATankTurret>(ChildTurret->GetChildActor()) )
	//	Turret->StopFiring();
}

void ATank::BeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// only collide when moving
	if(TankInput.MovementInput.IsNearlyZero())
		return;

	if ( APawnWithAbilities* OtherPawn = Cast<APawnWithAbilities>(OtherActor) )
		OnOverlapPawnWithAbilities(OtherPawn);
}

void ATank::OnOverlapPawnWithAbilities(APawnWithAbilities* OtherPawn)
{
	if(OtherPawn->GetClass() != this->GetClass())
	{
		//const float TargetHealth = OtherPawn->GetHealthRemaining();
		//const float TargetHealth = OtherPawn->GetHealth();
		
		//if(TargetHealth > 0)
		//	OtherPawn->ReceiveDamage(TargetHealth, EDamageType::EDT_Crushed);

		FGameplayEventData Data;
		Data.Target = OtherPawn;
		Data.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(OtherPawn);
		//FGameplayTag::RequestGameplayTag(FName(TEXT("Ability.Porojectile.Hit")))
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, GameplayEventHitZombieTag, Data);
		
	}
}

void ATank::HandleDeath()
{
	if(APlayerController* PC = Cast<APlayerController>(GetController()))
		PC->RestartLevel();
}
