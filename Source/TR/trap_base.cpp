
#include "trap_base.h"

#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "StatusEffectInterface.h"
#include "TrapDestroyInterface.h"


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

void Atrap_base::MoveRotate()
{

	
	
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

	const bool bDurationEnded = ElapsedMoveTime >= MoveDuration;
	const float MoveDeltaTime = bDurationEnded
		? DeltaTime - (ElapsedMoveTime - MoveDuration)
		: DeltaTime;

	const FVector CurrentLocation = GetActorLocation();
	FVector NextLocation = CurrentLocation + (TargetVelocity * MoveDeltaTime);

	if (bBounceX)
	{
		if (NextLocation.X <= MinX)
		{
			NextLocation.X = MinX;
			TargetVelocity.X = FMath::Abs(TargetVelocity.X);
		}
		else if (NextLocation.X >= MaxX)
		{
			NextLocation.X = MaxX;
			TargetVelocity.X = -FMath::Abs(TargetVelocity.X);
		}
	}

	SetActorLocation(NextLocation);

	if (bDurationEnded)
	{
		if (IsLoop)
		{
			ElapsedMoveTime = 0.0f;
			TargetVelocity.Y = -TargetVelocity.Y;
		}
		else
		{
			IsMove = false;
			SetActorTickEnabled(false);
		}

		return;
	}
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

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Overlap: %s"), *OtherActor->GetName())
		);
	}

	if (OtherActor->GetClass()->ImplementsInterface(UStatusEffectReceiver::StaticClass()))
	{
		IStatusEffectReceiver::Execute_ApplyStatusEffect(
			OtherActor,
			EffectType
		);
	}

	if (OtherActor->GetClass()->ImplementsInterface(UTrapDestroyReceiver::StaticClass()))
	{
		ITrapDestroyReceiver::Execute_OnTrapDestroyTriggered(OtherActor, this);
		OnTrapTriggered(OtherActor);
		Destroy();
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
		TEXT("%s BeginPlay: IsMove=%s TargetVelocity=%s MoveDuration=%.2f TickEnabled=%s"),
		*GetName(),
		IsMove ? TEXT("true") : TEXT("false"),
		*TargetVelocity.ToString(),
		MoveDuration,
		IsActorTickEnabled() ? TEXT("true") : TEXT("false")
	);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *MoveDebugMessage);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Cyan, MoveDebugMessage);
	}
}
