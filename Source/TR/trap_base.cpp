
#include "trap_base.h"

#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "StatusEffectInterface.h"


Atrap_base::Atrap_base()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(SceneRoot);

	CollisionBox->SetGenerateOverlapEvents(true);
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &Atrap_base::OnObstacleBeginOverlap);
}

const FVector& Atrap_base::GetInitialSpawnLocation() const
{
	return InitialSpawnLocation;
}

	
void Atrap_base::MoveObject(float DeltaTime)
{
	if (!IsMove)
	{
		return;
	}

	if (MoveDuration <= 0.0f || TargetVelocity.IsNearlyZero())
	{
		IsMove = false;
		SetActorTickEnabled(false);
		return;
	}

	ElapsedMoveTime += DeltaTime;

	const FVector CurrentLocation = GetActorLocation();
	const bool bUseTargetDistance = TargetDistance > 0.0f;
	const FVector MoveDirection = TargetVelocity.GetSafeNormal();
	const FVector EffectiveVelocity = bUseTargetDistance
		? MoveDirection * (TargetDistance / MoveDuration)
		: TargetVelocity;
	const FVector NextLocation = CurrentLocation + (EffectiveVelocity * DeltaTime);

	if (bUseTargetDistance)
	{
		const float CurrentDistance = FVector::Dist(StartLocation, NextLocation);
		if (CurrentDistance >= TargetDistance)
		{
			const FVector TargetLocation = StartLocation + (MoveDirection * TargetDistance);
			SetActorLocation(TargetLocation);

			if (IsLoop)
			{
				StartLocation = TargetLocation;
				ElapsedMoveTime = 0.0f;
				TargetVelocity = -TargetVelocity;
			}
			else
			{
				IsMove = false;
				SetActorTickEnabled(false);
			}

			return;
		}
	}
	else if (ElapsedMoveTime >= MoveDuration)
	{
		const float OvershootTime = ElapsedMoveTime - MoveDuration;
		const float ValidDeltaTime = DeltaTime - OvershootTime;

		SetActorLocation(CurrentLocation + (TargetVelocity * ValidDeltaTime));

		if (IsLoop)
		{
			ElapsedMoveTime = 0.0f;
			TargetVelocity = -TargetVelocity;
		}
		else
		{
			IsMove = false;
			SetActorTickEnabled(false);
		}

		return;
	}

	SetActorLocation(NextLocation);
}

	void Atrap_base::OnObstacleBeginOverlap(
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

	void Atrap_base::Tick(float DeltaTime)
	{
		Super::Tick(DeltaTime);
		MoveObject(DeltaTime);
	}

	void Atrap_base::BeginPlay()
	{
		Super::BeginPlay();

		StartLocation = GetActorLocation();
		InitialSpawnLocation = StartLocation;
		SetActorTickEnabled(IsMove);

		const FString MoveDebugMessage = FString::Printf(
			TEXT("%s BeginPlay: IsMove=%s TargetVelocity=%s MoveDuration=%.2f TargetDistance=%.2f TickEnabled=%s"),
			*GetName(),
			IsMove ? TEXT("true") : TEXT("false"),
			*TargetVelocity.ToString(),
			MoveDuration,
			TargetDistance,
			IsActorTickEnabled() ? TEXT("true") : TEXT("false")
		);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *MoveDebugMessage);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Cyan, MoveDebugMessage);
		}
	}

