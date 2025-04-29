#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UGridComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGridComponent();

protected:
	virtual void OnRegister() override;

	UPROPERTY()
	class AGridManager* Manager;

public:
	void OnUpdatedProperties();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UProceduralMeshComponent* LineMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UProceduralMeshComponent* SquareMeshComp;

	UPROPERTY(EditAnywhere)
	class UMaterialInstance* GridMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	float LineThickness = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	FLinearColor LineColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	FLinearColor SquareColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	float LineOpacity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	float SquareOpacity = 0.25f;

	void SetVisibility(bool bIsVisible);

private:
	void DrawLine(FVector Start, FVector End, float Thickness, TArray<FVector>& Vertices, TArray<int32>& Triangles);

	UMaterialInstanceDynamic* CreateMaterialInstance(FLinearColor Color, float Opacity, float Power = 2.f) const;
};
