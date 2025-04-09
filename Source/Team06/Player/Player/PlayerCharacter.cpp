#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/GameStateBase.h"
#include "Player/Controller/PCController_GamePlay.h"

APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bUseControllerDesiredRotation = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->TargetArmLength = 400.f;
    SpringArm->bUsePawnControlRotation = true;
    SpringArm->SetupAttachment(GetRootComponent());

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->bUsePawnControlRotation = false;
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    bCanAttack = true;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

    EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleMoveInput);
    EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleLookInput);
    EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
    EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
    EIC->BindAction(LeftHandAttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleLeftHandMeleeAttack);
    EIC->BindAction(RightHandAttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleRightHandMeleeAttack);
    EIC->BindAction(BKeyAction, ETriggerEvent::Started, this, &APlayerCharacter::HandleBKey);
    EIC->BindAction(ESCKeyAction, ETriggerEvent::Started, this, &APlayerCharacter::HandleESCKey);

}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority() == false && IsLocallyControlled() == true)
    {
        APlayerController* PC = Cast<APlayerController>(GetController());
        checkf(IsValid(PC), TEXT("PlayerController is invalid."));

        UEnhancedInputLocalPlayerSubsystem* EILPS = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
        checkf(IsValid(EILPS), TEXT("EnhancedInputLocalPlayerSubsystem is invalid."));

        EILPS->AddMappingContext(InputMappingContext, 0);
    }

    UpdateStatsFromDataTable();
}

void APlayerCharacter::HandleMoveInput(const FInputActionValue& InValue)
{
    if (!IsValid(Controller))
    {
        UE_LOG(LogTemp, Error, TEXT("Controller is invalid."));
        return;
    }

    const FVector2D InMovementVector = InValue.Get<FVector2D>();

    const FRotator ControlRotation = Controller->GetControlRotation();
    const FRotator ControlYawRotation(0.0f, ControlRotation.Yaw, 0.0f);

    const FVector ForwardDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDirection, InMovementVector.X);
    AddMovementInput(RightDirection, InMovementVector.Y);
}

void APlayerCharacter::HandleLookInput(const FInputActionValue& InValue)
{
    if (!IsValid(Controller))
    {
        UE_LOG(LogTemp, Error, TEXT("Controller is invalid."));
        return;
    }

    const FVector2D InLookVector = InValue.Get<FVector2D>();

    AddControllerYawInput(InLookVector.X);
    AddControllerPitchInput(InLookVector.Y);
}

///////////////////////////////////////////
//           MeleeAttack                 //
///////////////////////////////////////////
#pragma region Attack
void APlayerCharacter::HandleLeftHandMeleeAttack(const FInputActionValue& InValue)
{
    if (!bCanAttack || GetbIsStunned())
    {
        return;
    }

    float StartAttackTime = GetWorld()->GetTimeSeconds();
    bCanAttack = false;
    ServerRPCLeftHandMeleeAttack(StartAttackTime);
}

void APlayerCharacter::HandleRightHandMeleeAttack(const FInputActionValue& InValue)
{
    if (!bCanAttack || GetbIsStunned())
    {
        return;
    }

    // 들고 있는 아이템이 없는 지 확인(none)일 경우 아래 코드 실행

    float StartAttackTime = GetWorld()->GetTimeSeconds();
    bCanAttack = false;
    ServerRPCRightHandMeleeAttack(StartAttackTime);

    // 들고 있는 템이 none이 아닐 경우 
        // 장착 아이템 테이블에서 들고있는 아이템 이름을 키로 해당 아이템이 melee 아이템인지 ranged 아이템인지 확인

        // melee 아이템일 경우
            // float StartAttackTime = GetWorld()->GetTimeSeconds();
            // bCanAttack = false;
            // ServerRPCItemMeleeAttack
            
        // Ranged 아이템일 경우
            // float StartAttackTime = GetWorld()->GetTimeSeconds();
            // bCanAttack = false; 
            // ServerRPCItemRangedAttack
}

// 근거리 아이템 RPC
//void APlayerCharacter::ServerRPCRightHandMeleeAttack_Implementation(float InStartAttackTime)
//{
    //FVector RightOffset = GetActorRightVector() * 50.0f;
    //PerformMeleeAttack(RightOffset, ItemRightMeleeAttackMontage);
    //MulticastPlayMeleeAttackMontage(ItemRightMeleeAttackMontage);
//}

//bool APlayerCharacter::ServerRPCRightHandMeleeAttack_Validate(float InStartAttackTime)
//{
    //return true;
//}


// 원거리 아이템 RPC
//void APlayerCharacter::ServerRPCItemRangedAttack_Implementation(float InStartAttackTime)
//{
    //PerformMeleeAttack(RightOffset, ItemRightMeleeAttackMontage);
    //MulticastPlayMeleeAttackMontage(ItemRightRangedAttackMontage);
//}

//bool APlayerCharacter::ServerRPCItemRangedAttack_Validate(float InStartAttackTime)
//{
    //return true;
//}

void APlayerCharacter::PerformMeleeAttack(const FVector& AttackOffset, UAnimMontage* AttackMontage)
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && AttackMontage)
    {
        AnimInstance->Montage_Play(AttackMontage);

        float MontageDuration = AttackMontage->GetPlayLength();
        GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &APlayerCharacter::ResetAttack, MontageDuration, false);
        CheckMeleeAttackHit(AttackOffset);
    }
    else
    {
        ResetAttack();
    }
}

void APlayerCharacter::CheckMeleeAttackHit(const FVector& AttackOffset)
{
    TArray<FHitResult> OutHitResults;
    FCollisionQueryParams Params(NAME_None, false, this);

    const float MeleeAttackRange = 50.f;
    const float MeleeAttackRadius = 50.f;
    const FVector Forward = GetActorForwardVector();
    const FVector Start = GetActorLocation() + AttackOffset + Forward * GetCapsuleComponent()->GetScaledCapsuleRadius();
    const FVector End = Start + Forward * MeleeAttackRange;

    bool bHit = GetWorld()->SweepMultiByChannel(
        OutHitResults,
        Start,
        End,
        FQuat::Identity,
        ECC_Camera,
        FCollisionShape::MakeSphere(MeleeAttackRadius),
        Params
    );
    TSet<APlayerBase*> ProcessedPlayers;

    for (const FHitResult& Hit : OutHitResults)
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor && HitActor != this)
        {
            if (APlayerBase* DamagedPlayer = Cast<APlayerBase>(HitActor))
            {
                if (ProcessedPlayers.Contains(DamagedPlayer))
                {
                    continue;
                }
                ProcessedPlayers.Add(DamagedPlayer);

                ServerRPCPerformMeleeHit(DamagedPlayer, GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
            }
        }
    }

    FColor DrawColor = bHit ? FColor::Green : FColor::Red;
    DrawDebugMeleeAttack(DrawColor, Start, End, Forward);
}


void APlayerCharacter::ServerRPCPerformMeleeHit_Implementation(AActor* DamagedActor, float InCheckTime)
{
    if (APlayerBase* DamagedPlayer = Cast<APlayerBase>(DamagedActor))
    {
        float Damage = this->GetAttackDamage();
        UE_LOG(LogTemp, Log, TEXT("%s AttackDamage : %f"), *GetName(), Damage);

        FDamageEvent DamageEvent;
        DamagedPlayer->TakeDamage(Damage, DamageEvent, GetController(), this);

        FString ClientName = TEXT("no Client");
        if (APlayerController* PC = Cast<APlayerController>(DamagedPlayer->GetController()))
        {
            ClientName = PC->GetName();
        }
        UE_LOG(LogTemp, Log, TEXT("%s - HP : %f / %f"), *ClientName, DamagedPlayer->GetHealth(), DamagedPlayer->GetMaxHealth());
    }
}


bool APlayerCharacter::ServerRPCPerformMeleeHit_Validate(AActor* DamagedActor, float InCheckTime)
{
    return true;
}

void APlayerCharacter::DrawDebugMeleeAttack(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward)
{
    const float MeleeAttackRange = 50.f;
    const float MeleeAttackRadius = 50.f;
    FVector CapsuleOrigin = TraceStart + (TraceEnd - TraceStart) * 0.5f;
    float CapsuleHalfHeight = MeleeAttackRange * 0.5f;
    DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, MeleeAttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);
}

void APlayerCharacter::ResetAttack()
{
    if (HasAuthority())
    {
        MulticastResetAttack();
    }
}

void APlayerCharacter::ServerRPCLeftHandMeleeAttack_Implementation(float InStartAttackTime)
{
    FVector LeftOffset = GetActorRightVector() * -50.0f;
    PerformMeleeAttack(LeftOffset, LeftMeleeAttackMontage);
    MulticastPlayMeleeAttackMontage(LeftMeleeAttackMontage);
}

bool APlayerCharacter::ServerRPCLeftHandMeleeAttack_Validate(float InStartAttackTime)
{
    return true;
}

void APlayerCharacter::ServerRPCRightHandMeleeAttack_Implementation(float InStartAttackTime)
{
    FVector RightOffset = GetActorRightVector() * 50.0f;
    PerformMeleeAttack(RightOffset, RightMeleeAttackMontage);
    MulticastPlayMeleeAttackMontage(RightMeleeAttackMontage);
}

bool APlayerCharacter::ServerRPCRightHandMeleeAttack_Validate(float InStartAttackTime)
{
    return true;
}

void APlayerCharacter::MulticastPlayMeleeAttackMontage_Implementation(UAnimMontage* AttackMontage)
{
    if (AttackMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance && !AnimInstance->Montage_IsPlaying(AttackMontage))
        {
            AnimInstance->Montage_Play(AttackMontage);
        }
    }
}

void APlayerCharacter::MulticastResetAttack_Implementation()
{
    bCanAttack = true;
    OnRep_CanAttack();
}

void APlayerCharacter::OnRep_CanAttack()
{
    if (bCanAttack)
    {
        GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    }
    else
    {
        GetCharacterMovement()->SetMovementMode(MOVE_None);
    }
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(APlayerCharacter, bCanAttack);
}

#pragma endregion

void APlayerCharacter::OnStunned()
{
    Super::OnStunned();

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        DisableInput(PC);

        if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
            {
                InputSubsystem->RemoveMappingContext(InputMappingContext);
            }
        }

        PC->SetIgnoreMoveInput(true);
        PC->SetIgnoreLookInput(true);
    }
}

void APlayerCharacter::RecoverFromStun()
{
    Super::RecoverFromStun();

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        EnableInput(PC);
        if (ULocalPlayer* LP = PC->GetLocalPlayer())
    {
            if (UEnhancedInputLocalPlayerSubsystem* Subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
            {
                Subsys->AddMappingContext(InputMappingContext, 0);
            }
        }
        PC->SetIgnoreMoveInput(false);
        PC->SetIgnoreLookInput(false);
    }
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void APlayerCharacter::HandleBKey(const FInputActionValue& Value)
{
    APCController_GamePlay* PlayerController_GamePlay = GetController<APCController_GamePlay>();
    if (IsValid(PlayerController_GamePlay))
    {
        if (PlayerController_GamePlay->IsLocalController() || PlayerController_GamePlay->LobbyWidget)
        {
            PlayerController_GamePlay->OnHandleLobbyUI();
        }
    }
}

void APlayerCharacter::HandleESCKey(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Log, TEXT("ESCkey"));
}