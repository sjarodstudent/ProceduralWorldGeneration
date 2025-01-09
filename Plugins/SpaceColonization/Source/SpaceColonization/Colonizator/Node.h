// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Node.generated.h"

class AAttractor;

UCLASS(BlueprintType, Blueprintable)
class SPACECOLONIZATION_API USpaceColonizationNode : public UObject
{
	GENERATED_BODY()
	
private:
	FTransform transform;

	USpaceColonizationNode* parent = nullptr;
	TArray<USpaceColonizationNode*> children;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SegmentLength = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxThickness = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThicknessGrowth = 1.1f;

public:
	TArray<AAttractor*> CurrentNearbyAttractors;

	/**
	 * grow a child branch in the direction of the attracted attractors
	 * grow in the specified default direction if no attractors are attracted 
	 * @param defaultGrowDirection 
	 * @return 
	 */
	USpaceColonizationNode* GrowChildNode(const FVector& defaultGrowDirection = FVector::ZeroVector);

	void ThickenParent();

public:
	inline bool HasAttractors() const { return CurrentNearbyAttractors.Num() > 0; }

	UFUNCTION(BlueprintCallable)
	inline USpaceColonizationNode* GetParent() const { return parent; }
	UFUNCTION(BlueprintCallable)
	inline bool HasChildren() const { return children.Num() > 0; }
	
	UFUNCTION(BlueprintCallable)
	inline float GetSegmentLength() const { return SegmentLength; }
	inline void SetSegmentLength(const float& segmentLength) { SegmentLength = segmentLength; }

	inline float GetMaxThickness() const { return MaxThickness; }
	inline void SetMaxThickness(const float& maxThickness) { MaxThickness = maxThickness; }

	UFUNCTION(BlueprintCallable)
	inline float GetThickness() const { return transform.GetScale3D().Length(); }

	UFUNCTION(BlueprintCallable)
	inline float GetThicknessGrowth() const { return ThicknessGrowth; }
	UFUNCTION(BlueprintCallable)
	inline void SetThicknessGrowth(const float newThickness) { ThicknessGrowth = newThickness; }

	UFUNCTION(BlueprintCallable)
	inline FVector GetLocation() const { return transform.GetLocation(); }
	inline void SetLocation(const FVector& location) { transform.SetLocation(location); }
	UFUNCTION(BlueprintCallable)
	inline FQuat GetRotation() const { return transform.GetRotation(); }
	inline void SetRotation(const FRotator& rotation) { transform.SetRotation(rotation.Quaternion()); }
	UFUNCTION(BlueprintCallable)
	inline FVector GetScale() const { return transform.GetScale3D(); }
	inline void SetScale(const FVector& scale) { transform.SetScale3D(scale); }
};
