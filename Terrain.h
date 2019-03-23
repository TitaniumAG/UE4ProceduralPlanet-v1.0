// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Terrain.generated.h"

UCLASS()
class PROJECTQUADTREE_API ATerrain : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATerrain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called in editor
	virtual void PostActorCreated() override;

	//Called when a property is changed
	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent);

	// Subdivision count
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		int32 Recursions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		FVector PlayerPos;

	// Create our procedural mesh component
	UProceduralMeshComponent * TerrainMesh;

	// Variables for the component
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	// Variables for subdivision
	TArray<FVector> Vertices_New;
	TArray<int32> Triangles_New;
	int32 IndexA, IndexB, IndexC;

	// Handle Subdivision count
	void HandleSubdivision();

	//Do the actual subdivision
	void Subdivide(int32 a, int32 b, int32 c);

	//Rebuild triangle list 
	void BuildTriangleList();

	//Check within bounds
	void CheckPlayerInBounds();

	// Within bounds boolean
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		bool InBounds;

	// Within bounds boolean
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		FVector BoundsLoc;

	
	
};
