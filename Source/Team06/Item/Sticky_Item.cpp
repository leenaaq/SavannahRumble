#include "Item/Sticky_Item.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ASticky_Item::ASticky_Item()
{
	PrimaryActorTick.bCanEverTick = false;

	// SlowArea 초기화
	SlowArea = CreateDefaultSubobject<USphereComponent>(TEXT("SlowArea"));
	SlowArea->SetupAttachment(RootComponent);
	SlowArea->InitSphereRadius(100.0f);
	SlowArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SlowArea->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void ASticky_Item::BeginPlay()
{
	Super::BeginPlay();

	SlowArea->OnComponentBeginOverlap.AddDynamic(this, &ASticky_Item::OnOverlapBegin);
}

void ASticky_Item::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character && Character->GetCharacterMovement())
	{
		UE_LOG(LogTemp, Warning, TEXT("[StickyItem] %s 슬로우 적용!"), *Character->GetName());
		float OriginalSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
		Character->GetCharacterMovement()->MaxWalkSpeed *= SlowMultiplier;

		FTimerHandle TimerHandle;
		FTimerDelegate TimerDel;
		TimerDel.BindLambda([Character, OriginalSpeed]() {
			if (Character && Character->GetCharacterMovement())
			{
				Character->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
				UE_LOG(LogTemp, Warning, TEXT("[StickyItem] %s 속도 복원"), *Character->GetName());
			}
			});
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, SlowDuration, false);
	}
}