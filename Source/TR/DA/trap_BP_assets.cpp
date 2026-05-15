// Fill out your copyright notice in the Description page of Project Settings.


#include "trap_BP_assets.h"

TSubclassOf<Atrap_base> Utrap_BP_assets::GetTrapClass(TrapType TrapType) const
{
	const TSubclassOf<Atrap_base>* TrapClass = ObjClass.Find(TrapType);
	return TrapClass ? *TrapClass : nullptr;
}
