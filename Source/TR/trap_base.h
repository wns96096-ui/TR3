#pragma once

#include "CoreMinimal.h"
#include "StatusEffectInterface.h"
#include "Enum/enum.h"
#include "GameFramework/Actor.h"
#include "trap_base.generated.h"

class UBoxComponent;
class Utrap_BP_assets;

UCLASS()
class TR_API Atrap_base : public AActor
{
	GENERATED_BODY()

public:
	Atrap_base();

	const FVector& GetInitialSpawnLocation() const;

protected:
	/*********************************************************************************/
	/* 이동 관련
	 * bMoveObject Boolean true = tick 사용 , false = 사용 X 
	 *  움직임만 틱 사용함 . 
	*/

	FVector StartLocation;
	FVector InitialSpawnLocation;
	FVector Scale = FVector(2.0f, 2.0f, 1.0f);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Object")
	Utrap_BP_assets* ObjAssets;
	// FVector CurrentLocation;
	// FVector NextLocation;
	// FVector Direction;
	// float Distance;
	// float Speed;
	// float Angle;
	// float AngleSpeed;
	// float AngleDistance;
	// float AngleDistanceSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float MoveDuration = 2.0f;
	float ElapsedMoveTime = 0.0f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement",
		meta = (DisplayName = "Use Movement", ToolTip = "이동 기능 활성화."))
	bool IsMove = false;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement",
	// 	meta = (DisplayName = "RocateMoveMent", ToolTip = "COS 왕복.",
	// 		EditCondition= "IsMove"
	// 		))
	//
	// bool IsRoate = false;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement",
		meta = (
			DisplayName = "Target Velocity",
			EditCondition = "IsMove",
			EditConditionHides,
			ToolTip = "초당 이동할 방향과 속도 "
		))
	FVector TargetVelocity = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement",
		meta = (
			DisplayName = "Loop Movement",
			EditCondition = "IsMove",
			EditConditionHides,
			ToolTip = "목표 거리까지 이동한 뒤 방향을 반전하여 왕복 이동합니다."
		))
	bool IsLoop = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement",
		meta = (
			DisplayName = "Bounce X",
			EditCondition = "IsMove",
			EditConditionHides,
			ToolTip = "X축 경계에 닿으면 X 방향만 반전합니다."
		))
	bool bBounceX = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement",
		meta = (
			DisplayName = "Min X",
			EditCondition = "bBounceX",
			EditConditionHides
		))
	float MinX = -1300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement",
		meta = (
			DisplayName = "Max X",
			EditCondition = "bBounceX",
			EditConditionHides
		))
	float MaxX = -100.0f;


	void MoveObject(float DeltaTime);
	void MoveRotate();
	/**
	 *  처음위치  = GetActorLocation(); 
	 *  CurrentLocation현재 이동 위치(Tick)계산  =처음위치 + (TargetVelocity + DeltaTime) 
	 *  이동거리  = Fvector::Dist(StartLocation, CurrentLocation)  
	 *  후 SetActorLocation (이동위치(Tick))
	 *  
	 *  만약 이동 거리가 오버샷 됫을경우 해야하나 ? 
	 *  
	 *
	 */

	virtual void BeginPlay() override;


	/*********************************************************************************/
	// 이벤트 관련 
	// Blueprint PROPERTY
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status Effect")
	StatusType EffectType = StatusType::Stun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UBoxComponent* CollisionBox;


	UFUNCTION()
	void OnObstacleBeginOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
			);
	/*********************************************************************************/
	virtual void Tick(float DeltaTime) override;
};
