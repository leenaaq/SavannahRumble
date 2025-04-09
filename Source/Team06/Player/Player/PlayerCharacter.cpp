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
#include "Player/Component/ItemManagerComponent.h"

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
    EIC->BindAction(BKeyAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleBKey);
    EIC->BindAction(ESCKeyAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleESCKey);
    
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

    
    ValidateEssentialReferences();
    UpdateStatsFromDataTable();
}

void APlayerCharacter::ValidateEssentialReferences()
{
    Super::ValidateEssentialReferences();

    if (ItemRightMeleeAttackMontage == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[CHECK] BP_PlayerCharacter : ItemRightMeleeAttackMontage is not set!"));
    }

    if (ItemRightRangedAttackMontage == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[CHECK] BP_PlayerCharacter : ItemRightRangedAttackMontage is not set!"));
    }

    if (RightMeleeAttackMontage == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[CHECK] BP_PlayerCharacter : RightMeleeAttackMontage is not set!"));
    }

    if (LeftMeleeAttackMontage == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[CHECK] BP_PlayerCharacter : LeftMeleeAttackMontage is not set!"));
    }

    if (ItemManager == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[CHECK] BP_PlayerCharacter : ItemManagerComponent is missing!"));
    }
    else if (ItemManager->ItemDataTable == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[CHECK] BP_PlayerCharacter : ItemDataTable is not set in ItemManagerComponent!"));
    }

    if (InputMappingContext == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[CHECK] BP_PlayerCharacter : InputMappingContext is not set!"));
    }

    if (MoveAction == nullptr) UE_LOG(LogTemp, Error, TEXT("[CHECK] BP_PlayerCharacter : MoveAction is not set!"));
    if (LookAction == nullptr) UE_LOG(LogTemp, Error, TEXT("[CHECK] BP_PlayerCharacter : LookAction is not set!"));
    if (JumpAction == nullptr) UE_LOG(LogTemp, Error, TEXT("[CHECK] BP_PlayerCharacter : JumpAction is not set!"));
    if (LeftHandAttackAction == nullptr) UE_LOG(LogTemp, Error, TEXT("[CHECK] BP_PlayerCharacter : LeftHandAttackAction is not set!"));
    if (RightHandAttackAction == nullptr) UE_LOG(LogTemp, Error, TEXT("[CHECK] BP_PlayerCharacter : RightHandAttackAction is not set!"));
    if (BKeyAction == nullptr) UE_LOG(LogTemp, Error, TEXT("[CHECK] BP_PlayerCharacter : BKeyAction is not set!"));
    if (ESCKeyAction == nullptr) UE_LOG(LogTemp, Error, TEXT("[CHECK] BP_PlayerCharacter : ESCKeyAction is not set!"));
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

    const FName EquippedItem = CurrentEquippedItemName;
    float StartAttackTime = GetWorld()->GetTimeSeconds();
    bCanAttack = false;

    if (EquippedItem == "NONE")
    {
        ServerRPCRightHandMeleeAttack(StartAttackTime);
    }
    else
    {
        static const FString Context = TEXT("EquipItemContext");
        if (const UDataTable* Table = ItemManager->ItemDataTable)
        {
            if (const FEquipItemDataRow* Row = Table->FindRow<FEquipItemDataRow>(EquippedItem, Context))
            {
                switch (Row->ItemType)
                {
                case EEquipItemType::Melee:
                    ServerRPCItemMeleeAttack(StartAttackTime);
                    break;
                case EEquipItemType::Ranged:
                    ServerRPCItemRangedAttack(StartAttackTime);
                    break;
                default:
                    UE_LOG(LogTemp, Warning, TEXT("Invalid item type: %s"), *EquippedItem.ToString());
                    break;
                }
            }
        }
    }
}

void APlayerCharacter::ServerRPCItemMeleeAttack_Implementation(float InStartTime)
{
    FVector Offset = GetActorRightVector() * 50.0f;
    PerformMeleeAttack(Offset, ItemRightMeleeAttackMontage);
    MulticastPlayMeleeAttackMontage(ItemRightMeleeAttackMontage);
    ServerSetEquippedItemName("NONE");
}

bool APlayerCharacter::ServerRPCItemMeleeAttack_Validate(float InStartTime)
{
    return true;
}

void APlayerCharacter::ServerRPCItemRangedAttack_Implementation(float InStartTime)
{
    FVector Offset = GetActorRightVector() * 50.0f;
    PerformMeleeAttack(Offset, ItemRightRangedAttackMontage);
    MulticastPlayMeleeAttackMontage(ItemRightRangedAttackMontage);
    ServerSetEquippedItemName("NONE");
}

bool APlayerCharacter::ServerRPCItemRangedAttack_Validate(float InStartTime)
{
    return true;
}


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
    UE_LOG(LogTemp, Log, TEXT("Bkey"));
}

void APlayerCharacter::HandleESCKey(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Log, TEXT("ESCkey"));
}