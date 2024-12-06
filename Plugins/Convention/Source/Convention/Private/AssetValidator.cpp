// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetValidator.h"

bool UAssetValidator::CanValidateAsset_Implementation(const FAssetData& InAssetData, UObject* InObject,
	FDataValidationContext& InContext) const
{
	return false;
}

EDataValidationResult UAssetValidator::ValidateLoadedAsset_Implementation(UObject* InAsset,
	TArray<FText>& ValidationErrors)
{
	return EDataValidationResult::Valid;
}
