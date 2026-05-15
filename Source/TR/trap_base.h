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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Move")
	float MoveDuration = 2.0f;
	float ElapsedMoveTime = 0.0f;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement",
		  meta = (DisplayName = "Use Movement", ToolTip = "이동 기능 활성화."))
	bool IsMove = false;

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
				  DisplayName = "Target Distance",
				  EditCondition = "IsMove",
				  EditConditionHides,
				  ToolTip = "시작위치에서 이동거리."
		  ))
	float TargetDistance=0.0f;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement",
		  meta = (
				  DisplayName = "Loop Movement",
				  EditCondition = "IsMove",
				  EditConditionHides,
				  ToolTip = "목표 거리까지 이동한 뒤 방향을 반전하여 왕복 이동합니다."
		  ))
	bool IsLoop = false;

	
	void MoveObject(float DeltaTime) ;
	
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
