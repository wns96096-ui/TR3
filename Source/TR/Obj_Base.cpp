
#include "Obj_base.h"
#include "Components/BoxComponent.h"
#include "StatusEffectInterface.h"

AObj_Base::AObj_Base()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	CollisionBox->OnComponentBeginOverlap.AddDynamic(
			this,
			&AObj_Base::OnObstacleBeginOverlap
	);
}


void AObj_Base::MoveObject(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("MoveObject"));

	if (!IsMove)
	{
		return;
	}

	FVector CurrentLocation = GetActorLocation();
	FVector NextLocation = CurrentLocation + (TargetVelocity * DeltaTime);

	float MovedDistance = FVector::Dist(StartLocation, NextLocation);

	if (MovedDistance >= TargetDistance)
	{
		// 오버슛 방지: 목표 위치로 보정
		FVector Direction = TargetVelocity.GetSafeNormal();
		NextLocation = StartLocation + Direction * TargetDistance;

		SetActorLocation(NextLocation);

		if (IsLoop)
		{
			// 현재 목표 지점을 새로운 시작점으로 잡고 반대 방향으로 이동
			StartLocation = NextLocation;
			TargetVelocity = -TargetVelocity;
		}
		else
		{
			// IsLoop 없다면  Tick 중지
			SetActorTickEnabled(false);
		}

		return;
	}

	SetActorLocation(NextLocation);
}

void AObj_Base::OnObstacleBeginOverlap(
	  UPrimitiveComponent* OverlappedComponent,
	  AActor* OtherActor,
	  UPrimitiveComponent* OtherComp,
	  int32 OtherBodyIndex,
	  bool bFromSweep,
	  const FHitResult& SweepResult
)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Overlap: %s"), *OtherActor->GetName())
	);

	if
(OtherActor->GetClass()->ImplementsInterface(UStatusEffectReceiver::StaticClass()))
{
	GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Green,
			TEXT("Interface Found")
	);

	IStatusEffectReceiver::Execute_ApplyStatusEffect(
			OtherActor,
			EffectType
	);
}
}

void AObj_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveObject(DeltaTime);
}

void AObj_Base::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
	SetActorTickEnabled(IsMove);
		
}
