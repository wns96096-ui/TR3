#pragma once

UENUM(BlueprintType)
enum class StatusType :uint8
{
	Stun,
	Back,
	Electric,
	
};

UENUM(BlueprintType)
enum class TrapType :uint8
{
	Box = 0,
	X_Elect = 1,
	Y_Elect = 2,
}; 

